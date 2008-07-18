//------------------------------------------------------------------------------
//  frameshaderloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "frame/frameshaderloader.h"
#include "io/ioserver.h"
#include "coregraphics/rendertarget.h"
#include "coregraphics/displaydevice.h"
#include "coregraphics/shaderserver.h"
#include "coregraphics/renderdevice.h"
#include "models/modelnodetype.h"
#include "resources/sharedresourceserver.h"

namespace Frame
{
using namespace Resources;
using namespace CoreGraphics;
using namespace Models;
using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
Array<Ptr<FrameShader> >
FrameShaderLoader::LoadFrameShaders(const URI& uri)
{
    Array<Ptr<FrameShader> > result;
    Ptr<Stream> stream = IoServer::Instance()->CreateStream(uri);
    Ptr<XmlReader> xmlReader = XmlReader::Create();
    xmlReader->SetStream(stream);
    if (xmlReader->Open())
    {
        // make sure it's a valid frame shader file
        if (!xmlReader->HasNode("/Nebula3/FrameShader"))
        {
            n_error("FrameShaderLoader: '%s' is not a valid frame shader!", uri.AsString().AsCharPtr());
            return result;
        }
        xmlReader->SetToNode("/Nebula3");

        // for each frame shader definition in the file...
        if (xmlReader->SetToFirstChild("FrameShader")) do
        {
            Ptr<FrameShader> frameShader = FrameShader::Create();
            ParseFrameShader(xmlReader, frameShader);
            result.Append(frameShader);
        }
        while (xmlReader->SetToNextChild("FrameShader"));
        xmlReader->Close();
    }
    else
    {
        n_error("FrameShaderLoader: failed to load frame shader '%s'!", uri.AsString().AsCharPtr());
    }
    return result;
}

//------------------------------------------------------------------------------
/**
*/
void
FrameShaderLoader::ParseFrameShader(const Ptr<XmlReader>& xmlReader, const Ptr<FrameShader>& frameShader)
{
    frameShader->SetName(xmlReader->GetString("name"));
    
    // parse render target declarations
    if (xmlReader->SetToFirstChild("DeclareRenderTarget")) do
    {
        ParseRenderTarget(xmlReader, frameShader);
    }
    while (xmlReader->SetToNextChild("DeclareRenderTarget"));

    // parse frame passes
    if (xmlReader->SetToFirstChild("Pass")) do
    {
        ParseFramePass(xmlReader, frameShader);
    }
    while (xmlReader->SetToNextChild("Pass"));

    // parse posteffects
    if (xmlReader->SetToFirstChild("PostEffect")) do
    {
        ParsePostEffect(xmlReader, frameShader);
    }
    while (xmlReader->SetToNextChild("PostEffect"));
}

//------------------------------------------------------------------------------
/**
*/
void
FrameShaderLoader::ParseRenderTarget(const Ptr<XmlReader>& xmlReader, const Ptr<FrameShader>& frameShader)
{
    n_assert(DisplayDevice::Instance()->IsOpen());

    // create and configure a new render target
    Ptr<RenderTarget> renderTarget = RenderTarget::Create();
    const DisplayMode& displayMode = DisplayDevice::Instance()->GetDisplayMode();
    String name = xmlReader->GetString("name");
    renderTarget->SetResolveTextureResourceId(name);
    renderTarget->AddColorBuffer(PixelFormat::FromString(xmlReader->GetString("format")));
    if (xmlReader->HasAttr("format2"))
    {
        renderTarget->AddColorBuffer(PixelFormat::FromString(xmlReader->GetString("format2")));
    }
    if (xmlReader->HasAttr("format3"))
    {
        renderTarget->AddColorBuffer(PixelFormat::FromString(xmlReader->GetString("format3")));
    }
    if (xmlReader->HasAttr("format4"))
    {
        renderTarget->AddColorBuffer(PixelFormat::FromString(xmlReader->GetString("format4")));
    }
    if (xmlReader->HasAttr("depth"))
    {
        if (xmlReader->GetBool("depth"))
        {
            renderTarget->AddDepthStencilBuffer();
        }
    }
    if (xmlReader->HasAttr("width"))
    {
        renderTarget->SetWidth(xmlReader->GetInt("width"));
    }
    if (xmlReader->HasAttr("height"))
    {
        renderTarget->SetHeight(xmlReader->GetInt("height"));
    }
    if (xmlReader->HasAttr("relWidth"))
    {
        renderTarget->SetWidth(uint(float(displayMode.GetWidth()) * xmlReader->GetFloat("relWidth")));
    }
    if (xmlReader->HasAttr("relHeight"))
    {
        renderTarget->SetHeight(uint(float(displayMode.GetHeight()) * xmlReader->GetFloat("relHeight")));
    }
    if (xmlReader->HasAttr("msaa"))
    {
        if (xmlReader->GetBool("msaa"))
        {
            renderTarget->SetAntiAliasQuality(DisplayDevice::Instance()->GetAntiAliasQuality());
        }
    }
    renderTarget->Setup();
    frameShader->AddRenderTarget(name, renderTarget);
}

//------------------------------------------------------------------------------
/**
*/
Ptr<ShaderVariableInstance>
FrameShaderLoader::ParseShaderVariableInstance(const Ptr<XmlReader>& xmlReader, const Ptr<ShaderInstance>& shd)
{
    /// create a shader variable instance by semantic
    String semantic = xmlReader->GetString("sem");
    if (!shd->HasVariableBySemantic(semantic))
    {
        n_error("FrameShaderLoader: shader '%s' has no variable '%s' (%s, line %d)",
            shd->GetOriginalShader()->GetResourceId().Value().AsCharPtr(),
            semantic.AsCharPtr(),
            xmlReader->GetStream()->GetURI().AsString().AsCharPtr(),
            xmlReader->GetCurrentNodeLineNumber());
    }
    const Ptr<ShaderVariable>& shdVar = shd->GetVariableBySemantic(semantic);
    Ptr<ShaderVariableInstance> shdVarInst = shdVar->CreateInstance();

    /// get the default value of the shader variable
    String valueStr = xmlReader->GetString("value");
    switch (shdVarInst->GetShaderVariable()->GetType())
    {
        case ShaderVariable::IntType:
            shdVarInst->SetInt(valueStr.AsInt());
            break;

        case ShaderVariable::FloatType:
            shdVarInst->SetFloat(valueStr.AsFloat());
            break;

        case ShaderVariable::VectorType:
            shdVarInst->SetVector(valueStr.AsFloat4());
            break;

        case ShaderVariable::MatrixType:
            shdVarInst->SetMatrix(valueStr.AsMatrix44());
            break;

        case ShaderVariable::BoolType:
            shdVarInst->SetBool(valueStr.AsBool());
            break;

        case ShaderVariable::TextureType:
            shdVarInst->SetTexture(SharedResourceServer::Instance()->LookupSharedResource(valueStr).downcast<Texture>());            
            break;
    }
    return shdVarInst;
}

//------------------------------------------------------------------------------
/**
*/
void
FrameShaderLoader::ParseFramePass(const Ptr<XmlReader>& xmlReader, const Ptr<FrameShader>& frameShader)
{
    // create and configure a new frame pass
    Ptr<FramePass> framePass = FramePass::Create();
    framePass->SetName(xmlReader->GetString("name"));

    // setup the pass shader
    ResourceId shdResId = ResourceId("shd:" + xmlReader->GetString("shader"));
    Ptr<ShaderInstance> shader = ShaderServer::Instance()->CreateShaderInstance(shdResId);
    framePass->SetShader(shader);
    
    // setup the render target (if not render to default render target)
    if (xmlReader->HasAttr("renderTarget"))
    {
        String rtName = xmlReader->GetString("renderTarget");
        if (frameShader->HasRenderTarget(rtName))
        {
            framePass->SetRenderTarget(frameShader->GetRenderTargetByName(rtName));
        }
        else
        {
            n_error("FrameShaderLoader: render target '%s' not declared (%s, line %d)",
                xmlReader->GetStream()->GetURI().AsString().AsCharPtr(), 
                xmlReader->GetCurrentNodeLineNumber());
        }
    }
    else
    {
        framePass->SetRenderTarget(RenderDevice::Instance()->GetDefaultRenderTarget());
    }

    // setup the clear color, depth and stencil (if defined)
    if (xmlReader->HasAttr("clearColor"))
    {
        framePass->SetClearColor(xmlReader->GetFloat4("clearColor"));
    }
    if (xmlReader->HasAttr("clearDepth"))
    {
        framePass->SetClearDepth(xmlReader->GetFloat("clearDepth"));
    }
    if (xmlReader->HasAttr("clearStencil"))
    {
        framePass->SetClearStencil(uchar(xmlReader->GetInt("clearStencil")));
    }

    // add shader variable instances
    if (xmlReader->SetToFirstChild("ApplyShaderVariable")) do
    {
        Ptr<ShaderVariableInstance> var = ParseShaderVariableInstance(xmlReader, shader);
        framePass->AddVariable(var);
    }
    while (xmlReader->SetToNextChild("ApplyShaderVariable"));

    // add instance batches
    if (xmlReader->SetToFirstChild("Batch")) do
    {
        ParseFrameBatch(xmlReader, framePass);
    }
    while (xmlReader->SetToNextChild("Batch"));

    // add frame pass to frame shader
    frameShader->AddFramePass(framePass);
}

//------------------------------------------------------------------------------
/**
*/
void
FrameShaderLoader::ParseFrameBatch(const Ptr<XmlReader>& xmlReader, const Ptr<FramePass>& framePass)
{
    Ptr<FrameBatch> frameBatch = FrameBatch::Create();
    
    // setup the batch shader
    ResourceId shdResId = ResourceId("shd:" + xmlReader->GetString("shader"));
    Ptr<ShaderInstance> shader = ShaderServer::Instance()->CreateShaderInstance(shdResId);
    frameBatch->SetShader(shader);

    // setup batch type, model node filter, lighting and sorting mode
    frameBatch->SetType(BatchType::FromString(xmlReader->GetString("type")));
    frameBatch->SetNodeFilter(ModelNodeType::FromString(xmlReader->GetString("nodeFilter")));
    frameBatch->SetLightingMode(LightingMode::FromString(xmlReader->GetString("lighting")));
    frameBatch->SetSortingMode(SortingMode::FromString(xmlReader->GetString("sorting")));
    frameBatch->SetShaderFeatures(ShaderServer::Instance()->FeatureStringToMask(xmlReader->GetString("shdFeatures")));

    // add shader variable instances
    if (xmlReader->SetToFirstChild("ApplyShaderVariable")) do
    {
        Ptr<ShaderVariableInstance> var = ParseShaderVariableInstance(xmlReader, shader);
        framePass->AddVariable(var);
    }
    while (xmlReader->SetToNextChild("ApplyShaderVariable"));

    // add new batch to frame pass
    framePass->AddBatch(frameBatch);
}

//------------------------------------------------------------------------------
/**
*/
void
FrameShaderLoader::ParsePostEffect(const Ptr<XmlReader>& xmlReader, const Ptr<FrameShader>& frameShader)
{
    // create and configure a new post effect
    Ptr<FramePostEffect> framePostEffect = FramePostEffect::Create();
    framePostEffect->SetName(xmlReader->GetString("name"));

    // setup the pass shader
    ResourceId shdResId = ResourceId("shd:" + xmlReader->GetString("shader"));
    Ptr<ShaderInstance> shader = ShaderServer::Instance()->CreateShaderInstance(shdResId);
    framePostEffect->SetShader(shader);

    // test if a pre-shader has been defined
    if (xmlReader->HasAttr("preShader"))
    {
        // create a pre-shader object and attach it to the posteffect shader
        Ptr<PreShader> preShader = (PreShader*) Core::Factory::Instance()->Create(xmlReader->GetString("preShader"));
        shader->AddPreShader(preShader);
    }

    // setup the render target (if not render to default render target)
    if (xmlReader->HasAttr("renderTarget"))
    {
        String rtName = xmlReader->GetString("renderTarget");
        if (frameShader->HasRenderTarget(rtName))
        {
            framePostEffect->SetRenderTarget(frameShader->GetRenderTargetByName(rtName));
        }
        else
        {
            n_error("FrameShaderLoader: render target '%s' not declared (%s, line %d)",
                rtName.AsCharPtr(),
                xmlReader->GetStream()->GetURI().AsString().AsCharPtr(), 
                xmlReader->GetCurrentNodeLineNumber());
        }
    }
    else
    {
        framePostEffect->SetRenderTarget(RenderDevice::Instance()->GetDefaultRenderTarget());
    }

    // add shader variable instances
    if (xmlReader->SetToFirstChild("ApplyShaderVariable")) do
    {
        Ptr<ShaderVariableInstance> var = ParseShaderVariableInstance(xmlReader, shader);
        framePostEffect->AddVariable(var);
    }
    while (xmlReader->SetToNextChild("ApplyShaderVariable"));

    // add post effect to frame shader
    frameShader->AddPostEffect(framePostEffect);
}

} // namespace Frame