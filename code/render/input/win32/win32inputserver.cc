//------------------------------------------------------------------------------
//  win32inputserver.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "input/win32/win32inputserver.h"
#include "coregraphics/displaydevice.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "input/gamepad.h"
#include "asyncgraphics/displayproxy.h"

namespace Win32
{
ImplementClass(Win32::Win32InputServer, 'W3IS', Base::InputServerBase);
ImplementSingleton(Win32::Win32InputServer);

using namespace Input;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
Win32InputServer::Win32InputServer() :
    di8(0),
    di8Mouse(0),
    wndHasFocus(false),
    hWndParent(0)
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
Win32InputServer::~Win32InputServer()
{
    if (this->IsOpen())
    {
        this->Close();
    }
    DestructSingleton;
}

//------------------------------------------------------------------------------
/**    
*/
void
Win32InputServer::Open()
{
    n_assert(!this->IsOpen());
    InputServerBase::Open();

    // initialize DirectInput for raw mouse events
    this->OpenDInputMouse();

    // setup a display event handler which translates
    // some display events into input events
    this->eventHandler = Win32InputDisplayEventHandler::Create();
    if (DisplayDevice::HasInstance())
    {
        // non-multithreaded rendering
        DisplayDevice::Instance()->AttachEventHandler(this->eventHandler.upcast<DisplayEventHandler>());
    }
    else if (AsyncGraphics::DisplayProxy::HasInstance())
    {
        // multithreaded rendering
        AsyncGraphics::DisplayProxy::Instance()->AttachDisplayEventHandler(this->eventHandler.upcast<ThreadSafeDisplayEventHandler>());
    }

    // create a default keyboard and mouse handler
    this->defaultKeyboard = Keyboard::Create();
    this->AttachInputHandler(InputPriority::Game, this->defaultKeyboard.upcast<InputHandler>());
    this->defaultMouse = Mouse::Create();
    this->AttachInputHandler(InputPriority::Game, this->defaultMouse.upcast<InputHandler>());

    // create 4 default gamepads (none of them have to be connected)
    IndexT playerIndex;
    for (playerIndex = 0; playerIndex < GamePad::GetMaxNumPlayers(); playerIndex++)
    {
        this->defaultGamePad[playerIndex] = GamePad::Create();
        this->defaultGamePad[playerIndex]->SetPlayerIndex(playerIndex);
        this->AttachInputHandler(InputPriority::Game, this->defaultGamePad[playerIndex].upcast<InputHandler>());
    }
}

//------------------------------------------------------------------------------
/**    
*/
void
Win32InputServer::Close()
{
    n_assert(this->IsOpen());

    // remove our event handler from the display device
    if (DisplayDevice::HasInstance())
    {
        DisplayDevice::Instance()->RemoveEventHandler(this->eventHandler.upcast<DisplayEventHandler>());
    }
    else if (AsyncGraphics::DisplayProxy::HasInstance())
    {
        AsyncGraphics::DisplayProxy::Instance()->RemoveDisplayEventHandler(this->eventHandler.upcast<ThreadSafeDisplayEventHandler>());
    }

    // shutdown the DirectInput mouse device
    this->CloseDInputMouse();

    // call parent class
    InputServerBase::Close();
}

//------------------------------------------------------------------------------
/**    
*/
void
Win32InputServer::OnFrame()
{
    this->eventHandler->HandlePendingEvents();
    this->ReadDInputMouse();
    InputServerBase::OnFrame();
}

//------------------------------------------------------------------------------
/**
    Calling this has no effect when the application window is not a child window.
*/
void
Win32InputServer::SetFocus(bool gotFocus)
{
    // When the application window is a child window the DirectInput mouse 
    // device is created with a coop level of DISCL_BACKGROUND, this means the 
    // device is not automatically unacquired when the application window loses 
    // focus. Thus, we must unacquire the device manually.
    if (this->hWndParent)
    {
        this->wndHasFocus = gotFocus;
        if (!this->wndHasFocus)
        {
            n_assert(0 != this->di8Mouse);
            this->di8Mouse->Unacquire();
        }
    }
}

//------------------------------------------------------------------------------
/**    
*/
static bool IsTopLevelWindow(HWND hWnd)
{
    WINDOWINFO wi;
    ZeroMemory(&wi, sizeof(wi));
    wi.cbSize = sizeof(wi);
    if (GetWindowInfo(hWnd, &wi))
        return (wi.dwStyle & WS_CHILD) == 0;
    return false;
}

//------------------------------------------------------------------------------
/**    
    This initializes a DirectInput mouse device in order to track
    raw mouse movement (WM mouse events stop at the screen borders).
*/
bool
Win32InputServer::OpenDInputMouse()
{
    n_assert(0 == this->di8);
    n_assert(0 == this->di8Mouse);
    HRESULT hr;

    // create DirectInput interface
    hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&(this->di8), NULL);
    n_assert(SUCCEEDED(hr));
    n_assert(0 != this->di8);

    // create a DirectInput mouse device
    hr = this->di8->CreateDevice(GUID_SysMouse, &(this->di8Mouse), NULL);
    n_assert(SUCCEEDED(hr));
    n_assert(0 != this->di8Mouse);

    // tell DInput what we're interested in
    hr = this->di8Mouse->SetDataFormat(&c_dfDIMouse2);
    n_assert(SUCCEEDED(hr));

    // set the cooperative level of the device, we're friendly
    HWND hWnd = 0;
    if (this->hWndParent)
    {
        if (IsTopLevelWindow(this->hWndParent))
            hWnd = this->hWndParent;
        else
            hWnd = GetAncestor(this->hWndParent, GA_ROOT);
    }
    else
    {
        // note: use Win32's FindWindow() to find our top level window because 
        // the DisplayDevice may be running in a different thread
        hWnd = FindWindow(NEBULA3_WINDOW_CLASS, NULL);
    }
    n_assert(0 != hWnd);
    DWORD coopFlags = DISCL_NOWINKEY | DISCL_NONEXCLUSIVE;
    if (this->hWndParent)
        coopFlags |= DISCL_BACKGROUND;
    else
        coopFlags |= DISCL_FOREGROUND;
    hr = this->di8Mouse->SetCooperativeLevel(hWnd, coopFlags);
    n_assert(SUCCEEDED(hr));

    // set buffer size and relative axis mode on the mouse
    DIPROPDWORD dipdw;
    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;

    dipdw.dwData = DInputMouseBufferSize;
    hr = this->di8Mouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
    n_assert(SUCCEEDED(hr));
    dipdw.dwData = DIPROPAXISMODE_REL;
    hr = this->di8Mouse->SetProperty(DIPROP_AXISMODE, &dipdw.diph);
    n_assert(SUCCEEDED(hr));

    if (!this->hWndParent || this->wndHasFocus)
        this->di8Mouse->Acquire();

    return true;
}

//------------------------------------------------------------------------------
/**    
    Close the DirectInput mouse and DirectInput.
*/
void
Win32InputServer::CloseDInputMouse()
{
    n_assert(0 != this->di8);
    n_assert(0 != this->di8Mouse);
    this->di8Mouse->Unacquire();
    this->di8Mouse->Release();
    this->di8Mouse = 0;
    this->di8->Release();
    this->di8 = 0;
}

//------------------------------------------------------------------------------
/**    
    Read data from the DirectInput mouse (relative mouse movement
    since the last frame).
*/
void
Win32InputServer::ReadDInputMouse()
{
    n_assert(0 != this->di8Mouse);
    DIDEVICEOBJECTDATA didod[DInputMouseBufferSize];
    HRESULT hr;
    
    this->mouseMovement.set(0.0f, 0.0f);

    if (this->hWndParent && !this->wndHasFocus)
        return;

    // read buffered mouse data
    DWORD inOutNumElements = DInputMouseBufferSize;
    hr = this->di8Mouse->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), didod, &inOutNumElements, 0);
    if (DI_OK != hr)
    {
        hr = this->di8Mouse->Acquire();
    }
    else
    {
        IndexT i;
        for (i = 0; i < inOutNumElements; i++)
        {
            switch (didod[i].dwOfs)
            {
                case DIMOFS_X:
                    this->mouseMovement.x() += float(int(didod[i].dwData)) * 0.1f;
                    break;

                case DIMOFS_Y:
                    this->mouseMovement.y() += float(int(didod[i].dwData)) * 0.1f;
                    break;
            }
        }
    }
}

} // namespace Win32