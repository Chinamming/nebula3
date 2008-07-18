#pragma once
#ifndef LIGHTING_LIGHTSERVERBASE_H
#define LIGHTING_LIGHTSERVERBASE_H
//------------------------------------------------------------------------------
/**
    @class Lighting::LightServerBase
    
    Base class for the light server. The light server collects all lights
    contributing to the scene. Subclasses of LightServerBase implement 
    specific lighting techniques.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "graphics/cameraentity.h"
#include "graphics/modelentity.h"
#include "lighting/globallightentity.h"

//------------------------------------------------------------------------------
namespace Lighting
{
class LightServerBase : public Core::RefCounted
{
    DeclareClass(LightServerBase);
    DeclareSingleton(LightServerBase);
public:
    /// constructor
    LightServerBase();
    /// destructor
    virtual ~LightServerBase();
    
    /// open the light server
    void Open();
    /// close the light server
    void Close();
    /// return true if light server is open
    bool IsOpen() const;

    /// begin lighting frame
    void BeginFrame(const Ptr<Graphics::CameraEntity>& cameraEntity);
    /// begin attaching visible light sources
    void BeginAttachVisibleLights();
    /// attach a visible light source
    void AttachVisibleLight(const Ptr<AbstractLightEntity>& lightEntity);
    /// end attaching visible light sources
    void EndAttachVisibleLights();
    /// apply lighting parameters for a visible model entity 
    void ApplyModelEntityLights(const Ptr<Graphics::ModelEntity>& modelEntity);
    /// end lighting frame
    void EndFrame();

    bool isOpen;
    bool inBeginFrame;
    bool inBeginAttach;
    Ptr<Graphics::CameraEntity> cameraEntity;
    Ptr<GlobalLightEntity> globalLightEntity;
    Util::Array<Ptr<AbstractLightEntity> > visibleLightEntities;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
LightServerBase::IsOpen() const
{
    return this->isOpen;
}

} // namespace Lighting
//------------------------------------------------------------------------------
#endif
    
    