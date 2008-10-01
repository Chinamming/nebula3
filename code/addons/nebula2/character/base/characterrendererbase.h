#pragma once
#ifndef BASE_CHARACTERRENDERERBASE_H
#define BASE_CHARACTERRENDERERBASE_H
//------------------------------------------------------------------------------
/**
    @class Base::CharacterRendererBase
  
    !!! FIXME FIXME FIXME !!!
    This shouldn't go into CoreGraphics, but instead into some
    future Character subsystem. Also CoreGraphics should never never
    know about the Models namespace!!!
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "nebula2/nebula2wrapper.h"
#include "coregraphics/shadervariable.h"
#include "coregraphics/mesh.h"

//------------------------------------------------------------------------------
namespace Base
{
class CharacterRendererBase : public Core::RefCounted
{
    DeclareClass(CharacterRendererBase);
public:
    /// constructor
    CharacterRendererBase();
    /// destructor
    virtual ~CharacterRendererBase();    
    
    /// init with character and src mesh
    virtual void Setup(const Ptr<CoreGraphics::Mesh>& mesh, const Ptr<Char::Character>& character, const Ptr<CoreGraphics::ShaderVariable>& shdVar);    
    /// render skinning
    virtual void RenderSkinning(const Util::FixedArray<Char::CharFragment>& fragmentArray);
        
protected:   
    Ptr<CoreGraphics::Mesh> skinningMesh;
    Ptr<Char::Character> skinningCharacter;
    Ptr<CoreGraphics::ShaderVariable> paletteShdVar;
};

} // namespace Base
//------------------------------------------------------------------------------
#endif
