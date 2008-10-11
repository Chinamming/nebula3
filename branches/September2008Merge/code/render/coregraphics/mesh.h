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
#if (__WIN32__ || __XBOX360__)
#include "coregraphics/base/meshbase.h"
namespace CoreGraphics
{
class Mesh : public Base::MeshBase
{
    __DeclareClass(Mesh);
};
}
#elif __WII__
#include "coregraphics/wii/wiimesh.h"
#include "coregraphics/wii/wiistreammeshloader.h"
namespace CoreGraphics
{
class Mesh : public Wii::WiiMesh
{
    __DeclareClass(Mesh);
private:
    friend class Wii::WiiStreamMeshLoader;
};
}
#else
#error "Mesh not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif

