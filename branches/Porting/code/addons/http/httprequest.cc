//------------------------------------------------------------------------------
//  httprequest.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "http/httprequest.h"

#if __WIN32__
namespace Net
{
ImplementClass(Net::HttpRequest, 'HTRQ', Win32::Win32HttpRequest);
}
#else
#error "IMPLEMENT ME!"
#endif
