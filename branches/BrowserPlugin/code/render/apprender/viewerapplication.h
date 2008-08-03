#pragma once
#ifndef APP_VIEWERAPPLICATION_H
#define APP_VIEWERAPPLICATION_H
//------------------------------------------------------------------------------
/**
    @class App::ViewerApplication

    Nebula3's default viewer application.

    (C) 2007 Radon Labs GmbH
*/
#include "apprender/renderapplication.h"
#include "graphics/stage.h"
#include "graphics/view.h"
#include "graphics/cameraentity.h"
#include "lighting/spotlightentity.h"
#include "graphics/modelentity.h"
#include "renderutil/mayacamerautil.h"

//------------------------------------------------------------------------------
namespace App
{
class ViewerApplication : public RenderApplication
{
public:
    /// constructor
    ViewerApplication();
    /// destructor
    virtual ~ViewerApplication();
    /// open the application
    virtual bool Open();
    /// close the application
    virtual void Close();

protected:
    /// process input (called before rendering)
    virtual void OnProcessInput();
    /// render current frame
    virtual void OnRenderFrame();

    Resources::ResourceId resId;
    Ptr<Frame::FrameShader> frameShader;
    Ptr<Graphics::Stage> stage;
    Ptr<Graphics::View> view;
    Ptr<Graphics::CameraEntity> cameraEntity;
    Ptr<Graphics::ModelEntity> modelEntity;
    Ptr<Lighting::SpotLightEntity> lightEntity;
    RenderUtil::MayaCameraUtil mayaCameraUtil;
#if __WIN32__
    HWND hWndPrevParent;
#endif // __WIN32__
};

} // namespace App
//------------------------------------------------------------------------------
#endif
