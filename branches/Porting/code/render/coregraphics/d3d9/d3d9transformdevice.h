#pragma once
#ifndef DIRECT3D9_D3D9TRANSFORMDEVICE_H
#define DIRECT3D9_D3D9TRANSFORMDEVICE_H
//------------------------------------------------------------------------------
/**
    @class Base::D3D9TransformDevice
    
    
    (C) 2007 Radon Labs GmbH
*/
#include "coregraphics/base/transformdevicebase.h"
#include "coregraphics/shadervariable.h"
#include "core/singleton.h"
#include "math/matrix44.h"

//------------------------------------------------------------------------------
namespace Direct3D9
{
class D3D9TransformDevice : public Base::TransformDeviceBase
{
    DeclareClass(D3D9TransformDevice);
    DeclareSingleton(D3D9TransformDevice);
public:
    /// constructor
    D3D9TransformDevice();
    /// destructor
    virtual ~D3D9TransformDevice();

    /// open the transform device
    virtual bool Open();
    /// close the transform device
    virtual void Close();

    /// updates shared shader variables dependent on view matrix
    virtual void ApplyViewSettings();
    /// apply current modelviewproj and model matrix in shader
    virtual void ApplyModelTransforms();

private:
    /// shd variable for eyepos
    Ptr<CoreGraphics::ShaderVariable> shdEyePos;
};


} // namespace Direct3D9
//------------------------------------------------------------------------------
#endif
    