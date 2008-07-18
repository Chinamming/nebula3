#include "stdneb.h"
#include "particlesystemnodeinstance.h"
#include "models/nodes/particlesystemnode.h"
#include "models/modelinstance.h"

//just for testing
#include "addons/nebula2/nebula2wrapper.h"

namespace Models
{
ImplementClass(Models::ParticleSystemNodeInstance, 'PSNI', Models::ShapeNodeInstance);

//------------------------------------------------------------------------------
/**
*/
ParticleSystemNodeInstance::ParticleSystemNodeInstance():
    particleEmitter(0)
{

}

//------------------------------------------------------------------------------
/**
*/
ParticleSystemNodeInstance::~ParticleSystemNodeInstance()
{
}

//------------------------------------------------------------------------------
/**
*/
void ParticleSystemNodeInstance::OnAttachToModelInstance(const Ptr<ModelInstance>& inst, 
                                 const Ptr<ModelNode>& node, 
                                 const Ptr<ModelNodeInstance>& parentNodeInst)
{
	ShapeNodeInstance::OnAttachToModelInstance(inst,node,parentNodeInst);
}

//------------------------------------------------------------------------------
/**
*/
void ParticleSystemNodeInstance::OnRemoveFromModelInstance()
{
	ShapeNodeInstance::OnRemoveFromModelInstance();
}

//------------------------------------------------------------------------------
/**
*/
void ParticleSystemNodeInstance::Update()
{
    this->particleEmitter->Update((float)this->GetModelInstance()->GetTime());
	ShapeNodeInstance::Update();
}

//------------------------------------------------------------------------------
/**
*/
void ParticleSystemNodeInstance::Render()
{
    this->particleEmitter->Render();
    ShapeNodeInstance::Render();
}

//------------------------------------------------------------------------------
/**
*/
void
ParticleSystemNodeInstance::OnNotifyVisible(IndexT frameIndex)
{
    // just tell our model node that we are a visible instance
    this->modelNode->AddVisibleNodeInstance(frameIndex, this);
}


};// namespace Models