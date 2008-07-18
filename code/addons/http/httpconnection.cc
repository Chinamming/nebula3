//------------------------------------------------------------------------------
//  httpconnection.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "http/httpconnection.h"

#if __WIN32__
namespace Net
{
ImplementClass(Net::HttpConnection, 'HTPC', Win32::Win32HttpConnection);
}
#else
#error "IMPLEMENT ME!"
#endif
