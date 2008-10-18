//------------------------------------------------------------------------------
//  tcperver.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "net/tcpserver.h"
#include "io/console.h"

namespace Net
{
__ImplementClass(Net::TcpServer, 'NTCS', Core::RefCounted);
__ImplementClass(Net::TcpServer::ListenerThread, 'tclt', Threading::Thread);

Threading::CriticalSection TcpServer::connectionCritSect;

using namespace Util;
using namespace Threading;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
TcpServer::TcpServer() :
    isOpen(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
TcpServer::~TcpServer()
{
    n_assert(!this->IsOpen());
}

//------------------------------------------------------------------------------
/**
*/
bool
TcpServer::Open()
{
    n_assert(!this->isOpen);
    n_assert(!this->listenerThread.isvalid());
    n_assert(this->clientConnections.IsEmpty());

    // create the listener thread
    this->listenerThread = ListenerThread::Create();
    this->listenerThread->SetName("TcpServer::ListenerThread");
    this->listenerThread->SetTcpServer(this);
    this->listenerThread->SetAddress(this->ipAddress);
    this->listenerThread->SetCoreId(System::Cpu::MiscThreadCore);
    this->listenerThread->Start();
    
    this->isOpen = true;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
TcpServer::Close()
{
    n_assert(this->isOpen);
    n_assert(this->listenerThread.isvalid());
    
    // stop the listener thread
    this->listenerThread->Stop();
    this->listenerThread = 0;

    // disconnect client connections
    this->connectionCritSect.Enter();
    IndexT clientIndex;
    for (clientIndex = 0; clientIndex < this->clientConnections.Size(); clientIndex++)
    {
        const Ptr<TcpClientConnection>& curConnection = this->clientConnections[clientIndex];
        curConnection->Shutdown();
    }
    this->clientConnections.Clear();
    this->connectionCritSect.Leave();

    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void
TcpServer::AddClientConnection(const Ptr<TcpClientConnection>& conn)
{
    n_assert(conn.isvalid());
    this->connectionCritSect.Enter();
    this->clientConnections.Append(conn);
    this->connectionCritSect.Leave();
}

//------------------------------------------------------------------------------
/**
*/
Array<Ptr<TcpClientConnection> >
TcpServer::Recv()
{
    Array<Ptr<TcpClientConnection> > clientsWithData;

    // iterate over all clients, and check for new data,
    // if the client connection has been closed, remove
    // the client from the list
    this->connectionCritSect.Enter();
    IndexT clientIndex;
    for (clientIndex = 0; clientIndex < this->clientConnections.Size();)
    {
        const Ptr<TcpClientConnection>& cur = this->clientConnections[clientIndex];
        bool dropClient = false;
        if (cur->IsConnected())
        {
            Socket::Result res = cur->Recv();
            if (res == Socket::Success)
            {
                clientsWithData.Append(cur);
            }
            else if ((res == Socket::Error) || (res == Socket::Closed))
            {
                // some error occured, drop the connection
                dropClient = true;
            }
        }
        else
        {
            dropClient = true;
        }
        if (dropClient)
        {
            // connection has been closed, remove the client
            cur->Shutdown();
            this->clientConnections.EraseIndex(clientIndex);
        }
        else
        {
            clientIndex++;
        }
    }
    this->connectionCritSect.Leave();
    return clientsWithData;
}

//------------------------------------------------------------------------------
/**
*/
void
TcpServer::ListenerThread::SetTcpServer(TcpServer* serv)
{
    n_assert(0 != serv);
    this->tcpServer = serv;
}

//------------------------------------------------------------------------------
/**
*/
void
TcpServer::ListenerThread::SetAddress(const IpAddress& a)
{
    this->ipAddress = a;
}

//------------------------------------------------------------------------------
/**
    This is the actual listening method which runs in a separat thread.
    It creates a server socket and listens for incoming connections. When
    a client connects, a new TcpClientConnection will be created and
    added to the array of active connections.
*/
void
TcpServer::ListenerThread::DoWork()
{
    // we need a minimal Nebula3 runtime to work
    Ptr<Console> console = Console::Create();
    console->Open();
    n_printf("ListenerThread started!\n");

    // create a server socket
    Ptr<Socket> serverSocket = Socket::Create();
    if (serverSocket->Open(Socket::TCP))
    {
        serverSocket->SetAddress(this->ipAddress);
        serverSocket->SetReUseAddr(true);
        if (serverSocket->Bind())
        {
            n_printf("ListenerThread listening...\n");
            while ((!this->ThreadStopRequested() && serverSocket->Listen()))
            {
                // check if this was the wakeup function which woke us up...
                if (!this->ThreadStopRequested())
                {
                    Ptr<Socket> newSocket;
                    if (serverSocket->Accept(newSocket))
                    {
                        // create a new connection object and add to connection array
                        Ptr<TcpClientConnection> newConnection = TcpClientConnection::Create();
                        if (newConnection->Connect(newSocket))
                        {
                            this->tcpServer->AddClientConnection(newConnection);
                        }
                    }
                }
            }
        }
        else
        {
            n_printf("TcpServer::ListenerThread: Socket::Bind() failed!");
        }
        serverSocket->Close();
    }
    n_printf("ListenerThread shutting down!\n");

    // NOTE: we don't close the thread locale console manually, because
    // the other objects may have something to say while they're destroyed
}

//------------------------------------------------------------------------------
/**
    Emit a wakeup signal to the listener thread, this will just connect
    to the socket which wakes up the Socket::Listen() method.
*/
void
TcpServer::ListenerThread::EmitWakeupSignal()
{
    Ptr<Socket> socket = Socket::Create();
    if (socket->Open(Socket::TCP))
    {
        socket->SetAddress(IpAddress("127.0.0.1", this->ipAddress.GetPort()));
        socket->Connect();
        socket->Close();
    }
}

} // namespace Net