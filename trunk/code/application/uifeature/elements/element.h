#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H
//------------------------------------------------------------------------------
/**
    @class UI::Element

    The base class for all 2D user interface elements describes a rectangle
    and implements a hierarchy.
    
	The position of each element is a relative pixel position to the parent element.

	If the parent element is invalid, it is the canvas. wich usually has the position
	(0, 0) and is at the same position as the parent window. If the parent window 
	is not screen aligned, the canvas position (this->relPixelPos) is the absolute
	position of 

    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "math/rectangle.h"
#include "input/key.h"
#include "math/float2.h"
#include "math/rectangle.h"
#include "graphics/display.h"

//------------------------------------------------------------------------------
namespace UI
{

class Window;

class Element : public Core::RefCounted
{
    __DeclareClass(Element);
public:
    /// constructor
    Element();
    /// destructor
    virtual ~Element();
    
    /// set id
    void SetId(const Util::String& i);
    /// get id
    const Util::String& GetId() const;

    /// set visible flag of the element
    virtual void SetVisible(bool b);
    /// get visible flag of the element
    bool IsVisible() const;   	

	/// set relativ position
	void SetRelativePixelPosition(Math::float2& relPos);
    /// returns the screenspace position of an element
    const Math::float2 GetRelativePixelPosition() const;
	/// set relativ position
	void SetRelativeScreenPosition(Math::float2& relPos);
    /// returns the screenspace position of an element
    const Math::float2 GetRelativeScreenPosition() const;

	/// set relativ position
	void SetPixelSize(Math::float2& size);
    /// returns the screenspace position of an element
    const Math::float2 GetPixelSize() const;
	/// set relativ position
	void SetScreenSize(Math::float2& size);
    /// returns the screenspace position of an element
    const Math::float2 GetScreenSize() const;

	/// set the color
	void SetColor(Math::float4& color);
	/// get the color
	const Math::float4& GetColor() const;

	/// called when gui hierarchy is created
	virtual void OnCreate();
    /// called when gui hierarchy is destroyed
    virtual void OnDestroy();
	/// return true if element is valid (between OnCreate() and OnDestroy())
    bool IsValid() const;

    /// attach child element
    void AttachElement(Ptr<Element>& elm);
    /// remove child element
    void RemoveElement(Ptr<Element>& elm);
    /// erase the childelements array
    void EraseElement(Ptr<Element>& elm);

    /// get the number of childelements
    int GetNumChildElements() const;
	/// access to child elements
	const Util::Array<Ptr<Element>>& GetChildElements() const;
    /// acccess to a single child element
    const Ptr<Element>& GetChildElementAt(int index) const;
    /// find a direct child element by name
    Ptr<Element> FindChildElement(const Util::String& name) const;

    /// get pointer to parent element (can be 0)
    const Ptr<Element>& GetParentElement() const;    
    /// get window that contains this element
    const Ptr<Window>& GetParentWindow() const;
    
    /// called once per frame after input has been delivered
    virtual void OnFrame();
    /// called before the gui hierarchy is rendered
    virtual void OnRender();
	/// render debug yourself (just the shape)
	virtual void OnRenderDebug();

	/// return true if mouse is within element area
    virtual bool Inside(const Math::float2& mousePos);
	/// returns all child elements at the position
	virtual void GetElementsAt(const Math::float2& mousePos, Util::Array<Ptr<Element>>& elements);

    /// enable/disable the element (won't call OnX() methods if disabled)
    void SetEnabled(bool b);
    /// get enabled state of element
    const bool IsEnabled() const;

    /// called when mouse is moved
    virtual bool OnMouseMove(const Math::float2& mousePos);
    /// called when mousewheel is moved (false = down, true = up)
    virtual bool OnMouseWheelMoved(const bool& up);
    /// called when left mouse button is pressed over element
    virtual bool OnLeftButtonDown(const Math::float2& mousePos);
    /// called when left mouse button is held pressed
    virtual bool OnLeftButtonPressed(const Math::float2& mousePos);
    /// called when left mouse button is released over element
    virtual bool OnLeftButtonUp(const Math::float2& mousePos);
    /// called when left mouse button is double clicked
    virtual bool OnLeftButtonDoubleClicked(const Math::float2& mousePos);
    /// called when right mouse button is pressed over element
    virtual bool OnRightButtonDown(const Math::float2& mousePos);
    /// called when right mouse button is held pressed
    virtual bool OnRightButtonPressed(const Math::float2& mousePos);
    /// called when right mouse button is released over element
    virtual bool OnRightButtonUp(const Math::float2& mousePos);
    /// called when right mouse button is double clicked
    virtual bool OnRightButtonDoubleClicked(const Math::float2& mousePos);
    /// called when a character is input
    virtual bool OnChar(const uchar& code);
    /// called when a key is pressed
    virtual bool OnKeyDown(Input::Key::Code key);
    /// called when a key is released
    virtual bool OnKeyUp(Input::Key::Code key);
    /// called when input focus is gained
    virtual void OnFocusGain();
    /// called when input focus is lost
    virtual void OnFocusLost();    

    /// register control as input focus element
    void RegisterAsFocusElement();
    /// unregister control as input focus element
    void UnregisterAsFocusElement();
    /// return true if this is currently the focus element
    bool IsFocusElement() const;

protected:
	friend class Window;
	friend class UIXmlParser;

    /// appends an exiting child element to the childs array
    void AppendChildElement(Ptr<Element>& elem);

	/// set parent element
	void SetParentElement(Ptr<Element>& parentElement);
	/// set parent window
	void SetParentWindow(Ptr<Window>& parentWindow);

    Util::String id;    
    bool visible;
    bool enabled;
    Ptr<Element> parentElement;
    Ptr<Window> parentWindow;
	Util::Array<Ptr<Element> > childElements;    
    bool isValid;     

	Math::float4 color;
	Math::rectangle<float> screenSpaceRect;
};

//------------------------------------------------------------------------------
/**
*/
inline 
void
Element::SetColor(Math::float4& color)
{
	this->color = color;
}

//------------------------------------------------------------------------------
/**
*/
inline 
const Math::float4&
Element::GetColor() const
{
	return this->color;
}

//------------------------------------------------------------------------------
/**
*/
inline 
void 
Element::SetPixelSize(Math::float2& size)
{
	// transform to screen coord
	Ptr<Graphics::Display> display = Graphics::Display::Instance();
	Math::float2 screenSize(size.x() / display->GetDisplayMode().GetWidth(),
							size.y() / display->GetDisplayMode().GetHeight());
	// set new
	this->SetScreenSize(screenSize);
}

//------------------------------------------------------------------------------
/**
*/
inline     
const Math::float2 
Element::GetPixelSize() const
{
	// get screen size
	Math::float2 screenSize = this->GetScreenSize();
	// transform to pixels
	Ptr<Graphics::Display> display = Graphics::Display::Instance();
	return Math::float2(display->GetDisplayMode().GetWidth() * screenSize.x(),
						display->GetDisplayMode().GetHeight() * screenSize.y());
}

//------------------------------------------------------------------------------
/**
*/
inline 
void 
Element::SetScreenSize(Math::float2& size)
{
	// get size and pos
	Math::float2 screenPos = this->GetRelativeScreenPosition();
	Math::float2 sizeHalf = size * 0.5f;
	// set new
	this->screenSpaceRect.set(	screenPos.x() - sizeHalf.x(),
								screenPos.y() - sizeHalf.y(),
								screenPos.x() + sizeHalf.x(),
								screenPos.y() + sizeHalf.y());
}

//------------------------------------------------------------------------------
/**
*/
inline 
const Math::float2 
Element::GetScreenSize() const
{
	return Math::float2(this->screenSpaceRect.width(), this->screenSpaceRect.height());
}

//------------------------------------------------------------------------------
/**
*/
inline 
void
Element::SetRelativePixelPosition(Math::float2& relPos)
{
	// transform to screen coord
	Ptr<Graphics::Display> display = Graphics::Display::Instance();
	Math::float2 screenPos(	relPos.x() / display->GetDisplayMode().GetWidth(),
							relPos.y() / display->GetDisplayMode().GetHeight());
	// set new
	this->SetRelativeScreenPosition(screenPos);

}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::float2
Element::GetRelativePixelPosition() const
{
	// get screen pos
	Math::float2 screenPos = this->GetRelativeScreenPosition();
	// transform to pixels
	Ptr<Graphics::Display> display = Graphics::Display::Instance();
	return Math::float2(display->GetDisplayMode().GetWidth() * screenPos.x(),
						display->GetDisplayMode().GetHeight() * screenPos.y());
}

//------------------------------------------------------------------------------
/**
*/
inline
void
Element::SetRelativeScreenPosition(Math::float2& relPos)
{
	// get actuall size
	float widthHalf = this->screenSpaceRect.width() / 2.0f;
	float heightHalf = this->screenSpaceRect.height() / 2.0f;

	// set new pos with old size
	this->screenSpaceRect.set(	relPos.x() - widthHalf,
								relPos.y() - heightHalf,
								relPos.x() + widthHalf,
								relPos.y() + widthHalf);											
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::float2
Element::GetRelativeScreenPosition() const
{
	return Math::float2((this->screenSpaceRect.left + this->screenSpaceRect.right) / 2.0f,
						(this->screenSpaceRect.top + this->screenSpaceRect.bottom) / 2.0f);
}

//------------------------------------------------------------------------------
/**
*/
inline 
bool
Element::IsValid() const
{
    return this->isValid;
}

//------------------------------------------------------------------------------
/**
*/
inline 
void
Element::SetVisible(bool b)
{
    this->visible = b;
}

//------------------------------------------------------------------------------
/**
*/
inline 
void
Element::SetId(const Util::String& i)
{
    this->id = i;
}

//------------------------------------------------------------------------------
/**
*/
inline 
const Util::String&
Element::GetId() const
{
    return this->id;
}

//------------------------------------------------------------------------------
/**
    Get pointer to parent element. This method may return 0 if no parent
    element exists.
*/
inline 
const Ptr<Element>&
Element::GetParentElement() const
{
    return this->parentElement;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Element::SetEnabled(bool b)
{
    this->enabled = b;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Element::AppendChildElement(Ptr<Element>& elem)
{
	n_assert(elem.isvalid());
    this->childElements.Append(elem);
}

//------------------------------------------------------------------------------
/**
    Provides access to the child element array.
*/
inline 
const Util::Array<Ptr<Element>>&
Element::GetChildElements() const
{
    return this->childElements;
}

//------------------------------------------------------------------------------
/**
    Returns the number of child elements
*/
inline
int
Element::GetNumChildElements() const
{
    return this->childElements.Size();
}

}; // namespace
//------------------------------------------------------------------------------
#endif
