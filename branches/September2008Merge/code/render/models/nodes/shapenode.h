#pragma once
#ifndef MODELS_SHAPENODE_H
#define MODELS_SHAPENODE_H
//------------------------------------------------------------------------------
/**
    @class Models::ShapeNode
  
    ModelNode which describes a simple static shape. Since the ShapeNode
    is derived from the StateNode (which in turn is derived from the
    TransformNode), a ShapeNode contains all the necessary information
    for rendering a mesh.
    
    (C) 2007 Radon Labs GmbH
*/    
#include "models/nodes/statenode.h"
#include "resources/managedmesh.h"

//------------------------------------------------------------------------------
namespace Models
{
class ShapeNode : public StateNode
{
    __DeclareClass(ShapeNode);
public:
    /// constructor
    ShapeNode();
    /// destructor
    virtual ~ShapeNode();

    /// get overall state of contained resources (Initial, Loaded, Pending, Failed, Cancelled)
    virtual Resources::Resource::State GetResourceState() const;
    /// apply state shared by all my ModelNodeInstances
    virtual void ApplySharedState();
	
    /// get managed mesh
    const Ptr<Resources::ManagedMesh>& GetManagedMesh() const;

protected:
    /// create a model node instance
    virtual Ptr<ModelNodeInstance> CreateNodeInstance() const;
    /// called when resources should be loaded
    virtual void LoadResources();
    /// called when resources should be unloaded
    virtual void UnloadResources();

    Ptr<Resources::ManagedMesh> managedMesh;
    IndexT primGroupIndex;
};

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Resources::ManagedMesh>& 
ShapeNode::GetManagedMesh() const
{
    return this->managedMesh;
}
} // namespace Models
//------------------------------------------------------------------------------
#endif
