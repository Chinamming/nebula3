#pragma once
#ifndef DIRECT3D9_D3D9SHADERSERVER_H
#define DIRECT3D9_D3D9SHADERSERVER_H
//------------------------------------------------------------------------------
/**
    @class Direct3D9::D3D9ShaderServer
    
    D3D9 implementation of ShaderServer.
    
    (C) 2007 Radon Labs GmbH
*/
#include "coregraphics/base/shaderserverbase.h"
#include "coregraphics/shader.h"

//------------------------------------------------------------------------------
namespace Direct3D9
{
class D3D9ShaderServer : public Base::ShaderServerBase
{
    DeclareClass(D3D9ShaderServer);
    DeclareSingleton(D3D9ShaderServer);
public:
    /// constructor
    D3D9ShaderServer();
    /// destructor
    virtual ~D3D9ShaderServer();
    
    /// open the shader server
    bool Open();
    /// close the shader server
    void Close();

    /// return true if a shared variable exists by name
    bool HasSharedVariableByName(const CoreGraphics::ShaderVariable::Name& name) const;
    /// return true if a shared variable exists by semantic
    bool HasSharedVariableBySemantic(const CoreGraphics::ShaderVariable::Semantic& sem) const;
    /// get number of shared variables
    SizeT GetNumSharedVariables() const;
    /// get a shared variable by index
    const Ptr<CoreGraphics::ShaderVariable>& GetSharedVariableByIndex(IndexT i) const;
    /// get a shared variable by name
    const Ptr<CoreGraphics::ShaderVariable>& GetSharedVariableByName(const CoreGraphics::ShaderVariable::Name& name) const;
    /// get a shared variable by semantic
    const Ptr<CoreGraphics::ShaderVariable>& GetSharedVariableBySemantic(const CoreGraphics::ShaderVariable::Semantic& sem) const;

    /// get pointer to global effect pool
    ID3DXEffectPool* GetD3D9EffectPool() const;

private:
    ID3DXEffectPool* d3d9EffectPool;
    Ptr<D3D9ShaderInstance> sharedVariableShaderInst;
};

//------------------------------------------------------------------------------
/**
*/
inline ID3DXEffectPool* 
D3D9ShaderServer::GetD3D9EffectPool() const
{
    n_assert(0 != this->d3d9EffectPool);
    return this->d3d9EffectPool;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
D3D9ShaderServer::HasSharedVariableByName(const CoreGraphics::ShaderVariable::Name& name) const
{
    return this->sharedVariableShaderInst->HasVariableByName(name);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
D3D9ShaderServer::HasSharedVariableBySemantic(const CoreGraphics::ShaderVariable::Semantic& sem) const
{
    return this->sharedVariableShaderInst->HasVariableBySemantic(sem);
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
D3D9ShaderServer::GetNumSharedVariables() const
{
    return this->sharedVariableShaderInst->GetNumVariables();
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::ShaderVariable>&
D3D9ShaderServer::GetSharedVariableByIndex(IndexT i) const
{
    return this->sharedVariableShaderInst->GetVariableByIndex(i);
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::ShaderVariable>&
D3D9ShaderServer::GetSharedVariableByName(const CoreGraphics::ShaderVariable::Name& name) const
{
    return this->sharedVariableShaderInst->GetVariableByName(name);
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::ShaderVariable>&
D3D9ShaderServer::GetSharedVariableBySemantic(const CoreGraphics::ShaderVariable::Semantic& sem) const
{
    return this->sharedVariableShaderInst->GetVariableBySemantic(sem);
}

} // namespace Direct3D9
//------------------------------------------------------------------------------
#endif
    