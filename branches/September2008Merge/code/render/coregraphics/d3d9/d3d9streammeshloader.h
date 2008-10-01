#pragma once
#ifndef DIRECT3D9_D3D9STREAMMESHLOADER_H
#define DIRECT3D9_D3D9STREAMMESHLOADER_H
//------------------------------------------------------------------------------
/**
    @class Direct3D9::D3D9StreamMeshLoader
    
    Setup a Mesh object from a stream. Supports the following file formats:
    
    - nvx2 (Nebula2 binary mesh file format)
    - nvx3 (Nebula3 binary mesh file format)
    - n3d3 (Nebula3 ascii mesh file format)
    
    @todo: document file formats
    
    (C) 2007 Radon Labs GmbH
*/
#include "resources/streamresourceloader.h"

//------------------------------------------------------------------------------
namespace Direct3D9
{
class D3D9StreamMeshLoader : public Resources::StreamResourceLoader
{
    DeclareClass(D3D9StreamMeshLoader);
private:
    /// setup mesh from generic stream, branches to specialized loader methods
    virtual bool SetupResourceFromStream(const Ptr<IO::Stream>& stream);
    #if NEBULA3_LEGACY_SUPPORT
    /// setup mesh from nvx2 file in memory
    bool SetupMeshFromNvx2(const Ptr<IO::Stream>& stream);
    #endif
    /// setup mesh from nvx3 file in memory
    bool SetupMeshFromNvx3(const Ptr<IO::Stream>& stream);
    /// setup mesh from n3d3 file in memory
    bool SetupMeshFromN3d3(const Ptr<IO::Stream>& stream);
};

} // namespace Direct3D9
//------------------------------------------------------------------------------
#endif
