#pragma once
#ifndef MODELS_SKINSHAPENODE_H
#define MODELS_SKINSHAPENODE_H
//------------------------------------------------------------------------------
/**
    @class Models::SkinShapeNode
  
    A ModelNode which describes a skinned shape. Since the SkinShapeNode
    is derived from the ShapeNode (which in turn is derived from the
    TransformNode), a SkinShapeNode contains all the necessary information
    for rendering a skinned mesh.

    It supports the use of variations and skinlists for multi exchangeable 
    animated skins and textures (all the nebula2 character3 functionalities).
            
    (C) 2007 Radon Labs GmbH
*/    
#include "models/nodes/shapenode.h"
#include "addons/nebula2/nebula2wrapper.h"
#include "attr/attribute.h"

//------------------------------------------------------------------------------
namespace Models
{
class SkinShapeNode : public ShapeNode
{
    __DeclareClass(SkinShapeNode);
public:
    /// constructor
    SkinShapeNode();
    /// destructor
    virtual ~SkinShapeNode();

    /// create a model node instance
    virtual Ptr<ModelNodeInstance> CreateNodeInstance() const;    
    /// apply state shared by all my SkinShapeNodeInstances
    virtual void ApplySharedState();

    /// begin defining mesh fragments
    void BeginFragments(int num);
    /// set mesh group index of a skin fragment
    void SetFragGroupIndex(int fragIndex, int primGroupIndex);
    /// get mesh group index of a skin fragment
    int GetFragGroupIndex(int fragIndex) const;
    /// begin defining the joint palette of a fragment
    void BeginJointPalette(int fragIndex, int numJoints);
    /// add up to 8 joint indices to the fragments joint palette
    void SetJointIndices(int fragIndex, int paletteIndex, int ji0, int ji1, int ji2, int ji3, int ji4, int ji5, int ji6, int ji7);
    /// add a single joint index to the fragments joint palette
    void SetJointIndex(int fragIndex, int paletteIndex, int jointIndex);
    /// finish adding joints to the joint palette
    void EndJointPalette(int fragIndex);
    /// finish defining fragments
    void EndFragments();
    /// get number of fragments
    int GetNumFragments() const;
    /// get fragment array
    const Util::FixedArray<Char::CharFragment>& GetFragmentArray() const;
    /// get number of joints in a fragment's joint palette
    int GetJointPaletteSize(int fragIndex) const;
    /// get a joint index from a fragment's joint palette
    int GetJointIndex(int fragIndex, int paletteIndex) const;
    /// request load resources, if not loaded yet
    void RequestLoadResources();
    /// request to unload our resources
    void RequestUnloadResources();
   
protected:   
    /// called when resources should be loaded
    virtual void LoadResources();    

private:    
    Util::FixedArray<Char::CharFragment> fragmentArray;
    int resourceRefCount;
};

//------------------------------------------------------------------------------
/**
    Get number of fragments.
*/
inline int
SkinShapeNode::GetNumFragments() const
{
    return this->fragmentArray.Size();
}

//------------------------------------------------------------------------------
/**
    Get fragments.
*/
inline const Util::FixedArray<Char::CharFragment>& 
SkinShapeNode::GetFragmentArray() const
{
    return this->fragmentArray;
}

//------------------------------------------------------------------------------
/**
    Get joint palette size of a skin fragment.
*/
inline int
SkinShapeNode::GetJointPaletteSize(int fragIndex) const
{
    return this->fragmentArray[fragIndex].GetJointPalette().GetNumJoints();
}

//------------------------------------------------------------------------------
/**
    Get a joint index from a fragment's joint index.
*/
inline int
SkinShapeNode::GetJointIndex(int fragIndex, int paletteIndex) const
{
    return this->fragmentArray[fragIndex].GetJointPalette().GetJointIndexAt(paletteIndex);
}

} // namespace Models
//------------------------------------------------------------------------------
#endif
