#pragma once
#ifndef NET_SOCKET_H
#define NET_SOCKET_H
//------------------------------------------------------------------------------
/**
    @class Net::Socket
    
    Platform independent wrapper class for the Sockets API.
    
    (C) 2006 Radon Labs GmbH
*/
#if __WIN32__
#include "net/win32/win32socket.h"
namespace Net
{
class Socket : public Win32::Win32Socket
{
    DeclareClass(Socket);
};
}
#elif __XBOX360__
#include "net/xbox360/xbox360socket.h"
namespace Net
{
class Socket : public Xbox360::Xbox360Socket
{ 
    DeclareClass(Socket);
};
}
#elif __WII__
#include "net/wii/wiisocket.h"
namespace Net
{
class Socket : public Wii::WiiSocket
{ 
    DeclareClass(Socket);
};
}
#else
#error "Socket class not implemented on this platform"
#endif
//------------------------------------------------------------------------------
#endif
