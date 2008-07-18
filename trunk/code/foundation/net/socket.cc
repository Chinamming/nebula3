//------------------------------------------------------------------------------
//  socket.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "net/socket.h"

namespace Net
{
#if __WIN32__
ImplementClass(Net::Socket, 'SOCK', Win32::Win32Socket);
#elif __XBOX360__
ImplementClass(Net::Socket, 'SOCK', Xbox360::Xbox360Socket);
#else
#error "Socket class not implemented on this platform!"
#endif
}
