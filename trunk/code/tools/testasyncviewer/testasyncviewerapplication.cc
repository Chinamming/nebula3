//------------------------------------------------------------------------------
//  testasyncviewerapplication.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/testasyncviewer/testasyncviewerapplication.h"
#include "timing/time.h"
#include "input/keyboard.h"

namespace Tools
{
using namespace Util;
using namespace CoreGraphics;
using namespace AsyncGraphics;
using namespace Input;

//------------------------------------------------------------------------------
/**
*/
TestAsyncViewerApplication::TestAsyncViewerApplication()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
TestAsyncViewerApplication::~TestAsyncViewerApplication()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
TestAsyncViewerApplication::Open()
{
    // call parent class
    if (App::AsyncViewerApplication::Open())
    {
        // display available display modes
        Array<DisplayMode> modes = DisplayProxy::Instance()->GetAvailableDisplayModes(Adapter::Primary, PixelFormat::X8R8G8B8);
        IndexT i;
        for (i = 0; i < modes.Size(); i++)
        {
            n_printf("%d: %dx%d %s\n", i, modes[i].GetWidth(), modes[i].GetHeight(), PixelFormat::ToString(modes[i].GetPixelFormat()).AsCharPtr());
        }
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
TestAsyncViewerApplication::OnProcessInput()
{
    InputServer* inputServer = InputServer::Instance();
    const Ptr<Keyboard>& keyboard = inputServer->GetDefaultKeyboard();
        
    // process keyboard input
    if (keyboard->KeyDown(Key::Escape))
    {
        this->SetQuitRequested(true);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
TestAsyncViewerApplication::OnUpdateFrame()
{
    // sleep a little while
    Timing::Sleep(0.1);
    App::AsyncViewerApplication::OnUpdateFrame();    
}

} // namespace Tools
