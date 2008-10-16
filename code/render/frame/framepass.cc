//------------------------------------------------------------------------------
//  framepass.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "frame/framepass.h"
#include "coregraphics/renderdevice.h"

namespace Frame
{
__ImplementClass(Frame::FramePass, 'FPSS', Core::RefCounted);

using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
FramePass::FramePass() :
    clearColor(0.0f, 0.0f, 0.0f, 0.0f),
    clearDepth(1.0f),
    clearStencil(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
FramePass::~FramePass()
{
    // make sure Discard() has been called
    n_assert(!this->shader.isvalid());
    n_assert(!this->renderTarget.isvalid());
    n_assert(this->shaderVariables.IsEmpty());
    n_assert(this->batches.IsEmpty());
}

//------------------------------------------------------------------------------
/**
*/
void
FramePass::Discard()
{
    if (this->shader.isvalid())
    {
        this->shader->Discard();
        this->shader = 0;
    }
    if (this->renderTarget.isvalid())
    {
        if (this->renderTarget->IsValid())
        {
            this->renderTarget->Discard();
        }
        this->renderTarget = 0;
    }
    this->shaderVariables.Clear();
    IndexT i;
    for (i = 0; i < this->batches.Size(); i++)
    {
        this->batches[i]->Discard();
    }
    this->batches.Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
FramePass::Render()
{
    n_assert(this->renderTarget.isvalid());
    RenderDevice* renderDevice = RenderDevice::Instance();

    // update render target
    this->renderTarget->SetClearColor(this->clearColor);
    this->renderTarget->SetClearDepth(this->clearDepth);
    this->renderTarget->SetClearStencil(this->clearStencil);

    // apply shader variables
    IndexT varIndex;
    for (varIndex = 0; varIndex < this->shaderVariables.Size(); varIndex++)
    {
        this->shaderVariables[varIndex]->Apply();
    }

    // render batches...
    renderDevice->BeginPass(this->renderTarget, this->shader);
    IndexT batchIndex;
    for (batchIndex = 0; batchIndex < this->batches.Size(); batchIndex++)
    {
        this->batches[batchIndex]->Render();
    }
    renderDevice->EndPass();
}

} // namespace Frame
