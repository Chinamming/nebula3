//------------------------------------------------------------------------------
//  D3D9StreamMeshLoader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/d3d9/d3d9streammeshloader.h"
#include "coregraphics/mesh.h"
#include "coregraphics/legacy/nvx2streamreader.h"

namespace Direct3D9
{
ImplementClass(Direct3D9::D3D9StreamMeshLoader, 'D9ML', Resources::StreamResourceLoader);

using namespace IO;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
bool
D3D9StreamMeshLoader::SetupResourceFromStream(const Ptr<Stream>& stream)
{
    n_assert(stream.isvalid());
    n_assert(this->resource.isvalid());
    #if NEBULA3_LEGACY_SUPPORT
    if (this->resource->GetResourceId().Value().GetFileExtension() == "nvx2")
    {
        return this->SetupMeshFromNvx2(stream);
    }
    else 
    #endif
    if (this->resource->GetResourceId().Value().GetFileExtension() == "nvx3")
    {
        return this->SetupMeshFromNvx3(stream);
    }
    else if (this->resource->GetResourceId().Value().GetFileExtension() == "n3d3")
    {
        return this->SetupMeshFromN3d3(stream);
    }
    else
    {
        n_error("D3D9StreamMeshLoader::SetupMeshFromStream(): unrecognized file extension in '%s'\n", this->resource->GetResourceId().Value().AsCharPtr());
        return false;
    }
}

//------------------------------------------------------------------------------
/**
    Setup the mesh resource from legacy nvx2 file (Nebula2 binary mesh format).
*/
#if NEBULA3_LEGACY_SUPPORT
bool
D3D9StreamMeshLoader::SetupMeshFromNvx2(const Ptr<Stream>& stream)
{
    n_assert(stream.isvalid());    
    Ptr<Legacy::Nvx2StreamReader> nvx2Reader = Legacy::Nvx2StreamReader::Create();
    nvx2Reader->SetStream(stream);
    if (nvx2Reader->Open())
    {
        const Ptr<Mesh>& res = this->resource.downcast<Mesh>();
        n_assert(!res->IsLoaded());
        res->SetVertexBuffer(nvx2Reader->GetVertexBuffer().downcast<CoreGraphics::VertexBuffer>());
        res->SetIndexBuffer(nvx2Reader->GetIndexBuffer().downcast<CoreGraphics::IndexBuffer>());
        res->SetPrimitiveGroups(nvx2Reader->GetPrimitiveGroups());
        nvx2Reader->Close();
        return true;
    }
    return false;
}
#endif

//------------------------------------------------------------------------------
/**
    Setup the mesh resource from a nvx3 file (Nebula3's
    native binary mesh file format).
*/
bool
D3D9StreamMeshLoader::SetupMeshFromNvx3(const Ptr<Stream>& stream)
{
    // FIXME!
    n_error("D3D9StreamMeshLoader::SetupMeshFromNvx3() not yet implemented");
    return false;
}

//------------------------------------------------------------------------------
/**
    Setup the mesh resource from a n3d3 file (Nebula3's
    native ascii mesh file format).
*/
bool
D3D9StreamMeshLoader::SetupMeshFromN3d3(const Ptr<Stream>& stream)
{
    // FIXME!
    n_error("D3D9StreamMeshLoader::SetupMeshFromN3d3() not yet implemented");
    return false;
}

} // namespace Direct3D9
