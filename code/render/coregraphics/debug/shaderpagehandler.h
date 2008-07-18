#pragma once
#ifndef DEBUG_SHADERPAGEHANDLER_H
#define DEBUG_SHADERPAGEHANDLER_H
//------------------------------------------------------------------------------
/**
    @class Debug::ShaderPageHandler
  
    Provide a HTML debug page for shaders.
    
    Usage:
    http://host/shader                      - list of all shaders
    http://host/shader?shaderinfo=[resId]   - information about a specific shader
    
    (C) 2007 Radon Labs GmbH
*/
#include "http/httprequesthandler.h"
#include "resources/resourceid.h"
#include "http/htmlpagewriter.h"
#include "coregraphics/shader.h"

//------------------------------------------------------------------------------
namespace Debug
{
class ShaderPageHandler : public Http::HttpRequestHandler
{
    DeclareClass(ShaderPageHandler);
public:
    /// constructor
    ShaderPageHandler();
    /// return true if the http request is accepted by the request handler
    virtual bool AcceptsRequest(const Ptr<Http::HttpRequest>& request);
    /// handle a http request, the handler is expected to fill the content stream with response data
    virtual void HandleRequest(const Ptr<Http::HttpRequest>& request);

private:
    /// display information about specific shader
    Http::HttpStatus::Code HandleShaderInfoRequest(const Resources::ResourceId& resId, const Ptr<IO::Stream>& responseContentStream);
    /// write a shader variable table to the HTML stream
    void WriteShaderVariableTable(const Ptr<Http::HtmlPageWriter>& htmlWriter, const Util::Array<Ptr<CoreGraphics::ShaderVariable> >& vars);
};

} // namespace Debug
//------------------------------------------------------------------------------
#endif
