#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "scripting/command.h"
#include "io/ioserver.h"
#include "io/assign.h"
#include "io/console.h"

//------------------------------------------------------------------------------
namespace Commands
{
class IOLibrary
{
public:
    /// register commands
    static void Register();
};
//------------------------------------------------------------------------------
class SetAssign : public Scripting::Command
{
    DeclareClass(SetAssign);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    void Callback(const Util::String& assign, const Util::String& path);
};
//------------------------------------------------------------------------------
class GetAssign : public Scripting::Command
{
    DeclareClass(GetAssign);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    Util::String Callback(const Util::String& assign);
};
//------------------------------------------------------------------------------
class HasAssign : public Scripting::Command
{
    DeclareClass(HasAssign);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    bool Callback(const Util::String& assign);
};
//------------------------------------------------------------------------------
class ClearAssign : public Scripting::Command
{
    DeclareClass(ClearAssign);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    void Callback(const Util::String& assign);
};
//------------------------------------------------------------------------------
class ResolveAssigns : public Scripting::Command
{
    DeclareClass(ResolveAssigns);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    Util::String Callback(const Util::String& path);
};
//------------------------------------------------------------------------------
class CreateDirectory : public Scripting::Command
{
    DeclareClass(CreateDirectory);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    bool Callback(const Util::String& path);
};
//------------------------------------------------------------------------------
class CopyFile : public Scripting::Command
{
    DeclareClass(CopyFile);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    bool Callback(const Util::String& sourceFile, const Util::String& targetFile);
};
//------------------------------------------------------------------------------
class DeleteFile : public Scripting::Command
{
    DeclareClass(DeleteFile);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    bool Callback(const Util::String& fileName);
};
//------------------------------------------------------------------------------
class DeleteDirectory : public Scripting::Command
{
    DeclareClass(DeleteDirectory);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    bool Callback(const Util::String& path);
};
//------------------------------------------------------------------------------
class FileExists : public Scripting::Command
{
    DeclareClass(FileExists);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    bool Callback(const Util::String& file);
};
//------------------------------------------------------------------------------
class ListFiles : public Scripting::Command
{
    DeclareClass(ListFiles);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    Util::Array<Util::String> Callback(const Util::String& path, const Util::String& pattern);
};
//------------------------------------------------------------------------------
class ListDirectories : public Scripting::Command
{
    DeclareClass(ListDirectories);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    Util::Array<Util::String> Callback(const Util::String& path, const Util::String& pattern);
};
//------------------------------------------------------------------------------
class Print : public Scripting::Command
{
    DeclareClass(Print);
public:
    virtual void OnRegister();
    virtual bool OnExecute();
    virtual Util::String GetHelp() const;
private:
    void Callback(const Util::String& str);
};
} // namespace Commands
//------------------------------------------------------------------------------
