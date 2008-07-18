#pragma once

#ifndef MODELS_PARTICLESYSTEMNODE_H
#define MODELS_PARTICLESYSTEMNODE_H

//------------------------------------------------------------------------------
/**
    @class Models::ParticleSystemNode
  
    A model node which holds particle system information and applies a shared dynamiceMeshes 
    for its set of ParticleSystemNodeInstances.
    
    (C) 2008 Radon Labs GmbH
*/

#include "models/nodes/shapenode.h"
#include "addons/nebula2/nebula2wrapper.h"


namespace Models
{
class ParticleSystemNodeInstance;
class ParticleSystemNode : public ShapeNode
{
	DeclareClass(ParticleSystemNode);
public:
	/// constructor
    ParticleSystemNode();
    /// destructor
    virtual ~ParticleSystemNode();

    /// called when resources should be loaded
    virtual void LoadResources();
    /// called when resources should be unloaded
    virtual void UnloadResources();


    
    ///get the particle mesh
	Ptr<ParticleSystem::DynamicMesh> GetParticleMesh() const;
	/// set if invisible or not
    void SetInvisible(bool value);
    /// set the end time
    void SetEmissionDuration(float time);
    /// get the emission duration
    float GetEmissionDuration() const;
    /// set if loop emitter or not
    void SetLoop(bool b);
    /// is loop emitter ?
    bool GetLoop() const;
    /// set the activity distance
    void SetActivityDistance(float activityDistance);
    /// get the activity distance
    float GetActivityDistance() const;
    /// set the maximum start rotation angle
    void SetStartRotationMin(float rotMin);
    /// set the maximum start rotation angle
    void SetStartRotationMax(float rotMax);
    /// set gravity
    void SetGravity(float gravity);
    /// get gravity
    float GetGravity() const;
    /// set amount of stretching
    void SetParticleStretch(float particleStretch);
    /// set velocity randomize value
    void SetParticleVelocityRandomize(float velRandom);
    /// set rotation randomize value
    void SetParticleRotationRandomize(float rotRandom);
    /// set size randomize value
    void SetParticleSizeRandomize(float sizeRandom);
    /// randomize rotation direction on/off
    void SetRandomRotDir(bool isRandom);
    /// set texture tiling parts
    void SetTileTexture(int tileTexture);
    /// set if texture should be stretched to the emission start point
    void SetStretchToStart(bool isStretchToStart);
    /// set precalc time
    void SetPrecalcTime(float preClacTime);
    /// set wether to render oldest or youngest particles first
    void SetRenderOldestFirst(bool isOldestFirst);
    /// set the stretch detail
    void SetStretchDetail(int stretchDetail);
    /// set wether to render oldest or youngest particles first
    void SetViewAngleFade(bool viewAngelFade);
    /// get wether to render oldest or youngest particles first
    bool GetRenderOldestFirst() const;
    /// set start delay
    void SetStartDelay(float startDelay);
    /// set one of the envelope curves (not the color)
	void SetCurve(ParticleSystem::ParticleEmitter::CurveType curveType, const ParticleSystem::EnvelopeCurve& curve);
    /// get one of the envelope curves
	const ParticleSystem::EnvelopeCurve& GetCurve(ParticleSystem::ParticleEmitter::CurveType curveType) const;
    /// set the particle rgb curv
	void SetRGBCurve(const ParticleSystem::Vector3EnvelopeCurve& curve);
    /// get the particle rgb curve
	const ParticleSystem::Vector3EnvelopeCurve& GetRGBCurve() const;
    /// sets orientation to billboard
    void SetBillboardOrientation(bool b);
    /// gets billboard flag
    bool GetBillboardOrientation();
    /// gets the particle system mesh;

protected:
    /// create a model node instance
    virtual Ptr<ModelNodeInstance> CreateNodeInstance() const;
    
    float emissionDuration;
    float activityDistance;
    float startRotationMin;
    float startRotationMax;
    float gravity;         
    float particleStretch; 
    float particleVelocityRandomize;
    float particleRotationRandomize;
    float particleSizeRandomize;
    float precalcTime;
    float startDelay;
    int   tileTexture;     
    int   stretchDetail;
    bool  loop;             
    bool  randomRotDir;
    bool  stretchToStart;  
    bool  renderOldestFirst;
    bool  invisible;
    bool  viewAngleFade;
    bool  billboard;
    bool  curvesValid;

    ParticleSystem::EnvelopeCurve curves[ParticleSystem::ParticleEmitter::CurveTypeCount];
	ParticleSystem::Vector3EnvelopeCurve rgbCurve;
	Ptr<ParticleSystem::DynamicMesh> particleMesh;///a mesh for storing and rendering dynamic data. In this case particle data.
    float sampledCurves[ParticleSystem::ParticleEmitter::ParticleTimeDetail][ParticleSystem::ParticleEmitter::CurveTypeCount];   
private:
     static int numberOfCreatedNodes;///used for calculating the particleMesh resource name (every node = 1 particleMesh)

   
};

//------------------------------------------------------------------------------
/**
*/
inline Ptr<ParticleSystem::DynamicMesh> 
ParticleSystemNode::GetParticleMesh() const
{
    return this->particleMesh;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
ParticleSystemNode::SetEmissionDuration(float time)
{
    this->emissionDuration = time;
}

//------------------------------------------------------------------------------
/**
*/
inline float 
ParticleSystemNode::GetEmissionDuration() const
{
    return this->emissionDuration;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
ParticleSystemNode::SetLoop(bool b)
{
    this->loop = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
ParticleSystemNode::GetLoop() const
{
    return this->loop;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
ParticleSystemNode::SetActivityDistance(float f)
{
    this->activityDistance = f;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
ParticleSystemNode::SetStartRotationMin(float f)
{
    this->startRotationMin = f;
}
//------------------------------------------------------------------------------
/**
*/
inline void 
ParticleSystemNode::SetStartRotationMax(float f)
{
    this->startRotationMax = f;
}
//------------------------------------------------------------------------------
/**
*/
inline void 
ParticleSystemNode::SetGravity(float f)
{
    this->gravity = f;
}

//------------------------------------------------------------------------------
/**
*/
inline float 
ParticleSystemNode::GetGravity() const
{
    return this->gravity;
}
//------------------------------------------------------------------------------
/**
*/
inline float 
ParticleSystemNode::GetActivityDistance() const
{
    return this->activityDistance;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ParticleSystemNode::SetRenderOldestFirst(bool b)
{
    this->renderOldestFirst = b;
}
//------------------------------------------------------------------------------
/**
*/
inline void
ParticleSystemNode::SetParticleStretch(float value)
{
    this->particleStretch = value;
}
//------------------------------------------------------------------------------
/**
*/
inline void
ParticleSystemNode::SetPrecalcTime(float value)
{
    this->precalcTime = value;
}
//------------------------------------------------------------------------------
/**
*/
inline void
ParticleSystemNode::SetParticleVelocityRandomize(float value)
{
    this->particleVelocityRandomize = value;
}
//------------------------------------------------------------------------------
/**
*/
inline void
ParticleSystemNode::SetParticleRotationRandomize(float value)
{
    this->particleRotationRandomize = value;
}
//------------------------------------------------------------------------------
/**
*/
inline void
ParticleSystemNode::SetParticleSizeRandomize(float value)
{
    this->particleSizeRandomize = value;
}
//------------------------------------------------------------------------------
/**
*/
inline void
ParticleSystemNode::SetRandomRotDir(bool value)
{
    this->randomRotDir = value;
}
//------------------------------------------------------------------------------
/**
*/
inline void
ParticleSystemNode::SetTileTexture(int value)
{
    this->tileTexture = value;
}
//------------------------------------------------------------------------------
/**
*/
inline void
ParticleSystemNode::SetStretchToStart(bool value)
{
    this->stretchToStart = value;
}
//------------------------------------------------------------------------------
/**
*/
inline bool
ParticleSystemNode::GetRenderOldestFirst() const
{
    return this->renderOldestFirst;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ParticleSystemNode::SetCurve(ParticleSystem::ParticleEmitter::CurveType curveType, const ParticleSystem::EnvelopeCurve& curve)
{
    n_assert(curveType < ParticleSystem::ParticleEmitter::CurveTypeCount);
    n_assert(curveType >= 0);
    this->curvesValid = false;
    this->curves[curveType].SetParameters(curve);
}

//------------------------------------------------------------------------------
/**
*/
inline const ParticleSystem::EnvelopeCurve&
ParticleSystemNode::GetCurve(ParticleSystem::ParticleEmitter::CurveType curveType) const
{
    n_assert(curveType < ParticleSystem::ParticleEmitter::CurveTypeCount);
    n_assert(curveType >= 0);
    return this->curves[curveType];
}

//------------------------------------------------------------------------------
/**
*/
inline void
ParticleSystemNode::SetRGBCurve(const ParticleSystem::Vector3EnvelopeCurve& curve)
{
    this->curvesValid = false;
    this->rgbCurve.SetParameters(curve);
}

//------------------------------------------------------------------------------
/**
*/
inline const ParticleSystem::Vector3EnvelopeCurve&
ParticleSystemNode::GetRGBCurve() const
{
    return this->rgbCurve;
}
//------------------------------------------------------------------------------
/**
*/
inline void
ParticleSystemNode::SetInvisible(bool value)
{
    this->invisible = value;
}
//------------------------------------------------------------------------------
/**
*/
inline void
ParticleSystemNode::SetStretchDetail(int value)
{
    this->stretchDetail = value;
}
//------------------------------------------------------------------------------
/**
*/
inline void
ParticleSystemNode::SetViewAngleFade(bool value)
{
    this->viewAngleFade = value;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ParticleSystemNode::SetStartDelay(float value)
{
    this->startDelay = value;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ParticleSystemNode::SetBillboardOrientation(bool b)
{
    this->billboard = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
ParticleSystemNode::GetBillboardOrientation()
{
    return this->billboard;
}
};// namespace Models
#endif