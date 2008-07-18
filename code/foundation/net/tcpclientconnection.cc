//------------------------------------------------------------------------------
//  tcpclientconnection.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "net/tcpclientconnection.h"
#include "io/memorystream.h"

namespace Net
{
ImplementClass(Net::TcpClientConnection, 'NTCC', Core::RefCounted);

using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
TcpClientConnection::TcpClientConnection()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
TcpClientConnection::~TcpClientConnection()
{
    this->Shutdown();
}

//------------------------------------------------------------------------------
/**
*/
bool
TcpClientConnection::Connect(const Ptr<Socket>& sock)
{
    n_assert(!this->socket.isvalid());
    n_assert(sock.isvalid());
    n_assert(sock->IsOpen());
    
    // check if socket is really actually connected
    if (sock->IsConnected())
    {
        this->socket = sock;
        this->socket->SetBlocking(true);
        this->socket->SetNoDelay(true);
        this->sendStream = MemoryStream::Create();
        this->recvStream = IO::MemoryStream::Create();
        n_printf("Client from addr=%s connected!\n", 
            this->socket->GetAddress().GetHostAddr().AsCharPtr());
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
TcpClientConnection::IsConnected() const
{
    if (this->socket.isvalid())
    {
        return this->socket->IsConnected();
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
TcpClientConnection::Shutdown()
{
    if (this->socket.isvalid())
    {
        this->socket->Close();
        this->socket = 0;
    }
    this->sendStream = 0;
    this->recvStream = 0;
}

//------------------------------------------------------------------------------
/**
*/
const IpAddress&
TcpClientConnection::GetClientAddress() const
{
    n_assert(this->socket.isvalid());
    return this->socket->GetAddress();
}

//------------------------------------------------------------------------------
/**
*/
Socket::Result
TcpClientConnection::Send()
{
    n_assert(this->sendStream.isvalid());
    if (this->sendStream->GetSize() == 0)
    {
        // nothing to send
        return Socket::Success;
    }
    
    Socket::Result res = Socket::Success;
    this->sendStream->SetAccessMode(Stream::ReadAccess);
    if (this->sendStream->Open())
    {
        // we may not exceed the maximum message size...
        // so we may have to split the send data into
        // multiple packets
        SizeT maxMsgSize = this->socket->GetMaxMsgSize();
        SizeT sendSize = this->sendStream->GetSize();
        uchar* ptr = (uchar*) this->sendStream->Map();
        SizeT overallBytesSent = 0;
        while ((Socket::Success == res) && (overallBytesSent < sendSize))
        {
            SizeT bytesToSend = sendSize - overallBytesSent;
            if (bytesToSend > maxMsgSize)
            {
                bytesToSend = maxMsgSize;
            }
            SizeT bytesSent = 0;
            res = this->socket->Send(ptr, bytesToSend, bytesSent);
            ptr += bytesSent;
            overallBytesSent += bytesSent;
        }
        this->sendStream->Unmap();
        this->sendStream->Close();
        this->sendStream->SetSize(0);
    }
    return res;
}

//------------------------------------------------------------------------------
/**
*/
Socket::Result
TcpClientConnection::Recv()
{
    n_assert(this->recvStream.isvalid());
    this->recvStream->SetAccessMode(Stream::WriteAccess);
    this->recvStream->SetSize(0);
    Socket::Result res = Socket::Success;
    if (this->recvStream->Open())
    {
        // NOTE: the following loop will make sure that Recv()
        // never blocks
        uchar buf[1024];
        while ((Socket::Success == res) && (this->socket->HasRecvData()))
        {
            SizeT bytesReceived = 0;
            res = this->socket->Recv(&buf, sizeof(buf), bytesReceived);
            if ((bytesReceived > 0) && (Socket::Success == res))
            {
                this->recvStream->Write(buf, bytesReceived);
            }
        }
        this->recvStream->Close();
    }
    this->recvStream->SetAccessMode(Stream::ReadAccess);
    if (Socket::Success == res)
    {
        if (this->recvStream->GetSize() > 0)
        {
            return Socket::Success;
        }
        else
        {
            return Socket::WouldBlock;
        }
    }
    else
    {
        return res;
    }
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<IO::Stream>&
TcpClientConnection::GetSendStream()
{
    return this->sendStream;
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<IO::Stream>&
TcpClientConnection::GetRecvStream()
{
    return this->recvStream;
}

} // namespace Net
