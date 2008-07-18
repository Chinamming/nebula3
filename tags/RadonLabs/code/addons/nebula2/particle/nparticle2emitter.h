#ifndef N_PARTICLE2EMITTER_H
#define N_PARTICLE2EMITTER_H
//------------------------------------------------------------------------------
/**    
    @class nParticle2Emitter
    @ingroup Particle

    The particle emitter class for the ParticleSystem.

    (C) 2008 RadonLabs GmbH
*/

#include "core/refcounted.h"
#include "nparticleserver2.h"
#include "nparticle2.h"
#include "ndynamicmesh.h" 
#include "math/bbox.h"

#include "envelopecurve.h"
#include "vector3envelopecurve.h"

#include "resources/managedmesh.h"
#include "timing/time.h"

namespace Nebula2
{
class nParticleServer2;

//------------------------------------------------------------------------------
class nParticle2Emitter : public Core::RefCounted
{
    DeclareClass(nParticle2Emitter);
public:
    // enumeration of the envelope curves
    enum CurveType 
    {
        EmissionFrequency = 0,
        ParticleLifeTime,
        ParticleStartVelocity,
        ParticleRotationVelocity,
        ParticleScale,
        ParticleSpreadMin,
        ParticleSpreadMax,
        ParticleAlpha,
        ParticleAirResistance,
        StaticRGBCurve,
        ParticleVelocityFactor,
        ParticleMass,
        TimeManipulator,

        Filler1,            // filler for getting a CurveTypeCount that's power of two
        Filler2,            // filler for getting a CurveTypeCount that's power of two
        Filler3,            // filler for getting a CurveTypeCount that's power of two

        CurveTypeCount,
    };

    // helper structure for writing into the vertex stream fast
    #pragma pack(4)
    struct ParticleVertex
    {
		Math::float4 pos;            // model space position
        Math::float4 velocity;       // velocity (for stretched particles) or orientation
        float   u;              // u texture coordinate
        float   v;              // v texture coordinate
        float   rotation;       // sprite rotation angle
        float   dummy;          // placeholder (necessary because rotation+dummy is TexCoord1)
        float   alpha;          // alpha and uv-offsets (coded)
        float   cornerRotation; // per-corner rotation angle
        float   scale;          // size
        float   color;          // colors get coded into 1 float
    };
    #pragma pack()

    static const int ParticleTimeDetail = 100;

    /// constructor
    nParticle2Emitter();
    /// destructor
    virtual ~nParticle2Emitter();
    
    /// set current emitter transformation
	void SetTransform(const Math::matrix44& transform);
    /// get current emitter transformation
    const Math::matrix44& GetTransform() const;
    /// set the start time
    void SetStartTime(float time);
    /// get the start time
    float GetStartTime() const;
    /// set the end time
    void SetEmissionDuration(float time);
    /// get the emission duration
    float GetEmissionDuration() const;
    /// set if loop emitter or not
    void SetLooping(bool b);
    /// is it a loop emitter ?
    bool GetLooping() const;
    /// set the activity distance
    void SetActivityDistance(float f);
    /// get the distance to the viewer beyond which the emitter stops emitting
    float GetActivityDistance() const;
    /// set whether to render oldest or youngest particles first
    void SetRenderOldestFirst(bool b);
    /// get whether to render oldest or youngest particles first
    bool GetRenderOldestFirst() const;
    /// set whether use billboard orientation
    void SetBillboardOrientation(bool b);
    /// get whether use billboard orientation
    bool GetBillboardOrientation() const;

    /// update particles
    void Update(float curTime);
    /// render particles
    void Render();

    /// set the mesh that holds particle data
    void SetParticleMesh(Ptr<nDynamicMesh> mesh);
    /// get the mesh that holds particle data
    Ptr<nDynamicMesh> GetParticleMesh() const;

    /// set the mesh that emittes particles
    void SetEmitterMesh(Ptr<CoreGraphics::Mesh> mesh);
    /// get mesh that emits
    Ptr<CoreGraphics::Mesh> GetEmitterMesh() const;
    /// set emitter mesh group index
    void SetEmitterMeshGroupIndex(int index);
    /// get emitter mesh group index
    int GetEmitterMeshGroupIndex() const;

    /// set bounding box
	void SetBoundingBox(const Math::bbox& b);
    /// get bounding box
	const Math::bbox& GetBoundingBox() const;
    /// set the wind
	void SetWind(const Math::float4& wind);
    /// get the wind
	const Math::float4& GetWind() const;

    /// return true if particle system has been initialized
    bool IsValid() const;
    /// initializing, call once after setting parameters
    void Initialize();

    /// signal that all necessary values have been set and emitting can begin
    void SetIsSetup(bool b);
    /// return if emitter is set up
    bool IsSetup() const;

    /// set pointer to parameter curves (identical for all instances of a particle system)
    void SetStaticCurvePtr(float* ptr);
    /// return true if static curve pointer is valid
    bool IsStaticCurvePtrValid() const;
    /// called when remotecontrol (maya) changes the curves
    void NotifyCurvesChanged();
    /// static helper method to sample curves into key-arrays
	static void PresampleCurves(const EnvelopeCurve* envCurveArray, const Vector3EnvelopeCurve* rgbCurve, float* dstKeyArray);

    /// set gravity force
    void SetGravity(float gravity);
    /// set minimum rotation angle at emission
    void SetStartRotationMin(float value);
    /// set maximum rotation angle at emission
    void SetStartRotationMax(float value);
    /// set amount (time) of stretching
    void SetParticleStretch(float value);
    /// set texture tiling parts
    void SetTileTexture(int value);
    /// set if particles should be stretched to the emission startpoint 
    void SetStretchToStart(bool value);
    /// set Velocity Randomize
    void SetParticleVelocityRandomize(float value);
    /// set Rotation Randomize
    void SetParticleRotationRandomize(float value);
    /// set Size Randomize
    void SetParticleSizeRandomize(float value);
    /// set random rotation direction
    void SetRandomRotDir(bool value);
    /// set Precalculation time
    void SetPrecalcTime(float value);
    /// set random rotation direction
    void SetStretchDetail(int value);
    /// set random rotation direction
    void SetViewAngleFade(bool value);
    /// set start delay
    void SetStartDelay(float value);
    /// sets the emitter orientation
	void SetOrientation(const Math::matrix44& rot);
    /// gets the emitter orientation
	const Math::matrix44& GetOrientation();
   
    /// stop emission, returns the time till the last particle is dead
    float StopEmission();
    /// returns true, if the last particle is dead
    bool AllParticlesDead() const;

    /// returns true if this emitter has particles
    bool HasParticles() const;
    /// returns the particle lifetime
    Timing::Time GetParticleLifetime() const;
    
private:
    /// not implemented operator to prevent '=' - assignment
    nParticle2Emitter& operator=(const nParticle2Emitter &);
    /// allocate particle pool
    void AllocateParticles();
    /// delete particles pool
    void DeleteParticles();
    /// re-allocate particles
    void ReallocateParticles(int newMaxParticleCount);
    /// update particles
    void CalculateStep(float fdTime);
    /// render as "normal" particles
    int RenderSpriteParticles(float* dstVertices, int maxVertices);
    /// render as stretched particles
    int RenderStretchedParticles(float* dstVertices, int maxVertices);
    /// expensive-high-quality stretched rendering
    int RenderStretchedSmoothParticles(float* dstVertices, int maxVertices);
    /// check whether particle emitter is invisible and should go to sleep
    bool CheckInvisible(float deltaTime);

    float* pStaticCurves;
    Ptr<CoreGraphics::Mesh> refEmitterMesh;
	int emitterMeshGroupIndex;
    Ptr<nDynamicMesh> refParticleMesh;
	Math::matrix44 transform;
	Math::bbox box;
	Math::float4 wind;
    int lastEmissionVertex;
    float startTime;
    float lastEmission;

    // emitter settings
    float emissionDuration;
    bool looping;
    float activityDistance;
    float particleStretch;
    float precalcTime;             
    int tileTexture;
    bool renderOldestFirst;
    bool stretchToStart;
    bool randomRotDir;
    bool hasLooped;
    bool frameWasRendered;
    float invisibleTime;
    bool isSleeping;
    int stretchDetail;
    bool viewAngleFade;
    float startDelay;

    float gravity;
    float startRotationMin;
    float startRotationMax;
    float particleVelocityRandomize;
    float particleRotationRandomize;
    float particleSizeRandomize;

    nParticle2* particles;
    int particleCount;
    int maxParticleCount;

    float remainingTime;

    bool isValid;
    bool isSetup;
    bool billboard;
	Math::matrix44 orientation;

    // stops the emission
    bool stopEmission;
};

//------------------------------------------------------------------------------
/**
*/
inline
bool
nParticle2Emitter::HasParticles() const
{
    return this->particleCount > 0 && 0 != this->particles;
}

//------------------------------------------------------------------------------
/**
*/
inline
Timing::Time
nParticle2Emitter::GetParticleLifetime() const
{
    n_assert(this->particles);
    return this->particles->lifeTime;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
nParticle2Emitter::IsValid() const
{
    return this->isValid;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
nParticle2Emitter::SetTransform(const Math::matrix44& m)
{
    this->transform = m;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::matrix44&
nParticle2Emitter::GetTransform() const
{
    return this->transform;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
nParticle2Emitter::SetStartTime(float time)
{
    this->startTime = time;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
nParticle2Emitter::SetParticleMesh(Ptr<nDynamicMesh> mesh)
{
    this->refParticleMesh = mesh;
}

//------------------------------------------------------------------------------
/**
*/
inline
Ptr<nDynamicMesh>
nParticle2Emitter::GetParticleMesh() const
{
    return this->refParticleMesh;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
nParticle2Emitter::SetEmitterMesh(Ptr<CoreGraphics::Mesh> mesh)
{
    this->refEmitterMesh = mesh;
}

//------------------------------------------------------------------------------
/**
*/
inline
Ptr<CoreGraphics::Mesh>
nParticle2Emitter::GetEmitterMesh() const
{
    return this->refEmitterMesh;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nParticle2Emitter::SetEmitterMeshGroupIndex(int index)
{
    this->emitterMeshGroupIndex = index;
}

//------------------------------------------------------------------------------
/**
*/
inline
int
nParticle2Emitter::GetEmitterMeshGroupIndex() const
{
    return this->emitterMeshGroupIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
nParticle2Emitter::SetEmissionDuration(float time)
{
    this->emissionDuration = time;
}

//------------------------------------------------------------------------------
/**
*/
inline
float 
nParticle2Emitter::GetEmissionDuration() const
{
    return this->emissionDuration;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
nParticle2Emitter::SetLooping(bool b)
{
    this->looping = b;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool 
nParticle2Emitter::GetLooping() const
{
    return this->looping;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
nParticle2Emitter::SetActivityDistance(float f)
{
    this->activityDistance = f;
}

//------------------------------------------------------------------------------
/**
*/
inline
float 
nParticle2Emitter::GetActivityDistance() const
{
    return this->activityDistance;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nParticle2Emitter::SetRenderOldestFirst(bool b)
{
    this->renderOldestFirst = b;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
nParticle2Emitter::GetRenderOldestFirst() const
{
    return this->renderOldestFirst;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nParticle2Emitter::SetBillboardOrientation(bool b)
{
    this->billboard = b;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
nParticle2Emitter::GetBillboardOrientation() const
{
    return this->billboard;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nParticle2Emitter::SetBoundingBox(const Math::bbox& b)
{
    this->box = b;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::bbox&
nParticle2Emitter::GetBoundingBox() const
{
    return this->box;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nParticle2Emitter::SetWind(const Math::float4& wind)
{
    this->wind = wind;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::float4&
nParticle2Emitter::GetWind() const
{
    return this->wind;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
nParticle2Emitter::SetIsSetup(bool b)
{
    this->isSetup = b;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
nParticle2Emitter::IsSetup() const
{
    return this->isSetup;
}

//------------------------------------------------------------------------------
/**
*/
inline
void    
nParticle2Emitter::SetStaticCurvePtr(float* ptr)
{
    this->pStaticCurves = ptr;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
nParticle2Emitter::IsStaticCurvePtrValid() const
{
    return (0 != this->pStaticCurves);
}

//------------------------------------------------------------------------------
/**
*/
inline
void    
nParticle2Emitter::SetGravity(float gravity)
{
    this->gravity = gravity;
}

//------------------------------------------------------------------------------
/**
*/
inline
void    
nParticle2Emitter::SetStartRotationMin(float value)
{
    this->startRotationMin = value;
}

//------------------------------------------------------------------------------
/**
*/
inline
void    
nParticle2Emitter::SetStartRotationMax(float value)
{
    this->startRotationMax = value;
}

//------------------------------------------------------------------------------
/**
*/
inline
void    
nParticle2Emitter::SetParticleStretch(float value)
{
    this->particleStretch = value;
}

//------------------------------------------------------------------------------
/**
*/
inline
void    
nParticle2Emitter::SetTileTexture(int value)
{
    if (value < 1) value = 1;
    this->tileTexture = value; 
}

//------------------------------------------------------------------------------
/**
*/
inline
void    
nParticle2Emitter::SetStretchToStart(bool value)
{
    this->stretchToStart = value;
}

//------------------------------------------------------------------------------
/**
*/
inline
void    
nParticle2Emitter::SetParticleVelocityRandomize(float value)
{
    this->particleVelocityRandomize = value;
}

//------------------------------------------------------------------------------
/**
*/
inline
void    
nParticle2Emitter::SetParticleRotationRandomize(float value)
{
    this->particleRotationRandomize = value;
}

//------------------------------------------------------------------------------
/**
*/
inline
void    
nParticle2Emitter::SetParticleSizeRandomize(float value)
{
    this->particleSizeRandomize = value;
}

//------------------------------------------------------------------------------
/**
*/
inline
void    
nParticle2Emitter::SetPrecalcTime(float value)
{
    this->precalcTime = value;
}

//------------------------------------------------------------------------------
/**
*/
inline
void    
nParticle2Emitter::SetRandomRotDir(bool value)
{
    this->randomRotDir = value;
}

//------------------------------------------------------------------------------
/**
*/
inline
void    
nParticle2Emitter::SetStretchDetail(int value)
{
    this->stretchDetail = value;
}

//------------------------------------------------------------------------------
/**
*/
inline
void    
nParticle2Emitter::SetViewAngleFade(bool value)
{
    this->viewAngleFade = value;
}

//------------------------------------------------------------------------------
/**
*/
inline
void    
nParticle2Emitter::SetStartDelay(float value)
{
    this->startDelay = value;
}

//------------------------------------------------------------------------------
/**
*/
inline
void    
nParticle2Emitter::SetOrientation(const Math::matrix44& rot)
{
    this->orientation = rot;
	this->orientation.translate( Math::float4(0,0,0,0) );
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::matrix44& 
nParticle2Emitter::GetOrientation()
{
    return this->orientation;
}

//------------------------------------------------------------------------------
};// namespace Nebula2
#endif
