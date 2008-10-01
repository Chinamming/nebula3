#pragma once
#ifndef COREGRAPHICS_MESH_H
#define COREGRAPHICS_MESH_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::Mesh
  
    A mesh contains a vertex buffer, an optional index buffer
    and a number of PrimitiveGroup objects. Meshes can be loaded directly 
    from a mesh resource file.
    
    (C) 2007 Radon Labs GmbH
*/    
#if __WIN32__
#include "coregraphics/base/meshbase.h"
#include "coregraphics/d3d9/d3d9streammeshloader.h"
namespace CoreGraphics
{
class Mesh : public Base::MeshBase
{
    DeclareClass(Mesh);
private:
    friend class Direct3D9::D3D9StreamMeshLoader;
};
}
#elif __XBOX360__
#include "coregraphics/base/meshbase.h"
#include "coregraphics/xbox360/xbox360streammeshloader.h"
namespace CoreGraphics
{
class Mesh : public Base::MeshBase
{
    DeclareClass(Mesh);
private:
    friend class Xbox360::Xbox360StreamMeshLoader;
};
}
#elif __WII__
#include "coregraphics/wii/wiimesh.h"
#include "coregraphics/wii/wiistreammeshloader.h"
namespace CoreGraphics
{
class Mesh : public Wii::WiiMesh
{
    DeclareClass(Mesh);
private:
    friend class Wii::WiiStreamMeshLoader;
};
}
#else
#error "Mesh not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif

