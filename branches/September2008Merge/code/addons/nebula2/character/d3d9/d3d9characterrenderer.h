#pragma once
#ifndef D3D9_D3D9CHARACTERRENDERER_H
#define D3D9_D3D9CHARACTERRENDERER_H
//------------------------------------------------------------------------------
/**
    @class Base::D3D9CharacterRenderer
  
    Writes skinning matrices in matrix shader variable "JointPalette".
    The actual skinning is done in vertex shader on the gpu.

    !!! FIXME FIXME FIXME !!!
    This shouldn't go into CoreGraphics, but instead into some
    future Character subsystem. Also CoreGraphics should never never
    know about the Models namespace!!!
    
    (C) 2008 Radon Labs GmbH
*/
#include "nebula2/character/base/characterrendererbase.h"

//------------------------------------------------------------------------------
namespace Direct3D9
{
class D3D9CharacterRenderer : public Base::CharacterRendererBase
{
    DeclareClass(D3D9CharacterRenderer);
public:
    /// constructor
    D3D9CharacterRenderer();
    /// destructor
    virtual ~D3D9CharacterRenderer();
     
    /// render skinning
    virtual void RenderSkinning(const Util::FixedArray<Char::CharFragment>& fragmentArray);            
        
protected:
    /// render fragment
    void RenderFragment(int primGroupIndex, Char::CharJointPalette& jointPalette);

};

} // namespace Direct3D9
//------------------------------------------------------------------------------
#endif
