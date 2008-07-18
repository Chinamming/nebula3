#include "stdneb.h"
#include "models/nodes/particlesystemnode.h"
#include "models/nodes/particlesystemnodeinstance.h"
#include "addons/nebula2/nebula2wrapper.h"


namespace Models
{
int ParticleSystemNode::numberOfCreatedNodes = 0;

ImplementClass(Models::ParticleSystemNode, 'PSND', Models::ShapeNode)


//------------------------------------------------------------------------------
/**
*/

ParticleSystemNode::ParticleSystemNode():
	emissionDuration(10.0),
    loop(true),
    activityDistance(10.0f),
    startRotationMin(0.0f),
    startRotationMax(0.0f),
    gravity(-9.81f),
    particleStretch(0.0f),
    particleVelocityRandomize(0.0f),
    particleRotationRandomize(0.0f),
    particleSizeRandomize(0.0f),
    precalcTime(0.0f),
    randomRotDir(false),
    tileTexture(0),
    stretchToStart(false),
    renderOldestFirst(true),
    invisible(false),
    viewAngleFade(false),
    stretchDetail(0),
    startDelay(0.0f),
    billboard(true),
    curvesValid(false),
    particleMesh(0)
{
}

//------------------------------------------------------------------------------
/**
*/
ParticleSystemNode::~ParticleSystemNode()
{
}

//------------------------------------------------------------------------------
/**
*/
Ptr<ModelNodeInstance>
ParticleSystemNode::CreateNodeInstance() const
{
    
    Ptr<ParticleSystem::ParticleEmitter> emitter = ParticleSystem::ParticleEmitter::Create();


    if (!emitter->IsStaticCurvePtrValid())
    {
        emitter->SetStaticCurvePtr((float*)&this->sampledCurves);
        emitter->NotifyCurvesChanged();
    }

    // setup emitter
    
    emitter->SetEmitterMeshGroupIndex(this->primGroupIndex);
    emitter->SetParticleMesh(this->particleMesh);
    emitter->SetEmitterMesh(this->GetManagedMesh()->GetMesh());
    emitter->SetGravity(this->gravity);
    emitter->SetTileTexture(this->tileTexture);
    emitter->SetStretchToStart(this->stretchToStart);
    emitter->SetParticleStretch(this->particleStretch);
    emitter->SetStartRotationMin(this->startRotationMin);
    emitter->SetStartRotationMax(this->startRotationMax);
    emitter->SetParticleVelocityRandomize(this->particleVelocityRandomize);
    emitter->SetParticleRotationRandomize(this->particleRotationRandomize);
    emitter->SetParticleSizeRandomize(this->particleSizeRandomize);
    emitter->SetRandomRotDir(this->randomRotDir);
    emitter->SetPrecalcTime(this->precalcTime);
    emitter->SetViewAngleFade(this->viewAngleFade);
    emitter->SetStretchDetail(this->stretchDetail);
    emitter->SetStartDelay(this->startDelay);
   
    // set emitter settings
    emitter->SetEmissionDuration(this->emissionDuration);
    emitter->SetLooping(this->loop);
    emitter->SetActivityDistance(this->activityDistance);
    emitter->SetRenderOldestFirst(this->renderOldestFirst);
    emitter->SetBillboardOrientation(this->billboard);
    emitter->SetIsSetup(true);
    n_assert(emitter.isvalid());

    Ptr<ParticleSystemNodeInstance> particleSystemNodeInstance = ParticleSystemNodeInstance::Create();
    particleSystemNodeInstance->SetEmitter(emitter);
	Ptr<ModelNodeInstance> newInst = (ModelNodeInstance*) particleSystemNodeInstance;
    
    return newInst;
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemNode::LoadResources()
{   
    // sample curves to speed up calculating
    ParticleSystem::ParticleEmitter::PresampleCurves(this->curves, &(this->rgbCurve), &(this->sampledCurves[0][0]));
    
    this->particleMesh = ParticleSystem::DynamicMesh::Create();
    n_assert(particleMesh);

	Util::Array<CoreGraphics::VertexComponent> vertexComponents;
	vertexComponents.Append(CoreGraphics::VertexComponent(CoreGraphics::VertexComponent::Position,0,CoreGraphics::VertexComponent::Float4));
	vertexComponents.Append(CoreGraphics::VertexComponent(CoreGraphics::VertexComponent::Normal,0,CoreGraphics::VertexComponent::Float4));
	vertexComponents.Append(CoreGraphics::VertexComponent(CoreGraphics::VertexComponent::TexCoord,0,CoreGraphics::VertexComponent::Float2));
	vertexComponents.Append(CoreGraphics::VertexComponent(CoreGraphics::VertexComponent::TexCoord,1,CoreGraphics::VertexComponent::Float2));
	vertexComponents.Append(CoreGraphics::VertexComponent(CoreGraphics::VertexComponent::Color,0,CoreGraphics::VertexComponent::Float4));
   
    //create resource ID
    Util::String resourceID = "particle_resource";
    resourceID.AppendInt(this->numberOfCreatedNodes);
    Util::Atom<Util::String> atomResourceID(resourceID);

    particleMesh->Initialize(atomResourceID,CoreGraphics::PrimitiveTopology::TriangleList,vertexComponents,false);
 
	ShapeNode::LoadResources();

    //node counter ++
    this->numberOfCreatedNodes++;
}

//------------------------------------------------------------------------------
/**
*/
void 
ParticleSystemNode::UnloadResources()
{
	ShapeNode::UnloadResources();
}
};// namespace Models