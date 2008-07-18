//------------------------------------------------------------------------------
//  httpserver.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "http/httpserver.h"
#include "http/httprequestreader.h"
#include "io/memorystream.h"

namespace Http
{
ImplementClass(Http::HttpServer, 'HTPS', Core::RefCounted);
ImplementSingleton(Http::HttpServer);

using namespace Util;
using namespace Net;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
HttpServer::HttpServer() :
    isOpen(false)
{
    ConstructSingleton;
    this->ipAddress.SetHostName("any");
    this->ipAddress.SetPort(2100);
}

//------------------------------------------------------------------------------
/**
*/
HttpServer::~HttpServer()
{
    n_assert(!this->IsOpen());
    DestructSingleton;        
}

//------------------------------------------------------------------------------
/**
*/
bool
HttpServer::Open()
{
    n_assert(!this->isOpen);
    this->isOpen = true;
    
    // setup a new TcpServer object
    this->tcpServer = TcpServer::Create();
    this->tcpServer->SetAddress(this->ipAddress);
    bool success = this->tcpServer->Open();

    // create the default http request handler
    this->defaultRequestHandler = DefaultHttpRequestHandler::Create();
    return success;
}

//------------------------------------------------------------------------------
/**
*/
void
HttpServer::Close()
{
    n_assert(this->isOpen);
    
    // destroy the default http request handler
    this->defaultRequestHandler = 0;

    // remove request handlers
    this->requestHandlers.Clear();

    // shutdown TcpServer
    this->tcpServer->Close();
    this->tcpServer = 0;
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void
HttpServer::AttachRequestHandler(const Ptr<HttpRequestHandler>& requestHandler)
{
    n_assert(requestHandler.isvalid());
    n_assert(this->isOpen);
    n_assert(InvalidIndex == this->requestHandlers.FindIndex(requestHandler));
    this->requestHandlers.Append(requestHandler);
}

//------------------------------------------------------------------------------
/**
*/
void
HttpServer::RemoveRequestHandler(const Ptr<HttpRequestHandler>& requestHandler)
{
    n_assert(requestHandler.isvalid());
    n_assert(this->isOpen);    
    IndexT index = this->requestHandlers.FindIndex(requestHandler);
    n_assert(InvalidIndex != index);
    this->requestHandlers.EraseIndex(index);
}

//------------------------------------------------------------------------------
/**
*/
void
HttpServer::OnFrame()
{
    n_assert(this->isOpen);

    // handle pending client requests
    Array<Ptr<TcpClientConnection>> recvConns = this->tcpServer->Recv();
    IndexT i;
    for (i = 0; i < recvConns.Size(); i++)
    {
        if (this->HandleHttpRequest(recvConns[i]->GetRecvStream(), recvConns[i]->GetSendStream()))
        {
            // send response back to HTTP client
            recvConns[i]->Send();
        }
        else
        {
            // hmm, this is not a HTTP client on the other side
            recvConns[i]->Shutdown();
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
HttpServer::HandleHttpRequest(const Ptr<Stream>& recvStream, const Ptr<Stream>& sendStream)
{
    // decode the request
    Ptr<HttpRequestReader> httpRequestReader = HttpRequestReader::Create();
    httpRequestReader->SetStream(recvStream);
    if (httpRequestReader->Open())
    {
        httpRequestReader->ReadRequest();
        httpRequestReader->Close();
    }
    if (httpRequestReader->IsValidHttpRequest())
    {
        URI requestURI = httpRequestReader->GetRequestURI();
    
        // create a content stream for the response
        Ptr<MemoryStream> responseContentStream = MemoryStream::Create();
        
        // build a HttpRequest object
        Ptr<HttpRequest> httpRequest = HttpRequest::Create();
        httpRequest->SetMethod(httpRequestReader->GetHttpMethod());
        httpRequest->SetURI(httpRequestReader->GetRequestURI());
        httpRequest->SetResponseContentStream(responseContentStream.upcast<Stream>());
        httpRequest->SetStatus(HttpStatus::NotFound);

        // find a request handler which accepts the request
        IndexT i;
        for (i = 0; i < this->requestHandlers.Size(); i++)
        {
            if (this->requestHandlers[i]->AcceptsRequest(httpRequest))
            {
                this->requestHandlers[i]->HandleRequest(httpRequest);
                break;
            }
        }

        // if not attached request handler felt responsible, ask the default request handler
        // (which usually displays the home page)
        if (HttpStatus::NotFound == httpRequest->GetStatus())
        {
            this->defaultRequestHandler->HandleRequest(httpRequest);
        }

        // configure and send the response
        Ptr<HttpResponseWriter> responseWriter = HttpResponseWriter::Create();
        responseWriter->SetStream(sendStream);
        responseWriter->SetStatusCode(httpRequest->GetStatus());
        if (HttpStatus::OK != httpRequest->GetStatus())
        {
            // an error occured, need to write an error message to the response stream
            Ptr<TextWriter> textWriter = TextWriter::Create();
            textWriter->SetStream(httpRequest->GetResponseContentStream());
            textWriter->Open();
            textWriter->WriteFormatted("%s %s", HttpStatus::ToString(httpRequest->GetStatus()).AsCharPtr(), HttpStatus::ToHumanReadableString(httpRequest->GetStatus()).AsCharPtr());
            textWriter->Close();
            responseContentStream->SetMediaType(MediaType("text/plain"));
        }
        if (httpRequest->GetResponseContentStream()->GetSize() > 0)
        {
            httpRequest->GetResponseContentStream()->GetMediaType().IsValid();
            responseWriter->SetContent(httpRequest->GetResponseContentStream());
        }
        responseWriter->Open();
        responseWriter->WriteResponse();
        responseWriter->Close();
        return true;
    }
    else
    {
        // the received data was not a valid HTTP request
        return false;
    }
}

} // namespace Http

