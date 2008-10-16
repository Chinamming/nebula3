#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "util/array.h"
#include "threading/threadid.h"
#include "ui/uielement.h"

//------------------------------------------------------------------------------
namespace CoreUI
{
//------------------------------------------------------------------------------
class RenderUIElements : public Messaging::Message
{
    __DeclareClass(RenderUIElements);
    DeclareMsgId;
public:
    RenderUIElements() 
    { };
public:
    void SetThreadId(const Threading::ThreadId& val)
    {
        n_assert(!this->handled);
        this->threadid = val;
    };
    const Threading::ThreadId& GetThreadId() const
    {
        return this->threadid;
    };
private:
    Threading::ThreadId threadid;
public:
    void SetUIElements(const Util::Array<Ptr<UI::UIElement>>& val)
    {
        n_assert(!this->handled);
        this->uielements = val;
    };
    const Util::Array<Ptr<UI::UIElement>>& GetUIElements() const
    {
        return this->uielements;
    };
private:
    Util::Array<Ptr<UI::UIElement>> uielements;
};
} // namespace CoreUI
//------------------------------------------------------------------------------
