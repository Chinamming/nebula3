#pragma once
#ifndef COREUI_UIGRAPHICSHANDLER_H
#define COREUI_UIGRAPHICSHANDLER_H
//------------------------------------------------------------------------------
/**
    @class CoreUI::UIGraphicsHandler
    
    Handles visualization of the UI elements.
    
    (C) 2008 Radon Labs GmbH
*/
#include "messaging/handler.h"
#include "messaging/message.h"
#include "math/rectangle.h"

//------------------------------------------------------------------------------
namespace CoreUI
{
class RenderUIElements;

class UIGraphicsHandler : public Messaging::Handler
{
    __DeclareClass(UIGraphicsHandler);

public:
    /// constructor
    UIGraphicsHandler();
    /// destructor
    virtual ~UIGraphicsHandler();    
    /// handle a message, return true if handled
    virtual bool HandleMessage(const Ptr<Messaging::Message>& msg);

private:
    /// handle RenderDebugText message
    void OnRenderUIElement(const Ptr<RenderUIElements>& msg);
};

} // namespace Debug
//------------------------------------------------------------------------------
#endif
