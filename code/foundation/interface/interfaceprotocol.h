#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"

//------------------------------------------------------------------------------
namespace Interface
{
//------------------------------------------------------------------------------
class CopyFile : public Messaging::Message
{
    __DeclareClass(CopyFile);
    DeclareMsgId;
public:
    CopyFile() :
        result(false)
    { };
public:
    void SetFromURI(const IO::URI& val)
    {
        n_assert(!this->handled);
        this->fromuri = val;
    };
    const IO::URI& GetFromURI() const
    {
        return this->fromuri;
    };
private:
    IO::URI fromuri;
public:
    void SetToURI(const IO::URI& val)
    {
        n_assert(!this->handled);
        this->touri = val;
    };
    const IO::URI& GetToURI() const
    {
        return this->touri;
    };
private:
    IO::URI touri;
public:
    void SetResult(bool val)
    {
        n_assert(!this->handled);
        this->result = val;
    };
    bool GetResult() const
    {
        return this->result;
    };
private:
    bool result;
};
//------------------------------------------------------------------------------
class IOMessage : public Messaging::Message
{
    __DeclareClass(IOMessage);
    DeclareMsgId;
public:
    IOMessage() :
        result(false)
    { };
public:
    void SetURI(const IO::URI& val)
    {
        n_assert(!this->handled);
        this->uri = val;
    };
    const IO::URI& GetURI() const
    {
        return this->uri;
    };
private:
    IO::URI uri;
public:
    void SetResult(bool val)
    {
        n_assert(!this->handled);
        this->result = val;
    };
    bool GetResult() const
    {
        return this->result;
    };
private:
    bool result;
};
//------------------------------------------------------------------------------
class CreateDirectory : public IOMessage
{
    __DeclareClass(CreateDirectory);
    DeclareMsgId;
public:
    CreateDirectory() 
    { };
};
//------------------------------------------------------------------------------
class DeleteDirectory : public IOMessage
{
    __DeclareClass(DeleteDirectory);
    DeclareMsgId;
public:
    DeleteDirectory() 
    { };
};
//------------------------------------------------------------------------------
class DeleteFile : public IOMessage
{
    __DeclareClass(DeleteFile);
    DeclareMsgId;
public:
    DeleteFile() 
    { };
};
//------------------------------------------------------------------------------
class MountZipArchive : public IOMessage
{
    __DeclareClass(MountZipArchive);
    DeclareMsgId;
public:
    MountZipArchive() 
    { };
};
//------------------------------------------------------------------------------
class ReadStream : public IOMessage
{
    __DeclareClass(ReadStream);
    DeclareMsgId;
public:
    ReadStream() 
    { };
public:
    void SetStream(const Ptr<IO::Stream>& val)
    {
        n_assert(!this->handled);
        this->stream = val;
    };
    const Ptr<IO::Stream>& GetStream() const
    {
        return this->stream;
    };
private:
    Ptr<IO::Stream> stream;
};
//------------------------------------------------------------------------------
class WriteStream : public IOMessage
{
    __DeclareClass(WriteStream);
    DeclareMsgId;
public:
    WriteStream() 
    { };
public:
    void SetStream(const Ptr<IO::Stream>& val)
    {
        n_assert(!this->handled);
        this->stream = val;
    };
    const Ptr<IO::Stream>& GetStream() const
    {
        return this->stream;
    };
private:
    Ptr<IO::Stream> stream;
};
} // namespace Interface
//------------------------------------------------------------------------------
