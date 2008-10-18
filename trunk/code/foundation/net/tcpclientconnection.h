#pragma once
#ifndef NET_TCPCLIENTCONNECTION_H
#define NET_TCPCLIENTCONNECTION_H
//------------------------------------------------------------------------------
/**
    @class Net::TcpClientConnection
  
    A TcpClientConnection represents a connected TcpClient on the
    server side. TcpClientConnection objects are created and maintained
    by a TcpServer object over the lifetime of a client connection.
    TcpClientConnection objects are used to communicate directly with
    the specific client represented by the connection object. 

    TcpClientConnection objects are generally non-blocking. To receive
    data from the client, call the Recv() method until it returns true,
    this indicates that received data is available in the RecvStream. To
    read data from the RecvStream attach a StreamReader which matches the
    data format your expecting from the client (e.g. BinaryReader, TextReader,
    XmlReader, etc...). To send data back to the client just do the reverse:
    write data to the SendStream, and at any time call the Send() method which
    will send all data accumulated in the SendStream to the client.
    
    (C) 2006 Radon Labs GmbH
*/    
#include "core/refcounted.h"
#include "net/ipaddress.h"
#include "io/stream.h"
#include "net/socket.h"

//------------------------------------------------------------------------------
namespace Net
{
class TcpClientConnection : public Core::RefCounted
{
    __DeclareClass(TcpClientConnection);
public:
    /// constructor
    TcpClientConnection();
    /// destructor
    virtual ~TcpClientConnection();
    /// connect using provided socket
    bool Connect(const Ptr<Socket>& s);
    /// get the connection status
    bool IsConnected() const;
    /// shutdown the connection
    void Shutdown();
    /// get the client's ip address
    const IpAddress& GetClientAddress() const;
    /// send accumulated content of send stream to server
    Socket::Result Send();
    /// access to send stream
    const Ptr<IO::Stream>& GetSendStream();
    /// receive data from server into recv stream
    Socket::Result Recv();
    /// access to recv stream
    const Ptr<IO::Stream>& GetRecvStream();

private:
    Ptr<Socket> socket;
    Ptr<IO::Stream> sendStream;
    Ptr<IO::Stream> recvStream;
};

} // namespace Net
//------------------------------------------------------------------------------
#endif
