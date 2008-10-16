#pragma once
#ifndef MODELS_STREAMMODELLOADER_H
#define MODELS_STREAMMODELLOADER_H
//------------------------------------------------------------------------------
/**
    @class Models::StreamModelLoader
    
    A ResourceLoader class for setting up Models from Streams. Supports
    Nebula3 binary and XML formats, and the legacy Nebula3 nvx2 format.
    Relies on StreamReader classes which implement the actual stream
    parsing.
    
    (C) 2007 Radon Labs GmbH
*/
#include "resources/resourceloader.h"
#include "io/stream.h"
#include "interface/interfaceprotocol.h"

//------------------------------------------------------------------------------
namespace Models
{
class StreamModelLoader : public Resources::ResourceLoader
{
    __DeclareClass(StreamModelLoader);
public:
    /// constructor
    StreamModelLoader();
    /// destructor
    virtual ~StreamModelLoader();
    
    /// return true if asynchronous loading is supported
    virtual bool CanLoadAsync() const;
    /// called by resource when a load is requested
    virtual bool OnLoadRequested();
    /// called by resource to cancel a pending load
    virtual void OnLoadCancelled();
    /// call frequently while after OnLoadRequested() to put Resource into loaded state
    virtual bool OnPending();

private:
    /// setup Model resource from stream
    bool SetupModelFromStream(const Ptr<IO::Stream>& stream);

    Ptr<Interface::ReadStream> readStreamMsg;
};

} // namespace Models
//------------------------------------------------------------------------------
#endif
    