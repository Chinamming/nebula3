#ifndef UI_UIEVENT_H
#define UI_UIEVENT_H
//------------------------------------------------------------------------------
/**
    @class UI::UIEvent
    
    User interface events are emitted by user interface and handled by the
    currently registered UI event handler. UIs usually consist of 2 parts,
    the user interface itself (a hierarchy of UI::Element objects, constructed
    from an XUI file) which represents the visual part, and an event handler 
    which implements the logic behind the UI.

    Naming conventions:

    - events starts with "UI" followed by an underscore "_"
    - eventnames are UPPERCASE
    - next the element name in a shortform of is appended (i.e. "UI_L" for list)
    - childs of the element are appended directly (no underscore, i.e. "UI_LITEM")
    - states are appended with an underscore (i.e. "UI_LITEM_SEL" for " list item selected)
    - eventnames never should be equal or less than 14 letters
    
    (C) 2005 Radon Labs GmbH
*/    
#include "messaging/message.h"
#include "uifeature/elements/element.h"

//------------------------------------------------------------------------------
namespace UI
{
class UIEvent : public Messaging::Message
{
	__DeclareClass(UIEvent);
    DeclareMsgId;
public:
    /// constructor
    UIEvent();
    /// set event name
	void SetEventName(const Util::String& n);
    /// get event name
    const Util::String& GetEventName() const;
    /// set pointer to element which emitted the event
    void SetElement(const Ptr<Element>& e);
    /// get pointer to element which emitted the event
    const Ptr<Element>& GetElement() const;

private:
    Util::String eventName;
    Ptr<Element> element;
};

//------------------------------------------------------------------------------
/**
*/
inline
UIEvent::UIEvent()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
void
UIEvent::SetEventName(const Util::String& n)
{
    this->eventName = n;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String&
UIEvent::GetEventName() const
{
    return this->eventName;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
UIEvent::SetElement(const Ptr<Element>& e)
{
    this->element = e;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Ptr<Element>&
UIEvent::GetElement() const
{
    return this->element;
}

}; // namespace UI
//------------------------------------------------------------------------------
#endif