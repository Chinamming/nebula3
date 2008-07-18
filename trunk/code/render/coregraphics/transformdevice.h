#pragma once
#ifndef COREGRAPHICS_TRANSFORMDEVICE_H
#define COREGRAPHICS_TRANSFORMDEVICE_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::TransformDevice
    
    Manages global transform matrices and their combinations. Input transforms 
    are the view transform (transforms from world to view space),
    the projection transform (describes the projection from view space
    into projection space (pre-div-z)) and the current model matrix
    (transforms from model to world space). From these input transforms,
    the TransformDevice computes all useful combinations and
    inverted versions.

    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__
#include "coregraphics/d3d9/d3d9transformdevice.h"
namespace CoreGraphics
{
class TransformDevice : public Direct3D9::D3D9TransformDevice
{
    DeclareClass(TransformDevice);
    DeclareSingleton(TransformDevice);
public:
    /// constructor
    TransformDevice();
    /// destructor
    virtual ~TransformDevice();
};
}
#elif __XBOX360__
#include "coregraphics/xbox360/xbox360transformdevice.h"
namespace CoreGraphics
{
class TransformDevice : public Xbox360::Xbox360TransformDevice
{
    DeclareClass(TransformDevice);
    DeclareSingleton(TransformDevice);
public:
    /// constructor
    TransformDevice();
    /// destructor
    virtual ~TransformDevice();
};
}
#elif __WII__
#include "coregraphics/wii/wiitransformdevice.h"
namespace CoreGraphics
{
class TransformDevice : public Wii::WiiTransformDevice
{
    DeclareClass(TransformDevice);
    DeclareSingleton(TransformDevice);
public:
    /// constructor
    TransformDevice();
    /// destructor
    virtual ~TransformDevice();
};
}
#else
#error "TransformDevice class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
