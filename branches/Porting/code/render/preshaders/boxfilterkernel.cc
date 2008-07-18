//------------------------------------------------------------------------------
//  boxfilterkernel.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "preshaders/boxfilterkernel.h"
#include "coregraphics/shaderinstance.h"

namespace PreShaders
{
ImplementClass(PreShaders::BoxFilterKernel, 'BXFK', CoreGraphics::PreShader);

using namespace Math;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
void
BoxFilterKernel::OnAttach(const Ptr<ShaderInstance>& shdInst)
{
    // call parent class
    PreShader::OnAttach(shdInst);
    
    // lookup shader variables
    this->numSamplesVar = shdInst->GetVariableBySemantic(ShaderVariable::Semantic("NumSamples"));
    this->filterKernelVar = shdInst->GetVariableBySemantic(ShaderVariable::Semantic("FilterKernelSamples"));
}

//------------------------------------------------------------------------------
/**
*/
void
BoxFilterKernel::OnDetach()
{
    this->numSamplesVar = 0;
    this->filterKernelVar = 0;
    PreShader::OnDetach();
}

//------------------------------------------------------------------------------
/**
*/
void
BoxFilterKernel::OnApply()
{
    // setup the shader variables, a filter kernel sample is described
    // by relative coordinates in xy, and the sample weight in z
    const int numSamples = 4;
    this->numSamplesVar->SetInt(numSamples);
    float4 samples[numSamples];
    samples[0].set(-0.5f, -0.5f, 0.25f, 0.0f);
    samples[1].set(+0.5f, -0.5f, 0.25f, 0.0f);
    samples[2].set(+0.5f, +0.5f, 0.25f, 0.0f);
    samples[3].set(-0.5f, +0.5f, 0.25f, 0.0f);
    this->filterKernelVar->SetVectorArray(samples, numSamples);
}

} // namespace PreShaders