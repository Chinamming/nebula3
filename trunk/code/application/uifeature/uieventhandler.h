#ifndef UI_UIEVENTHANDLER_H
#define UI_UIEVENTHANDLER_H
//------------------------------------------------------------------------------
/**
    @class UI::UIEventHandler
    
    An UI event handler implements the logic behind an user interface. It is
    implemented as a Message::Port which accepts messages of the type 
    UI::Event. Derive a subclass to implement your UI logic inside
    the HandleEvent() method.
*/
#include "messaging/port.h"

//------------------------------------------------------------------------------
namespace UI
{
class UIEvent;

class UIEventHandler : public Messaging::Port
{
	__DeclareClass(UIEventHandler);
public:
    /// constructor
    UIEventHandler();
    /// destructor 
    virtual ~UIEventHandler();
    /// override this method to handle your user interface events
    virtual void HandleEvent(const Ptr<UIEvent>& e);

protected:
    /// setup accepted messages
    virtual void SetupAcceptedMessages();
    /// this simply routes the message to HandleEvent()!
	virtual void HandleMessage(const Ptr<Messaging::Message>& msg);
};

}; // namespace UI
//------------------------------------------------------------------------------
#endif