//------------------------------------------------------------------------------
//  viewerapplication.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "apprender/viewerapplication.h"
#include "graphics/graphicsserver.h"
#include "graphics/stage.h"
#include "graphics/view.h"
#include "graphics/simplestagebuilder.h"
#include "graphics/view.h"
#include "math/matrix44.h"
#include "math/quaternion.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "input/gamepad.h"
#include "frame/frameserver.h"

namespace App
{
using namespace Util;
using namespace CoreGraphics;
using namespace Resources;
using namespace Graphics;
using namespace Lighting;
using namespace Math;
using namespace Input;
using namespace Frame;

//------------------------------------------------------------------------------
/**
*/
ViewerApplication::ViewerApplication()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ViewerApplication::~ViewerApplication()
{
    n_assert(!this->IsOpen());
}

//------------------------------------------------------------------------------
/**
*/
bool
ViewerApplication::Open()
{
    n_assert(!this->IsOpen());
    if (RenderApplication::Open())
    {
        GraphicsServer* gfxServer = GraphicsServer::Instance();

        // setup the camera util object
        this->mayaCameraUtil.Setup(point(0.0f, 0.0f, 0.0f), point(0.0f, 0.0f, 10.0f), vector(0.0f, 1.0f, 0.0f));

        // setup a stage
        this->stage = gfxServer->CreateStage(StringAtom("DefaultStage"), SimpleStageBuilder::Create());

        // attach a camera to the stage
        this->cameraEntity = CameraEntity::Create();
        cameraEntity->SetTransform(this->mayaCameraUtil.GetCameraTransform());
        this->stage->AttachEntity(cameraEntity.upcast<GraphicsEntity>());

        // setup a default view
        this->view = gfxServer->CreateView(View::RTTI, StringAtom("DefaultView"), true);
        this->view->SetStage(this->stage);        
        this->view->SetFrameShader(FrameServer::Instance()->GetFrameShaderByName(ResourceId(DEFAULT_FRAMESHADER_NAME)));
        this->view->SetCameraEntity(cameraEntity);

        // setup stage content
        ResourceId modelResId;
        if (this->args.HasArg("-model"))
        {
            modelResId = this->args.GetString("-model");

            // attach a light entity
            matrix44 lightTransform = matrix44::multiply(matrix44::scaling(100.0f, 100.0f, 100.0f), matrix44::lookatrh(point(20.0f, 20.0f, 20.0f), point::origin(), vector::upvec()));
            this->lightEntity = SpotLightEntity::Create();
            this->lightEntity->SetCastShadows(true);
            this->lightEntity->SetTransform(lightTransform);
            this->lightEntity->SetColor(float4(4.0f, 2.0f, 1.0f, 1.0f));        
            this->stage->AttachEntity(this->lightEntity.upcast<GraphicsEntity>());

            // attach a model entity
            this->modelEntity = ModelEntity::Create();
            this->modelEntity->SetResourceId(modelResId);
            this->stage->AttachEntity(this->modelEntity.upcast<GraphicsEntity>());      
        }
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
ViewerApplication::Close()
{
    n_assert(this->IsOpen());
    
    GraphicsServer* gfxServer = GraphicsServer::Instance();
    gfxServer->DiscardView(this->view);
    this->view = 0;
    gfxServer->DiscardStage(this->stage);
    this->stage = 0;
    this->modelEntity = 0;
    this->lightEntity = 0;
    this->cameraEntity = 0;

    RenderApplication::Close();
}

//------------------------------------------------------------------------------
/**
*/
void
ViewerApplication::OnProcessInput()
{
    // @todo: include frame time!
    // update the camera from input
    InputServer* inputServer = InputServer::Instance();
    const Ptr<Keyboard>& keyboard = inputServer->GetDefaultKeyboard();
    const Ptr<Mouse>& mouse = inputServer->GetDefaultMouse();
    const Ptr<GamePad>& gamePad = inputServer->GetDefaultGamePad(0);

    if (gamePad->IsConnected() && gamePad->ButtonPressed(GamePad::LeftShoulderButton))
    {
        // rotate model if right shoulder button is pressed
        float yaw = gamePad->GetAxisValue(GamePad::LeftThumbXAxis) * 0.1f;
        float pitch = gamePad->GetAxisValue(GamePad::LeftThumbYAxis) * 0.1f;
        matrix44 rotMatrix = matrix44::rotationyawpitchroll(yaw, pitch, 0.0f);
        matrix44 m = matrix44::multiply(this->modelEntity->GetTransform(), rotMatrix);
        this->modelEntity->SetTransform(m);
    }
    else
    {
        // standard input handling: manipulate camera
        this->mayaCameraUtil.SetOrbitButton(mouse->ButtonPressed(MouseButton::LeftButton));
        this->mayaCameraUtil.SetPanButton(mouse->ButtonPressed(MouseButton::MiddleButton));
        this->mayaCameraUtil.SetZoomButton(mouse->ButtonPressed(MouseButton::RightButton));
        this->mayaCameraUtil.SetZoomInButton(mouse->WheelForward());
        this->mayaCameraUtil.SetZoomOutButton(mouse->WheelBackward());
        this->mayaCameraUtil.SetMouseMovement(mouse->GetMovement());
        
        // process gamepad input
        float zoomIn = 0.0f;
        float zoomOut = 0.0f;
        float2 panning(0.0f, 0.0f);
        float2 orbiting(0.0f, 0.0f);
        if (gamePad->IsConnected())
        {
            if (gamePad->ButtonDown(GamePad::AButton))
            {
                this->mayaCameraUtil.Reset();
                this->modelEntity->SetTransform(matrix44::identity());
            }
            if (gamePad->ButtonDown(GamePad::StartButton) ||
                gamePad->ButtonDown(GamePad::BackButton))
            {
                this->SetQuitRequested(true);
            }
            float frameTime = (float) this->GetFrameTime();
            zoomIn       += gamePad->GetAxisValue(GamePad::RightTriggerAxis) * frameTime;
            zoomOut      += gamePad->GetAxisValue(GamePad::LeftTriggerAxis) * frameTime;
            panning.x()  += gamePad->GetAxisValue(GamePad::RightThumbXAxis) * frameTime;
            panning.y()  += gamePad->GetAxisValue(GamePad::RightThumbYAxis) * frameTime;
            orbiting.x() += gamePad->GetAxisValue(GamePad::LeftThumbXAxis) * frameTime;
            orbiting.y() += gamePad->GetAxisValue(GamePad::LeftThumbYAxis) * frameTime;
        }

        // process keyboard input
        if (keyboard->KeyDown(Key::Escape))
        {
            this->SetQuitRequested(true);
        }
        if (keyboard->KeyDown(Key::Space))
        {
            this->mayaCameraUtil.Reset();
        }
        if (keyboard->KeyPressed(Key::Left))
        {
            panning.x() -= 0.1f;
        }
        if (keyboard->KeyPressed(Key::Right))
        {
            panning.x() += 0.1f;
        }
        if (keyboard->KeyPressed(Key::Up))
        {
            panning.y() -= 0.1f;
        }
        if (keyboard->KeyPressed(Key::Down))
        {
            panning.y() += 0.1f;
        }

        this->mayaCameraUtil.SetPanning(panning);
        this->mayaCameraUtil.SetOrbiting(orbiting);
        this->mayaCameraUtil.SetZoomIn(zoomIn);
        this->mayaCameraUtil.SetZoomOut(zoomOut);
        this->mayaCameraUtil.Update();
        this->cameraEntity->SetTransform(this->mayaCameraUtil.GetCameraTransform());

#if __WIN32__
        // switch between child window and popup window modes
        if (keyboard->KeyPressed(Key::P))
        {
            if (this->hWndParent)
            {
                this->hWndPrevParent = this->hWndParent;
                this->SetParentHwnd(0);
            }
            else if (this->hWndPrevParent)
            {
                this->SetParentHwnd(this->hWndPrevParent);
            }
        }
#endif // __WIN32__
    }
}

//------------------------------------------------------------------------------
/**
*/
void
ViewerApplication::OnRenderFrame()
{
    GraphicsServer::Instance()->OnFrame(this->timer.GetTime());

    // if we are running in windowed mode, be multitasking friendly
    // by giving up our time slice
    if (!DisplayDevice::Instance()->IsFullscreen())
    {
        Timing::Sleep(0.0);
    }
}

}
