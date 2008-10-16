#ifndef UI_UISERVER_H
#define UI_UISERVER_H
//------------------------------------------------------------------------------
/**
    @class UI::UIServer

    Server for the Nebula 3 GUI subsystem.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "util/array.h"
#include "timing/time.h"
#include "math/float2.h"
#include "uifeature/uieventhandler.h"
#include "uifeature/elements/window.h"

//------------------------------------------------------------------------------
namespace UI
{
class UIRenderer;
class UIFactory;
class UIEvent;
class Layer;
class Control;
class Element;

class UIServer : public Core::RefCounted
{
    __DeclareClass(UIServer);
    __DeclareSingleton(UIServer);

public:
    /// constructor
    UIServer();
    /// destructor
    virtual ~UIServer();        

    /// open the ui subsystem
    bool Open();
    /// close the ui subsystem
    void Close();
    /// return true if currently open
    bool IsOpen() const;

	/// trigger the server
    void Trigger();
    /// render the user interface
    void Render();
	/// debug rendering (just the shapes)
	void RenderDebug();
    
	/// return the active window (can be invalid)
    Ptr<Window> GetActiveWindow() const;
    /// get the current pointer to the focus element (might be invalid!)
    const Ptr<Element>& GetFocusElement() const;    
    
	/// set current time
	void SetTime(Timing::Time t);
    /// get current time
    Timing::Time GetTime() const;
    /// set current frame time
    void SetFrameTime(Timing::Time f);
    /// get current frame time
    Timing::Time GetFrameTime() const;

    /// get mouse position
	const Math::float2& GetMousePosition() const;
    /// get mouse postion in pixels
	const Math::float2& GetPixelMousePosition() const;

    /// is mouse over a UI element
    bool IsMouseOverGui() const;
        
    /// enable input handling
    void EnableInput();
    /// disable input handling
    void DisableInput();
    /// return true if input handling is enabled
    bool IsInputEnabled() const;

private:
    friend class Window;
    friend class Element;

    /// set current focus object DON'T CALL DIRECTLY IN CONTROL
    void SetFocusElement(Ptr<Element>& focusElement, bool reg);
    
	/// attach a window to the UI
    void AttachWindow(Ptr<Window>& window);
    /// remove a window from the UI
    void RemoveWindow(Ptr<Window>& window);

	/// handles each frame the imput
	void HandleInput();

    bool isOpen;
    bool inTrigger;
    bool globalHide;
	bool inputEnabled;

	Util::Array<Ptr<Window> > windows;
    
    Ptr<UIFactory> uiFactory;    
	Ptr<Element> focusElement; 
    Ptr<UIEventHandler> curEventHandler;

	Math::float2 mousePosition;
    Math::float2 pixelMousePosition;   

	Timing::Time time;
	Timing::Time frameTime;
};

//------------------------------------------------------------------------------
/**
*/
inline
Ptr<Window>
UIServer::GetActiveWindow() const
{
    // FIXME!
    // find the first valid window
    IndexT i;
    for (i = this->windows.Size()-1; i >= 0; i--)
    {
        if (this->windows[i].isvalid())
        {
            return this->windows[i];
        }
    }
    return Ptr<Window>(0);
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
UIServer::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::float2&
UIServer::GetMousePosition() const
{
    return this->mousePosition;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::float2&
UIServer::GetPixelMousePosition() const
{
    return this->pixelMousePosition;
}

//-----------------------------------------------------------------------------
/**
*/
inline
const Ptr<Element>&
UIServer::GetFocusElement() const
{
    return this->focusElement;
}

//-----------------------------------------------------------------------------
/**
*/
inline
void
UIServer::EnableInput()
{
    this->inputEnabled = true;
}

//-----------------------------------------------------------------------------
/**
*/
inline
void
UIServer::DisableInput()
{
    this->inputEnabled = false;
}

//-----------------------------------------------------------------------------
/**
*/
inline
bool
UIServer::IsInputEnabled() const
{
    return this->inputEnabled;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
UIServer::SetTime(Timing::Time t)
{
    this->time = t;
}

//------------------------------------------------------------------------------
/**
*/
inline
Timing::Time
UIServer::GetTime() const
{
    return this->time;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
UIServer::SetFrameTime(Timing::Time f)
{
    this->frameTime = f;
}

//------------------------------------------------------------------------------
/**
*/
inline
Timing::Time
UIServer::GetFrameTime() const
{
    return this->frameTime;
}

}; // namespace UI
//------------------------------------------------------------------------------
#endif
