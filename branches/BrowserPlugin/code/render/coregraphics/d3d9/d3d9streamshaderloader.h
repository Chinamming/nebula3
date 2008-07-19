#ifndef DIRECT3D9_D3D9STREAMSHADERLOADER_H
#define DIRECT3D9_D3D9STREAMSHADERLOADER_H
//------------------------------------------------------------------------------
/**
    @class Direct3D9::D3D9StreamShaderLoader
    
    D3D9 implementation of StreamShaderLoader.
    
    (C) 2007 Radon Labs GmbH
*/
#include "resources/resourceloader.h"
#include "io/stream.h"

//------------------------------------------------------------------------------
namespace Direct3D9
{
class D3D9StreamShaderLoader : public Resources::ResourceLoader
{
    DeclareClass(D3D9StreamShaderLoader);
public:
    /// return true if asynchronous loading is supported
    virtual bool CanLoadAsync() const;
    /// called by resource when a load is requested
    virtual bool OnLoadRequested();
    
private:
    /// setup the shader from a Nebula3 stream
    bool SetupShaderFromStream(const Ptr<IO::Stream>& stream);
};

} // namespace Direct3D9
//------------------------------------------------------------------------------
#endif
    