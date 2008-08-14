#pragma once
#ifndef APP_ASYNCVIEWERAPPLICATION_H
#define APP_ASYNCVIEWERAPPLICATION_H
//------------------------------------------------------------------------------
/**
    @class App::AsyncViewerApplication
    
    Derived from AsyncRenderApplication, adds support for Stages, Views
    and GraphicsEntities to the asynchronour render app.
    
    (C) 2007 Radon Labs GmbH
*/
#include "apprender/asyncrenderapplication.h"
#include "asyncgraphics/graphicsserverproxy.h"
#include "asyncgraphics/stageproxy.h"
#include "asyncgraphics/viewproxy.h"
#include "asyncgraphics/cameraentityproxy.h"
#include "renderutil/mayacamerautil.h"

//------------------------------------------------------------------------------
namespace App
{
class AsyncViewerApplication : public AsyncRenderApplication
{
public:
    /// constructor
    AsyncViewerApplication();
    /// destructor
    virtual ~AsyncViewerApplication();
    /// open the application
    virtual bool Open();
    /// close the application
    virtual void Close();

protected:    
    /// process input (called before rendering)
    virtual void OnProcessInput();
    /// update world 
    virtual void OnUpdateFrame();

    Ptr<AsyncGraphics::GraphicsServerProxy> graphicsServerProxy;
    Ptr<AsyncGraphics::StageProxy> stageProxy;
    Ptr<AsyncGraphics::ViewProxy> viewProxy;
    Ptr<AsyncGraphics::CameraEntityProxy> cameraProxy;
    RenderUtil::MayaCameraUtil mayaCameraUtil;
};

} // namespace AsyncGraphics
//------------------------------------------------------------------------------
#endif

    