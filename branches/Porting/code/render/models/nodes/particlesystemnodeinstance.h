#pragma once
#ifndef MODELS_PARTICLESYSTEMNODEINSTANCE_H
#define MODELS_PARTICLESYSTEMNODEINSTANCE_H
//------------------------------------------------------------------------------
/**
    @class Models::ParticleSystemNodeInstance

    Holds a particle system emitter and applies it.
    
    (C) 2008 Radon Labs GmbH
*/
#include "models/nodes/shapenodeinstance.h"
#include "addons/nebula2/nebula2wrapper.h"
namespace Models
{
class ParticleSystemNodeInstance : public ShapeNodeInstance
{
	DeclareClass(ParticleSystemNodeInstance);
public:
	/// constructor
    ParticleSystemNodeInstance();
    /// destructor
    virtual ~ParticleSystemNodeInstance();
	
	/// perform per-frame updates
	virtual void Update();
	/// perform rendering
    virtual void Render();
    /// sets the emitter which is used by the instance
    void SetEmitter(Ptr<ParticleSystem::ParticleEmitter> emitter);
	
protected:
    /// called when attached to ModelInstance
    virtual void OnAttachToModelInstance(const Ptr<ModelInstance>& inst, const Ptr<ModelNode>& node, const Ptr<ModelNodeInstance>& parentNodeInst);
    /// called when removed from ModelInstance
    virtual void OnRemoveFromModelInstance();
	/// notify that we are visible
	virtual void OnNotifyVisible(IndexT frameIndex);

    Ptr<ParticleSystem::ParticleEmitter> particleEmitter;
	 
};

//------------------------------------------------------------------------------
/**
*/
inline void 
ParticleSystemNodeInstance::SetEmitter(Ptr<ParticleSystem::ParticleEmitter> emitter)
{
    this->particleEmitter = emitter;
}
};// namespace Models

#endif