//------------------------------------------------------------------------------
//  preshader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/preshader.h"
#include "coregraphics/shaderinstance.h"

namespace CoreGraphics
{
ImplementClass(CoreGraphics::PreShader, 'PSHD', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
PreShader::PreShader()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
PreShader::~PreShader()
{
    n_assert(!this->shaderInstance.isvalid());
}

//------------------------------------------------------------------------------
/**
*/
void
PreShader::OnAttach(const Ptr<ShaderInstance>& shdInst)
{
    n_assert(shdInst.isvalid());
    n_assert(!this->shaderInstance.isvalid());
    this->shaderInstance = shdInst;
}

//------------------------------------------------------------------------------
/**
*/
void
PreShader::OnDetach()
{
    n_assert(this->shaderInstance.isvalid());
    this->shaderInstance = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
PreShader::OnApply()
{
    // empty, override in subclass
}

} // namespace CoreGraphics