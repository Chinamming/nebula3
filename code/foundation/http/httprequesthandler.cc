//------------------------------------------------------------------------------
//  httprequesthandler.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "http/httprequesthandler.h"

namespace Http
{
ImplementClass(Http::HttpRequestHandler, 'HRHD', Core::RefCounted);

using namespace IO;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
HttpRequestHandler::HttpRequestHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
HttpRequestHandler::~HttpRequestHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Overwrite this method in your subclass and decide if you want
    to react on the request or not.
*/
bool
HttpRequestHandler::AcceptsRequest(const Ptr<HttpRequest>& request)
{
    return false;
}

//------------------------------------------------------------------------------
/**
    Overwrite this method in your subclass. This method will be called by the
    HttpServer if AcceptsRequest() returned true. The request handler should
    properly process the request by filling the responseContentStream with
    data (for instance a HTML page), set the MediaType on the 
    responseContentStream (for instance "text/html") and return with a
    HttpStatus code (usually HttpStatus::OK).
*/
void
HttpRequestHandler::HandleRequest(const Ptr<HttpRequest>& request)
{
    request->SetStatus(HttpStatus::NotFound);
}

} // namespace Http
