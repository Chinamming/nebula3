//------------------------------------------------------------------------------
//  d3d9shaderserver.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/d3d9/d3d9shaderserver.h"

namespace Direct3D9
{
ImplementClass(Direct3D9::D3D9ShaderServer, 'D9SS', Base::ShaderServerBase);
ImplementSingleton(Direct3D9::D3D9ShaderServer);

using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
D3D9ShaderServer::D3D9ShaderServer() :
    d3d9EffectPool(0)
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
D3D9ShaderServer::~D3D9ShaderServer()
{
    if (this->IsOpen())
    {
        this->Close();
    }
    DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
bool
D3D9ShaderServer::Open()
{
    n_assert(!this->IsOpen());
    n_assert(0 == this->d3d9EffectPool);

    // create d3d9effect pool object
    HRESULT hr = D3DXCreateEffectPool(&this->d3d9EffectPool);
    n_assert(SUCCEEDED(hr));

    // let parent class load all shaders
    ShaderServerBase::Open();

    // create standard shader for access to shared variables
    this->sharedVariableShaderInst = this->CreateShaderInstance(ResourceId("shd:shared"));
    n_assert(this->sharedVariableShaderInst.isvalid());
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9ShaderServer::Close()
{
    n_assert(this->IsOpen());
    n_assert(this->sharedVariableShaderInst.isvalid());
    
    // release shared instance shader
    this->sharedVariableShaderInst->Discard();
    this->sharedVariableShaderInst = 0;

    // release the d3dx effect pool object
    this->d3d9EffectPool->Release();
    this->d3d9EffectPool = 0;

    ShaderServerBase::Close();
}

} // namespace Direct3D9