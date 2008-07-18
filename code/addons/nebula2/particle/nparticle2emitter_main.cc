//------------------------------------------------------------------------------
//  nparticle_main.cc
//  (C) 2003 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"

#include "nParticle2Emitter.h"
#include "nparticle2.h"
#include "math/vector.h"
#include "coregraphics/transformdevice.h"
#include "coregraphics/base/resourcebase.h"
#include "coregraphics/primitivegroup.h"
#include "addons/nebula2/particle/ndynamicmesh.h"
//

//------------------------------------------------------------------------------
/**
*/
namespace Nebula2
{

ImplementClass(nParticle2Emitter, 'PARE', Core::RefCounted);

nParticle2Emitter::nParticle2Emitter() :
    pStaticCurves(0),
    emitterMeshGroupIndex(0),
    lastEmissionVertex(0),
    startTime(-1.0),
    lastEmission(0),
    emissionDuration(10.0),
    looping(true),
    activityDistance(100.0f),
    particleStretch(0),
    precalcTime(0),
    tileTexture(1),
    renderOldestFirst(false),
    stretchToStart(false),
    randomRotDir(false),
    hasLooped(false),
    frameWasRendered(true),
    invisibleTime(0),
    isSleeping(false),
    stretchDetail(1),
    viewAngleFade(false),
    startDelay(0.0f),
    gravity(0.0f),
    startRotationMin(0.0f),
    startRotationMax(0.0f),
    particleVelocityRandomize(0.0f),
    particleRotationRandomize(0.0f),
    particleSizeRandomize(0.0f),
    particles(0),
    particleCount(0),
    maxParticleCount(0),
    remainingTime(0.0),
    billboard(true),
    isSetup(false),
    isValid(false),
    stopEmission(false)
{
    // empty
}


//------------------------------------------------------------------------------
/**
*/
nParticle2Emitter::~nParticle2Emitter()
{
    this->DeleteParticles();
}

//------------------------------------------------------------------------------
/**
    Allocates the particle pool.
*/
void
nParticle2Emitter::AllocateParticles()
{
    n_assert(this->maxParticleCount > 0);
    this->DeleteParticles();
    this->particles = n_new_array(nParticle2, this->maxParticleCount);
    this->particleCount = 0;
}

//------------------------------------------------------------------------------
/**
    Deletes the particle pool.
*/
void
nParticle2Emitter::DeleteParticles()
{
    if (this->particles)
    {
        n_delete_array(this->particles);
        this->particles = 0;
        this->particleCount = 0;
    }
}

//------------------------------------------------------------------------------
/**
    Reallocate particle array with new max particle count, copies
    existing particles into the new particle array.
*/
void
nParticle2Emitter::ReallocateParticles(int newMaxParticleCount)
{
    // allocate new array
    nParticle2* newPtr = n_new_array(nParticle2, newMaxParticleCount);

    // copy existing particles
    int partsToCopy = this->particleCount;
    if (partsToCopy > newMaxParticleCount)
    {
        partsToCopy = newMaxParticleCount;
    }
    Memory::Copy(this->particles, newPtr, partsToCopy * sizeof(nParticle2));

    // delete old array
    n_delete_array(this->particles);

    // set new values
    this->particleCount = partsToCopy;
    this->particles = newPtr;
    this->maxParticleCount = newMaxParticleCount;
}

//------------------------------------------------------------------------------
/**
*/
void
nParticle2Emitter::Initialize()
{
    n_assert(0 != this->pStaticCurves);

    // calculate maximum number of particles
    float maxFreq = 0;
    float maxLife = 0;
    int i;
    for (i = 0; i < ParticleTimeDetail; i++)
    {
        if (this->pStaticCurves[i*CurveTypeCount+EmissionFrequency] > maxFreq)
        {
            maxFreq = this->pStaticCurves[i*CurveTypeCount+EmissionFrequency];
        }
        if (this->pStaticCurves[i*CurveTypeCount+ParticleLifeTime] > maxLife)
        {
            maxLife = this->pStaticCurves[i*CurveTypeCount+ParticleLifeTime];
        }
    }
    this->maxParticleCount = (int) (maxFreq * maxLife);
    this->AllocateParticles();
    this->isValid = true;
}

//------------------------------------------------------------------------------
/**
    This is for the live material update from Maya.
*/
void    
nParticle2Emitter::NotifyCurvesChanged()
{
    n_assert(0 != this->pStaticCurves);
    if (this->particles != 0)
    {
        // we need to rearrange the particlearray, because the curves have changed
        float maxFreq = 0;
        float maxLife = 0;
        int i;
        for (i = 0; i < ParticleTimeDetail; i++)
        {
            if (this->pStaticCurves[i*CurveTypeCount+EmissionFrequency] > maxFreq)
            {
                maxFreq = this->pStaticCurves[i*CurveTypeCount+EmissionFrequency];
            }
            if (this->pStaticCurves[i*CurveTypeCount+ParticleLifeTime] > maxLife)
            {
                maxLife = this->pStaticCurves[i*CurveTypeCount+ParticleLifeTime];
            }
        }
        int newMaxParticleCount = (int)(maxFreq * maxLife);
        this->ReallocateParticles(newMaxParticleCount);
    }
}

//------------------------------------------------------------------------------
/**
    Updates the existing particles.
*/
void
nParticle2Emitter::CalculateStep(float stepTime)
{
    n_assert(stepTime >= 0.0f);
    n_assert(0 != this->particles);
    n_assert(0 != this->pStaticCurves);

    // nothing to do?
    if (0 == this->particleCount)
    {
        return;
    }

	//Removed: Nebula3 doesent support wind param
    //vector3 windVector(this->wind.x * this->wind.w, this->wind.y * this->wind.w, this->wind.z * this->wind.w);
	//Math::float4 windVector(this->wind.x() * this->wind.w(), this->wind.y() * this->wind.w(), this->wind.z() * this->wind.w(),0);
	
	Math::float4 acc, freeAcc, boundMin, boundMax;
    this->box.begin_extend();

	
    nParticle2* particleSource = this->particles;
    nParticle2* particle = this->particles;
    int particleIndex;
    for (particleIndex = 0; particleIndex < this->particleCount; particleIndex++)
    {
        // FIXME: hmm, this removes dead particles from the array,
        // the copy operation sucks though...
        if (particle != particleSource)
        {
            *particle = *particleSource;
        }

        // update times
        particle->lifeTime += stepTime;
        float relParticleAge = particle->lifeTime * particle->oneDivMaxLifeTime;
        if ((relParticleAge >= 0.0f) && (relParticleAge < 1.0f))
        {
            // get pointer to anim curves
            int curveIndex = int(relParticleAge * float(ParticleTimeDetail));
            curveIndex = Math::n_iclamp(curveIndex, 0, ParticleTimeDetail - 1);
            float* curCurves = &this->pStaticCurves[curveIndex * CurveTypeCount];

            // compute acceleration vector
            //HACK: delete windvector dec later
            Math::vector windVector(0,0,0);
            acc = windVector * curCurves[ParticleAirResistance]; 
			acc.set(acc.x(),acc.y() + this->gravity, acc.z(),acc.w());
            acc *= curCurves[ParticleMass];

            // update particle
            particle->acc = acc;
            particle->position += particle->velocity * (stepTime * curCurves[ParticleVelocityFactor]);
            particle->velocity += acc * stepTime;
            particle->rotation += curCurves[ParticleRotationVelocity] * particle->rotationVariation * stepTime;

            // update boundary values
            this->box.extend(particle->position);

            // advance to next particle
            particle++;
        } 
        else 
        {
            // particle's lifetime is over
            // don't advance particle pointer 
            this->particleCount--;
            particleIndex--;
        }
        particleSource++;
    }
}

//------------------------------------------------------------------------------
/**
    Checks if the particle system should go to sleep because it is
    too far away or was invisible for some time.
*/
bool
nParticle2Emitter::CheckInvisible(float deltaTime)
{
    // check for activity distance
	const Math::matrix44& viewer = CoreGraphics::TransformDevice::Instance()->GetInvViewTransform(); //nGfxServer2::Instance()->GetTransform(nGfxServer2::InvView);
	Math::float4 emitterViewer = viewer.getpos_component() - this->transform.getpos_component();

	if ((emitterViewer.length() >= this->activityDistance) ||                 // viewer out of range
        (!this->frameWasRendered && this->looping && this->hasLooped))     // skip if invisible, looping and has looped at least once
    {
        // adjust starttime by missed timedelta
        this->startTime += deltaTime;
        this->lastEmission += deltaTime;
        if (!this->frameWasRendered)
        {
            this->invisibleTime += deltaTime;
		}
        else
        {
            this->invisibleTime = 0.0f;
        }

        // go to sleep after beeing invisible for too long
        if (this->looping && this->hasLooped && !this->isSleeping && (this->invisibleTime > 3.0f))    
        {
            this->isSleeping = true;
            this->DeleteParticles();
        }
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
    Returns true, if the last emitted particle is dead
*/
bool
nParticle2Emitter::AllParticlesDead() const
{
    return this->particleCount == 0 ? true : false;
}

//------------------------------------------------------------------------------
/**
    Returns the time, till the last particle is dead
*/
float
nParticle2Emitter::StopEmission()
{
    // stop emission
    this->stopEmission = true;

    return this->particles->lifeTime;
}

//------------------------------------------------------------------------------
/**
    Updates the particle system. This will create new particles.
*/
void
nParticle2Emitter::Update(float curTime)
{
 /*   if (!this->IsSetup() || 
        !nParticleServer2::Instance()->AreParticlesEnabled() ||
        !this->refEmitterMesh.isvalid())
    {
        // not completely setup yet
        return;
    }*///TODO: Fix EmitterMesh

    // need to initialize?
    bool firstRun = false;
    if (!this->IsValid())
    {
        this->Initialize();
        firstRun = true;
    }

    // check for time exception
    int numSteps = 1;
    float stepSize = 0.0f;
    if (firstRun || (curTime < this->lastEmission))
    {
        // a time exception has occured, or we are run for the first
        // time, so reset the particle system
        if (this->precalcTime > 0.0f)
        {
            stepSize = 0.1f;    // calculate 1/10s steps
            numSteps = int(this->precalcTime / stepSize) + 1;
            curTime -= (numSteps - 1) * stepSize;
        }
        this->particleCount = 0;
        this->startTime = curTime;
        this->lastEmission = curTime;
        this->remainingTime = 0;
    }

    // for each step...
    int curStep;
    for (curStep = 0; curStep < numSteps; curStep++)
    {
        // calculate timestep
        float deltaTime = float(curTime - this->lastEmission);
        n_assert(deltaTime >= 0.0f);

        // compute stepTime, which is deltaTime manipulated by
        // time manipulator curve
        float relAge = float((curTime - this->startTime - this->startDelay) / this->emissionDuration);
        int curveIndex = int(relAge * ParticleTimeDetail);
		curveIndex = Math::n_iclamp(curveIndex, 0, ParticleTimeDetail - 1);
        float stepTime = n_abs(deltaTime * this->pStaticCurves[curveIndex * CurveTypeCount + TimeManipulator]);
        n_assert(stepTime >= 0.0f);

        // invisibility- and out-of-range-check
        // (only if this is not an initial update)
        if ((1 == numSteps) && this->CheckInvisible(deltaTime))
        {
            return;
        }

        this->frameWasRendered = false;
        this->invisibleTime = 0;

        // update existing particle positions, remove dead ones
        this->CalculateStep(stepTime);

        // just do it if we should
        if(!this->stopEmission)
        {
            // emit new particles if we are inside the emissiontimeframe
            if ((curTime >= this->startTime) && (lastEmission < this->startTime + this->startDelay + this->emissionDuration)) 
            {
                if (curTime >= (this->startTime + this->startDelay))
                {
                    // FIXME: it may happen that relAge becomes greater 1.0 here!
                    float relAge = float((curTime - this->startTime - this->startDelay) / this->emissionDuration);
                    int curveIndex = int(relAge * ParticleTimeDetail);
					curveIndex = Math::n_iclamp(curveIndex, 0, ParticleTimeDetail - 1);
                    float* curCurves = &this->pStaticCurves[curveIndex * CurveTypeCount];
                    //float* vertices = (float*)this->refEmitterMesh->GetVertexBuffer()->Map(CoreGraphics::VertexBuffer::MapRead);//->LockVertices();
					//int vertexWidth = this->refEmitterMesh->GetVertexBuffer()->GetVertexLayout()->GetVertexByteSize()/sizeof(float);//Possible flaw ->GetVertexWidth()
                    //ushort* indices = (ushort*)this->refEmitterMesh->GetIndexBuffer()->Map(CoreGraphics::IndexBuffer::MapRead);
                    //const nMeshGroup& meshGroup = this->refEmitterMesh->Group(this->emitterMeshGroupIndex);
                    ////TODO: Fix EmitterMesh
					const CoreGraphics::PrimitiveGroup& meshGroup = this->refEmitterMesh->GetPrimitiveGroupAtIndex(this->emitterMeshGroupIndex);
					
					//int firstIndex  = meshGroup.GetFirstIndex();
					int firstIndex = meshGroup.GetBaseIndex();
		            //int numIndices = meshGroup.GetNumIndices();
					int numIndices = meshGroup.GetNumIndices();
					//Math::mat matrix33 transform33(this->transform.x_component(), this->transform.y_component(), this->transform.z_component());
					Math::matrix44 transform33(this->transform.getx_component(), this->transform.gety_component(), this->transform.getz_component(),this->transform.getrow3());
					Math::float4 emissionPos, emissionNormal;

                    float curEmissionFrequency = curCurves[EmissionFrequency];

                    // for correct emission we perform 1s/freq - steps
                    float timeToDo = stepTime + this->remainingTime;
                    if (0.0f < curEmissionFrequency)
                    {
                        float emitTimeStep = 1.0f / curEmissionFrequency;

                        while (timeToDo >= emitTimeStep)
                        {
                            float particleEmissionLifeTime = timeToDo;
                            float oneDivLifeTime = 1.0f;
                            if (0 != curCurves[ParticleLifeTime])
                            {
                                oneDivLifeTime = 1.0f / curCurves[ParticleLifeTime];
                            }

                            if (this->particleCount < this->maxParticleCount)
                            {
                                // emit a new particle
                                nParticle2* newParticle = &this->particles[this->particleCount];
                                this->particleCount++;

                                // get emission position
								//int indexIndex = firstIndex + int(Math::n_rand(0.0f, float(numIndices - 1)));
                                //float* vertexPtr = &(vertices[indices[indexIndex] * vertexWidth]);
                                emissionPos.set(0, 0, 0,1);
								emissionPos.transform(emissionPos, this->transform);

                                // get emission normal//TODO: Fix with mesh
                                emissionNormal.set( Math::n_rand(-1.0f, 1.0f), 1, 0,0);
                                emissionNormal.normalize(emissionNormal);
                                emissionNormal.transform(emissionNormal, transform33);

                                // find orthogonal vectors to spread normal vector
								
								Math::float4 ortho1;
/* TODO: Fix find ortho							
								ortho1 = emissionNormal.findortho();
								ortho1.normalize();//norm
								Math::float4 normBackup = emissionNormal;
                                float spreadMin = curCurves[ParticleSpreadMin];
                                float spreadMax = curCurves[ParticleSpreadMax];
                                spreadMin = n_min(spreadMin, spreadMax);
								float spread = Math::n_lerp(spreadMin, spreadMax, Math::n_rand());
								float rotRandom = Math::n_rand() * 360.0f;
								emissionNormal.rotate(ortho1, Math::n_deg2rad(spread));
                                emissionNormal.rotate(normBackup, n_deg2rad(rotRandom));
*/
								float velocityVariation = 1.0f - Math::n_rand(0.0f, this->particleVelocityRandomize);
                                float startVelocity = curCurves[ParticleStartVelocity] * velocityVariation;
                                
                                // apply texture tiling
                                // uvmax and uvmin are arranged a bit strange, because they need to be flipped 
                                // horizontally and be rotated
                                float vStep = 1.0f / float(this->tileTexture);
								int tileNr = int(Math::n_rand(0.0f, float(this->tileTexture)));
                                
                                newParticle->uvmin.set(1.0f, vStep * float(tileNr));
                                newParticle->uvmax.set(0.0f, newParticle->uvmin.y() + vStep);
                                newParticle->lifeTime = particleEmissionLifeTime;
                                newParticle->oneDivMaxLifeTime = oneDivLifeTime;
                                newParticle->position = emissionPos;
								newParticle->rotation = Math::n_lerp(this->startRotationMin, this->startRotationMax, Math::n_rand());
								newParticle->rotationVariation = 1.0f - Math::n_rand() * this->particleRotationRandomize;
								if (this->randomRotDir && (Math::n_rand() < 0.5f))
                                {
                                    newParticle->rotationVariation = -newParticle->rotationVariation;
                                }
                                newParticle->velocity = emissionNormal * startVelocity;
                                newParticle->startPos = newParticle->position;
								newParticle->sizeVariation = 1.0f - Math::n_rand() * this->particleSizeRandomize;

                                // add velocity*lifetime
                                // FIXME FLOH: why this??
                                newParticle->position += newParticle->velocity * newParticle->lifeTime;
                            }
                            timeToDo -= emitTimeStep;
                        }
                    }
                    this->remainingTime = timeToDo;
//					this->refEmitterMesh->GetVertexBuffer()->Unmap();//TODO: Fix EmitterMesh
//					this->refEmitterMesh->GetIndexBuffer()->Unmap();
                }
            } 
            else 
            {
                if (this->looping)
                {
                    this->startTime = curTime;
                    this->remainingTime = 0;
                    this->hasLooped = true;
                }
            }
        }
        this->lastEmission = curTime;
        curTime += stepSize;
    }
}

//------------------------------------------------------------------------------
/**
    Render the Particles

    FIXME: CLEANUP
*/
void nParticle2Emitter::Render()
{
    /*
    if (!this->IsSetup() || !this->IsValid() || !nParticleServer2::Instance()->AreParticlesEnabled()) 
    {
        return;
    }

    if (this->isSleeping)    // do we have to wakeup ?
    {
        this->isSleeping = false;
        this->AllocateParticles();
        this->frameWasRendered = true;
        this->Update(curTime - 0.001f);    // trigger with a little difference, so that the emitter will reset

        // ok, we're up-to-date again
        };
    if (!this->refEmitterMesh->IsValid())
    {
        this->refEmitterMesh->Initialize(CoreGraphics::PrimitiveTopology::TriangleList,true);
    n_assert(this->refEmitterMesh.isvalid());
    }*///TODO: Fix emittermesh and implement particleserver into nebula engine.

    float* dstVertices = 0;
    int maxVertices = 0;
    int remVertices = 0;
    this->refParticleMesh->Begin(dstVertices, maxVertices);

    if ((this->particleStretch == 0.0f) && (!this->stretchToStart))
    {
        remVertices = RenderSpriteParticles(dstVertices, maxVertices);
    }
    else 
    {
        if (this->stretchToStart || (this->stretchDetail == 1) )
        {
            remVertices = RenderStretchedParticles(dstVertices, maxVertices);
        } 
        else 
        {
            remVertices = RenderStretchedSmoothParticles(dstVertices, maxVertices);
        }

    };
    // draw remaining vertices
    this->refParticleMesh->End(remVertices);
    this->frameWasRendered = true;
}

//------------------------------------------------------------------------------
/**
    Render as normal sprite.
*/
int nParticle2Emitter::RenderSpriteParticles(float* dstVertices, int maxVertices)
{
    nParticle2* curParticle = this->particles;
    int particlePitch = 1;
    if (this->renderOldestFirst)
    {
        // reverse iterating order
        particlePitch = -1;
        curParticle = &this->particles[this->particleCount -1];
    }

    // note: use the velocity vertex component for orientation,
    // because velocity is not need for sprite particles
	Math::float4 extrudeVec;
    if (billboard)
    {
		const Math::matrix44& viewer = CoreGraphics::TransformDevice::Instance()->GetInvViewTransform();//nGfxServer2::Instance()->GetTransform(nGfxServer2::InvView);
        extrudeVec = viewer.getx_component();
    }
    else
    {
		extrudeVec = Math::float4(1, 0, 0,0);
    }

    // write particle vertices
    // NOTE: it is essential to write data sequentially to write-combined memory!!
    int curveIndex;
    float* curCurves;
    int p;
    int curVertexIndex = 0;
    ParticleVertex* curVertexPtr = (ParticleVertex*) dstVertices;
    ParticleVertex curVertex;
    curVertex.velocity = extrudeVec;
    

    for (p = 0; p < this->particleCount; p++)
    {
        // life-time-check is not needed, it is assured that the relative age is >=0 and <1
        curveIndex = (int)((curParticle->lifeTime * curParticle->oneDivMaxLifeTime) * (float)ParticleTimeDetail);
		curveIndex = Math::n_iclamp(curveIndex, 0, ParticleTimeDetail - 1);
        curCurves = &this->pStaticCurves[curveIndex*CurveTypeCount];

        // setup vertex components which are identical for the entire particle
        //TODO: implement shaders
 

        // first triangle
        
        curVertex.u              = curParticle->uvmax.x();
        curVertex.v              = curParticle->uvmin.y();
        curVertex.cornerRotation = curParticle->rotation + PI/4.0f + PI/2.0f;
        curVertex.rotation = curParticle->rotation;
        curVertex.dummy    = 0.0f;
        curVertex.alpha    = curCurves[ParticleAlpha];
        curVertex.scale    = curCurves[ParticleScale] * curParticle->sizeVariation;
        curVertex.color    = curCurves[StaticRGBCurve];
        curVertex.pos      = Math::float4(curParticle->position.x()-curVertex.scale, 
                                            curParticle->position.y()+curVertex.scale,
                                            curParticle->position.z(),1);
        *curVertexPtr++ = curVertex;
        
        
        curVertex.u = curParticle->uvmax.x();
        curVertex.v = curParticle->uvmax.y();
        curVertex.cornerRotation = curParticle->rotation + PI/4.0f + 2 * PI/2.0f;
        curVertex.rotation = curParticle->rotation;
        curVertex.dummy    = 0.0f;
        curVertex.alpha    = curCurves[ParticleAlpha];
        curVertex.scale    = curCurves[ParticleScale] * curParticle->sizeVariation;
        curVertex.color    = curCurves[StaticRGBCurve];
        curVertex.pos      = Math::float4(curParticle->position.x()+curVertex.scale, 
                                            curParticle->position.y()+curVertex.scale,
                                            curParticle->position.z(),1);
        *curVertexPtr++ = curVertex;
        
        
        curVertex.u = curParticle->uvmin.x();
        curVertex.v = curParticle->uvmax.y();
        curVertex.cornerRotation = curParticle->rotation + PI/4.0f + 3 * PI/2.0f;
        curVertex.rotation = curParticle->rotation;
        curVertex.dummy    = 0.0f;
        curVertex.alpha    = curCurves[ParticleAlpha];
        curVertex.scale    = curCurves[ParticleScale] * curParticle->sizeVariation;
        curVertex.color    = curCurves[StaticRGBCurve];
        curVertex.pos      = Math::float4(curParticle->position.x()+curVertex.scale, 
                                            curParticle->position.y()-curVertex.scale,
                                            curParticle->position.z(),1);
        *curVertexPtr++ = curVertex;

        // second triangle
        
        
        curVertex.u = curParticle->uvmax.x();
        curVertex.v = curParticle->uvmin.y();
        curVertex.cornerRotation = curParticle->rotation + PI/4.0f + PI/2.0f;
        curVertex.rotation = curParticle->rotation;
        curVertex.dummy    = 0.0f;
        curVertex.alpha    = curCurves[ParticleAlpha];
        curVertex.scale    = curCurves[ParticleScale] * curParticle->sizeVariation;
        curVertex.color    = curCurves[StaticRGBCurve];
        curVertex.pos      = Math::float4(curParticle->position.x()+curVertex.scale, 
                                            curParticle->position.y()-curVertex.scale,
                                            curParticle->position.z(),1);
        *curVertexPtr++ = curVertex;
        
        
        curVertex.u = curParticle->uvmin.x();
        curVertex.v = curParticle->uvmax.y();
        curVertex.cornerRotation = curParticle->rotation + PI/4.0f + 3 * PI/2.0f;
        curVertex.rotation = curParticle->rotation;
        curVertex.dummy    = 0.0f;
        curVertex.alpha    = curCurves[ParticleAlpha];
        curVertex.scale    = curCurves[ParticleScale] * curParticle->sizeVariation;
        curVertex.color    = curCurves[StaticRGBCurve];
        curVertex.pos      = Math::float4(curParticle->position.x()-curVertex.scale, 
                                            curParticle->position.y()-curVertex.scale,
                                            curParticle->position.z(),1);
        *curVertexPtr++ = curVertex;

        
        curVertex.u = curParticle->uvmin.x();
        curVertex.v = curParticle->uvmin.y();
        curVertex.cornerRotation = curParticle->rotation + PI/4.0f + 4 * PI/2.0f;
        curVertex.rotation = curParticle->rotation;
        curVertex.dummy    = 0.0f;
        curVertex.alpha    = curCurves[ParticleAlpha];
        curVertex.scale    = curCurves[ParticleScale] * curParticle->sizeVariation;
        curVertex.color    = curCurves[StaticRGBCurve];
        curVertex.pos      = Math::float4(curParticle->position.x()-curVertex.scale, 
                                            curParticle->position.y()+curVertex.scale,
                                            curParticle->position.z(),1);
        *curVertexPtr++ = curVertex;

        // check for vertex buffer overflow
        curVertexIndex += 6;
        if (curVertexIndex > (maxVertices - 6))
        {
			this->refParticleMesh->Swap(curVertexIndex, dstVertices);
			curVertexPtr = (ParticleVertex*) dstVertices;
            curVertexIndex = 0;
        }

      

        curParticle += particlePitch;
    }

    return curVertexIndex;
}

//------------------------------------------------------------------------------
/**
    Render stretched particles (without smoothing).
*/
int nParticle2Emitter::RenderStretchedParticles(float* dstVertices, int maxVertices)
{
    nParticle2* curParticle = this->particles;
    int particlePitch = 1;
    if(this->renderOldestFirst)
    {
        // reverse iterating order
        particlePitch = -1;
        curParticle = &this->particles[this->particleCount -1];
    };

    float viewFadeOut = 0.0f;
    if (this->viewAngleFade) 
    {
        viewFadeOut = 256.0f;
    }

    // ok, let's stretch
    int curveIndex;
    float* curCurves;
	Math::float4 stretchPos;
    int p;
    int curVertexIndex = 0;
    ParticleVertex* curVertexPtr = (ParticleVertex*) dstVertices;
    ParticleVertex curVertex;
    for (p = 0; p < particleCount ; p++)
    {
        // life-time-check is not needed, it is assured that the relative age is >=0 and <1
        curveIndex = (int)((curParticle->lifeTime * curParticle->oneDivMaxLifeTime) * (float)ParticleTimeDetail);
		curveIndex = Math::n_iclamp(curveIndex, 0, ParticleTimeDetail - 1);
        curCurves = &this->pStaticCurves[curveIndex*CurveTypeCount];

        float stretchTime = this->particleStretch;
        if (stretchTime > curParticle->lifeTime) 
        {
            stretchTime = curParticle->lifeTime;
        }
        stretchPos = curParticle->position - (curParticle->velocity - curParticle->acc * (stretchTime * 0.5f)) * (stretchTime * curCurves[ParticleVelocityFactor]);
        if (this->stretchToStart) 
        {
            stretchPos = curParticle->startPos;
        }

        // setup vertex components which are identical for the entire particle
        curVertex.velocity = curParticle->position - stretchPos;
        curVertex.rotation = 0.0f;
        curVertex.dummy    = 0.0f;
        curVertex.alpha    = curCurves[ParticleAlpha] + viewFadeOut;
        curVertex.scale    = curCurves[ParticleScale] * curParticle->sizeVariation;
        curVertex.color    = curCurves[StaticRGBCurve];

        // first triangle
        curVertex.pos = curParticle->position;
        curVertex.u   = curParticle->uvmax.x();
        curVertex.v   = curParticle->uvmin.y();
        curVertex.cornerRotation = PI/4.0;
        *curVertexPtr++ = curVertex;

        curVertex.pos = stretchPos;
        curVertex.u   = curParticle->uvmax.x();
        curVertex.v   = curParticle->uvmax.y();
        curVertex.cornerRotation = PI/4.0 + PI/2.0;
        *curVertexPtr++ = curVertex;

        curVertex.pos = stretchPos;
        curVertex.u   = curParticle->uvmin.x();
        curVertex.v   = curParticle->uvmax.y();
        curVertex.cornerRotation = PI/4.0 + 2 * PI/2.0;
        *curVertexPtr++ = curVertex;

        // second triangle
        curVertex.pos = curParticle->position;
        curVertex.u   = curParticle->uvmax.x();
        curVertex.v   = curParticle->uvmin.y();
        curVertex.cornerRotation = PI/4.0f;
        *curVertexPtr++ = curVertex;

        curVertex.pos = stretchPos;
        curVertex.u   = curParticle->uvmin.x();
        curVertex.v   = curParticle->uvmax.y();
        curVertex.cornerRotation = PI/4.0f + 2 * PI/2.0f;
        *curVertexPtr++ = curVertex;

        curVertex.pos = curParticle->position;
        curVertex.u   = curParticle->uvmin.x();
        curVertex.v   = curParticle->uvmin.y();
        curVertex.cornerRotation = PI/4.0f + 3 * PI/2.0f;
        *curVertexPtr++ = curVertex;
 
        // check for vertex buffer overflow
        curVertexIndex += 6;
        if (curVertexIndex > (maxVertices - 6))
        {
            this->refParticleMesh->Swap(curVertexIndex, dstVertices);
            curVertexPtr = (ParticleVertex*) dstVertices;
            curVertexIndex = 0;
        }
        curParticle += particlePitch;
    }
    return curVertexIndex;
}

//------------------------------------------------------------------------------
/**
    Render stretched and smoothed particles.
*/
int 
nParticle2Emitter::RenderStretchedSmoothParticles(float* dstVertices, int maxVertices)
{
    nParticle2* curParticle = this->particles;
    int particlePitch = 1;
    if(this->renderOldestFirst)
    {
        // reverse iterating order
        particlePitch = -1;
        curParticle = &this->particles[this->particleCount -1];
    };
    float oneDivStretchDetail = 1.0f / (float)this->stretchDetail;

    // ok, let's stretch
	Math::float4 stretchPos;
	Math::float4 velPitch;
	Math::float4 velPitchHalf;

    // set coded flag for viewangle fading
    float viewFadeOut = 0.0f;
    if (this->viewAngleFade) 
    {
        viewFadeOut = 256.0f;
    }

    int curveIndex;
    float* curCurves;
    int p;
    int curVertexIndex = 0;
    ParticleVertex* curVertexPtr = (ParticleVertex*) dstVertices;
    ParticleVertex curVertex;
    for (p = 0; p < particleCount ; p++)
    {
        // life-time-check is not needed, it is assured that the relative age is >=0 and <1
        curveIndex = (int)((curParticle->lifeTime * curParticle->oneDivMaxLifeTime) * (float)ParticleTimeDetail);
		curveIndex = Math::n_iclamp(curveIndex, 0, ParticleTimeDetail - 1);
        curCurves = &this->pStaticCurves[curveIndex*CurveTypeCount];

        // calculate stretch steps
        float stretchTime = this->particleStretch;
        if (stretchTime > curParticle->lifeTime) 
        {
            stretchTime = curParticle->lifeTime;
        }
        float stretchStep = -(stretchTime * oneDivStretchDetail);
        velPitch = curParticle->acc * stretchStep;
        velPitchHalf = velPitch * 0.5f;
        float stretchStepVel = stretchStep * curCurves[ParticleVelocityFactor];

        float scale = curCurves[ParticleScale] * curParticle->sizeVariation;
        float vPitch = (curParticle->uvmax.y() - curParticle->uvmin.y()) * oneDivStretchDetail;
       
        curVertex.color = curCurves[StaticRGBCurve];
        curVertex.v = curParticle->uvmin.y();
        curVertex.pos = curParticle->position;
        curVertex.velocity = curParticle->velocity;
        curVertex.alpha = curCurves[ParticleAlpha] + viewFadeOut;
        curVertex.scale = scale;
        curVertex.rotation = 0.0;

        // FIXME: this is very slow because it doesn't write 
        // sequentially to write-combined memory (and it even reads back values... BAAAAD)
        int d;
        for(d = 0; d < this->stretchDetail; d++)
        {
            curVertex.u = curParticle->uvmin.x();
            curVertex.cornerRotation = 3.0f*PI/2.0f;
            curVertexPtr[0] = curVertex;
            curVertexPtr[3] = curVertex;

            curVertex.cornerRotation = PI/2.0f;
            curVertex.u = curParticle->uvmax.x();
            curVertexPtr[5] = curVertex;

            curVertex.cornerRotation = 3.0f*PI/2.0f;
            curVertex.u = curParticle->uvmin.x();
            curVertex.v += vPitch;
            curVertex.pos += (curVertex.velocity + velPitchHalf) * stretchStepVel;
            curVertex.velocity += velPitch;
            curVertexPtr[1] = curVertex;

            curVertex.cornerRotation = PI/2.0f;
            curVertex.u = curParticle->uvmax.x();
            curVertexPtr[2] = curVertex;
            curVertexPtr[4] = curVertex;

            // check for vertex buffer overflow
            curVertexIndex += 6;
            curVertexPtr += 6;
            if (curVertexIndex > (maxVertices - 6))
            {
                this->refParticleMesh->Swap(curVertexIndex, dstVertices);
                curVertexPtr = (ParticleVertex*) dstVertices;
                curVertexIndex = 0;
            }
        }
        curParticle += particlePitch;
    }
    return curVertexIndex;
}

//------------------------------------------------------------------------------
/**
    Helper method to pre-sample envelope curves into a key array.
*/
void
nParticle2Emitter::PresampleCurves(const EnvelopeCurve* envCurveArray, const Vector3EnvelopeCurve* rgbCurve, float* dstKeyArray)
{
    n_assert(0 != envCurveArray);
    n_assert(0 != rgbCurve);
    n_assert(0 != dstKeyArray);

    int c, s;
    for (s = 0; s < ParticleTimeDetail; s++)
    {
        for (c = 0; c < CurveTypeCount; c++)
        {
            dstKeyArray[s * CurveTypeCount + c] = envCurveArray[c].GetValue((float)s / (float) ParticleTimeDetail);
        }
    }

    // encode colors
    for (s = 0; s < ParticleTimeDetail; s++)
    {
		Math::float4 col = rgbCurve->GetValue((float)s / (float)ParticleTimeDetail);
        dstKeyArray[s * CurveTypeCount + StaticRGBCurve] = (float)((((uint)(col.x()*255.0f)) << 16) |
                                                                    (((uint)(col.y()*255.0f)) << 8) |
                                                                    (((uint)(col.z()*255.0f))));
    }

    // encode alpha values from [0,1] to [0,255] 
    for (s = 0; s < ParticleTimeDetail ; s++)
    {
        dstKeyArray[s * CurveTypeCount + ParticleAlpha] = (float)(((int)(dstKeyArray[s * CurveTypeCount + ParticleAlpha] * 255.0f)));
    }
}
}; //namespace Nebula2