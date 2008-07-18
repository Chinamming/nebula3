//------------------------------------------------------------------------------
//  asyncviewerapplication.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "apprender/asyncviewerapplication.h"
#include "apprender/platformconfig.h"
#include "graphics/simplestagebuilder.h"
#include "graphics/view.h"

namespace App
{
using namespace Math;
using namespace Graphics;
using namespace AsyncGraphics;
using namespace Util;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
AsyncViewerApplication::AsyncViewerApplication()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
AsyncViewerApplication::~AsyncViewerApplication()
{
    n_assert(!this->stageProxy.isvalid());
}

//------------------------------------------------------------------------------
/**
*/
bool
AsyncViewerApplication::Open()
{
    if (AsyncRenderApplication::Open())
    {
        StringAtom defaultStageName("DefaultStage");
        StringAtom defaultViewName("DefaultView");

        // setup the camera util object
        this->mayaCameraUtil.Setup(point(0.0f, 0.0f, 0.0f), point(0.0f, 0.0f, 10.0f), vector(0.0f, 1.0f, 0.0f));

        // create a GraphicServerProxy, StageProxy and ViewProxy
        this->graphicsServerProxy = GraphicsServerProxy::Create();
        this->graphicsServerProxy->Open();

        // create a default stage
        Attr::AttributeContainer stageBuilderAttrs;
        this->stageProxy = this->graphicsServerProxy->CreateStageProxy(defaultStageName, 
                                                                       Graphics::SimpleStageBuilder::RTTI, 
                                                                       stageBuilderAttrs);

        // create a default view
        this->viewProxy = this->graphicsServerProxy->CreateViewProxy(Graphics::View::RTTI,
                                                                     defaultViewName,
                                                                     defaultStageName,
                                                                     ResourceId(DEFAULT_FRAMESHADER_NAME),
                                                                     true);

        // create a camera entity
        this->cameraProxy = CameraEntityProxy::Create();
        this->cameraProxy->SetTransform(this->mayaCameraUtil.GetCameraTransform());
        this->cameraProxy->SetViewName(defaultViewName);
        this->stageProxy->AttachEntityProxy(this->cameraProxy.cast<GraphicsEntityProxy>());

        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncViewerApplication::Close()
{
    this->stageProxy->RemoveEntityProxy(this->cameraProxy.cast<GraphicsEntityProxy>());
    this->cameraProxy = 0;

    this->graphicsServerProxy->DiscardViewProxy(this->viewProxy);
    this->viewProxy = 0;
    
    this->graphicsServerProxy->DiscardStageProxy(this->stageProxy);
    this->stageProxy = 0;

    this->graphicsServerProxy->Close();
    this->graphicsServerProxy = 0;

    // call parent class
    AsyncRenderApplication::Close();
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncViewerApplication::OnProcessInput()
{
    // FIXME!
}

//------------------------------------------------------------------------------
/**
*/
void
AsyncViewerApplication::OnUpdateFrame()
{
    this->graphicsServerProxy->OnFrame();
}

} // namespace App

    
