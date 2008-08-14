#pragma once
#ifndef LIGHTING_SHADOWSERVERBASE_H
#define LIGHTING_SHADOWSERVERBASE_H
//------------------------------------------------------------------------------
/**
    class Lighting::ShadowServerBase
    
    The ShadowServer setups and controls the global aspects of the dynamic
    shadow system.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "graphics/cameraentity.h"
#include "lighting/abstractlightentity.h"
#include "lighting/globallightentity.h"
    
//------------------------------------------------------------------------------
namespace Lighting
{
class ShadowServerBase : public Core::RefCounted
{
    DeclareClass(ShadowServerBase);
    DeclareSingleton(ShadowServerBase);
public:
    /// constructor
    ShadowServerBase();
    /// destructor
    virtual ~ShadowServerBase();

    /// open the shadow server
    void Open();
    /// close the shadow server
    void Close();
    /// return true if shadow server is open
    bool IsOpen() const;

    /// begin shadow frame
    void BeginFrame(const Ptr<Graphics::CameraEntity>& cameraEntity);
    /// begin attaching visible shadow casting light sources
    void BeginAttachVisibleLights();
    /// attach a visible shadow casting light source
    void AttachVisibleLight(const Ptr<AbstractLightEntity>& lightEntity);
    /// end attaching visible shadow casting light sources
    void EndAttachVisibleLights();
    /// update shadow buffer
    void UpdateShadowBuffers();
    /// end lighting frame
    void EndFrame();
    
protected:
    bool isOpen;
    bool inBeginFrame;
    bool inBeginAttach;
    Ptr<Graphics::CameraEntity> cameraEntity;
    Ptr<GlobalLightEntity> globalLightEntity;
    Util::Array<Ptr<AbstractLightEntity> > localLightEntities;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
ShadowServerBase::IsOpen() const
{
    return this->isOpen;
}

} // namespace Lighting
//------------------------------------------------------------------------------
#endif
