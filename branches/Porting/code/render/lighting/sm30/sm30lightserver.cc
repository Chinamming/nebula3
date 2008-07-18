//------------------------------------------------------------------------------
//  sm30lightserver.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "lighting/sm30/sm30lightserver.h"
#include "coregraphics/shaderserver.h"
#include "resources/resourcemanager.h"
#include "lighting/shadowserver.h"

namespace Lighting
{
ImplementClass(Lighting::SM30LightServer, 'S3LS', Lighting::LightServerBase);

using namespace Math;
using namespace Util;
using namespace Graphics;
using namespace CoreGraphics;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
SM30LightServer::SM30LightServer() :
    shdFeatureMasks(MaxLights, 0)
{
    Memory::Clear(this->lightPos, sizeof(this->lightPos));
    Memory::Clear(this->lightInvRange, sizeof(this->lightInvRange));
    Memory::Clear(this->lightProjTransform, sizeof(this->lightProjTransform));
    Memory::Clear(this->lightColor, sizeof(this->lightColor));
    Memory::Clear(this->lightProjMapUvOffset, sizeof(this->lightProjMapUvOffset));
    Memory::Clear(this->lightType, sizeof(this->lightType));
    Memory::Clear(this->lightCastShadows, sizeof(this->lightCastShadows));
    Memory::Clear(this->lightShadowBufferUvOffset, sizeof(this->lightShadowBufferUvOffset));
}

//------------------------------------------------------------------------------
/**
*/
SM30LightServer::~SM30LightServer()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
SM30LightServer::Open()
{
    // call parent class
    LightServerBase::Open();

    // setup the shared light project map resource
    this->lightProjMap = ResourceManager::Instance()->CreateManagedResource(Texture::RTTI, ResourceId("tex:lighting/lightcones.dds")).downcast<ManagedTexture>();

    // setup shared shader variables
    ShaderServer* shdServer = ShaderServer::Instance();
    this->shdGlobalLightDir              = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic("GlobalLightDirection"));
    this->shdGlobalLightColor            = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic("GlobalLightColor"));
    this->shdGlobalBackLightColor        = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic("GlobalBackLightColor"));
    this->shdGlobalLightCastShadows      = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic("GlobalLightCastShadows"));
    this->shdGlobalLightPSSMDistances    = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic("GlobalLightPSSMDistances"));
    this->shdGlobalLightPSSMTransforms   = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic("GlobalLightPSSMTransforms"));
    this->shdGlobalLightPSSMShadowBuffer = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic("GlobalLightPSSMShadowBuffer"));
    this->shdLightPos                    = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic("LightPosition"));
    this->shdLightInvRange               = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic("LightInvRange"));
    this->shdLightProjTransform          = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic("LightProjTransform"));
    this->shdLightColor                  = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic("LightColor"));
    this->shdLightProjMapUvOffset        = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic("LightProjMapUvOffset"));
    this->shdLightType                   = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic("LightType"));
    this->shdLightCastShadows            = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic("LightCastShadows"));
    this->shdLightShadowBufferUvOffset   = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic("LightShadowBufferUvOffset"));
    this->shdLightShadowBuffer           = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic("LightShadowBuffer"));
    this->shdLightShadowBufferSize       = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic("LightShadowBufferSize"));
    this->shdLightProjMap                = shdServer->GetSharedVariableBySemantic(ShaderVariable::Semantic("LightProjMap"));

    // setup the shader feature masks for number of local lights
    // (this is used to select the right shader variation by number of lights)
    this->allLightsFeatureMask = 0;
    IndexT i;
    for (i = 0; i < MaxLights; i++)
    {
        String featureName;
        featureName.Format("LocalLights%d", i);
        this->shdFeatureMasks[i] = shdServer->FeatureStringToMask(featureName);
        this->allLightsFeatureMask |= this->shdFeatureMasks[i];
    }
}

//------------------------------------------------------------------------------
/**
*/
void
SM30LightServer::Close()
{
    // discard resources
    ResourceManager::Instance()->DiscardManagedResource(this->lightProjMap.upcast<ManagedResource>());
    this->lightProjMap = 0;

    // discared shader variables
    this->shdGlobalLightDir = 0;
    this->shdGlobalLightColor = 0;
    this->shdGlobalBackLightColor = 0;
    this->shdGlobalLightCastShadows = 0;
    this->shdGlobalLightPSSMDistances = 0;
    this->shdGlobalLightPSSMTransforms = 0;
    this->shdGlobalLightPSSMShadowBuffer = 0;
    this->shdLightPos = 0;
    this->shdLightInvRange = 0;
    this->shdLightProjTransform = 0;
    this->shdLightColor = 0;
    this->shdLightProjMapUvOffset = 0;
    this->shdLightType = 0;
    this->shdLightCastShadows = 0;
    this->shdLightShadowBufferUvOffset = 0;
    this->shdLightShadowBuffer = 0;
    this->shdLightShadowBufferSize = 0;
    this->shdLightProjMap = 0;

    // call parent class
    LightServerBase::Close();
}

//------------------------------------------------------------------------------
/**
    @todo: set light properties only once per-frame and only set a
    bool array with active per-model-entity-lights here!
*/
void
SM30LightServer::ApplyModelEntityLights(const Ptr<ModelEntity>& modelEntity)
{
    // get light entities which influence the model entity
    const Array<Ptr<GraphicsEntity> >& localLights = modelEntity->GetLinks(GraphicsEntity::LightLink);
        
    // @todo: if model influenced by more then max number of lights,
    // sort by priority and drop the least-priority-lights
    if (localLights.Size() > MaxLights)
    {
        // FIXME!
        n_error("Too many lights!");
    }

    // let each light source prepare for applying shader params
    SizeT numLights = localLights.Size();
    IndexT i;
    for (i = 0; i < numLights; i++)
    {
        const Ptr<AbstractLightEntity>& lightEntity = localLights[i].downcast<AbstractLightEntity>();
        this->lightPos[i]             = lightEntity->GetTransform().getrow3();
        this->lightInvRange[i]        = 1.0f / lightEntity->GetTransform().getrow2().length();
        this->lightProjTransform[i]   = lightEntity->GetInvLightProjTransform();
        this->lightColor[i]           = lightEntity->GetColor();
        this->lightProjMapUvOffset[i] = lightEntity->GetProjMapUvOffsetAndScale();
        this->lightType[i]            = lightEntity->GetLightType();
        if (lightEntity->GetCastShadows())
        {
            this->lightCastShadows[i] = true;
            this->lightShadowBufferUvOffset[i] = lightEntity->GetShadowBufferUvOffsetAndScale();
        }
        else
        {
            this->lightCastShadows[i] = false;
            this->lightShadowBufferUvOffset[i].set(0.5f, 0.5f, 0.5f, -0.5f);
        }
    }

    // apply shader parameters
    ShadowServer* shadowServer = ShadowServer::Instance();
    const Ptr<Texture>& sbTexture = shadowServer->GetLocalLightShadowBufferTexture();
    float4 sbSize(float(sbTexture->GetWidth()), float(sbTexture->GetHeight()), 0.0f, 0.0f);

    this->shdLightPos->SetVectorArray(this->lightPos, numLights);
    this->shdLightInvRange->SetFloatArray(this->lightInvRange, numLights);
    this->shdLightProjTransform->SetMatrixArray(this->lightProjTransform, numLights);
    this->shdLightColor->SetVectorArray(this->lightColor, numLights);
    this->shdLightProjMapUvOffset->SetVectorArray(this->lightProjMapUvOffset, numLights);
    this->shdLightType->SetIntArray((int*) this->lightType, numLights);
    this->shdLightCastShadows->SetBoolArray(this->lightCastShadows, numLights);
    this->shdLightShadowBufferUvOffset->SetVectorArray(this->lightShadowBufferUvOffset, numLights);
    this->shdLightShadowBuffer->SetTexture(sbTexture);
    this->shdLightShadowBufferSize->SetVector(sbSize);
    this->shdLightProjMap->SetTexture(this->lightProjMap->GetTexture());
    if (this->globalLightEntity.isvalid())
    {
        this->shdGlobalLightDir->SetVector(this->globalLightEntity->GetLightDirection());
        this->shdGlobalLightColor->SetVector(this->globalLightEntity->GetColor());
        this->shdGlobalBackLightColor->SetVector(this->globalLightEntity->GetBackLightColor());
        this->shdGlobalLightCastShadows->SetBool(this->globalLightEntity->GetCastShadows());
        this->shdGlobalLightPSSMShadowBuffer->SetTexture(shadowServer->GetGlobalLightShadowBufferTexture());
        this->shdGlobalLightPSSMDistances->SetFloatArray(shadowServer->GetPSSMSplitDistances(), PSSMUtil::NumSplits + 1);
        this->shdGlobalLightPSSMTransforms->SetMatrixArray(shadowServer->GetPSSMSplitLightProjTransforms(), PSSMUtil::NumSplits);
    }
    else
    {
        // no global light set, still need to setup shader variables
        this->shdGlobalLightDir->SetVector(-vector::upvec());
        this->shdGlobalLightColor->SetVector(vector::nullvec());
        this->shdGlobalBackLightColor->SetVector(vector::nullvec());
        this->shdGlobalLightCastShadows->SetBool(false);
    }

    // set feature bit mask for the number of light sources, this is used
    // to select the right shader variation for the number of active light sources
    ShaderServer* shdServer = ShaderServer::Instance();
    shdServer->ClearFeatureBits(this->allLightsFeatureMask);
    shdServer->SetFeatureBits(this->shdFeatureMasks[numLights]);
}

} // namespace Lighting
