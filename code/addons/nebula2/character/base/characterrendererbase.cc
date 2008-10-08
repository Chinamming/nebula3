//------------------------------------------------------------------------------
//  characterrendererbase.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "nebula2/character/base/characterrendererbase.h"

namespace Base
{
__ImplementClass(Base::CharacterRendererBase, 'CHRB', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
CharacterRendererBase::CharacterRendererBase()
{
}

//------------------------------------------------------------------------------
/**
*/
CharacterRendererBase::~CharacterRendererBase()
{
}

//------------------------------------------------------------------------------
/**
*/
void 
CharacterRendererBase::Setup(const Ptr<CoreGraphics::Mesh>& mesh, const Ptr<Char::Character>& character, const Ptr<CoreGraphics::ShaderVariable>& shdVar)
{
    n_assert(mesh.isvalid());
    n_assert(character.isvalid());    
    
    this->skinningMesh = mesh;
    this->skinningCharacter = character;
    this->paletteShdVar = shdVar;
}
    
//------------------------------------------------------------------------------
/**
*/
void 
CharacterRendererBase::RenderSkinning(const Util::FixedArray<Char::CharFragment>& fragmentArray)
{
    n_assert(this->skinningCharacter.isvalid());    
}

} // namespace Base
