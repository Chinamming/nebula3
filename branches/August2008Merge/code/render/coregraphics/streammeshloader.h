#pragma once
#ifndef COREGRAPHICS_STREAMMESHLOADER_H
#define COREGRAPHICS_STREAMMESHLOADER_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::StreamMeshLoader
    
    Setup a Mesh object from a stream. Supports the following file formats:
    
    - nvx2 (Nebula2 binary mesh file format)
    - nvx3 (Nebula3 binary mesh file format)
    - n3d3 (Nebula3 ascii mesh file format)
    
    @todo: document file formats
    
    (C) 2007 Radon Labs GmbH
*/
#include "resources/resourceloader.h"
#include "io/stream.h"
#include "interface/iomsg/readstream.h"

//------------------------------------------------------------------------------
namespace CoreGraphics
{
class StreamMeshLoader : public Resources::ResourceLoader
{
    DeclareClass(StreamMeshLoader);
public:
    /// constructor
    StreamMeshLoader();
    /// destructor
    virtual ~StreamMeshLoader();

    /// return true if asynchronous loading is supported
    virtual bool CanLoadAsync() const;
    /// called by resource when a load is requested
    virtual bool OnLoadRequested();
    /// called by resource to cancel a pending load
    virtual void OnLoadCancelled();
    /// call frequently while after OnLoadRequested() to put Resource into loaded state
    virtual bool OnPending();

private:
    /// setup mesh from generic stream, branches to specialized loader methods
    bool SetupMeshFromStream(const Ptr<IO::Stream>& stream);
    #if NEBULA3_LEGACY_SUPPORT
    /// setup mesh from nvx2 file in memory
    bool SetupMeshFromNvx2(const Ptr<IO::Stream>& stream);
    #endif
    /// setup mesh from nvx3 file in memory
    bool SetupMeshFromNvx3(const Ptr<IO::Stream>& stream);
    /// setup mesh from n3d3 file in memory
    bool SetupMeshFromN3d3(const Ptr<IO::Stream>& stream);

    Ptr<Interface::ReadStream> readStreamMsg;
};

} // namespace CoreGraphics
//------------------------------------------------------------------------------
#endif
