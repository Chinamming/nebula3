//------------------------------------------------------------------------------
//  d3d9transformdevice.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/win360/d3d9transformdevice.h"
#include "coregraphics/shaderserver.h"
#include "coregraphics/displaydevice.h"

namespace Win360
{
__ImplementClass(Win360::D3D9TransformDevice, 'D9TD', Base::TransformDeviceBase);
__ImplementSingleton(Win360::D3D9TransformDevice);

using namespace Util;
using namespace Math;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
D3D9TransformDevice::D3D9TransformDevice() :
    mvpSemantic("ModelViewProjection"),
    modelSemantic("Model"),
    viewSemantic("View"),
    eyePosSemantic("EyePos"),
    displayResolutionSemantic("DisplayResolution")
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
    this->shdEyePos = shdServer->GetSharedVariableBySemantic(this->eyePosSemantic);
    this->viewMatrix = shdServer->GetSharedVariableBySemantic(this->viewSemantic);
    this->displayResolution = shdServer->GetSharedVariableBySemantic(this->displayResolutionSemantic);
    return TransformDeviceBase::Open();
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9TransformDevice::Close()
{
    this->shdEyePos = 0;
    this->viewMatrix = 0;
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
    
    // apply view matrix to shader
    this->viewMatrix->SetMatrix(this->GetViewTransform());   

    // set display resolution variable
    DisplayMode mode = DisplayDevice::Instance()->GetDisplayMode();
    this->displayResolution->SetVector(Math::float4((float)mode.GetWidth(), (float)mode.GetHeight(), mode.GetAspectRatio(), 0.0f));
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9TransformDevice::ApplyModelTransforms()
{    
    const Ptr<ShaderInstance>& shaderInst = ShaderServer::Instance()->GetActiveShaderInstance();    
    
    // apply ModelViewProjection matrix to shader 
    n_assert(shaderInst->HasVariableBySemantic(this->mvpSemantic));
    const Ptr<CoreGraphics::ShaderVariable>& modelViewProjMatrix = shaderInst->GetVariableBySemantic(mvpSemantic);       
    modelViewProjMatrix->SetMatrix(this->GetModelViewProjTransform());

    // apply optional Model matrix to shader
    if (shaderInst->HasVariableBySemantic(modelSemantic))
    {
        const Ptr<CoreGraphics::ShaderVariable>& modelMatrix = shaderInst->GetVariableBySemantic(modelSemantic);
        modelMatrix->SetMatrix(this->GetModelTransform());
    }
}

} // namespace Win360
