//------------------------------------------------------------------------------
//  statenode.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/nodes/statenode.h"
#include "models/nodes/statenodeinstance.h"
#include "coregraphics/shaderserver.h"
#include "coregraphics/shadervariable.h"
#include "coregraphics/renderdevice.h"
#include "models/attributes.h"
#include "resources/resourcemanager.h"

namespace Models
{
__ImplementClass(Models::StateNode, 'STND', Models::TransformNode);

using namespace Attr;
using namespace Util;
using namespace CoreGraphics;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
StateNode::StateNode()
{
    // empty    
}

//------------------------------------------------------------------------------
/**
*/
StateNode::~StateNode()
{
    n_assert(!this->shaderInstance.isvalid());
    n_assert(this->managedTextureVariables.IsEmpty());
}

//------------------------------------------------------------------------------
/**
*/
Ptr<ModelNodeInstance>
StateNode::CreateNodeInstance() const
{
    Ptr<ModelNodeInstance> newInst = (ModelNodeInstance*) StateNodeInstance::Create();
    return newInst;
}

//------------------------------------------------------------------------------
/**
*/
Resource::State
StateNode::GetResourceState() const
{
    Resource::State state = TransformNode::GetResourceState();
    IndexT i;
    for (i = 0; i < this->managedTextureVariables.Size(); i++)
    {
        const Ptr<ManagedTexture>& managedTexture = this->managedTextureVariables[i].managedTexture;
        if (managedTexture->GetState() > state)
        {
            state = managedTexture->GetState();
        }
    }
    return state;
}

//------------------------------------------------------------------------------
/**
*/
void
StateNode::LoadResources()
{
    n_assert(!this->shaderInstance.isvalid());
    n_assert(this->HasAttr(Attr::Shader));
    ShaderServer::ShaderParamBindMode paramBindMode = ShaderServer::Instance()->GetShaderParamBindMode();

    // create a new shader instance from the Shader attribute
    const ResourceId& resId = this->GetString(Attr::Shader);
    this->shaderInstance = ShaderServer::Instance()->CreateShaderInstance(resId);

    // iterate through shader variables and set their attribute values on the shader
    IndexT i;
    for (i = 0; i < this->shaderInstance->GetNumVariables(); i++)
    {
        const Ptr<ShaderVariable>& var = this->shaderInstance->GetVariableByIndex(i);
        
        // convert the shader variable name into an attribute id
        const String& bindName = (paramBindMode == ShaderServer::BindBySemantic) ? var->GetSemantic().Value() : var->GetName().Value();
        if (AttrId::IsValidName(bindName))
        {
            AttrId attrId(bindName);
            if (this->HasAttr(attrId))
            {
                switch (var->GetType())
                {
                    case ShaderVariable::IntType:
                        var->SetInt(this->GetInt(attrId));
                        break;

                    case ShaderVariable::FloatType:
                        var->SetFloat(this->GetFloat(attrId));
                        break;

                    case ShaderVariable::BoolType:
                        var->SetBool(this->GetBool(attrId));
                        break;

                    case ShaderVariable::VectorType:
                        var->SetVector(this->GetFloat4(attrId));
                        break;

                    case ShaderVariable::MatrixType:
                        var->SetMatrix(this->GetMatrix44(attrId));
                        break;

                    case ShaderVariable::TextureType:
                        this->SetupManagedTextureVariable(attrId, var);
                        break;
                }
            }
        }
    }
    ModelNode::LoadResources();
}

//------------------------------------------------------------------------------
/**
*/
void
StateNode::UnloadResources()
{
    n_assert(this->shaderInstance.isvalid());

    // discard managed textures
    ResourceManager* resManager = ResourceManager::Instance();
    IndexT i;
    for (i = 0; i < this->managedTextureVariables.Size(); i++)
    {
        resManager->DiscardManagedResource(this->managedTextureVariables[i].managedTexture.upcast<ManagedResource>());
    }
    this->managedTextureVariables.Clear();

    // discard shader instance
    this->shaderInstance->Discard();
    this->shaderInstance = 0;
    ModelNode::UnloadResources();
}

//------------------------------------------------------------------------------
/**
    Create a new managed texture resource and bind it to the provided
    shader variable.
*/
void
StateNode::SetupManagedTextureVariable(const AttrId& attrId, const Ptr<ShaderVariable>& var)
{
    ResourceId resId = this->GetString(attrId);
    Ptr<ManagedTexture> managedTexture = ResourceManager::Instance()->CreateManagedResource(Texture::RTTI, resId).downcast<ManagedTexture>();
    this->managedTextureVariables.Append(ManagedTextureVariable(managedTexture, var));
}

//------------------------------------------------------------------------------
/**
    This method transfers texture from our managed texture objects into
    their associated shader variable. This is necessary since
    the actual texture of a managed texture may change from frame to frame
    because of resource management.
*/
void
StateNode::UpdateManagedTextureVariables()
{
    // @todo: don't update if contained texture hasn't changed,
    // should be a method of ManagedResource
    IndexT i;
    for (i = 0; i < this->managedTextureVariables.Size(); i++)
    {
        const Ptr<ManagedTexture>& tex = this->managedTextureVariables[i].managedTexture;
        const Ptr<ShaderVariable>& var = this->managedTextureVariables[i].shaderVariable;
        var->SetTexture(tex->GetTexture());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
StateNode::ApplySharedState()
{
    // up to parent class
    TransformNode::ApplySharedState();

    // apply managed textures (actually contained texture may have changed)
    this->UpdateManagedTextureVariables();

    // set our shader instance as current
    ShaderServer::Instance()->SetActiveShaderInstance(this->shaderInstance);
}

} // namespace Models
