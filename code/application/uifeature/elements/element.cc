//------------------------------------------------------------------------------
//  uifeature/elements/element.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "uifeature/elements/element.h"
#include "uifeature/elements/window.h"
#include "uifeature/uifactory.h"
#include "uifeature/uiserver.h"
#include "uifeature/uievent.h"
#include "ui/uirenderer.h"

namespace UI
{
__ImplementClass(Element, 'UIEL', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
Element::Element() :
    visible(true),
    parentElement(0),
    isValid(false),              
    parentWindow(0),
    enabled(true),
	screenSpaceRect(0,0,0,0)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
Element::~Element()
{
	// empty
}

//------------------------------------------------------------------------------
/**
    Called by parent to build the gui hierarchy.
*/
void
Element::AttachElement(Ptr<Element>& elm)
{
	n_assert(elm.isvalid());
    this->childElements.Append(elm);
	elm->SetParentElement(Ptr<Element>(this));
	elm->SetParentWindow(this->parentWindow);
    elm->OnCreate();
}

//------------------------------------------------------------------------------
/**
    Erase a child element. This will invoke the OnDestroy() method on the
    child. The child will be destroyed
*/
void
Element::EraseElement(Ptr<Element>& elm)
{
	n_assert(elm.isvalid());
    elm->OnDestroy();
	Util::Array<Ptr<Element>>::Iterator iter = this->childElements.Find(elm);
    n_assert(iter);
    this->childElements.Erase(iter);
}

//------------------------------------------------------------------------------
/**
    Remove a child element from the childElements array. This won't destroy the child.
*/
void
Element::RemoveElement(Ptr<Element>& elm)
{
	n_assert(elm.isvalid());
    Util::Array<Ptr<Element>>::Iterator iter = this->childElements.Find(elm);
    n_assert(iter);
    this->childElements.Erase(iter);
}

//------------------------------------------------------------------------------
/**
    Provides access to a single child element by an index.
*/
const Ptr<Element>&
Element::GetChildElementAt(int index) const
{
    n_assert(index>=0 && (IndexT)index < this->childElements.Size());    
    return this->childElements[index];
}

//------------------------------------------------------------------------------
/**
    Find a child element by its id. Returns 0 if not found.
*/
Ptr<Element>
Element::FindChildElement(const Util::String& id) const
{
    IndexT i;
    for (i = 0; i < this->childElements.Size(); i++)
    {
        if (id == this->childElements[i]->GetId())
        {
            return this->childElements[i];
        }
    }
    // fallthrough: not found
    return Ptr<Element>(0);
}


//------------------------------------------------------------------------------
/**
*/
void
Element::SetParentWindow(Ptr<Window>& window)
{
	n_assert(window.isvalid());
	n_assert(!this->parentElement.isvalid());
    
	this->parentWindow = window;    
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<Window>&
Element::GetParentWindow() const
{
    return this->parentWindow;
}

//------------------------------------------------------------------------------
/**
    This method is called when the element has been attached to a parent.
*/
void
Element::OnCreate()
{
    n_assert(!this->IsValid());
    this->isValid = true;
}

//------------------------------------------------------------------------------
/**
    This method is called when the element has been removed from its parent.
*/
void
Element::OnDestroy()
{
    if (this->IsValid())
    {
        // release focus if necessary
        if (this->IsFocusElement())
        {
            this->UnregisterAsFocusElement();
        }

        // destroy all children
        IndexT i;
        for (i = 0; i < this->childElements.Size(); i++)
        {
            this->childElements[i]->OnDestroy();
        }
        this->childElements.Clear();   
        
         // clear own parent pointer
        this->parentElement = 0;
        this->parentWindow = 0;
        this->isValid = false;
    }
}

//------------------------------------------------------------------------------
/**
    This method is called once per frame during UIServer::Trigger() after
    the input has been delivered to the gui.
*/
void
Element::OnFrame()
{
    // distribute to children
    if (this->IsVisible())
    {
        IndexT i;
        for (i = 0; i < this->childElements.Size(); i++)
        {
            this->childElements[i]->OnFrame();
        }
    }
}

//------------------------------------------------------------------------------
/**
    This method is called when the element should render itself.
*/
void
Element::OnRender()
{
    // distribute to children
    IndexT i;
    for (i = 0; i < this->childElements.Size(); i++)
    {
        this->childElements[i]->OnRender();
    }
}

//------------------------------------------------------------------------------
/**
    This method is called when the element should render itself in debug mode.
	(Just the shapes)
*/
void
Element::OnRenderDebug()
{
	// render yourself
	UIRenderer::Instance()->DrawElement(this->screenSpaceRect, this->color);

    // distribute to children
    /*IndexT i;
    for (i = 0; i < this->childElements.Size(); i++)
    {
        this->childElements[i]->OnRenderDebug();
    }*/
}

//------------------------------------------------------------------------------
/**
    This method returns true if the given mouse position is inside the
    elements screen space area. By default this checks the mouse position
    against the screen space rectangle,
    but you can override the method in a subclass if you need more 
    advanced picking.
*/
bool
Element::Inside(const Math::float2& mousePos)
{
    if (this->IsVisible() && this->IsValid())
    {
        //rough check on screenSpaceRect
		bool inside =  this->screenSpaceRect.inside(mousePos.x(), mousePos.y());
        return inside;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
    Checks all childs, appends them, if they are inside the position.
*/
void
Element::GetElementsAt(const Math::float2& mousePos, Util::Array<Ptr<Element>>& elements)
{
	// check all childs
	IndexT index;
	for(index = 0; index < this->childElements.Size(); index++)
	{
		if(this->childElements[index]->Inside(mousePos))
		{	
			elements.Append(this->childElements[index]);
			this->childElements[index]->GetElementsAt(mousePos, elements);
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
bool
Element::IsVisible() const
{
    // recursively check if me or one of my parents is invisible
    if (!this->visible)
    {
        return false;
    }
	else if (this->GetParentElement().isvalid())
    {
        return this->GetParentElement()->IsVisible();
    }    
    else
    {
        return true;
    }
}

//------------------------------------------------------------------------------
/**
    Set the pointer to the parent element.
*/
void
Element::SetParentElement(Ptr<Element>& parent)
{
	n_assert(parent.isvalid());
	n_assert(!this->parentElement.isvalid());	
}


//------------------------------------------------------------------------------
/**
    If set to false the element won't call the OnX() methods anymore and doesn't
    distribute the input to it's children.
*/
const bool
Element::IsEnabled() const
{
    // recursively check if me or one of my parents is disabled
    if (!this->enabled)
    {
        return false;
    }
	else if (this->GetParentElement().isvalid())
    {
        return this->GetParentElement()->IsEnabled();
    }    
    else
    {
        return true;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
Element::OnMouseMove(const Math::float2& mousePos)
{
    return false;    
}

//------------------------------------------------------------------------------
/**
*/
bool
Element::OnLeftButtonDown(const Math::float2& mousePos)
{
    return false;    
}

//------------------------------------------------------------------------------
/**
*/
bool
Element::OnLeftButtonPressed(const Math::float2& mousePos)
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
Element::OnLeftButtonUp(const Math::float2& mousePos)
{
    return false;    
}

//------------------------------------------------------------------------------
/**
*/
bool
Element::OnLeftButtonDoubleClicked(const Math::float2& mousePos)
{
    return false;    
}

//------------------------------------------------------------------------------
/**
*/
bool
Element::OnRightButtonDown(const Math::float2& mousePos)
{
    return false;    
}

//------------------------------------------------------------------------------
/**
*/
bool
Element::OnRightButtonPressed(const Math::float2& mousePos)
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
Element::OnRightButtonUp(const Math::float2& mousePos)
{
    return false;    
}

//------------------------------------------------------------------------------
/**
*/
bool
Element::OnRightButtonDoubleClicked(const Math::float2& mousePos)
{
    return false;    
}

//------------------------------------------------------------------------------
/**
*/
bool
Element::OnChar(const uchar& code)
{
    return false;    
}

//------------------------------------------------------------------------------
/**
*/
bool
Element::OnKeyDown(Input::Key::Code key)
{
    return false;    
}

//------------------------------------------------------------------------------
/**
*/
bool
Element::OnKeyUp(Input::Key::Code key)
{
    return false;    
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnFocusGain()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
Element::OnFocusLost()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void 
Element::RegisterAsFocusElement()
{
    UI::UIServer::Instance()->SetFocusElement(Ptr<Element>(this), true);  
}

//------------------------------------------------------------------------------
/**
*/
void 
Element::UnregisterAsFocusElement()
{
    UI::UIServer::Instance()->SetFocusElement(Ptr<Element>(this), false);
}

//------------------------------------------------------------------------------
/**
*/
bool
Element::IsFocusElement() const
{
    return UI::UIServer::Instance()->GetFocusElement() == this;
}

//------------------------------------------------------------------------------
/**
*/
bool
Element::OnMouseWheelMoved(const bool& up)
{
    return false;
}
} // namespace UI
