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
namespace CoreGraphics
{
class Mesh : public Base::MeshBase
{
    DeclareClass(Mesh);
};
}
#elif __XBOX360__
#include "coregraphics/base/meshbase.h"
namespace CoreGraphics
{
class Mesh : public Base::MeshBase
{
    DeclareClass(Mesh);
};
}
#elif __WII__
#include "coregraphics/wii/wiimesh.h"
namespace CoreGraphics
{
class Mesh : public Wii::WiiMesh
{
    DeclareClass(Mesh);
};
}
#else
#error "Mesh not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif

