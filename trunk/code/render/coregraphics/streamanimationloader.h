#pragma once
#ifndef COREGRAPHICS_STREAMANIMATIONLOADER_H
#define COREGRAPHICS_STREAMANIMATIONLOADER_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::StreamAnimationLoader
    
    Setup a animation object from a stream. Supports the following file formats:
    
    - nax2 (Nebula2 binary animation file format)
    - nax3 (Nebula3 binary animation file format)
    - nanim3 (Nebula3 ascii animation file format)
    
    @todo: document file formats
    
    (C) 2007 Radon Labs GmbH
*/
#include "resources/resourceloader.h"
#include "io/stream.h"
#include "interface/iomsg/readstream.h"

//------------------------------------------------------------------------------
namespace CoreGraphics
{
class StreamAnimationLoader : public Resources::ResourceLoader
{
    DeclareClass(StreamAnimationLoader);
public:
    /// constructor
    StreamAnimationLoader();
    /// destructor
    virtual ~StreamAnimationLoader();

    /// return true if asynchronous loading is supported
    virtual bool CanLoadAsync() const;
    /// called by resource when a load is requested
    virtual bool OnLoadRequested();
    /// called by resource to cancel a pending load
    virtual void OnLoadCancelled();
    /// call frequently while after OnLoadRequested() to put Resource into loaded state
    virtual bool OnPending();

private:
    /// setup animation from generic stream, branches to specialized loader methods
    bool SetupAnimationFromStream(const Ptr<IO::Stream>& stream);
    #if NEBULA3_LEGACY_SUPPORT
    /// setup animation from nax2 file in memory
    bool SetupAnimationFromNax2(const Ptr<IO::Stream>& stream);
    #endif
    /// setup animation from nvx3 file in memory
    bool SetupAnimationFromNax3(const Ptr<IO::Stream>& stream);
    /// setup animation from n3d3 file in memory
    bool SetupAnimationFromNanim3(const Ptr<IO::Stream>& stream);

    Ptr<Interface::ReadStream> readStreamMsg;
};

} // namespace CoreGraphics
//------------------------------------------------------------------------------
#endif
