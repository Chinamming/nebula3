#pragma once
#ifndef INTERFACE_COPYFILE_H
#define INTERFACE_COPYFILE_H
//------------------------------------------------------------------------------
/**
    @class Interface::CopyFile
    
    IOInterface message to asynchronously copy a file.
    
    (C) 2006 Radon Labs GmbH
*/
#include "messaging/message.h"

//------------------------------------------------------------------------------
namespace Interface
{
class CopyFile : public Messaging::Message
{
    DeclareClass(CopyFile);
    DeclareMsgId;
public:
    /// constructor
    CopyFile();
    /// set origin URI
    void SetFromURI(const IO::URI& uri);
    /// get origin URI
    const IO::URI& GetFromURI() const;
    /// set destination URI
    void SetToURI(const IO::URI& uri);
    /// get destination URI
    const IO::URI& GetToURI() const;
    /// set result of operation (true is success)
    void SetResult(bool b);
    /// get result of operation
    bool GetResult() const;
    /// encode message into a stream
    virtual void Encode(const Ptr<IO::BinaryWriter>& writer);
    /// decode message from a stream
    virtual void Decode(const Ptr<IO::BinaryReader>& reader);
private:
    IO::URI from;
    IO::URI to;
    bool result;
};

//------------------------------------------------------------------------------
/**
*/
inline
CopyFile::CopyFile() :
    result(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline void
CopyFile::SetFromURI(const IO::URI& uri)
{
    this->from = uri;
}

//------------------------------------------------------------------------------
/**
*/
inline const IO::URI&
CopyFile::GetFromURI() const
{
    return this->from;
}

//------------------------------------------------------------------------------
/**
*/
inline void
CopyFile::SetToURI(const IO::URI& uri)
{
    this->to = uri;
}

//------------------------------------------------------------------------------
/**
*/
inline const IO::URI&
CopyFile::GetToURI() const
{
    return this->to;
}

//------------------------------------------------------------------------------
/**
*/
inline void
CopyFile::SetResult(bool b)
{
    this->result = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
CopyFile::GetResult() const
{
    return this->result;
}

} // namespace Interface
//------------------------------------------------------------------------------
#endif
