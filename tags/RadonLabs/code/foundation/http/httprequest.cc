//------------------------------------------------------------------------------
//  httprequest.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "http/httprequest.h"

namespace Http
{
ImplementClass(Http::HttpRequest, 'HTRQ', Messaging::Message);
ImplementMsgId(HttpRequest);

//------------------------------------------------------------------------------
/**
*/
HttpRequest::HttpRequest() :
    method(HttpMethod::InvalidHttpMethod),
    status(HttpStatus::InvalidHttpStatus)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
HttpRequest::~HttpRequest()
{
    // empty
}

} // namespace Http
