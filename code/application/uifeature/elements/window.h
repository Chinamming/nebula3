#ifndef UI_WINDOW_H
#define UI_WINDOW_H
//------------------------------------------------------------------------------
/**
    @class UI::Window

    Base class for windows. A window generally has exactly one canvas
    attached.

    Override this class to implement windows with new behaviour.
    The window class generally implements only functionality, not layout!
	The virtual position of the window is in the center of the screen.
	The alignment descripes the center position of the canvas.

    //If Activate() is called before Open() (the window has already been attached
    //to server for receiving messages) it will not be deactivated on Close(). So
    //the window is able to receive further messages after closing.

    (C) 2008 Radon Labs GmbH
*/
#include "messaging/port.h"
#include "uifeature/elements/canvas.h"

//------------------------------------------------------------------------------
namespace UI
{
class Window : public Core::RefCounted
{
    __DeclareClass(Window);
public:
	
    enum ScreenAlignmentX
    {
        UnalignedX = 0,
        LeftSide,
        CenterX,
        RightSide
    };
    enum ScreenAlignmentY
    {
        UnalignedY = 0,
        UpSide,
        CenterY,
        DownSide
    };

    /// constructor
    Window();
    /// destructor
    virtual ~Window();

    /// open the window at position(will attach the window to the UI server)
	virtual void Open();
    /// close the window (will remove the window from the UI server)
    virtual void Close();
    /// return true if window is open
    bool IsOpen() const;
    /// hide the window
    virtual void Hide();
    /// shows the window (will open if needed)
    virtual void Show();
    /// return true if window is hidden
    bool IsHidden() const;
    /// return true if window is open and not hidden
    bool IsShown() const;

    /// get the canvas of the window
    const Ptr<Canvas>& GetCanvas() const;    

    /// called once per frame after input has been delivered
    virtual void OnFrame();
    /// called before the gui hierarchy is rendered
    virtual void OnRender();
	/// render debug (just the shapes of window and all elements)
	virtual void OnRenderDebug();

	/// return true if mouse is within element area
	virtual bool Inside(const Math::float2& mousePos);
	/// returns all elements at the position
	virtual void GetElementsAt(const Math::float2& mousePos, Util::Array<Ptr<Element>>& elements);

	/// set screen alignment
    void SetScreenAlignmentX(ScreenAlignmentX align);
    /// set screen alignment
    ScreenAlignmentX GetScreenAlignmentX() const;
    /// set screen alignment
    void SetScreenAlignmentY(ScreenAlignmentY align);
    /// set screen alignment
    ScreenAlignmentY GetScreenAlignmentY() const;

	/// convert x align to string
	static Util::String AlignmentXToString(const ScreenAlignmentX& align);
	/// convert y align to string
	static Util::String AlignmentYToString(const ScreenAlignmentY& align);
	/// convert string to x align
	static ScreenAlignmentX StringToAlignmentX(const Util::String& align);
	/// convert string to y align
	static ScreenAlignmentY StringToAlignmentY(const Util::String& align);

protected:
	friend class UIXmlParser;

	/// updates his own and the canvas position
	virtual void UpdateCanvasPosition();

	/// set the canvas of the window
	void SetCanvas(const Ptr<Canvas>& canvas);

    bool isOpen;    
    bool isHidden;

    Ptr<Canvas> canvas;

    ScreenAlignmentX screenAlignmentX;
    ScreenAlignmentY screenAlignmentY;
};

//------------------------------------------------------------------------------
/**
*/
inline
void
Window::SetCanvas(const Ptr<Canvas>& canvas)
{
	this->canvas = canvas;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
Window::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Ptr<Canvas>&
Window::GetCanvas() const
{
    return this->canvas;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
Window::IsHidden() const
{
    if (this->isOpen && this->isHidden)
    {
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
Window::IsShown() const
{
    if (this->isOpen && !this->isHidden)
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
/**
*/
inline
void 
Window::SetScreenAlignmentX(Window::ScreenAlignmentX align)
{
    this->screenAlignmentX = align;
}

//------------------------------------------------------------------------------
/**
*/
inline
Window::ScreenAlignmentX 
Window::GetScreenAlignmentX() const
{
    return this->screenAlignmentX;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
Window::SetScreenAlignmentY(Window::ScreenAlignmentY align)
{
    this->screenAlignmentY = align;
}

//------------------------------------------------------------------------------
/**
*/
inline
Window::ScreenAlignmentY 
Window::GetScreenAlignmentY() const
{
    return this->screenAlignmentY;
}
}; // namespace UI
//------------------------------------------------------------------------------
#endif