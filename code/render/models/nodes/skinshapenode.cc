//------------------------------------------------------------------------------
//  shapenode.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/nodes/SkinShapeNode.h"
#include "models/nodes/SkinShapeNodeinstance.h"
#include "models/attributes.h"
#include "resources/resourcemanager.h"
#include "coregraphics/renderdevice.h"
#include "coregraphics/shaderserver.h"
#include "coregraphics/shaderfeature.h"
#include "addons/nebula2/nebula2wrapper.h"

namespace Models
{
ImplementClass(Models::SkinShapeNode, 'SKSN', Models::ShapeNode);

using namespace Util;
using namespace Attr;
using namespace Math;
using namespace Resources;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
SkinShapeNode::SkinShapeNode():
resourceRefCount(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
SkinShapeNode::~SkinShapeNode()
{
}

//------------------------------------------------------------------------------
/**
*/
Ptr<ModelNodeInstance>
SkinShapeNode::CreateNodeInstance() const
{
    Ptr<ModelNodeInstance> newInst = (ModelNodeInstance*) SkinShapeNodeInstance::Create();
    return newInst;
}

//------------------------------------------------------------------------------
/**
    This method is called once before rendering the ModelNode's visible 
    instance nodes through the ModelNodeInstance::ApplyState() and 
    ModelNodeInstance::Render() methods. The method must apply the 
    shader state that is shared across all instances. Since this state is
    constant across all instance nodes, this only happens once before
    rendering an instance set.
*/
void
SkinShapeNode::ApplySharedState()
{     
    // set mesh
    ShapeNode::ApplySharedState();
   
    // set skinned shader feature
    ShaderServer* shdServer = ShaderServer::Instance();
    shdServer->SetFeatureBits(shdServer->FeatureStringToMask("Skinned"));
}

//------------------------------------------------------------------------------
/**
    Begin defining mesh fragment. A skin mesh may be divided into several
    fragments to account for gfx hardware which an only render a skinned
    mesh with a limited number of influence objects (joints).
*/
void
SkinShapeNode::BeginFragments(int num)
{
    n_assert(num > 0);
    this->fragmentArray.SetSize(num);
}

//------------------------------------------------------------------------------
/**
    Set the mesh group index for a skin fragment.
*/
void
SkinShapeNode::SetFragGroupIndex(int fragIndex, int primGroupIndex)
{
    this->fragmentArray[fragIndex].SetMeshGroupIndex(primGroupIndex);
}

//------------------------------------------------------------------------------
/**
    Get the mesh group index for a skin fragment.
*/
int
SkinShapeNode::GetFragGroupIndex(int fragIndex) const
{
    return this->fragmentArray[fragIndex].GetMeshGroupIndex();
}

//------------------------------------------------------------------------------
/**
    Begin defining the joint palette of a skin fragment.
*/
void
SkinShapeNode::BeginJointPalette(int fragIndex, int numJoints)
{
    this->fragmentArray[fragIndex].GetJointPalette().BeginJoints(numJoints);
}

//------------------------------------------------------------------------------
/**
    Add up to 8 joints to a fragments joint palette starting at a given
    palette index.
*/
void
SkinShapeNode::SetJointIndices(int fragIndex, int paletteIndex, int ji0, int ji1, int ji2, int ji3, int ji4, int ji5, int ji6, int ji7)
{
    Char::CharJointPalette& pal = this->fragmentArray[fragIndex].GetJointPalette();
    int numJoints = pal.GetNumJoints();
    if (paletteIndex < numJoints) pal.SetJointIndex(paletteIndex++, ji0);
    if (paletteIndex < numJoints) pal.SetJointIndex(paletteIndex++, ji1);
    if (paletteIndex < numJoints) pal.SetJointIndex(paletteIndex++, ji2);
    if (paletteIndex < numJoints) pal.SetJointIndex(paletteIndex++, ji3);
    if (paletteIndex < numJoints) pal.SetJointIndex(paletteIndex++, ji4);
    if (paletteIndex < numJoints) pal.SetJointIndex(paletteIndex++, ji5);
    if (paletteIndex < numJoints) pal.SetJointIndex(paletteIndex++, ji6);
    if (paletteIndex < numJoints) pal.SetJointIndex(paletteIndex++, ji7);
}

//------------------------------------------------------------------------------
/**
*/
void
SkinShapeNode::SetJointIndex(int fragIndex, int paletteIndex, int jointIndex)
{
    Char::CharJointPalette& pal = this->fragmentArray[fragIndex].GetJointPalette();
    pal.SetJointIndex(paletteIndex, jointIndex);
}

//------------------------------------------------------------------------------
/**
    Finish defining the joint palette of a skin fragment.
*/
void
SkinShapeNode::EndJointPalette(int fragIndex)
{
    this->fragmentArray[fragIndex].GetJointPalette().EndJoints();
}

//------------------------------------------------------------------------------
/**
    Finish defining fragments.
*/
void
SkinShapeNode::EndFragments()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Do not load resources by default.
*/
void
SkinShapeNode::LoadResources()
{
    // nothing done
    ShapeNode::LoadResources();
}

//------------------------------------------------------------------------------
/**    
    Is call by instances which are getting visible.
*/
void
SkinShapeNode::RequestLoadResources()
{
    if (this->GetResourceState() == Resources::Resource::Initial)
    {
        ShapeNode::LoadResources();
    }
    this->resourceRefCount++;
}

//------------------------------------------------------------------------------
/**    
    Request an unload of our resources, if no instance node references
    out resource, we try to unload it
*/
void
SkinShapeNode::RequestUnloadResources()
{
    if (this->resourceRefCount > 0)
    {
        this->resourceRefCount--;
        if (this->resourceRefCount == 0)
        {
            this->UnloadResources();
        }
    }
}

} // namespace Models