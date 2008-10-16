//------------------------------------------------------------------------------
//  uifeature/elements/button.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "uifeature/elements/button.h"
#include "uifeature/uiserver.h"

namespace UI
{
__ImplementClass(Button, 'UIBT', Control);

//------------------------------------------------------------------------------
/**
*/
Button::Button() :
    isMouseOver(false),
    isPressed(false),
    activeState(Normal),
    pressedTime(0.0)
{
}

//------------------------------------------------------------------------------
/**
*/
Button::~Button()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
Button::OnDestroy()
{
    Control::OnDestroy();
}

//------------------------------------------------------------------------------
/**
    Called when the button is created. This will lookup the visuals for
    the diverse visual states of the button from the Nebula2 node hierarchy.
*/
void
Button::OnCreate()
{
    // first hand to parent
    Control::OnCreate();  
}

//------------------------------------------------------------------------------
/**
    Called on mouse move. This sets the mouse over state of the button,
    and may cancel the pressed state if the mouse moves outside of the button.
*/
bool
Button::OnMouseMove(const Math::float2& mousePos)
{
    if (this->IsVisible() && this->IsEnabled() && this->Inside(mousePos))
    {
        this->isMouseOver = true;
    }
    else
    {
        this->isMouseOver = false;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
    Called on left mouse button down. This sets the pressed state of the
    button.
*/
bool
Button::OnLeftButtonDown(const Math::float2& mousePos)
{
    if (this->IsVisible() && this->IsEnabled())
    {
        // just update the pressed state, this does not handle the event
        if (this->Inside(mousePos))
        {
            this->RegisterAsFocusElement();
            this->SetPressed(true);
            return true;
        }
        else
        {
            this->SetPressed(false);
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
    Called on left mouse button up. This sends the button event.
*/
bool
Button::OnLeftButtonUp(const Math::float2& mousePos)
{
    bool handled = false;
    if (this->IsVisible() && this->IsEnabled() && this->Inside(mousePos) && this->IsPressed())
    {
        //this->PutEvent(this->GetEventName());
        handled = true;
    }
    if (this->IsFocusElement())
    {
        this->UnregisterAsFocusElement();
    }
    this->SetPressed(false);
    return handled;
}

//------------------------------------------------------------------------------
/**
*/
bool
Button::OnLeftButtonPressed(const Math::float2& mousePos)
{
    if (this->IsVisible() && this->IsEnabled() && this->Inside(mousePos))
    {
        //this->PutEvent(Util::String(this->GetEventName() + "_pressed"));
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
    Called before rendering happens. This updates the visibility of our
    Nebula2 visual nodes.
*/
void
Button::OnFrame()
{
    Control::OnFrame();    

    // update pressed state
    if (this->IsPressed())
    {
        this->pressedTime += UIServer::Instance()->GetFrameTime();

        if (!this->IsEnabled())
        {
            if (!this->OnLeftButtonUp(Math::float2()))
            {
                //this->PutEvent(this->GetEventName());
            }
        }
    }

    // now decide which one is visible
    //if (this->IsVisible())
    //{
    //    if (this->IsEnabled())
    //    {
    //        if (this->IsPressed() && this->IsMouseOver())
    //        {
    //            if (this->visuals[Pressed].isvalid())
    //            {
    //                this->SetActiveVisualState(Pressed);
    //            }
    //        }
    //        else if (this->IsMouseOver())
    //        {
    //            if (this->visuals[MouseOver].isvalid())
    //            {
    //                this->SetActiveVisualState(MouseOver);                    
    //            }
    //        }
    //        else
    //        {
    //            if (this->visuals[Normal].isvalid())
    //            {
    //                this->SetActiveVisualState(Normal);                    
    //            }
    //        }
    //    }
    //    else
    //    {
    //        if (this->visuals[Disabled].isvalid())
    //        {
    //            this->SetActiveVisualState(Disabled);                
    //        }
    //    }
    //}
}

//------------------------------------------------------------------------------
/**
*/
void
Button::SetStateTexture(VisualState visualState, const Util::String& texName)
{
    //if (visualState == NumVisualStates)
    //{
    //    IndexT i;
    //    for (i = 0; i < this->visuals.Size(); i++)
    //    {            
    //        if(this->visuals[i].isvalid())
    //        {
    //            this->SetStateTexture((VisualState)i, texName);
    //        }
    //    }
    //}
    //else
    //{        
    //    if(this->visuals[visualState].isvalid())
    //    {
    //        this->RecurseSetTexture(this->visuals[visualState], texName);
    //    }
    //    else
    //    {
    //        n_error("GetStateTexture called on invalid visual \"%s\" of Button \"%s\"", Button::VisualStateToString(visualState).Get(), this->id.Get());
    //    }
    //}
}

//------------------------------------------------------------------------------
/**
*/
Util::String
Button::GetStateTexture(VisualState visualState)
{   
    Util::String texName;
    //if (this->visuals[visualState].isvalid())
    //{
    //    texName = this->RecurseGetTexture(this->visuals[visualState]);
    //    n_assert2(texName.IsValid() && !texName.IsEmpty(), "Texture of Button not found!");
    //}
    //else
    //{
    //    n_error("GetStateTexture called on invalid visual \"%s\" of Button \"%s\"", Button::VisualStateToString(visualState).Get(), this->id.Get());
    //}
    return texName;
}

//------------------------------------------------------------------------------
/**
*/    
Util::String
Button::VisualStateToString(VisualState visualState)
{
    n_assert(visualState < NumVisualStates);
    switch (visualState)
    {
        case Normal:
            return "Normal";
        case Pressed:
            return "Pressed";
        case MouseOver:
            return "MouseOver";
        case Disabled:
            return "Disabled";
        default:
            return "Normal";
    }
}

//------------------------------------------------------------------------------
/**
*/
//void
//Button::CloneAttributes(Element* target)
//{
//    // don't call texture overwrite of label (!)
//    Control::CloneAttributes(target);
//
//    n_assert(target->IsA(UI::Button::RTTI));
//
//    Button* button = (Button*) target;
//
//    button->SetEventName(Util::String(this->eventName));
//    button->SetStateTexture(Normal, this->GetStateTexture(Normal));
//    button->SetStateTexture(Pressed, this->GetStateTexture(Pressed));
//    button->SetStateTexture(MouseOver, this->GetStateTexture(MouseOver));
//    button->SetStateTexture(Disabled, this->GetStateTexture(Disabled));    
//}

//------------------------------------------------------------------------------
/**
*/
void
Button::SetActiveVisualState(VisualState state)
{
    n_assert(state < NumVisualStates);
    
    //// first, hide all visuals
    //int i;
    //for (i = 0; i < NumVisualStates; i++)
    //{
    //    if (this->visuals[i].isvalid())
    //    {
    //        this->visuals[i]->SetActive(false);
    //    }
    //}

    //this->visuals[state]->SetActive(true);

    //if(this->activeState != state)
    //{
    //    this->activeState = state;
    //    this->SetDirty();
    //}
}

}; // namespace UI

