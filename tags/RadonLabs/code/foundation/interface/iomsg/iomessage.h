#pragma once
#ifndef INTERFACE_IOMESSAGE_H
#define INTERFACE_IOMESSAGE_H
//------------------------------------------------------------------------------
/**
    @class Interface::IOMessage
    
    Base class for IOInterface messages.
    
    (C) 2006 Radon Labs GmbH
*/
#include "messaging/message.h"

//------------------------------------------------------------------------------
namespace Interface
{
class IOMessage : public Messaging::Message
{
    DeclareClass(IOMessage);
    DeclareMsgId;
public:
    /// constructor
    IOMessage();
    /// in: set URI of new directory
    void SetURI(const IO::URI& uri);
    /// in: get URI of new directory
    const IO::URI& GetURI() const;
    /// set result of operation (true is success)
    void SetResult(bool b);
    /// get result of operation
    bool GetResult() const;
    /// encode message into a stream
    virtual void Encode(const Ptr<IO::BinaryWriter>& writer);
    /// decode message from a stream
    virtual void Decode(const Ptr<IO::BinaryReader>& reader);

protected:
    IO::URI uri;
    bool result;
};

//------------------------------------------------------------------------------
/**
*/
inline void
IOMessage::SetURI(const IO::URI& u)
{
    this->uri = u;
}

//------------------------------------------------------------------------------
/**
*/
inline const IO::URI&
IOMessage::GetURI() const
{
    return this->uri;
}

//------------------------------------------------------------------------------
/**
*/
inline void
IOMessage::SetResult(bool b)
{
    this->result = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
IOMessage::GetResult() const
{
    return this->result;
}

} // namespace Interface
//------------------------------------------------------------------------------
#endif

