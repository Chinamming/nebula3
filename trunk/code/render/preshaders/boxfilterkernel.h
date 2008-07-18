#pragma once
#ifndef PRESHADERS_BOXFILTERKERNEL_H
#define PRESHADERS_BOXFILTERKERNEL_H
//------------------------------------------------------------------------------
/**
    @class PreShaders::BoxFilterKernel
    
    Implements a simple pre-shader which writes a 2x2 box filter kernel
    for generating mipmaps.
    
    (C) 2007 Radon Labs GmbH
*/
#include "coregraphics/preshader.h"
#include "coregraphics/shadervariable.h"

//------------------------------------------------------------------------------
namespace PreShaders
{
class BoxFilterKernel : public CoreGraphics::PreShader
{
    DeclareClass(BoxFilterKernel);
public:
    /// called when attached to shader instance
    virtual void OnAttach(const Ptr<CoreGraphics::ShaderInstance>& shdInst);
    /// called when detached from shader instance
    virtual void OnDetach();
    /// called before rendering the shader
    virtual void OnApply();

private:
    Ptr<CoreGraphics::ShaderVariable> numSamplesVar;
    Ptr<CoreGraphics::ShaderVariable> filterKernelVar;
};

} // namespace PreShaders
//------------------------------------------------------------------------------
#endif
