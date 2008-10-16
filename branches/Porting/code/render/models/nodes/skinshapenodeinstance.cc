//------------------------------------------------------------------------------
//  shapenodeinstance.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/nodes/skinshapenodeinstance.h"
#include "coregraphics/renderdevice.h"
#include "coregraphics/shaderserver.h"
#include "coregraphics/transformdevice.h"
#include "models/nodes/SkinShapeNode.h"
#include "models/modelinstance.h"
#include "models/nodes/characternodeinstance.h"
#include "addons/nebula2/character/characterrenderer.h"
#include "timing/timer.h"

namespace Models
{
__ImplementClass(Models::SkinShapeNodeInstance, 'SSPI', Models::ShapeNodeInstance);

using namespace CoreGraphics;
using namespace Math;
using namespace Char;

int SkinShapeNodeInstance::curBuffer = 0;
    
//------------------------------------------------------------------------------
/**
*/
SkinShapeNodeInstance::SkinShapeNodeInstance()
{
}

//------------------------------------------------------------------------------
/**
*/
SkinShapeNodeInstance::~SkinShapeNodeInstance()
{
}

//------------------------------------------------------------------------------
/**
*/
void 
SkinShapeNodeInstance::OnAttachToModelInstance(const Ptr<ModelInstance>& inst, const Ptr<ModelNode>& node, const Ptr<ModelNodeInstance>& parentNodeInst)
{    
    ShapeNodeInstance::OnAttachToModelInstance(inst, node, parentNodeInst);
 
    this->ValidateCharacter();

    n_assert(node->IsA(SkinShapeNode::RTTI));
    const Ptr<SkinShapeNode> skinNode = node.cast<SkinShapeNode>();        
    ShaderVariable::Semantic paletteSemantic = ShaderVariable::Semantic("JointPalette");
    Ptr<ShaderVariable> shdVar;
    if  (skinNode->GetShaderInstance()->HasVariableBySemantic(paletteSemantic))
    {        
        shdVar = skinNode->GetShaderInstance()->GetVariableBySemantic(paletteSemantic);
    }    
    n_assert(!this->charRenderer.isvalid());
    this->charRenderer = CharacterRenderer::Create();        
    this->charRenderer->Setup(skinNode->GetManagedMesh()->GetMesh(), this->character, shdVar);    
}

//------------------------------------------------------------------------------
/**
*/
void
SkinShapeNodeInstance::Update()
{
    this->ValidateCharacter();
    ShapeNodeInstance::Update();
}

//------------------------------------------------------------------------------
/**
*/
void
SkinShapeNodeInstance::ValidateCharacter()
{
    if (!this->character.isvalid())
    {
        // get character from characternodeinstance
        // find characternodeinstance
        int searchLevel = 4;
        Ptr<ModelNodeInstance> parent = this->GetParent();
        while (!parent->IsA(CharacterNodeInstance::RTTI) && searchLevel > 0)
        {
            parent = parent->GetParent();
            searchLevel--;
        }
        n_assert2(parent->IsA(CharacterNodeInstance::RTTI), "No CharacterNodeInstance found as parent!");
        this->character = parent.downcast<CharacterNodeInstance>()->GetCharacter();
        n_assert(this->character.isvalid());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
SkinShapeNodeInstance::OnRemoveFromModelInstance()
{
    this->charRenderer = 0;
    this->character = 0;    

    ShapeNodeInstance::OnRemoveFromModelInstance();
}

//------------------------------------------------------------------------------
/**
*/
void
SkinShapeNodeInstance::Render()
{
    // is mesh loaded for skinning?
    const Ptr<SkinShapeNode>& skinShapeNode = GetModelNode().downcast<SkinShapeNode>();
    if (skinShapeNode->GetManagedMesh()->GetState() == Resources::Resource::Loaded)
    {
        // render skinning
        StateNodeInstance::Render();
        this->charRenderer->RenderSkinning(skinShapeNode->GetFragmentArray());
    }
    else
    {
        // unskinned placeholder has to be rendered
        ShapeNodeInstance::Render();
    }
}    

//------------------------------------------------------------------------------
/**
*/
void
SkinShapeNodeInstance::OnNotifyVisible(IndexT frameIndex)
{
    if (this->IsVisible())
    {
        // just tell our model node that we are a visible instance
        this->modelNode->AddVisibleNodeInstance(frameIndex, this);
    }
}

//------------------------------------------------------------------------------
/**
    Set visibility of node and its children, should not be call per frame!
*/
inline void 
SkinShapeNodeInstance::SetVisible(bool b)
{
    // @todo:
    if (b && !this->IsVisible())
    {
        //this->GetModelNode().downcast<SkinShapeNode>()->RequestLoadResources();
    }
    else if (this->IsVisible())
    {
        //this->GetModelNode().downcast<SkinShapeNode>()->RequestUnloadResources();
    }
    ShapeNodeInstance::SetVisible(b);
}

} // namespace Models
