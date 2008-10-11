//------------------------------------------------------------------------------
//  framebatch.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "frame/framebatch.h"
#include "coregraphics/renderdevice.h"
#include "coregraphics/shaderserver.h"
#include "coregraphics/shaperenderer.h"
#include "coregraphics/textrenderer.h"
#include "models/visresolver.h"
#include "models/model.h"
#include "models/modelnodeinstance.h"
#include "internalgraphics/internalmodelentity.h"
#include "lighting/lightserver.h"

namespace Frame
{
__ImplementClass(Frame::FrameBatch, 'FBTH', Core::RefCounted);

using namespace InternalGraphics;
using namespace CoreGraphics;
using namespace Models;
using namespace Util;
using namespace Lighting;

//------------------------------------------------------------------------------
/**
*/
FrameBatch::FrameBatch() :
    batchType(BatchType::InvalidBatchType),
    nodeFilter(ModelNodeType::InvalidModelNodeType),
    lightingMode(LightingMode::None),
    sortingMode(SortingMode::None),
    shaderFeatures(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
FrameBatch::~FrameBatch()
{
    // make sure Discard() has been called
    n_assert(!this->shader.isvalid());
    n_assert(this->shaderVariables.IsEmpty());
}

//------------------------------------------------------------------------------
/**
*/
void
FrameBatch::Discard()
{
    if (this->shader.isvalid())
    {
        this->shader->Discard();
        this->shader = 0;
    }
    this->shaderVariables.Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
FrameBatch::Render()
{
    RenderDevice* renderDevice = RenderDevice::Instance();

    // apply shader variables
    IndexT varIndex;
    for (varIndex = 0; varIndex < this->shaderVariables.Size(); varIndex++)
    {
        this->shaderVariables[varIndex]->Apply();
    }

    // render the batch
    renderDevice->BeginBatch(this->batchType, this->shader);
    this->RenderBatch();
    renderDevice->EndBatch();
}

//------------------------------------------------------------------------------
/**
*/
void
FrameBatch::RenderBatch()
{
    ShaderServer* shaderServer = ShaderServer::Instance();
    VisResolver* visResolver = VisResolver::Instance();      
    LightServer* lightServer = LightServer::Instance(); 

    // handle special cases
    if (BatchType::Shapes == this->batchType)
    {
        ShapeRenderer::Instance()->DrawShapes();
    }
    else if (BatchType::Text == this->batchType)
    {
        TextRenderer::Instance()->DrawTextElements();
    }
    else
    {
        // default case: render models...
        const Array<Ptr<Model> >& models = visResolver->GetVisibleModels(this->nodeFilter);
        IndexT modelIndex;
        for (modelIndex = 0; modelIndex < models.Size(); modelIndex++)
        {
            // for each visible model node of the model...
            const Array<Ptr<ModelNode> >& modelNodes = visResolver->GetVisibleModelNodes(this->nodeFilter, models[modelIndex]);
            IndexT modelNodeIndex;  
            for (modelNodeIndex = 0; modelNodeIndex < modelNodes.Size(); modelNodeIndex++)
            {
                // apply render state which is shared by all instances
                shaderServer->ResetFeatureBits();
                shaderServer->SetFeatureBits(this->shaderFeatures);
                const Ptr<ModelNode>& modelNode = modelNodes[modelNodeIndex];            
                modelNode->ApplySharedState();

                // if lighting mode is Off, we can render all node instances with the same shader
                const Ptr<ShaderInstance>& shaderInst = shaderServer->GetActiveShaderInstance();
                if (LightingMode::None == this->lightingMode)
                {
                    shaderInst->SelectActiveVariation(shaderServer->GetFeatureBits());
                    SizeT numPasses = shaderInst->Begin();
                    n_assert(1 == numPasses);
                    shaderInst->BeginPass(0);
                }

                // render instances
                const Array<Ptr<ModelNodeInstance> >& nodeInstances = visResolver->GetVisibleModelNodeInstances(this->nodeFilter, modelNode);
                IndexT nodeInstIndex;
                for (nodeInstIndex = 0; nodeInstIndex < nodeInstances.Size(); nodeInstIndex++)
                {
                    const Ptr<ModelNodeInstance>& nodeInstance = nodeInstances[nodeInstIndex];

                    // if single-pass lighting is enabled, we need to setup the lighting 
                    // shader states
                    // FIXME: This may set a new shader variation for every node instance
                    // which is expensive! Would be better to sort node instances by number
                    // of active lights!!!
                    if (LightingMode::SinglePass == this->lightingMode)
                    {
                        // setup lighting render states
                        // NOTE: this may change the shader feature bit mask which may select
                        // a different shader variation per entity
                        const Ptr<InternalGraphics::InternalModelEntity>& modelEntity = nodeInstance->GetModelInstance()->GetModelEntity();
                        lightServer->ApplyModelEntityLights(modelEntity);
                        shaderInst->SelectActiveVariation(shaderServer->GetFeatureBits());
                        SizeT numPasses = shaderInst->Begin();
                        n_assert(1 == numPasses);
                        shaderInst->BeginPass(0);
                    }

                    // render the node instance
                    nodeInstance->ApplyState();
                    shaderInst->Commit();
                    nodeInstance->Render();

                    if (LightingMode::SinglePass == this->lightingMode)
                    {
                        shaderInst->EndPass();
                        shaderInst->End();
                    }
                }

                if (LightingMode::None == this->lightingMode)
                {
                    shaderInst->EndPass();
                    shaderInst->End();
                }                
            }
        }
    }
}

} // namespace Frame
