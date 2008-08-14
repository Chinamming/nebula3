#pragma once
#ifndef COREGRAPHICS_PRESHADER_H
#define COREGRAPHICS_PRESHADER_H
//------------------------------------------------------------------------------
/**
    @class Base::PreShader
    
    A PreShader is attached to a shader instance and will be called back
    to setup shader variables before the shader is "rendered".
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"

//------------------------------------------------------------------------------
namespace CoreGraphics
{
class ShaderInstance;
    
class PreShader : public Core::RefCounted
{
    DeclareClass(PreShader);
public:
    /// constructor
    PreShader();
    /// destructor
    virtual ~PreShader();
    /// called when attached to shader instance
    virtual void OnAttach(const Ptr<CoreGraphics::ShaderInstance>& shdInst);
    /// called when detached from shader instance
    virtual void OnDetach();
    /// called before rendering the shader
    virtual void OnApply();

protected:
    Ptr<CoreGraphics::ShaderInstance> shaderInstance;
};

} // namespace Base
//------------------------------------------------------------------------------
#endif
    