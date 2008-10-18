#pragma once
#ifndef MODELS_CHARACTERNODEINSTANCE_H
#define MODELS_CHARACTERNODEINSTANCE_H
//------------------------------------------------------------------------------
/**
    @class Models::CharacterNodeInstance
  
    The CharacterNodeInstance actually renders a skinned shape, and holds
    all the necessary per-instance state to do so.

    It knows the selected visible skins an active textures for
    actual rendering.
    
    (C) 2007 Radon Labs GmbH
*/
#include "models/nodes/shapenodeinstance.h"
#include "addons/nebula2/nebula2wrapper.h"

//------------------------------------------------------------------------------
namespace Models
{
class CharacterNodeInstance : public TransformNodeInstance
{
    __DeclareClass(CharacterNodeInstance);
public:
    /// constructor
    CharacterNodeInstance();
    /// destructor
    virtual ~CharacterNodeInstance();
    
    /// perform per-frame updates
    virtual void Update();    

    /// get character
    const Ptr<Char::Character>& GetCharacter() const;
    /// get character set
    const Ptr<Char::CharacterSet>& GetCharacterSet() const;

protected:
    /// called when attached to ModelInstance
    virtual void OnAttachToModelInstance(const Ptr<ModelInstance>& inst, const Ptr<ModelNode>& node, const Ptr<ModelNodeInstance>& parentNodeInst);
    /// called when removed from ModelInstance
    virtual void OnRemoveFromModelInstance();
    /// render node specific debug shape
    virtual void RenderDebug();
    /// validate character 
    void ValidateCharacter();
    
private:    
    Ptr<Char::Character> character;
    Ptr<Char::CharacterSet> characterSet;
    Util::Array<Char::SkinInfo> skinInfos;
};

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Char::Character>& 
CharacterNodeInstance::GetCharacter() const
{
    return this->character;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Char::CharacterSet>& 
CharacterNodeInstance::GetCharacterSet() const
{
    return this->characterSet;
}
} // namespace Models
//------------------------------------------------------------------------------
#endif

