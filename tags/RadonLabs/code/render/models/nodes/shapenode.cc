//------------------------------------------------------------------------------
//  shapenode.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/nodes/shapenode.h"
#include "models/nodes/shapenodeinstance.h"
#include "models/attributes.h"
#include "resources/resourcemanager.h"
#include "coregraphics/renderdevice.h"

namespace Models
{
ImplementClass(Models::ShapeNode, 'SPND', Models::StateNode);

using namespace Util;
using namespace Attr;
using namespace Resources;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
ShapeNode::ShapeNode() :
    primGroupIndex(InvalidIndex)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ShapeNode::~ShapeNode()
{
    n_assert(!this->managedMesh.isvalid());
}

//------------------------------------------------------------------------------
/**
*/
Ptr<ModelNodeInstance>
ShapeNode::CreateNodeInstance() const
{
    Ptr<ModelNodeInstance> newInst = (ModelNodeInstance*) ShapeNodeInstance::Create();
    return newInst;
}

//------------------------------------------------------------------------------
/**
*/
Resource::State
ShapeNode::GetResourceState() const
{
    Resource::State state = StateNode::GetResourceState();
    if (this->managedMesh->GetState() > state)
    {
        state = this->managedMesh->GetState();
    }
    return state;
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeNode::LoadResources()
{
    n_assert(!this->managedMesh.isvalid());
    n_assert(InvalidIndex == this->primGroupIndex);
    n_assert(this->HasAttr(Attr::MeshResourceId));
    n_assert(this->HasAttr(Attr::MeshGroupIndex));

    // create a managed mesh resource
    ResourceId meshResId = this->GetString(Attr::MeshResourceId);
    this->primGroupIndex = this->GetInt(Attr::MeshGroupIndex);
    this->managedMesh = ResourceManager::Instance()->CreateManagedResource(Mesh::RTTI, meshResId).downcast<ManagedMesh>();
    n_assert(this->managedMesh.isvalid());

    // call parent class
    StateNode::LoadResources();
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeNode::UnloadResources()
{
    n_assert(this->managedMesh.isvalid());
    n_assert(this->primGroupIndex != InvalidIndex);

    // discard managed resource
    ResourceManager::Instance()->DiscardManagedResource(this->managedMesh.upcast<ManagedResource>());
    this->managedMesh = 0;
    this->primGroupIndex = InvalidIndex;

    // call parent class
    StateNode::UnloadResources();
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeNode::ApplySharedState()
{
    n_assert(this->managedMesh.isvalid());
    n_assert(this->primGroupIndex != InvalidIndex);
    
    // first call parent class
    StateNode::ApplySharedState();

    // setup the render device to render our mesh (maybe placeholder mesh
    // if asynchronous resource loading hasn't finished yet)
    const Ptr<Mesh>& mesh = this->managedMesh->GetMesh();    
    if (this->managedMesh->GetState() == Resource::Loaded)
    {
        mesh->ApplyPrimitives(this->primGroupIndex);        
    }
    else
    {
        mesh->ApplyPrimitives(0);        
    }

    // @todo: visible ShapeNodeInstances must provide render feedback (screen size)
    // to our managed mesh!
}

} // namespace Models