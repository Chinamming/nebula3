//------------------------------------------------------------------------------
//  remoterequest.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "remote/remoterequest.h"

namespace Remote
{
__ImplementClass(Remote::RemoteRequest, 'RERQ', Messaging::Message);
ImplementMsgId(RemoteRequest);

//------------------------------------------------------------------------------
/**
*/
RemoteRequest::RemoteRequest() :
//    method(RemoteMethod::InvalidRemoteMethod),
    status(RemoteStatus::InvalidRemoteStatus)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
RemoteRequest::~RemoteRequest()
{
    // empty
}

} // namespace Remote
