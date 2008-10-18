#pragma once
#ifndef NET_TCPSERVER_H
#define NET_TCPSERVER_H
//------------------------------------------------------------------------------
/**
    @class Net::TcpServer

    A TcpServer opens a socket and listens for connecting TcpClients. This
    listen process happens in its own listener-thread and thus doesn't block
    the application. Each connected client is represented through a
    TcpClientConnection object which can be used by the application
    to communicate with a specific client.

    (C) 2006 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "threading/thread.h"
#include "util/array.h"
#include "net/tcpclientconnection.h"
#include "net/socket.h"
#include "threading/criticalsection.h"

//------------------------------------------------------------------------------
namespace Net
{
class TcpServer : public Core::RefCounted
{
    __DeclareClass(TcpServer);
public:
    /// constructor
    TcpServer();
    /// destructor
    virtual ~TcpServer();
    /// set address, hostname can be "any", "self" or "inetself"
    void SetAddress(const IpAddress& addr);
    /// get address
    const IpAddress& GetAddress() const;
    /// open the server
    bool Open();
    /// close the server
    void Close();
    /// return true if server is open
    bool IsOpen() const;
    /// poll clients connections for received data, call this frequently!
    Util::Array<Ptr<TcpClientConnection> > Recv();

private:
    /// a private listener thread class
    class ListenerThread : public Threading::Thread
    {
        __DeclareClass(ListenerThread);
    public:
        /// set pointer to parent tcp server
        void SetTcpServer(TcpServer* tcpServer);
        /// set ip address
        void SetAddress(const IpAddress& a);
    private:
        /// implements the actual listener method
        virtual void DoWork();
        /// send a wakeup signal
        virtual void EmitWakeupSignal();

        Ptr<TcpServer> tcpServer;
        IpAddress ipAddress;
        Ptr<Socket> socket;
    };
    friend class ListenerThread;
    /// add a client connection (called by the listener thread)
    void AddClientConnection(const Ptr<TcpClientConnection>& connection);

    IpAddress ipAddress;
    Ptr<ListenerThread> listenerThread;
    bool isOpen;
    Util::Array<Ptr<TcpClientConnection> > clientConnections;
    static Threading::CriticalSection connectionCritSect;
};

//------------------------------------------------------------------------------
/**
*/
inline
bool
TcpServer::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
TcpServer::SetAddress(const IpAddress& addr)
{
    this->ipAddress = addr;
}

//------------------------------------------------------------------------------
/**
*/
inline
const IpAddress&
TcpServer::GetAddress() const
{
    return this->ipAddress;
}

} // namespace Net
//------------------------------------------------------------------------------
#endif
