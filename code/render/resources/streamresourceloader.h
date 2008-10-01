#pragma once
#ifndef RESOURCES_STREAMRESOURCELOADER_H
#define RESOURCES_STREAMRESOURCELOADER_H
//------------------------------------------------------------------------------
/**
    @class Resources::StreamResourceLoader
    
    Base class for resource loaders which load their resource from an
    IO::Stream. Handles the details of synchronous vs. asynchronous
    resource loading. Subclasses only need to override the 
    SetupResourceFromStream() virtual method.
    
    (C) 2008 Radon Labs GmbH
*/
#include "resources/resourceloader.h"
#include "io/stream.h"
#include "interface/interfaceprotocol.h"

//------------------------------------------------------------------------------
namespace Resources
{
class StreamResourceLoader : public ResourceLoader
{
    DeclareClass(StreamResourceLoader);
public:
    /// constructor
    StreamResourceLoader();
    /// destructor
    virtual ~StreamResourceLoader();

    /// override in subclass: return true if asynchronous loading is supported (default is true)
    virtual bool CanLoadAsync() const;
    /// called by resource when a load is requested
    virtual bool OnLoadRequested();
    /// called by resource to cancel a pending load
    virtual void OnLoadCancelled();
    /// call frequently while after OnLoadRequested() to put Resource into loaded state
    virtual bool OnPending();

protected:
    /// override in subclass: setup the resource from the provided stream
    virtual bool SetupResourceFromStream(const Ptr<IO::Stream>& stream);

    Ptr<Interface::ReadStream> readStreamMsg;
};

} // namespace Resources
//------------------------------------------------------------------------------
#endif
    