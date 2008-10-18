#pragma once
#ifndef MESSAGING_MESSAGE_H
#define MESSAGING_MESSAGE_H
//------------------------------------------------------------------------------
/**
    @class Messaging::Message

    Messages are packets of data which can be sent to a message port.
    This implements a universal communication mechanism within the same
    thread, different threads, or even different machines.

    Messages are implemented as normal C++ objects which can encode and
    decode themselves from and to a stream.

    (C) 2006 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "io/binaryreader.h"
#include "io/binarywriter.h"
#include "messaging/id.h"

//------------------------------------------------------------------------------
/**
    Message Id macros.
*/
#define DeclareMsgId \
public:\
    static Messaging::Id Id; \
    virtual const Messaging::Id& GetId() const;\
private:

#define ImplementMsgId(type) \
    Messaging::Id type::Id; \
    const Messaging::Id& type::GetId() const { return type::Id; }

//------------------------------------------------------------------------------
namespace Messaging
{
class MessageReader;
class MessageWriter;
class Port;

class Message : public Core::RefCounted
{
    __DeclareClass(Message);
    DeclareMsgId;
public:
    /// constructor
    Message();
    /// return true if message is of the given id
    bool CheckId(const Messaging::Id& id) const;
    /// encode message into a stream
    virtual void Encode(const Ptr<IO::BinaryWriter>& writer);
    /// decode message from a stream
    virtual void Decode(const Ptr<IO::BinaryReader>& reader);
    /// set the handled flag
    void SetHandled(bool b);
    /// return true if the message has been handled
    bool Handled() const;
protected:
    bool handled;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
Message::CheckId(const Messaging::Id& id) const
{
    return (id == this->GetId());
}

//------------------------------------------------------------------------------
/**
*/
inline void
Message::SetHandled(bool b)
{
    this->handled = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Message::Handled() const
{
    return this->handled;
}

} // namespace Messaging
//------------------------------------------------------------------------------
#endif