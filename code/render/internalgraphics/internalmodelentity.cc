//------------------------------------------------------------------------------
//  internalmodelentity.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "internalgraphics/internalmodelentity.h"
#include "models/modelserver.h"
#include "coregraphics/shaperenderer.h"
#include "threading/thread.h"

namespace InternalGraphics
{
__ImplementClass(InternalGraphics::InternalModelEntity, 'IMDE', InternalGraphics::InternalGraphicsEntity);

using namespace Threading;
using namespace CoreGraphics;
using namespace Math;
using namespace Models;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
InternalModelEntity::InternalModelEntity():
    setModelNodesVisible(true)
{
    this->SetType(InternalGraphicsEntityType::Model);
}

//------------------------------------------------------------------------------
/**
*/
InternalModelEntity::~InternalModelEntity()
{
    n_assert(!this->modelInstance.isvalid());
}

//------------------------------------------------------------------------------
/**
    This will initialize the managed model inside the ModelEntity.
*/
void
InternalModelEntity::OnActivate()
{
    n_assert(!this->IsActive());
    n_assert(this->resId.IsValid());
    InternalGraphicsEntity::OnActivate();
    
    // note: we will remain invalid until at least our model has loaded
    this->SetValid(false);
    this->managedModel = ModelServer::Instance()->LoadManagedModel(this->resId);
    this->ValidateModelInstance();
}

//------------------------------------------------------------------------------
/**
    Cleanup our managed model, and ModelInstance if it is already 
    initialized.
*/
void
InternalModelEntity::OnDeactivate()
{
    n_assert(this->IsActive());
    n_assert(this->managedModel.isvalid());

    // discard our model instance (if exists)
    if (this->modelInstance.isvalid())
    {
        this->modelInstance->Discard();
        this->modelInstance = 0;
    }

    // discard our managed model
    ModelServer::Instance()->DiscardManagedModel(this->managedModel);
    this->managedModel = 0;

    // up to parent class
    InternalGraphicsEntity::OnDeactivate();
}

//------------------------------------------------------------------------------
/**
    In OnTransformChanged() we need to update the transformation
    of our ModelInstance (if it has already been initialised).
*/
void
InternalModelEntity::OnTransformChanged()
{
    if (this->modelInstance.isvalid())
    {
        this->modelInstance->SetTransform(this->GetTransform());
    }
}

//------------------------------------------------------------------------------
/**
    In OnUpdate() the ModelEntity first needs to check whether the
    ManagedModel has already been loaded, and if yes, a ModelInstance
    will be created, finally, the ModelInstance will be prepared for
    rendering.
*/
void
InternalModelEntity::OnUpdate()
{
    // first check if our model has been loaded already, if yes
    // our model instance must be initialized
    if (!this->IsValid())
    {
        this->ValidateModelInstance();
    }

    // if our model instance is valid, let it update itself
    if (this->modelInstance.isvalid())
    {
        this->modelInstance->SetTime(this->GetTime());
        this->modelInstance->Update();
    }

    // important: call parent class!
    InternalGraphicsEntity::OnUpdate();
}

//------------------------------------------------------------------------------
/**
    This creates and initializes our ModelInstance if needed. Since Model 
    loading happens asynchronously this may happen at any time after
    the ModelEntity is activated.
*/
void
InternalModelEntity::ValidateModelInstance()
{
    n_assert(!this->IsValid());
    if (!this->modelInstance.isvalid())
    {
        if (this->managedModel->GetState() == Resource::Loaded)
        {
            const Ptr<Model> model = this->managedModel->GetModel();
            n_assert(model->IsLoaded());
            
            // only set the entity to valid if the complete 
            // model node is completely loaded (no placeholders)
            // FIXME: this should be better adjustable...
            // e.g. meshes must be loaded, but textures may
            // still be loading?
            if (Resource::Loaded == model->GetResourceState())
            {
                this->SetLocalBoundingBox(model->GetBoundingBox());
                this->modelInstance = model->CreateInstance();
                this->modelInstance->SetTransform(this->GetTransform());
                this->modelInstance->SetModelEntity(this);
                this->modelInstance->SetAllNodeInstancesVisible(this->setModelNodesVisible);
                this->SetValid(true);
            }
        }
        else
        {
            // @todo: check for load failed!
        }
    }
}

//------------------------------------------------------------------------------
/**
    Render our debug visualization (the bounding box).
*/
void
InternalModelEntity::OnRenderDebug()
{
    float4 color(1.0f, 0.0f, 0.0f, 0.5f);
    ShapeRenderer::Instance()->AddShape(Shape(Thread::GetMyThreadId(), Shape::Box, this->globalBox.to_matrix44(), color));
    this->GetModelInstance()->RenderDebug();
}

} // namespace InternalGraphics
