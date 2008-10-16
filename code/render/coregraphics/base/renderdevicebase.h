#pragma once
#ifndef BASE_RENDERDEVICEBASE_H
#define BASE_RENDERDEVICEBASE_H
//------------------------------------------------------------------------------
/**
    @class Base::RenderDeviceBase
  
    The central rendering object of the Nebula3 core graphics system. This
    is basically an encapsulation of the Direct3D device. The render device
    will presents its backbuffer to the display managed by the
    CoreGraphics::DisplayDevice singleton.
    
    (C) 2006 Radon Labs GmbH
*/    
#include "core/refcounted.h"
#include "core/singleton.h"
#include "coregraphics/rendereventhandler.h"
#include "coregraphics/primitivegroup.h"
#include "coregraphics/batchtype.h"
#include "coregraphics/imagefileformat.h"
#include "io/stream.h"
#include "debug/debugcounter.h"

namespace CoreGraphics
{
class Texture;
class VertexBuffer;
class IndexBuffer;
class ShaderInstance;
class RenderTarget;
};

//------------------------------------------------------------------------------
namespace Base
{
class RenderDeviceBase : public Core::RefCounted
{
    __DeclareClass(RenderDeviceBase);
    __DeclareSingleton(RenderDeviceBase);
public:
    /// constructor
    RenderDeviceBase();
    /// destructor
    virtual ~RenderDeviceBase();
    /// test if a compatible render device can be created on this machine
    static bool CanCreate();

    /// open the device
    bool Open();
    /// close the device
    void Close();
    /// return true if currently open
    bool IsOpen() const;
    /// attach a render event handler
    void AttachEventHandler(const Ptr<CoreGraphics::RenderEventHandler>& h);
    /// remove a render event handler
    void RemoveEventHandler(const Ptr<CoreGraphics::RenderEventHandler>& h);

    /// get default render target
    const Ptr<CoreGraphics::RenderTarget>& GetDefaultRenderTarget() const;

    /// begin complete frame
    bool BeginFrame();
    /// begin rendering a frame pass
    void BeginPass(const Ptr<CoreGraphics::RenderTarget>& rt, const Ptr<CoreGraphics::ShaderInstance>& passShader);
    /// begin rendering a batch inside 
    void BeginBatch(CoreGraphics::BatchType::Code batchType, const Ptr<CoreGraphics::ShaderInstance>& batchShader);
    /// set current vertex buffer
    void SetVertexBuffer(const Ptr<CoreGraphics::VertexBuffer>& vb);
    /// get current vertex buffer
    const Ptr<CoreGraphics::VertexBuffer>& GetVertexBuffer() const;
    /// set current index buffer
    void SetIndexBuffer(const Ptr<CoreGraphics::IndexBuffer>& ib);
    /// get current index buffer
    const Ptr<CoreGraphics::IndexBuffer>& GetIndexBuffer() const;
    /// set current primitive group
    void SetPrimitiveGroup(const CoreGraphics::PrimitiveGroup& pg);
    /// get current primitive group
    const CoreGraphics::PrimitiveGroup& GetPrimitiveGroup() const;
    /// draw current primitives
    void Draw();
    /// end current batch
    void EndBatch();
    /// end current pass
    void EndPass();
    /// end current frame
    void EndFrame();
    /// check if inside BeginFrame
    bool IsInBeginFrame() const;
    /// present the rendered scene
    void Present();
    /// save a screenshot to the provided stream
    void SaveScreenshot(CoreGraphics::ImageFileFormat::Code fmt, const Ptr<IO::Stream>& outStream);

protected:
    /// notify event handlers about an event
    bool NotifyEventHandlers(const CoreGraphics::RenderEvent& e);
    
    Util::Array<Ptr<CoreGraphics::RenderEventHandler> > eventHandlers;
    Ptr<CoreGraphics::RenderTarget> defaultRenderTarget;
    Ptr<CoreGraphics::VertexBuffer> vertexBuffer;
    Ptr<CoreGraphics::IndexBuffer> indexBuffer;
    CoreGraphics::PrimitiveGroup primitiveGroup;
    Ptr<CoreGraphics::RenderTarget> passRenderTarget;
    Ptr<CoreGraphics::ShaderInstance> passShader;
    Ptr<CoreGraphics::ShaderInstance> batchShader;
    bool isOpen;
    bool inNotifyEventHandlers;
    bool inBeginFrame;
    bool inBeginPass;
    bool inBeginBatch;

    _declare_counter(RenderDeviceNumPrimitives);
};

//------------------------------------------------------------------------------
/**
*/
inline void
RenderDeviceBase::SetPrimitiveGroup(const CoreGraphics::PrimitiveGroup& pg)
{
    this->primitiveGroup = pg;
}

//------------------------------------------------------------------------------
/**
*/
inline const CoreGraphics::PrimitiveGroup&
RenderDeviceBase::GetPrimitiveGroup() const
{
    return this->primitiveGroup;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::RenderTarget>&
RenderDeviceBase::GetDefaultRenderTarget() const
{
    n_assert(this->defaultRenderTarget.isvalid());
    return this->defaultRenderTarget;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
RenderDeviceBase::IsInBeginFrame() const
{
    return this->inBeginFrame;
}

} // namespace Base
//------------------------------------------------------------------------------
#endif
