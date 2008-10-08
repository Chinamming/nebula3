//------------------------------------------------------------------------------
//  uifeature/elements/control.cc
//  (C) 2005 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "uifeature/elements/control.h"
#include "uifeature/uiserver.h"
#include "uifeature/elements/window.h"

namespace UI
{
__ImplementClass(Control, 'UICT', Element);

//------------------------------------------------------------------------------
/**
*/
Control::Control()/* :    
    tooltipDelay(0.1),
    mouseWithin(false),
    mouseWithinTime(0.0),
    tooltipEnabled(true)*/
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Control::~Control()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Parse GUI attributes on create.
*/
void
Control::OnCreate()
{
    Element::OnCreate();

    //// send an initial mouse move (so that highlights etc... will be switched
    //// on if the mouse is already over the element)
    //this->OnMouseMove(Server::Instance()->GetMousePosition());
}

//------------------------------------------------------------------------------
/**
*/
void
Control::OnDestroy()
{
    this->SetEnabled(false);
    Element::OnDestroy();
}

//------------------------------------------------------------------------------
/**
    Display tooltip if necessary.
*/
void
Control::OnFrame()
{
    Element::OnFrame();

    //Server* uiServer = Server::Instance();

//    // activate tooltip after mouse was over element for a while
//    if (this->IsVisible() && (uiServer->GetModalWindow() == 0 || uiServer->GetModalWindow() == this->GetParentWindow()))
//    {        
//        if (this->IsEnabled() 
//            && this->tooltipEnabled
//            && this->GetTooltip().IsValid() 
//            && this->Inside(uiServer->GetMousePosition()))
//        {
//            nTime time = uiServer->GetTime();
//            if (!this->mouseWithin)
//            {
//                this->mouseWithinTime = time;
//                this->mouseWithin = true;
//            }
//            else if(time > this->mouseWithinTime + this->GetTooltipDelay())
//            {
//                uiServer->ShowToolTip(this->GetTooltip());
//            }
//        }
//        else
//        {            
//            this->mouseWithin = false;
//        }
//    }
} 

//------------------------------------------------------------------------------
/**
*/
//void
//Control::CloneAttributes(Element* target)
//{
//    Element::CloneAttributes(target);
//
//    n_assert(target->IsA(UI::Control::RTTI));
//
//    Control* control = (Control*) target;
//
//    control->SetNavDown(this->navDown);
//    control->SetNavUp(this->navUp);
//    control->SetNavLeft(this->navLeft);
//    control->SetNavRight(this->navRight);        
//    control->SetEnabled(this->enabled);
//    control->SetTooltipDelay(this->tooltipDelay);
//    control->SetTooltip(this->tooltip);    
//}

}; // namespace UI
