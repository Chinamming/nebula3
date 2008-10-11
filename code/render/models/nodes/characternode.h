#pragma once
#ifndef MODELS_CHARACTERNODE_H
#define MODELS_CHARACTERNODE_H
//------------------------------------------------------------------------------
/**
    @class Models::CharacterNode
  
    A TransformNode which describes a skinned shape. Since the CharacterNode
    is derived from the TransformNode (which in turn is derived from the
    TransformNode), a CharacterNode contains all the necessary information
    for rendering a skinned mesh.

    It supports the use of variations and skinlists for multi exchangeable 
    animated skins and textures (all the nebula2 character3 functionalities).
            
    (C) 2007 Radon Labs GmbH
*/    
#include "models/nodes/transformnode.h"
#include "addons/nebula2/nebula2wrapper.h"
#include "attr/attribute.h"
#include "math/vector.h"
#include "math/quaternion.h"

namespace Attr
{
    DeclareString(SkinCategory, 'SKCA', ReadWrite);
}

//------------------------------------------------------------------------------
namespace Models
{
class CharacterNode : public TransformNode
{
    __DeclareClass(CharacterNode);
public:
    /// constructor
    CharacterNode();
    /// destructor
    virtual ~CharacterNode();

    /// get overall state of contained resources (Initial, Loaded, Pending, Failed, Cancelled)
    virtual Resources::Resource::State GetResourceState() const;

    /// get number of skin infos
    SizeT GetNumSkins() const;
    /// get skin index by name
    IndexT GetSkinIndexByName(const Util::String& n) const;
    /// get skin info by index
    const Char::SkinInfo& GetSkinInfoAt(IndexT i) const;
    /// get whole skin info array
    const Util::Array<Char::SkinInfo>& GetSkinInfoArray() const;

    /// create a model node instance
    virtual Ptr<ModelNodeInstance> CreateNodeInstance() const;
    /// called when resources should be loaded
    virtual void LoadResources();
    /// called when resources should be unloaded
    virtual void UnloadResources();
    
    /// set name of an anim resource
    void SetAnim(const Util::String& filename);
    /// get name of an anim resource
    const Util::String& GetAnim() const;
        
    /// set variations animation resource
    void SetVariationsUri(const Util::String& uri);
    /// get variations animation resource
    const Util::String& GetVariationsUri() const;
    /// begin defining variations
    void BeginVariations(SizeT num);
    /// set a variation
    void SetVariation(IndexT varIndex, IndexT animGroupIndex, const Util::String& varName);
    /// finish defining variations
    void EndVariations();
    /// get number of variations
    SizeT GetNumVariations() const;
    /// get variation name at index
    const Util::String& GetVariationNameAt(IndexT i) const;
    /// get variation joints at index
    const Util::Array<Char::CharJoint>& GetVariationJointsAt(IndexT i) const;
    /// get variation index by name
    IndexT GetVariationIndexByName(const Util::String& n) const;

    /// begin adding clips
    void BeginClips(int numClips);
    /// add an animation clip
    void SetClip(int clipIndex, int animGroupIndex, const Util::String& clipName);
    /// finish adding clips
    void EndClips();
    /// get number of clips in the animation
    int GetNumClips() const;
    /// get clip at index
    const Anim::AnimClip& GetClipAt(int clipIndex) const;
    /// get clip by name
    IndexT GetClipIndexByName(const Util::String& name) const;
    /// get clip duration
    Timing ::Time GetClipDuration(int index) const;
    /// get character set
    const Ptr<Char::CharacterSet>& GetCharacterSet() const;

    /// begin adding joints
    void BeginJoints(int numJoints);
    /// add a joint to the skeleton
    void SetJoint(int index, int parentJointIndex, const Math::vector& poseTranslate, const Math::quaternion& poseRotate, const Math::vector& poseScale, const Util::String& name);
    /// finish adding joints
    void EndJoints();
    /// get number of joints in skeleton
    int GetNumJoints();
    /// get joint attributes
    void GetJoint(int index, int& parentJointIndex, Math::vector& poseTranslate, Math::quaternion& poseRotate, Math::vector& poseScale, Util::String& name);

    /// get managed animation
    const Ptr<Anim::ManagedAnimation>& GetManagedAnimation() const;
    /// get character
    const Ptr<Char::Character>& GetCharacter() const;
    
protected:
    /// load variation
    bool LoadVariation();
    /// extract skeleton variation from a variation clip
    Util::Array<Char::CharJoint> EvaluateVariation(const Ptr<Anim::Animation>& varAnim, IndexT groupIndex);    
    /// setup skin infos from child objects
    void SetupSkinInfos();

private:        
    Ptr<Char::Character> character;
    Ptr<Char::CharacterSet> characterSet;

    Util::String animName;
    Ptr<Anim::ManagedAnimation> managedAnimation;
    Util::FixedArray<Anim::AnimClip> clips;   ///< array of animation clips
    Util::Dictionary<Util::String,IndexT> clipIndexMap;     ///< map clip names to clip indices

    // FIXME: redundant data, also in characterset, remove variation and skininfo from characternode!
    Util::String variationUri;    
    Util::FixedArray<Char::CharVariation> variations;
    Util::Dictionary<Util::String,IndexT> variationIndexMap;

    Util::Array<Char::SkinInfo> skinInfos;
    Util::Dictionary<Util::String,IndexT> skinIndexMap;
};

//------------------------------------------------------------------------------
/**
*/
inline void
CharacterNode::SetAnim(const Util::String& name)
{    
    this->animName = name;
}

//------------------------------------------------------------------------------
/**    
*/
inline const Util::String&
CharacterNode::GetAnim() const
{
    return this->animName;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
CharacterNode::SetVariationsUri(const Util::String& uri)
{
    this->variationUri = uri;
}

//------------------------------------------------------------------------------
/**  
*/
inline const Util::String& 
CharacterNode::GetVariationsUri() const
{
    return this->variationUri;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
CharacterNode::GetNumVariations() const
{
    return this->variations.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String&
CharacterNode::GetVariationNameAt(IndexT i) const
{
    return this->variations[i].name;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Char::CharJoint>&
CharacterNode::GetVariationJointsAt(IndexT i) const
{
    return this->variations[i].joints;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
CharacterNode::GetVariationIndexByName(const Util::String& name) const
{
    IndexT varIndexMapIndex = this->variationIndexMap.FindIndex(name);
    if (InvalidIndex == varIndexMapIndex)
    {
        return InvalidIndex;
    }
    else
    {
        return this->variationIndexMap.ValueAtIndex(varIndexMapIndex);
    }
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
CharacterNode::GetNumSkins() const
{
    return this->skinInfos.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
CharacterNode::GetSkinIndexByName(const Util::String& n) const
{
    IndexT skinIndexMapIndex = this->skinIndexMap.FindIndex(n);
    if (InvalidIndex == skinIndexMapIndex)
    {
        return InvalidIndex;
    }
    else
    {
        return this->skinIndexMap.ValueAtIndex(skinIndexMapIndex);
    }
}

//------------------------------------------------------------------------------
/**
*/
inline const Char::SkinInfo&
CharacterNode::GetSkinInfoAt(IndexT i) const
{
    return this->skinInfos[i];
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Char::CharacterSet>& 
CharacterNode::GetCharacterSet() const
{
    return this->characterSet;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Char::SkinInfo>& 
CharacterNode::GetSkinInfoArray() const
{
    return this->skinInfos;
}


//------------------------------------------------------------------------------
/**
    Begin configuring the joint skeleton.
*/
inline void
CharacterNode::BeginJoints(int numJoints)
{
    this->character->GetSkeleton().BeginJoints(numJoints);
}

//------------------------------------------------------------------------------
/**
    Add a joint to the joint skeleton.
*/
inline void
CharacterNode::SetJoint(int jointIndex, int parentJointIndex, const Math::vector& poseTranslate, const Math::quaternion& poseRotate, const Math::vector& poseScale, const Util::String& name )
{
    this->character->GetSkeleton().SetJoint(jointIndex, parentJointIndex, poseTranslate, poseRotate, poseScale, name);
}

//------------------------------------------------------------------------------
/**
    Finish adding joints to the joint skeleton.
*/
inline void
CharacterNode::EndJoints()
{
    this->character->GetSkeleton().EndJoints();
}

//------------------------------------------------------------------------------
/**
    Get number of joints in joint skeleton.
*/
inline int
CharacterNode::GetNumJoints()
{
    return this->character->GetSkeleton().GetNumJoints();
}

//------------------------------------------------------------------------------
/**
    Get joint attributes.
*/
inline void
CharacterNode::GetJoint(int index, int& parentJointIndex, Math::vector& poseTranslate, Math::quaternion& poseRotate, Math::vector& poseScale, Util::String& name)
{
    Char::CharJoint& joint = this->character->GetSkeleton().GetJointAt(index);
    parentJointIndex = joint.GetParentJointIndex();
    poseTranslate = joint.GetPoseTranslate();
    poseRotate    = joint.GetPoseRotate();
    poseScale     = joint.GetPoseScale();
    name          = joint.GetName();
}

//------------------------------------------------------------------------------
/**  
*/
inline const Ptr<Anim::ManagedAnimation>& 
CharacterNode::GetManagedAnimation() const
{
    return this->managedAnimation;
}

//------------------------------------------------------------------------------
/**  
*/
inline const Ptr<Char::Character>& 
CharacterNode::GetCharacter() const
{
    return this->character;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
CharacterNode::GetClipIndexByName(const Util::String& name) const
{
    IndexT clipIndexMapIndex = this->clipIndexMap.FindIndex(name);
    if (InvalidIndex == clipIndexMapIndex)
    {
        return InvalidIndex;
    }
    else
    {
        return this->clipIndexMap.ValueAtIndex(clipIndexMapIndex);
    }
}

} // namespace Models
//------------------------------------------------------------------------------
#endif
