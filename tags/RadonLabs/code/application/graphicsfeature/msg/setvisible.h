#ifndef MSG_SETVISIBLEMSG_H
#define MSG_SETVISIBLEMSG_H
//------------------------------------------------------------------------------
/**
    @class GraphicsFeature::SetVisible

    Shows or hides all graphics entities of a game entity.
    
    (C) 2007 Radon Labs GmbH
*/
#include "messaging/message.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class SetVisibleMsg : public Messaging::Message
{
    DeclareClass(SetVisibleMsg);
    DeclareMsgId;

public:
    /// constructor
    SetVisibleMsg();

    /// set visibility (true = show, false = hide)
    void SetVisible(bool visible);
    /// get visibility (true = show, false = hide)
    bool GetVisible() const;

private:
    bool visible;
};

//------------------------------------------------------------------------------
/**
*/
inline void
SetVisibleMsg::SetVisible(bool visible)
{
    this->visible = visible;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
SetVisibleMsg::GetVisible() const
{
    return this->visible;
}

}; // namespace GraphicsFeature
//------------------------------------------------------------------------------
#endif