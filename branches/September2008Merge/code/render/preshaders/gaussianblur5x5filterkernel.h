#pragma once
#ifndef PRESHADERS_GAUSSIANBLUR5X5FILTERKERNEL_H
#define PRESHADERS_GAUSSIANBLUR5X5FILTERKERNEL_H
//------------------------------------------------------------------------------
/**
    @class PreShaders::GaussianBlur5x5FilterKernel
  
    Implement a pre-shader which writes a 5x5 Gaussian blur filter kernel
    to the shader.
    
    (C) 2007 Radon Labs GmbH
*/    
#include "coregraphics/preshader.h"
#include "coregraphics/shadervariable.h"

//------------------------------------------------------------------------------
namespace PreShaders
{
class GaussianBlur5x5FilterKernel : public CoreGraphics::PreShader
{
    DeclareClass(GaussianBlur5x5FilterKernel);
public:
    /// called when attached to shader instance
    virtual void OnAttach(const Ptr<CoreGraphics::ShaderInstance>& shdInst);
    /// called when detached from shader instance
    virtual void OnDetach();
    /// called before rendering the shader
    virtual void OnApply();
    
private:
    /// compute gaussian distribution weight
    float GaussianDistribution(float x, float y, float rho) const;

    Ptr<CoreGraphics::ShaderVariable> numSamplesVar;
    Ptr<CoreGraphics::ShaderVariable> filterKernelVar;
};

} // namespace PreShaders
//------------------------------------------------------------------------------
#endif

