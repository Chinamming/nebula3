//------------------------------------------------------------------------------
//  uifeature/uiserver.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "uifeature/uiserver.h"
#include "uifeature/uieventhandler.h"
#include "uifeature/uievent.h"
#include "uifeature/uifactory.h"
#include "uifeature/elements/window.h"
#include "uifeature/elements/element.h"
#include "uifeature/uixmlparser.h"
#include "input/inputserver.h"
#include "input/mouse.h"
#include "input/keyboard.h"
#include "ui/uirenderer.h"

namespace UI
{
__ImplementClass(UIServer, 'UISV', Core::RefCounted);
__ImplementSingleton(UIServer);

using namespace Input;

//------------------------------------------------------------------------------
/**
*/
UIServer::UIServer() :
    isOpen(false),
    mousePosition(0.f, 0.f),
    pixelMousePosition(0.f, 0.f),
    inTrigger(false),
    globalHide(false),
    inputEnabled(true)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
UIServer::~UIServer()
{
    if (this->IsOpen())
    {
        this->Close();
    }    
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
bool
UIServer::Open()
{
    n_assert(!this->isOpen);

    // create factory manager
    this->uiFactory = UIFactory::Create(); 
	
	// ----- HACK.. to the testcase here
	//Ptr<UIXmlParser> xml = UIXmlParser::Create();
	//xml->SetFileName("default.xml");
	//Util::Array<Ptr<Window>> wins = xml->ParseFile();
	//for(IndexT i = 0; i < wins.Size(); i++)
	//{
	//	wins[i]->Open();
	//}
	// ----- HACK	
	this->isOpen = true;

    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
UIServer::Close()
{
    n_assert(this->isOpen);

    // let focus element unregister itself
    if (this->focusElement.isvalid())
    {
        this->focusElement->OnFocusLost();
        this->focusElement = 0;
    }

    // destroy windows    
    IndexT i;
    for (i = 0; i < this->windows.Size(); i++)
    {
        if (this->windows[i].isvalid() && this->windows[i]->IsOpen())
        {
            // note window reference for checking if window is still active
            const Ptr<Window> window = this->windows[i];
            this->windows[i]->Close();
        }
    }
    this->windows.Clear();
	this->uiFactory = 0;
    
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
    Attaches a new window to the UIServer. This method is exclusively called
    by Window::Open()!
*/
void
UIServer::AttachWindow(Ptr<Window>& window)
{    
	n_assert(window.isvalid());
    n_assert(this->windows.Find(window) == 0);    
    this->windows.Append(window);
}

//------------------------------------------------------------------------------
/**
    Removes a window from the UIServer. This method is exclusively called
    by Window::Close()!
*/
void
UIServer::RemoveWindow(Ptr<Window>& window)
{
	n_assert(window.isvalid());
    int index = this->windows.FindIndex(window);

    if (index != InvalidIndex)
    {        
        this->windows[index] = 0;
        this->windows.EraseIndex(index);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
UIServer::Render()
{  
	// empty   	
}

//------------------------------------------------------------------------------
/**
*/
void
UIServer::RenderDebug()
{  
	// trigger all windows    
    IndexT i;
    for (i=0; i < this->windows.Size(); i++)
    {
        if (this->windows[i].isvalid())
        {
			this->windows[i]->OnRenderDebug();
        }
    }  	
}

//------------------------------------------------------------------------------
/**
    Trigger the ui server.
*/
void
UIServer::Trigger()
{
	this->HandleInput();

    this->inTrigger = true;
	// now trigger all windows
    // clean window array
    IndexT i;
    for (i=0; i < this->windows.Size(); i++)
    {
        if (this->windows[i].isvalid())
        {
			this->windows[i]->OnFrame();
        }
    }  
    this->inTrigger = false;

    // clean window array
    IndexT j;
    for (j=0; j < this->windows.Size(); j++)
    {
        if (!this->windows[j].isvalid())
        {
            this->windows.EraseIndex(j--);
        }
    }
}

//-----------------------------------------------------------------------------
/**
	Handle the mouse and keyboard input each frame
*/
void
UIServer::HandleInput()
{    
    if (!this->IsInputEnabled()) return;

	Ptr<Input::Mouse> mouse = Input::InputServer::Instance()->GetDefaultMouse();
	Ptr<Input::Keyboard> keyboard = Input::InputServer::Instance()->GetDefaultKeyboard();

    // record current mouse position
	this->mousePosition = mouse->GetScreenPosition();
    this->pixelMousePosition = mouse->GetPixelPosition();

	// MOUSE: check input and route
	if(this->IsMouseOverGui())
	{
		IndexT i;
        for (i = 0; i < this->windows.Size(); i++)
        {
			Util::Array<Ptr<Element>> elements;

			if (this->windows[i].isvalid() && this->windows[i]->Inside(this->mousePosition))
            {
				this->windows[i]->GetElementsAt(this->mousePosition, elements);
            }
        }
	}

	// KEYBOARD: check input and route 
	// TODO
}

//-----------------------------------------------------------------------------
/**
    Check if the mouse is currently over an GUI element.
*/
bool
UIServer::IsMouseOverGui() const
{
    int i;
    int num = this->windows.Size();
    for (i = 0; i < num; i++)
    {
        if (this->windows[i].isvalid())
        {
            if (this->windows[i]->Inside(this->GetMousePosition()))
            {
                return true;
            }
        }
    }
    return false;
}

//-----------------------------------------------------------------------------
/**
    This sets the input focus to a given control. All Events will first be sent
    to this object, then to all other control elements. This Method will be called
    by the control's register method. Don't call this method directly
*/
void
UIServer::SetFocusElement(Ptr<Element>& focus, bool reg)
{
	n_assert(focus.isvalid());
    if (this->focusElement.isvalid())
    {
        this->focusElement->OnFocusLost();
        this->focusElement = 0;
    }
    if (reg)
    {
        this->focusElement = focus;
        this->focusElement->OnFocusGain();
    }
}

} // namespace UI
