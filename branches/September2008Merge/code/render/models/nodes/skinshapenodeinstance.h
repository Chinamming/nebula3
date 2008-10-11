#pragma once
#ifndef MODELS_SKINSHAPENODEINSTANCE_H
#define MODELS_SKINSHAPENODEINSTANCE_H
//------------------------------------------------------------------------------
/**
    @class Models::SkinShapeNodeInstance
  
    The SkinShapeNodeInstance actually renders a skinned shape, and holds
    all the necessary per-instance state to do so.

    It knows the selected visible skins an active textures for
    actual rendering.
    
    (C) 2007 Radon Labs GmbH
*/
#include "models/nodes/shapenodeinstance.h"
#include "addons/nebula2/nebula2wrapper.h"

namespace Char
{    
    class CharacterRenderer;
}
//------------------------------------------------------------------------------
namespace Models
{
class SkinShapeNodeInstance : public ShapeNodeInstance
{
    __DeclareClass(SkinShapeNodeInstance);
public:
    /// constructor
    SkinShapeNodeInstance();
    /// destructor
    virtual ~SkinShapeNodeInstance();
    
    /// check for valid resources
    virtual void Update();
    /// perform rendering
    virtual void Render();
    /// set visible, used by charactersystem
    virtual void SetVisible(bool b);
    
    /// get character ptr
    const Ptr<Char::Character>& GetCharacter() const;

protected:
    /// notify that we are visible
    virtual void OnNotifyVisible(IndexT frameIndex);
    /// called when attached to ModelInstance
    virtual void OnAttachToModelInstance(const Ptr<ModelInstance>& inst, const Ptr<ModelNode>& node, const Ptr<ModelNodeInstance>& parentNodeInst);
    /// called when removed from ModelInstance
    virtual void OnRemoveFromModelInstance();
    /// validate character
    void ValidateCharacter();
    
private:    
    static int curBuffer;
    Ptr<Char::Character> character;
    Ptr<CoreGraphics::ShaderVariable> paletteShdVar;
    Ptr<Char::CharacterRenderer> charRenderer;
};


//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Char::Character>& 
SkinShapeNodeInstance::GetCharacter() const
{
    return this->character;    
}
} // namespace Models
//------------------------------------------------------------------------------
#endif

