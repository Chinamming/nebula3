//------------------------------------------------------------------------------
//  gaussianblur5x5filterkernel.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "preshaders/gaussianblur5x5filterkernel.h"
#include "coregraphics/shaderinstance.h"

namespace PreShaders
{
ImplementClass(PreShaders::GaussianBlur5x5FilterKernel, 'GBFK', CoreGraphics::PreShader);

using namespace Math;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
void
GaussianBlur5x5FilterKernel::OnAttach(const Ptr<ShaderInstance>& shdInst)
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
GaussianBlur5x5FilterKernel::OnDetach()
{
    this->numSamplesVar = 0;
    this->filterKernelVar = 0;
    PreShader::OnDetach();
}

//------------------------------------------------------------------------------
/**
*/
float
GaussianBlur5x5FilterKernel::GaussianDistribution(float x, float y, float rho) const
{
    float g = 1.0f / n_sqrt(2.0f * N_PI * rho * rho);
    g *= n_exp(-(x*x + y*y) / (2.0f * rho * rho));
    return g;
}

//------------------------------------------------------------------------------
/**
*/
void
GaussianBlur5x5FilterKernel::OnApply()
{
    float4 samples[13];
    float totalWeight = 0.0f;
    int numSamples = 0;
    int x;
    for (x = -2; x <= 2; x++)
    {
        int y;
        for (y = -2; y <= 2; y++)
        {
            // exclude pixels with block distance > 2 to reduce 5x5 filter
            // to 13 sample points instead of 25
            if ((abs(x) + abs(y)) <= 2)
            {
                n_assert(numSamples < 13);
                float xOffset = float(x) + 0.5f;
                float yOffset = float(y) + 0.5f;
                float weight = this->GaussianDistribution(float(x), float(y), 1.0f);
                samples[numSamples++].set(xOffset, yOffset, weight, 0.0f);
                totalWeight += weight;
            }
        }
    }    

    // normalize weights
    int i;
    for (i = 0; i < numSamples; i++)
    {
        samples[i].z() = samples[i].z() / totalWeight;
    }

    // set shader variables
    this->numSamplesVar->SetInt(numSamples);
    this->filterKernelVar->SetVectorArray(samples, numSamples);
}

} // namespace PreShaders