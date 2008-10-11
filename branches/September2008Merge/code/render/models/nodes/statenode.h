#pragma once
#ifndef MODELS_STATENODE_H
#define MODELS_STATENODE_H
//------------------------------------------------------------------------------
/**
    @class Models::StateNode
  
    A model node which holds shader state information and applies shared shader
    state for its set of StateNodeInstances.
    
    (C) 2007 Radon Labs GmbH
*/
#include "models/nodes/transformnode.h"
#include "coregraphics/shaderinstance.h"
#include "resources/managedtexture.h"

//------------------------------------------------------------------------------
namespace Models
{
class StateNode : public TransformNode
{
    __DeclareClass(StateNode);
public:
    /// constructor
    StateNode();
    /// destructor
    virtual ~StateNode();

    /// get overall state of contained resources (Initial, Loaded, Pending, Failed, Cancelled)
    virtual Resources::Resource::State GetResourceState() const;
    /// apply state shared by all my ModelNodeInstances
    virtual void ApplySharedState();
    /// get pointer to contained shader instance
    const Ptr<CoreGraphics::ShaderInstance>& GetShaderInstance() const;

protected:
    /// create a model node instance
    virtual Ptr<ModelNodeInstance> CreateNodeInstance() const;
    /// called when resources should be loaded
    virtual void LoadResources();
    /// called when resources should be unloaded
    virtual void UnloadResources();
    /// setup a new managed texture variable
    void SetupManagedTextureVariable(const Attr::AttrId& resAttrId, const Ptr<CoreGraphics::ShaderVariable>& var);
    /// update managed texture variables
    void UpdateManagedTextureVariables();

    class ManagedTextureVariable
    {
    public:
        /// default constructor
        ManagedTextureVariable() {};
        /// constructor
        ManagedTextureVariable(const Ptr<Resources::ManagedTexture>& tex, const Ptr<CoreGraphics::ShaderVariable>& var) :
            managedTexture(tex),
            shaderVariable(var)
            { };

            Ptr<Resources::ManagedTexture> managedTexture;
            Ptr<CoreGraphics::ShaderVariable> shaderVariable;
    };

    Ptr<CoreGraphics::ShaderInstance> shaderInstance;
    Util::Array<ManagedTextureVariable> managedTextureVariables;
};

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::ShaderInstance>&
StateNode::GetShaderInstance() const
{
    return this->shaderInstance;
}

} // namespace Models
//------------------------------------------------------------------------------
#endif
