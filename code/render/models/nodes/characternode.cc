//------------------------------------------------------------------------------
//  TransformNode.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/nodes/characternode.h"
#include "models/nodes/characternodeinstance.h"
#include "models/attributes.h"
#include "resources/resourcemanager.h"
#include "coregraphics/renderdevice.h"
#include "addons/nebula2/nebula2wrapper.h"

namespace Attr
{
    DefineString(SkinCategory, 'SKCA', ReadWrite);
}

namespace Models
{
ImplementClass(Models::CharacterNode, 'CHAN', Models::TransformNode);

using namespace Util;
using namespace Attr;
using namespace Math;
using namespace Resources;
using namespace CoreGraphics;
using namespace Char;
using namespace Anim;

//------------------------------------------------------------------------------
/**
*/
CharacterNode::CharacterNode()
{
    // create new character
    this->character = Character::Create();
}

//------------------------------------------------------------------------------
/**
*/
CharacterNode::~CharacterNode()
{
}

//------------------------------------------------------------------------------
/**
*/
Ptr<ModelNodeInstance>
CharacterNode::CreateNodeInstance() const
{    
    Ptr<ModelNodeInstance> newInst = (ModelNodeInstance*) CharacterNodeInstance::Create();
    return newInst;
}

//------------------------------------------------------------------------------
/**
*/
Resource::State
CharacterNode::GetResourceState() const
{
    Resource::State state = TransformNode::GetResourceState();
    // check managed animation state
    if (this->managedAnimation->GetState() > state)
    {
        state = this->managedAnimation->GetState();
    }
    return state;
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterNode::LoadResources()
{
    n_assert(!this->characterSet.isvalid());
    n_assert(!this->managedAnimation.isvalid());

    // create a managed animation resource
    n_assert(this->animName.IsValid());
    this->managedAnimation = AnimationServer::Instance()->NewMemoryAnimation(this->animName);
    // set animation in character if already loaded
    this->character->SetAnimation(this->managedAnimation->GetAnimation());    

    // create new characterset and load its skinlists from file on initialize
    this->characterSet = CharacterSet::Create();
    // collect skins from modelnodehierarchy
    this->SetupSkinInfos();
    this->characterSet->Initialize(this, this->character);
    
    // load variation animation
    this->LoadVariation();

    // call parent class, creates mesh
    TransformNode::LoadResources();
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterNode::UnloadResources()
{
    n_assert(this->characterSet.isvalid());
    n_assert(this->managedAnimation.isvalid());

    // discard managed resource
    ResourceManager::Instance()->DiscardManagedResource(this->managedAnimation.upcast<ManagedResource>());    
    this->managedAnimation = 0;
    this->characterSet = 0;

    // call parent class
    TransformNode::UnloadResources();
}

//------------------------------------------------------------------------------
/**
    Begin defining variations.
*/
void
CharacterNode::BeginVariations(SizeT num)
{
    this->variations.SetSize(num);
    this->variationIndexMap.Clear();
}

//------------------------------------------------------------------------------
/**
    Define a variation.
*/
void
CharacterNode::SetVariation(IndexT varIndex, IndexT animGroupIndex, const Util::String& varName)
{
    this->variations[varIndex].name = varName;
    this->variations[varIndex].animGroupIndex = animGroupIndex;
    this->variations[varIndex].joints.Clear();
}

//------------------------------------------------------------------------------
/**
    End defining variations.
*/
void
CharacterNode::EndVariations()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Util::Array<CharJoint> 
CharacterNode::EvaluateVariation(const Ptr<Animation>& varAnim, IndexT groupIndex)
{
    // force loop type to clamp
    varAnim->GetGroupAt(groupIndex).SetLoopType(Animation::Group::Clamp);

    // setup clip scheduler for sampling
    AnimClipScheduler* clipScheduler = this->character->GetClipScheduler();
    n_assert(0 != clipScheduler);
    
    // set variation clip on scheduler
    clipScheduler->BeginClips(1);
    clipScheduler->SetClip(0, "variationClip", 1.0f, 1.0f, 
                           (float)varAnim->GetGroupAt(groupIndex).GetDuration(), 
                           0.0f, 0.0f, groupIndex, Animation::Group::Clamp, 
                           AnimClipScheduler::CompleteSkeleton);
    clipScheduler->EndClips();
    
    // set variation as animation and evaluate the skeleton
    this->character->SetAnimation(varAnim);
    this->character->EvaluateSkeleton(0.0f, true);

    int i;
    int numJoints = this->character->GetSkeleton().GetNumJoints();
    Util::Array<CharJoint> result(numJoints, 0);
    for (i = 0; i < numJoints; i++)
    {
        CharJoint &joint = this->character->GetSkeleton().GetJointAt(i);
        result.Append(joint);
    }
    this->character->SetAnimation(this->managedAnimation->GetAnimation());
    return result;
}

//------------------------------------------------------------------------------
/**
*/
bool 
CharacterNode::LoadVariation()
{
    // load variations
    if ((this->variations.Size() > 0) && (!this->variationUri.IsEmpty()))
    {
        Ptr<Animation> varAnim = AnimationServer::Instance()->NewMemoryAnimation(this->variationUri)->GetAnimation();        
        n_assert(varAnim && !varAnim->IsLoaded());        
        if (!varAnim->Load())
        {
            n_printf("CharacterNode: Error loading variation anim file '%s'\n", this->variationUri.AsCharPtr());            
            return false;
        }
        
        // evaluate variations
        IndexT varIndex;
        for (varIndex = 0; varIndex < this->variations.Size(); varIndex++)
        {
            this->variations[varIndex].joints = this->EvaluateVariation(varAnim, varIndex);
        }

        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
    Setup the skin info table from my child skin objects.
*/
void
CharacterNode::SetupSkinInfos()
{
    // iterate through children, every transformnode is a skin
    const Array<Ptr<ModelNode> >& children = GetChildren();
    uint i;
    for (i = 0; i < children.Size(); i++)
    {
        if (children[i]->IsA(TransformNode::RTTI))
        {
            SkinInfo skinInfo;
            skinInfo.skinNode = children[i].downcast<TransformNode>();
            skinInfo.name = children[i]->GetName().Value();
            skinInfo.name.Strip("|");

            const Array<Ptr<ModelNode> >& skinChildren = children[i]->GetChildren();
            uint j;
            for (j = 0; j < skinChildren.Size(); j++)
            {                
                if (skinChildren[j]->GetName() == "model" && skinChildren[j]->HasAttr(Attr::SkinCategory))
                {  
                    skinInfo.category = skinChildren[j]->GetString(Attr::SkinCategory);
                }
            }
            this->skinInfos.Append(skinInfo);
            this->skinIndexMap.Add(skinInfo.name, this->skinInfos.Size() - 1);
        }
    }
}


//------------------------------------------------------------------------------
/**
*/
void
CharacterNode::BeginClips(int numClips)
{
    this->clips.SetSize(numClips);
    this->clipIndexMap.Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterNode::SetClip(int clipIndex, int animGroupIndex, const Util::String& clipName)
{
    // number of anim curves in a clip is identical to number of (joints * 3)
    // (one curve for translation, rotation and scale)
    const int numCurves = this->character->GetSkeleton().GetNumJoints() * 3;
    n_assert(numCurves > 0);

    AnimClip newClip(clipName, animGroupIndex, numCurves);
    this->clips[clipIndex] = newClip;
    this->clipIndexMap.Add(clipName, clipIndex);
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterNode::EndClips()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
int
CharacterNode::GetNumClips() const
{
    return this->clips.Size();
}

//------------------------------------------------------------------------------
/**
*/
const AnimClip&
CharacterNode::GetClipAt(int clipIndex) const
{
    return this->clips[clipIndex];
}

//------------------------------------------------------------------------------
/**
*/
Timing ::Time
CharacterNode::GetClipDuration(int clipIndex) const
{
    const AnimClip& clip = this->GetClipAt(clipIndex);
    return this->managedAnimation->GetAnimation()->GetGroupAt(clip.GetAnimGroupIndex()).GetDuration();
}

} // namespace Models