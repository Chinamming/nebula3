//------------------------------------------------------------------------------
//  D3D9TransformDevice.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/d3d9/d3d9transformdevice.h"
#include "coregraphics/shaderserver.h"

namespace Direct3D9
{
ImplementClass(Direct3D9::D3D9TransformDevice, 'D9TD', Base::TransformDeviceBase);
ImplementSingleton(Direct3D9::D3D9TransformDevice);

using namespace Util;
using namespace Math;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
D3D9TransformDevice::D3D9TransformDevice() 
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
D3D9TransformDevice::~D3D9TransformDevice()
{
    // empty
}


//------------------------------------------------------------------------------
/**
*/
bool
D3D9TransformDevice::Open()
{
    ShaderServer* shdServer = ShaderServer::Instance();
    this->shdEyePos = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic("EyePos"));

    return TransformDeviceBase::Open();
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9TransformDevice::Close()
{
    this->shdEyePos = 0;
    TransformDeviceBase::Close();
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9TransformDevice::ApplyViewSettings()
{
    // update shared shader variable eye pos from current view matrix
    this->shdEyePos->SetVector(GetViewTransform().getrow3());
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9TransformDevice::ApplyModelTransforms()
{    
    const Ptr<ShaderInstance>& shaderInst = ShaderServer::Instance()->GetActiveShaderInstance();    
    ShaderVariable::Semantic mvpSemantic = ShaderVariable::Semantic("ModelViewProjection");
    n_assert(shaderInst->HasVariableBySemantic(mvpSemantic));
    ShaderVariable::Semantic modelSemantic = ShaderVariable::Semantic("Model");
    
    // apply transform state to shader 
    const Ptr<CoreGraphics::ShaderVariable>& modelViewProjMatrix = shaderInst->GetVariableBySemantic(mvpSemantic);       
    modelViewProjMatrix->SetMatrix(this->GetModelViewProjTransform());

    if (shaderInst->HasVariableBySemantic(modelSemantic))
    {
        const Ptr<CoreGraphics::ShaderVariable>& modelMatrix = shaderInst->GetVariableBySemantic(modelSemantic);
        modelMatrix->SetMatrix(this->GetModelTransform());
    }
    
}

} // namespace Direct3D9
