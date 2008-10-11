#pragma once
#ifndef FRAME_FRAMESHADER_H
#define FRAME_FRAMESHADER_H
//------------------------------------------------------------------------------
/**
    @class Frame::FrameShader
    
    A FrameShader controls the rendering of an entire frame, and is
    configured by an XML file.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "util/dictionary.h"
#include "resources/resourceid.h"
#include "coregraphics/rendertarget.h"
#include "coregraphics/shadervariableinstance.h"
#include "frame/framepass.h"
#include "frame/frameposteffect.h"
#include "internalgraphics/internalcameraentity.h"

//------------------------------------------------------------------------------
namespace Frame
{
class FrameShader : public Core::RefCounted
{
    __DeclareClass(FrameShader);
public:
    /// constructor
    FrameShader();
    /// destructor
    virtual ~FrameShader();    
    /// discard the frame shader
    void Discard();
    /// render the frame shader from the given camera
    void Render();

    /// set the name of the frame shader
    void SetName(const Resources::ResourceId& id);
    /// get the name of the frame shader
    const Resources::ResourceId& GetName() const;
    /// set the main render target
    void SetMainRenderTarget(const Ptr<CoreGraphics::RenderTarget>& rt);
    /// get the main render target
    const Ptr<CoreGraphics::RenderTarget>& GetMainRenderTarget() const;

    /// add a render target to the frame
    void AddRenderTarget(const Resources::ResourceId& resId, const Ptr<CoreGraphics::RenderTarget>& rt);
    /// get number of render targets
    SizeT GetNumRenderTargets() const;
    /// get render target by index
    const Ptr<CoreGraphics::RenderTarget>& GetRenderTargetByIndex(IndexT i) const;
    /// return true if render target exists by name
    bool HasRenderTarget(const Resources::ResourceId& resId) const;
    /// get render target by name
    const Ptr<CoreGraphics::RenderTarget>& GetRenderTargetByName(const Resources::ResourceId& resId) const;

    /// add a frame pass to the frame shader
    void AddFramePass(const Ptr<FramePass>& framePass);
    /// get number of frame passes
    SizeT GetNumFramePasses() const;
    /// get frame pass by index
    const Ptr<FramePass>& GetFramePassByIndex(IndexT i) const;
    /// return true if names pass exists
    bool HasFramePass(const Resources::ResourceId& resId) const;
    /// get frame pass by name
    const Ptr<FramePass>& GetFramePassByName(const Resources::ResourceId& resId) const;

    /// add a post effect to the frame shader
    void AddPostEffect(const Ptr<FramePostEffect>& postEffect);
    /// get number of post effects
    SizeT GetNumPostEffects() const;
    /// get post effect by index
    const Ptr<FramePostEffect>& GetPostEffectByIndex(IndexT i) const;
    /// return true if post effect exists
    bool HasPostEffect(const Resources::ResourceId& resId) const;
    /// get post effect by name
    const Ptr<FramePostEffect>& GetPostEffectByName(const Resources::ResourceId& resId) const;

private:
    Resources::ResourceId name;
    Ptr<CoreGraphics::RenderTarget> mainRenderTarget;
    Util::Dictionary<Resources::ResourceId, Ptr<CoreGraphics::RenderTarget> > renderTargets;
    Util::Dictionary<Resources::ResourceId, Ptr<CoreGraphics::ShaderVariableInstance> > shaderVariables;
    Util::Array<Ptr<FramePass> > framePasses;
    Util::Dictionary<Resources::ResourceId, IndexT> framePassIndexMap;
    Util::Array<Ptr<FramePostEffect> > postEffects;
    Util::Dictionary<Resources::ResourceId, IndexT> postEffectIndexMap;
};

//------------------------------------------------------------------------------
/**
*/
inline void
FrameShader::SetName(const Resources::ResourceId& resId)
{
    this->name = resId;
}

//------------------------------------------------------------------------------
/**
*/
inline const Resources::ResourceId&
FrameShader::GetName() const
{
    return this->name;
}

//------------------------------------------------------------------------------
/**
*/
inline void
FrameShader::SetMainRenderTarget(const Ptr<CoreGraphics::RenderTarget>& rt)
{
    this->mainRenderTarget = rt;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::RenderTarget>&
FrameShader::GetMainRenderTarget() const
{
    return this->mainRenderTarget;
}

//------------------------------------------------------------------------------
/**
*/
inline void
FrameShader::AddRenderTarget(const Resources::ResourceId& resId, const Ptr<CoreGraphics::RenderTarget>& rt)
{
    n_assert(!this->renderTargets.Contains(resId));
    this->renderTargets.Add(resId, rt);
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
FrameShader::GetNumRenderTargets() const
{
    return this->renderTargets.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::RenderTarget>& 
FrameShader::GetRenderTargetByIndex(IndexT i) const
{
    return this->renderTargets.ValueAtIndex(i);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
FrameShader::HasRenderTarget(const Resources::ResourceId& resId) const
{
    return this->renderTargets.Contains(resId);
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::RenderTarget>&
FrameShader::GetRenderTargetByName(const Resources::ResourceId& resId) const
{
    return this->renderTargets[resId];
}

//------------------------------------------------------------------------------
/**
*/
inline void
FrameShader::AddFramePass(const Ptr<FramePass>& framePass)
{
    this->framePasses.Append(framePass);
    this->framePassIndexMap.Add(framePass->GetName(), this->framePasses.Size() - 1);
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
FrameShader::GetNumFramePasses() const
{
    return this->framePasses.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<FramePass>&
FrameShader::GetFramePassByIndex(IndexT i) const
{
    return this->framePasses[i];
}

//------------------------------------------------------------------------------
/**
*/
inline bool
FrameShader::HasFramePass(const Resources::ResourceId& resId) const
{
    return this->framePassIndexMap.Contains(resId);
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<FramePass>&
FrameShader::GetFramePassByName(const Resources::ResourceId& resId) const
{
    return this->framePasses[this->framePassIndexMap[resId]];
}

//------------------------------------------------------------------------------
/**
*/
inline void
FrameShader::AddPostEffect(const Ptr<FramePostEffect>& postEffect)
{
    postEffect->Setup();
    this->postEffects.Append(postEffect);
    this->postEffectIndexMap.Add(postEffect->GetName(), this->postEffects.Size() - 1);
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
FrameShader::GetNumPostEffects() const
{
    return this->postEffects.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<FramePostEffect>&
FrameShader::GetPostEffectByIndex(IndexT i) const
{
    return this->postEffects[i];
}

//------------------------------------------------------------------------------
/**
*/
inline bool
FrameShader::HasPostEffect(const Resources::ResourceId& resId) const
{
    return this->postEffectIndexMap.Contains(resId);
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<FramePostEffect>&
FrameShader::GetPostEffectByName(const Resources::ResourceId& resId) const
{
    return this->postEffects[this->postEffectIndexMap[resId]];
}

} // namespace Frame
//------------------------------------------------------------------------------
#endif
    