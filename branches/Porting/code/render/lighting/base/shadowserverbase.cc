//------------------------------------------------------------------------------
//  shadowserverbase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "lighting/base/shadowserverbase.h"

namespace Lighting
{
ImplementClass(Lighting::ShadowServerBase, 'SDSB', Core::RefCounted);
ImplementSingleton(Lighting::ShadowServerBase);

using namespace Graphics;

//------------------------------------------------------------------------------
/**
*/
ShadowServerBase::ShadowServerBase() :
    isOpen(false),
    inBeginFrame(false),
    inBeginAttach(false)
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
ShadowServerBase::~ShadowServerBase()
{
    n_assert(!this->isOpen);
    n_assert(!this->inBeginFrame);
    n_assert(!this->inBeginAttach);
    DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
ShadowServerBase::Open()
{
    n_assert(!this->isOpen);
    this->isOpen = true;
}

//------------------------------------------------------------------------------
/**
*/
void
ShadowServerBase::Close()
{
    n_assert(this->isOpen);
    n_assert(!this->inBeginFrame);
    n_assert(!this->inBeginAttach);
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void
ShadowServerBase::BeginFrame(const Ptr<CameraEntity>& camEntity)
{
    n_assert(this->isOpen);
    n_assert(!this->inBeginFrame);
    n_assert(!this->inBeginAttach);
    n_assert(this->localLightEntities.IsEmpty());
    n_assert(!this->globalLightEntity.isvalid());
    n_assert(!this->cameraEntity.isvalid());
    n_assert(camEntity.isvalid());
    this->cameraEntity = camEntity;
    this->inBeginFrame = true;
}

//------------------------------------------------------------------------------
/**
*/
void
ShadowServerBase::EndFrame()
{
    n_assert(this->inBeginFrame);
    n_assert(!this->inBeginAttach);
    this->inBeginFrame = false;
    this->localLightEntities.Clear();
    this->globalLightEntity = 0;
    this->cameraEntity = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
ShadowServerBase::BeginAttachVisibleLights()
{
    n_assert(this->inBeginFrame);
    n_assert(!this->inBeginAttach);
    n_assert(this->localLightEntities.IsEmpty());
    n_assert(!this->globalLightEntity.isvalid());
    this->inBeginAttach = true;
    this->globalLightEntity = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
ShadowServerBase::AttachVisibleLight(const Ptr<AbstractLightEntity>& lightEntity)
{
    n_assert(lightEntity->GetCastShadows());
    if (lightEntity->GetLightType() == LightType::Global)
    {
        this->globalLightEntity = lightEntity.downcast<GlobalLightEntity>();
    }
    else
    {
        this->localLightEntities.Append(lightEntity);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
ShadowServerBase::EndAttachVisibleLights()
{
    n_assert(this->inBeginAttach);
    this->inBeginAttach = false;

    // @todo: sort shadow casting light sources by priority
}

//------------------------------------------------------------------------------
/**
    This method updates the shadow buffers (if supported on the
    platform).
*/
void
ShadowServerBase::UpdateShadowBuffers()
{
    // override in subclass
}

} // namespace Lighting
