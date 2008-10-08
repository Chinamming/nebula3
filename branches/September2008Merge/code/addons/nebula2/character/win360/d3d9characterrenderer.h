#pragma once
#ifndef WIN360_D3D9CHARACTERRENDERER_H
#define WIN360_D3D9CHARACTERRENDERER_H
//------------------------------------------------------------------------------
/**
    @class Win360::D3D9CharacterRenderer
  
    Writes skinning matrices in matrix shader variable "JointPalette".
    The actual skinning is done in vertex shader on the gpu.
    
    (C) 2008 Radon Labs GmbH
*/
#include "nebula2/character/base/characterrendererbase.h"

//------------------------------------------------------------------------------
namespace Win360
{
class D3D9CharacterRenderer : public Base::CharacterRendererBase
{
    __DeclareClass(D3D9CharacterRenderer);
public:
    /// render skinning
    virtual void RenderSkinning(const Util::FixedArray<Char::CharFragment>& fragmentArray);            
        
protected:
    /// render fragment
    void RenderFragment(int primGroupIndex, Char::CharJointPalette& jointPalette);
};

} // namespace Win360
//------------------------------------------------------------------------------
#endif
