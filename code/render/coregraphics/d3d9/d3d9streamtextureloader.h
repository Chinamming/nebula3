#ifndef DIRECT3D9_D3D9STREAMTEXTURELOADER_H
#define DIRECT3D9_D3D9STREAMTEXTURELOADER_H
//------------------------------------------------------------------------------
/**
    @class Direct3D9::D3D9StreamTextureLoader
  
    D3D9 implementation of StreamTextureLoader.

    (C) 2007 Radon Labs GmbH
*/    
#include "resources/resourceloader.h"
#include "io/stream.h"
#include "interface/iomsg/readstream.h"

//------------------------------------------------------------------------------
namespace Direct3D9
{
class D3D9StreamTextureLoader : public Resources::ResourceLoader
{
    DeclareClass(D3D9StreamTextureLoader);
public:
    /// return true if asynchronous loading is supported
    virtual bool CanLoadAsync() const;
    /// called by resource when a load is requested
    virtual bool OnLoadRequested();
    /// called by resource to cancel a pending load
    virtual void OnLoadCancelled();
    /// call frequently while after OnLoadRequested() to put Resource into loaded state
    virtual bool OnPending();

private:
    /// setup the texture from a Nebula3 stream
    bool SetupTextureFromStream(const Ptr<IO::Stream>& stream);

    Ptr<Interface::ReadStream> readStreamMsg;
};

} // namespace Direct3D9
//------------------------------------------------------------------------------
#endif
