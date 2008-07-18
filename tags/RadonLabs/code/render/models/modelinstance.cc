//------------------------------------------------------------------------------
//  modelinstance.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/modelinstance.h"
#include "models/modelnodeinstance.h"
#include "graphics/modelentity.h"

namespace Models
{
ImplementClass(Models::ModelInstance, 'MODI', Core::RefCounted);

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
ModelInstance::ModelInstance() :
    time(0.0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ModelInstance::~ModelInstance()
{
    n_assert(!this->IsAttachedToModel());
}

//------------------------------------------------------------------------------
/**
*/
void
ModelInstance::SetModelEntity(const Ptr<Graphics::ModelEntity>& mdlEntity)
{
    this->modelEntity = mdlEntity;
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<Graphics::ModelEntity>&
ModelInstance::GetModelEntity() const
{
    return this->modelEntity;
}

//------------------------------------------------------------------------------
/**
*/
void
ModelInstance::Discard()
{
    n_assert(this->IsAttachedToModel());
    this->model->RemoveInstance(this);
    n_assert(!this->model.isvalid());
}

//------------------------------------------------------------------------------
/**
    Returns true if a ModelInstanceNode exists by name. ModelInstanceNodes
    are associated 1:1 with the ModelNodes of the Model which created
    this instance. Because of this we can just use the Model's 
    node map.
*/
bool
ModelInstance::HasNodeInstance(const Atom<String>& name) const
{
    return this->model->HasNode(name);
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<ModelNodeInstance>&
ModelInstance::LookupNodeInstance(const Atom<String>& name) const
{
    IndexT index = this->model->GetNodeIndexByName(name);
    return this->nodeInstances[index];
}


//------------------------------------------------------------------------------
/**
*/
const Ptr<ModelNodeInstance>& 
ModelInstance::LookupNodeInstanceByRTTI(const Core::Rtti& rtti) const
{
    IndexT i;
    for (i = 0; i < this->nodeInstances.Size(); i++)
    {
        if (this->nodeInstances[i]->IsA(rtti))
        {
            return this->nodeInstances[i];
        }
    }
    static Ptr<ModelNodeInstance> invalidNode;
    return invalidNode;
}

//------------------------------------------------------------------------------
/**
    This method is called when this instance is attached to its Model.
    The most important thing happening at initialization is for each
    ModelNode, a corresponding ModelInstanceNode will be created,
    which holds ModelNode-per-instance data.
*/
void
ModelInstance::OnAttachToModel(const Ptr<Model>& m)
{
    n_assert(!this->IsAttachedToModel());
    this->model = m;

    // create ModelInstanceNodes
    const Array<Ptr<ModelNode> >& modelNodes = this->model->GetNodes();
    SizeT numModelNodes = modelNodes.Size();
    IndexT modelNodeIndex;
    for (modelNodeIndex = 0; modelNodeIndex < numModelNodes; modelNodeIndex++)
    {
        const Ptr<ModelNode>& modelNode = modelNodes[modelNodeIndex];

        // lookup optional parent node instance
        Ptr<ModelNodeInstance> parentNodeInst;
        if (modelNode->HasParent())
        {
            // NOTE: it is guaranteed that the model node array and the
            // model node instance array have the same layout, so we
            // can re-use an index into the model node array into
            // the model node instance array
            const Ptr<ModelNode>& parentNode = modelNode->GetParent();
            IndexT parentIndex = modelNodes.FindIndex(parentNode);
            n_assert(InvalidIndex != parentIndex);
            parentNodeInst = this->nodeInstances[parentIndex];
        };
        Ptr<ModelNodeInstance> nodeInstance = modelNode->CreateNodeInstance();
        nodeInstance->OnAttachToModelInstance(this, modelNode, parentNodeInst);
        this->nodeInstances.Append(nodeInstance);
    }
}

//------------------------------------------------------------------------------
/**
    This method is called when this instance is removed from its Model.
*/
void
ModelInstance::OnRemoveFromModel()
{
    n_assert(this->IsAttachedToModel());
    this->model = 0;
    IndexT i;
    for (i = 0; i < this->nodeInstances.Size(); i++)
    {
        this->nodeInstances[i]->OnRemoveFromModelInstance();
    }
    this->nodeInstances.Clear();
}

//------------------------------------------------------------------------------
/**
    The Update() method is called once per frame on potentially visible
    ModelInstances. The method must be called AFTER transformation, visibility, 
    time or other parameters have been set. The Update() method will
    be propagated to all instance nodes.
*/
void
ModelInstance::Update()
{
    IndexT i;
    for (i = 0; i < this->nodeInstances.Size(); i++)
    {
        this->nodeInstances[i]->Update();
    }
}

//------------------------------------------------------------------------------
/**
    This method is called by the Graphics subsystem when we are currently
    visible. Once all visible model instances are notified, the Graphics
    subsystem can get a render-order-optimized list of all visible
    model-node-instances through the ModelServer.
*/
void
ModelInstance::NotifyVisible(IndexT frameIndex)
{
    // all we need to do is to distribute the method to all our
    // model node instances
    IndexT i;
    SizeT num = this->nodeInstances.Size();
    for (i = 0; i < num; i++)
    {
        this->nodeInstances[i]->OnNotifyVisible(frameIndex);
    }
}

//------------------------------------------------------------------------------
/**
    This method is called from the RenderDebug of Graphics::ModelEntity.
*/
void 
ModelInstance::RenderDebug()
{
    IndexT i;
    SizeT num = this->nodeInstances.Size();
    for (i = 0; i < num; i++)
    {
        this->nodeInstances[i]->RenderDebug();
    }
}

} // namespace Models
