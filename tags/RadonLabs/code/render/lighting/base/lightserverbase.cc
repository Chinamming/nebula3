//------------------------------------------------------------------------------
//  lightserverbase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "lighting/base/lightserverbase.h"

namespace Lighting
{
ImplementClass(Lighting::LightServerBase, 'LISB', Core::RefCounted);
ImplementSingleton(Lighting::LightServerBase);

using namespace Graphics;

//------------------------------------------------------------------------------
/**
*/
LightServerBase::LightServerBase() :
    isOpen(false),
    inBeginFrame(false),
    inBeginAttach(false)
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
LightServerBase::~LightServerBase()
{
    n_assert(!this->isOpen);
    n_assert(!this->inBeginFrame);
    n_assert(!this->inBeginAttach);
    n_assert(!this->cameraEntity.isvalid());
    n_assert(this->visibleLightEntities.IsEmpty());
    DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
LightServerBase::Open()
{
    n_assert(!this->isOpen);
    this->isOpen = true;
}

//------------------------------------------------------------------------------
/**
*/
void
LightServerBase::Close()
{
    n_assert(this->isOpen);
    n_assert(!this->inBeginFrame);
    n_assert(!this->inBeginAttach);
    n_assert(!this->cameraEntity.isvalid());
    n_assert(!this->globalLightEntity.isvalid());
    n_assert(this->visibleLightEntities.IsEmpty());
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void
LightServerBase::BeginFrame(const Ptr<CameraEntity>& camEntity)
{
    n_assert(this->isOpen);
    n_assert(!this->inBeginFrame);
    n_assert(!this->inBeginAttach);
    n_assert(!this->cameraEntity.isvalid());
    n_assert(camEntity.isvalid());
    this->cameraEntity = camEntity;
    this->inBeginFrame = true;
}

//------------------------------------------------------------------------------
/**
*/
void
LightServerBase::EndFrame()
{
    n_assert(this->isOpen);
    n_assert(this->inBeginFrame);
    n_assert(!this->inBeginAttach);
    this->visibleLightEntities.Clear();
    this->cameraEntity = 0;
    this->globalLightEntity = 0;
    this->inBeginFrame = false;
}

//------------------------------------------------------------------------------
/**
*/
void
LightServerBase::BeginAttachVisibleLights()
{
    n_assert(this->inBeginFrame);
    n_assert(!this->inBeginAttach);
    n_assert(this->visibleLightEntities.IsEmpty());
    n_assert(this->cameraEntity.isvalid());
    this->inBeginAttach = true;
    this->globalLightEntity = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
LightServerBase::AttachVisibleLight(const Ptr<AbstractLightEntity>& lightEntity)
{
    n_assert(this->inBeginAttach);
    if (lightEntity->GetLightType() == LightType::Global)
    {
        this->globalLightEntity = lightEntity.downcast<GlobalLightEntity>();
    }
    else
    {
        this->visibleLightEntities.Append(lightEntity);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
LightServerBase::EndAttachVisibleLights()
{
    n_assert(this->inBeginAttach);
    this->inBeginAttach = false;

    // @todo: sort light source by importance
}

//------------------------------------------------------------------------------
/**
    This method is called during rendering to apply lighting parameters 
    to the provided ModelEntity.
*/
void
LightServerBase::ApplyModelEntityLights(const Ptr<ModelEntity>& modelEntity)
{
    // empty, override in subclass
}

} // namespace Lighting
