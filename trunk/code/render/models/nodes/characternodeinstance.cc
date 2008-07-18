//------------------------------------------------------------------------------
//  shapenodeinstance.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/nodes/characternodeinstance.h"
#include "coregraphics/renderdevice.h"
#include "models/nodes/characternode.h"
#include "models/modelinstance.h"
#include "nebula2/anim/managedanimation.h"
#include "timing/timer.h"
#include "coregraphics/shaperenderer.h"

namespace Models
{
ImplementClass(Models::CharacterNodeInstance, 'CHNI', Models::TransformNodeInstance);

using namespace CoreGraphics;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
CharacterNodeInstance::CharacterNodeInstance()
{
}

//------------------------------------------------------------------------------
/**
*/
CharacterNodeInstance::~CharacterNodeInstance()
{
}

//------------------------------------------------------------------------------
/**
*/
void 
CharacterNodeInstance::OnAttachToModelInstance(const Ptr<ModelInstance>& inst, const Ptr<ModelNode>& node, const Ptr<ModelNodeInstance>& parentNodeInst)
{
    TransformNodeInstance::OnAttachToModelInstance(inst, node, parentNodeInst);
    n_assert(node->IsA(CharacterNode::RTTI));

    this->ValidateCharacter();
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterNodeInstance::ValidateCharacter()
{
    const Ptr<CharacterNode>& charNode = this->GetModelNode().downcast<CharacterNode>();
    if (!this->character.isvalid() && charNode->GetManagedAnimation()->GetState() == Resources::Resource::Loaded)
    {        
        // create character
        this->character = Char::Character::Create();
        // set character variables from char of characternode for use as instance character
        this->character->Set(charNode->GetCharacter());

        // create character set and copy nodes values (variations and skininfos)
        this->characterSet = Char::CharacterSet::Create();

        this->character->SetAnimation(charNode->GetManagedAnimation()->GetAnimation());
        this->character->SetupClipScheduler(charNode);    
        this->character->SetAnimEnabled(true);
        this->character->GetClipScheduler()->FadeClipAt(0, this->GetModelInstance()->GetTime(), Anim::AnimClipScheduler::FadeIn, 1.0);
        const Ptr<Char::CharacterSet>& nodeCharacterSet = charNode->GetCharacterSet();
        nodeCharacterSet->CopyTo(this->characterSet.get());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterNodeInstance::OnRemoveFromModelInstance()
{
    this->character = 0;
    this->characterSet = 0;

    TransformNodeInstance::OnRemoveFromModelInstance();
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterNodeInstance::Update()
{    
    TransformNodeInstance::Update();

    this->ValidateCharacter();

    const Ptr<CharacterNode>& charNode = GetModelNode().downcast<CharacterNode>();
    if (charNode->GetManagedAnimation()->GetState() == Resources::Resource::Loaded)
    {
        // get the sample time from the render context
        Timing::Time curTime = this->GetModelInstance()->GetTime();

        // check if characterset has to be applied
        if (characterSet->IsVariationDirty())
        {
            characterSet->ApplyCurrentVariation();
        }

        // evaluate the current state of the character skeleton
        this->character->EvaluateSkeleton(curTime);
        // make character dirty
        this->character->SetFrameDirty();

        // apply characterset if dirty
        this->characterSet->ApplySkinVisibility(this);
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
CharacterNodeInstance::RenderDebug()
{
    // FIXME:
    //// render the joints of the skeleton
    //int numJoints = this->character->GetSkeleton().GetNumJoints();
    //int i;

    //const Math::matrix44& modelMatrix = this->GetModelTransform();
    //Util::Array<point> lines;
    //CoreGraphics::ShapeRenderer* shapeRenderer = CoreGraphics::ShapeRenderer::Instance();

    ////this->character->EvaluateSkeleton(0.0, true);

    //for (i = 0; i < numJoints; i++)
    //{
    //    // add a joint
    //    lines.Clear();
    //    const Char::CharJoint& joint = this->character->GetSkeleton().GetJointAt(i);
    //    if (joint.GetParentJointIndex() != -1)
    //    {
    //        Char::CharJoint* parentJoint = joint.GetParentJoint();
    //        n_assert(parentJoint);

    //        // add start point
    //        lines.Append(parentJoint->GetMatrix().pos_component());

    //        // add end point
    //        lines.Append(joint.GetMatrix().pos_component());

    //        // draw
    //        float blue = (1.f / numJoints * (i+1));
    //        float red = (1.f - 1.f / numJoints * (i+1));
    //        float4 color(red, 1.f, blue, 1.f);
    //        shapeRenderer->DrawPrimitives(modelMatrix, CoreGraphics::PrimitiveTopology::LineList, lines.Size(), (float*)&(lines[0]), 3, color);
    //    }
    //}
}

} // namespace Models
