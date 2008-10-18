//------------------------------------------------------------------------------
//  uifeature/uieventhandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "uifeature/uieventhandler.h"
#include "uifeature/uievent.h"

namespace UI
{
__ImplementClass(UIEventHandler, 'UIEH', Messaging::Port);

//------------------------------------------------------------------------------
/**
*/
UIEventHandler::UIEventHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
UIEventHandler::~UIEventHandler ()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Only accepts UI::Event messages.
*/
void
UIEventHandler::SetupAcceptedMessages()
{
    this->RegisterMessage(UIEvent::Id);
}

//------------------------------------------------------------------------------
/**
    The HandleMessage() method makes sure the incoming message is an
    UI::Event and routes the method to HandleEvent(). In your subclass,
    HandleEvent() should be overriden to implement your UI logic.
*/
void
UIEventHandler::HandleMessage(const Ptr<Messaging::Message>& msg)
{
    n_assert(msg->CheckId(UIEvent::Id));
    this->HandleEvent(msg.cast<UIEvent>());
}

//------------------------------------------------------------------------------
/**
    The HandleEvent() method is the front end for your UI logic implementation.
    All user interface events emitted by the current UI will be routed to
    this method.
*/
void
UIEventHandler::HandleEvent(const Ptr<UIEvent>& e)
{
    // empty (implement in subclass)
}

}; // namespace UI