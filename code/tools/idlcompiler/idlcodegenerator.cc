//------------------------------------------------------------------------------
//  idlcodegenerator.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/idlcompiler/idlcodegenerator.h"
#include "io/ioserver.h"

namespace Tools
{
ImplementClass(Tools::IDLCodeGenerator, 'IDCG', Core::RefCounted);

using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
IDLCodeGenerator::IDLCodeGenerator()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
URI
IDLCodeGenerator::BuildHeaderUri() const
{
    Util::String str = this->uri.AsString();
    str.StripFileExtension();
    str.Append(".h");
    URI uri(str);
    return uri;
}

//------------------------------------------------------------------------------
/**
*/
URI
IDLCodeGenerator::BuildSourceUri() const
{
    Util::String str = this->uri.AsString();
    str.StripFileExtension();
    str.Append(".cc");
    URI uri(str);
    return uri;
}

//------------------------------------------------------------------------------
/**
*/
void
IDLCodeGenerator::SetError(const char* fmt, ...)
{
    va_list argList;
    va_start(argList, fmt);
    this->error.FormatArgList(fmt, argList);
    va_end(argList);
}

//------------------------------------------------------------------------------
/**
*/
bool
IDLCodeGenerator::GenerateIncludeFile()
{
    n_assert(this->uri.IsValid());
    n_assert(this->document.isvalid());
    
    // open header file
    URI uri = this->BuildHeaderUri();
    Ptr<Stream> stream = IoServer::Instance()->CreateStream(uri);
    stream->SetAccessMode(Stream::WriteAccess);
    if (stream->Open())
    {
        Ptr<TextWriter> writer = TextWriter::Create();
        writer->SetStream(stream);
        if (writer->Open())
        {
            this->WriteIncludeHeader(writer);
            this->WriteLibraryDeclarations(writer);
            this->WriteProtocolDeclarations(writer);
            this->WriteIncludeFooter(writer);
            writer->Close();
        }
        else
        {
            n_printf("Could not open text writer on stream '%s'!\n", uri.AsString().AsCharPtr());
            return false;
        }
        stream->Close();
    }
    else
    {
        n_printf("Could not open stream '%s' for writing!\n", uri.AsString().AsCharPtr());
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
IDLCodeGenerator::GenerateSourceFile()
{
    n_assert(this->uri.IsValid());
    n_assert(this->document.isvalid());

    // open source file
    URI uri = this->BuildSourceUri();
    Ptr<Stream> stream = IoServer::Instance()->CreateStream(uri);
    stream->SetAccessMode(Stream::WriteAccess);
    if (stream->Open())
    {
        Ptr<TextWriter> writer = TextWriter::Create();
        writer->SetStream(stream);
        if (writer->Open())
        {
            this->WriteSourceHeader(writer);
            this->WriteLibraryImplementations(writer);
            this->WriteSourceFooter(writer);
            writer->Close();
        }
        else
        {
            n_printf("Could not open text writer on stream '%s'!\n", uri.AsString().AsCharPtr());
            return false;
        }
        stream->Close();
    }
    else
    {
        n_printf("Could not open stream '%s' for writing!\n", uri.AsString().AsCharPtr());
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
/**
    Converts a IDL type string into a Nebula C++ reference type.
*/
String
IDLCodeGenerator::GetNebulaRefType(const String& type) const
{
    if ("string" == type)               return "const Util::String&";
    else if ("vector3" == type)         return "const Math::vector3&";
    else if ("vector4" == type)         return "const Math::vector4&";
    else if ("matrix44" == type)        return "const Math::matrix44&";
    else if ("bool" == type)            return "bool";
    else if ("guid" == type)            return "const Util::Guid&";
    else if ("int" == type)             return "int";
    else if ("intArray" == type)        return "const Util::Array<int>&";
    else if ("floatArray" == type)      return "const Util::Array<float>&";
    else if ("boolArray" == type)       return "const Util::Array<bool>&";
    else if ("stringArray" == type)     return "const Util::Array<Util::String>&";
    else if ("vector3Array" == type)    return "const Util::Array<Math::vector3>&";
    else if ("vector4Array" == type)    return "const Util::Array<Math::vector4>&";
    else if ("matrix44Array" == type)   return "const Util::Array<Math::matrix44>&";
    else if ("guidArray" == type)       return "const Util::Array<Util::Guid>&";
    else
    {
        n_error("Invalid IDL type '%s'!", type.AsCharPtr());
        return "";
    }
}

//------------------------------------------------------------------------------
/**
    Converts a IDL type string into a Nebula C++ type.
*/
String
IDLCodeGenerator::GetNebulaType(const String& type) const
{
    if ("string" == type)               return "Util::String";
    else if ("vector3" == type)         return "Math::vector3";
    else if ("vector4" == type)         return "Math::vector4";
    else if ("matrix44" == type)        return "Math::matrix44";
    else if ("bool" == type)            return "bool";
    else if ("guid" == type)            return "Util::Guid";
    else if ("int" == type)             return "int";
    else if ("intArray" == type)        return "Util::Array<int>";
    else if ("floatArray" == type)      return "Util::Array<float>";
    else if ("boolArray" == type)       return "Util::Array<bool>";
    else if ("stringArray" == type)     return "Util::Array<Util::String>";
    else if ("vector3Array" == type)    return "Util::Array<Math::vector3>";
    else if ("vector4Array" == type)    return "Util::Array<Math::vector4>";
    else if ("matrix44Array" == type)   return "Util::Array<Math::matrix44>";
    else if ("guidArray" == type)       return "Util::Array<Util::Guid>";
    else
    {
        n_error("Invalid IDL type '%s'!", type.AsCharPtr());
        return "";
    }
}

//------------------------------------------------------------------------------
/**
    Converts a IDL type string into a Nebula C++ type.
*/
String
IDLCodeGenerator::GetNebulaArgType(const String& type) const
{
    if ("string" == type)               return "Scripting::Arg::String";
    else if ("vector3" == type)         return "Scripting::Arg::Vector3";
    else if ("vector4" == type)         return "Scripting::Arg::Vector4";
    else if ("matrix44" == type)        return "Scripting::Arg::Matrix44";
    else if ("bool" == type)            return "Scripting::Arg::Bool";
    else if ("guid" == type)            return "Scripting::Arg::Guid";
    else if ("int" == type)             return "Scripting::Arg::Int";
    else if ("intArray" == type)        return "Scripting::Arg::IntArray";
    else if ("floatArray" == type)      return "Scripting::Arg::FloatArray";
    else if ("boolArray" == type)       return "Scripting::Arg::BoolArray";
    else if ("stringArray" == type)     return "Scripting::Arg::StringArray";
    else if ("vector3Array" == type)    return "Scripting::Arg::Vector3Array";
    else if ("vector4Array" == type)    return "Scripting::Arg::Vector4Array";
    else if ("matrix44Array" == type)   return "Scripting::Arg::Matrix44Array";
    else if ("guidArray" == type)       return "Scripting::Arg::GuidArray";
    else
    {
        n_error("Invalid IDL type '%s'!", type.AsCharPtr());
        return "";
    }
}

//------------------------------------------------------------------------------
/**
    Converts a IDL type string into a Nebula getter method name.
*/
String
IDLCodeGenerator::GetNebulaGetterMethod(const String& type) const
{
    if ("string" == type)               return "GetString()";
    else if ("vector3" == type)         return "GetVector3()";
    else if ("vector4" == type)         return "GetVector4()";
    else if ("matrix44" == type)        return "GetMatrix44()";
    else if ("bool" == type)            return "GetBool()";
    else if ("guid" == type)            return "GetGuid()";
    else if ("int" == type)             return "GetInt()";
    else if ("intArray" == type)        return "GetIntArray()";
    else if ("floatArray" == type)      return "GetFloatArray()";
    else if ("boolArray" == type)       return "GetBoolArray()";
    else if ("stringArray" == type)     return "GetStringArray()";
    else if ("vector3Array" == type)    return "GetVector3Array()";
    else if ("vector4Array" == type)    return "GetVector4Array()";
    else if ("matrix44Array" == type)   return "GetMatrix44Array()";
    else if ("guidArray" == type)       return "GetGuidArray";
    else
    {
        n_error("Invalid IDL type '%s'!", type.AsCharPtr());
        return "";
    }
}

//------------------------------------------------------------------------------
/**
    Converts a IDL type string into a Nebula setter method name.
*/
String
IDLCodeGenerator::GetNebulaSetterMethod(const String& type) const
{
    if ("string" == type)               return "SetString";
    else if ("vector3" == type)         return "SetVector3";
    else if ("vector4" == type)         return "SetVector4";
    else if ("matrix44" == type)        return "SetMatrix44";
    else if ("bool" == type)            return "SetBool";
    else if ("guid" == type)            return "SetGuid";
    else if ("int" == type)             return "SetInt";
    else if ("intArray" == type)        return "SetIntArray";
    else if ("floatArray" == type)      return "SetFloatArray";
    else if ("boolArray" == type)       return "SetBoolArray";
    else if ("stringArray" == type)     return "SetStringArray";
    else if ("vector3Array" == type)    return "SetVector3Array";
    else if ("vector4Array" == type)    return "SetVector4Array";
    else if ("matrix44Array" == type)   return "SetMatrix44Array";
    else if ("guidArray" == type)       return "SetGuidArray";
    else
    {
        n_error("Invalid IDL type '%s'!", type.AsCharPtr());
        return "";
    }
}

//------------------------------------------------------------------------------
/**
*/
String
IDLCodeGenerator::BuildCallbackPrototype(IDLCommand* cmd, bool withClassName) const
{
    const Util::Array<Ptr<IDLArg> > inArgs = cmd->GetInputArgs();
    const Util::Array<Ptr<IDLArg> > outArgs = cmd->GetOutputArgs();
    n_assert(outArgs.Size() <= 1);
    
    String str;
    if (outArgs.Size() > 0)
    {
        str.Append(this->GetNebulaType(outArgs[0]->GetType()));
    }
    else
    {
        str.Append("void");
    }
    str.Append(" ");
    if (withClassName)
    {
        str.Append(cmd->GetName());
        str.Append("::");
    }
    str.Append("Callback(");
    IndexT inArgIndex;
    for (inArgIndex = 0; inArgIndex < inArgs.Size(); inArgIndex++)
    {
        str.Append(this->GetNebulaRefType(inArgs[inArgIndex]->GetType()));
        str.Append(" ");
        str.Append(inArgs[inArgIndex]->GetName());
        if (inArgIndex < (inArgs.Size() - 1))
        {
            str.Append(", ");
        }
    }
    str.Append(")");
    return str;
}

//------------------------------------------------------------------------------
/**
*/
void
IDLCodeGenerator::WriteIncludeHeader(TextWriter* writer) const
{
    n_assert(0 != writer);
    writer->WriteLine("#pragma once");
    writer->WriteLine("//------------------------------------------------------------------------------");
    writer->WriteLine("/**");
    writer->WriteLine("    This file was generated with Nebula3's idlc compiler tool.");
    writer->WriteLine("    DO NOT EDIT");
    writer->WriteLine("*/");
    if (!this->document->GetLibraries().IsEmpty())
    {
        writer->WriteLine("#include \"scripting/command.h\"");
    }
    if (!this->document->GetProtocols().IsEmpty())
    {
        writer->WriteLine("#include \"messaging/message.h\"");
    }

    // write dependencies
    const Array<Ptr<IDLLibrary> >& libs = this->document->GetLibraries();
    IndexT libIndex;
    for (libIndex = 0; libIndex < libs.Size(); libIndex++)
    {
        const Ptr<IDLLibrary>& curLib = libs[libIndex];
        IndexT depIndex;
        for (depIndex = 0; depIndex < curLib->GetDependencies().Size(); depIndex++)
        {
            IDLDependency* dep = curLib->GetDependencies()[depIndex];
            writer->WriteFormatted("#include \"%s\"\n", dep->GetHeader().AsCharPtr());
        }
    }
    const Array<Ptr<IDLProtocol> >& protocols = this->document->GetProtocols();
    IndexT protIndex;
    for (protIndex = 0; protIndex < protocols.Size(); protIndex++)
    {
        const Ptr<IDLProtocol>& curProt = protocols[protIndex];
        IndexT depIndex;
        for (depIndex = 0; depIndex < curProt->GetDependencies().Size(); depIndex++)
        {
            IDLDependency* dep = curProt->GetDependencies()[depIndex];
            writer->WriteFormatted("#include \"%s\"\n", dep->GetHeader().AsCharPtr());
        }
    }
    writer->WriteLine("");
}

//------------------------------------------------------------------------------
/**
*/
void
IDLCodeGenerator::WriteLibraryDeclarations(TextWriter* writer) const
{
    n_assert(0 != writer);
    if (!this->document->GetLibraries().IsEmpty())
    {
        writer->WriteLine("//------------------------------------------------------------------------------");
        writer->WriteLine("namespace Commands");
        writer->WriteLine("{");

        // for each library definition...
        const Array<Ptr<IDLLibrary> >& libs = this->document->GetLibraries();
        IndexT libIndex;
        SizeT numLibs = libs.Size();
        for (libIndex = 0; libIndex < numLibs; libIndex++)
        {
            IDLLibrary* curLib = libs[libIndex];

            // write library class definition
            writer->WriteFormatted("class %s\n", curLib->GetName().AsCharPtr());
            writer->WriteLine("{");
            writer->WriteLine("public:");
            writer->WriteLine("    /// register commands");
            writer->WriteLine("    static void Register();");
            writer->WriteLine("};");

            // for each command in the library...
            const Array<Ptr<IDLCommand> >& cmds = curLib->GetCommands();
            IndexT cmdIndex;
            SizeT numCmds = cmds.Size();
            for (cmdIndex = 0; cmdIndex < numCmds; cmdIndex++)
            {
                this->WriteCommandDeclaration(cmds[cmdIndex], writer);
            }
        }
        writer->WriteLine("} // namespace Commands");
    }
}

//------------------------------------------------------------------------------
/**
    FIXME: hmm, enable different namespaces for message protocols?
*/
void
IDLCodeGenerator::WriteProtocolDeclarations(TextWriter* writer) const
{
    n_assert(0 != writer);
    if (!this->document->GetProtocols().IsEmpty())
    {
        // for each protocol definition...
        const Array<Ptr<IDLProtocol> >& protocols = this->document->GetProtocols();
        IndexT protocolIndex;
        SizeT numProtocols = protocols.Size();
        for (protocolIndex = 0; protocolIndex < numProtocols; protocolIndex++)
        {
            IDLProtocol* curProtocol = protocols[protocolIndex];

            writer->WriteLine("//------------------------------------------------------------------------------");
            writer->WriteFormatted("namespace %s\n", curProtocol->GetNameSpace().AsCharPtr());
            writer->WriteLine("{");

            // for each message in the protocol:
            const Array<Ptr<IDLMessage> >& msgs = curProtocol->GetMessages();
            IndexT msgIndex;
            for (msgIndex = 0; msgIndex < msgs.Size(); msgIndex++)
            {
                this->WriteMessageDeclaration(msgs[msgIndex], writer);
            }
            writer->WriteFormatted("} // namespace %s\n", curProtocol->GetNameSpace().AsCharPtr());
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
IDLCodeGenerator::WriteCommandDeclaration(IDLCommand* cmd, TextWriter* writer) const
{
    n_assert(0 != cmd);
    n_assert(0 != writer);

    writer->WriteLine("//------------------------------------------------------------------------------");
    writer->WriteFormatted("class %s : public Scripting::Command\n", cmd->GetName().AsCharPtr());
    writer->WriteLine("{");
    writer->WriteFormatted("    DeclareClass(%s);\n", cmd->GetName().AsCharPtr());
    writer->WriteLine("public:");
    writer->WriteLine("    virtual void OnRegister();");
    writer->WriteLine("    virtual bool OnExecute();");
    writer->WriteLine("    virtual Util::String GetHelp() const;");
    writer->WriteLine("private:");
    writer->WriteLine("    " + this->BuildCallbackPrototype(cmd, false) + ";");
    writer->WriteLine("};");
}

//------------------------------------------------------------------------------
/**
*/
void
IDLCodeGenerator::WriteMessageDeclaration(IDLMessage* msg, TextWriter* writer) const
{
    n_assert(0 != msg);
    n_assert(0 != writer);

    // build parent class name
    String parentClass = msg->GetParentClass();
    if (!parentClass.IsValid())
    {
        parentClass = "Messaging::Message";
    }

    writer->WriteLine("//------------------------------------------------------------------------------");
    writer->WriteFormatted("class %s : public %s\n", msg->GetName().AsCharPtr(), parentClass.AsCharPtr());
    writer->WriteLine("{");
    writer->WriteFormatted("    DeclareClass(%s);\n", msg->GetName().AsCharPtr());
    writer->WriteLine("    DeclareMsgId;");
    writer->WriteLine("public:");
    
    // write constructor 
    String initList;
    Array<Ptr<IDLArg> > args = msg->GetInputArgs();
    args.AppendArray(msg->GetOutputArgs());
    IndexT i;
    for (i = 0; i < args.Size(); i++)
    {
        if (args[i]->GetDefaultValue().IsValid())
        {
            if (!initList.IsEmpty())
            {
                initList.Append(",\n");
            }
            String argMemberName = args[i]->GetName();
            argMemberName.ToLower();
            initList.Append("        ");
            initList.Append(argMemberName);
            initList.Append("(");
            initList.Append(args[i]->GetDefaultValue());
            initList.Append(")");
        }
    }
    writer->WriteFormatted("    %s() %s\n", msg->GetName().AsCharPtr(), initList.IsValid() ? ":" : "");
    if (initList.IsValid())
    {
        writer->WriteString(initList);
        writer->WriteLine("");
    }
    writer->WriteLine("    { };");

    // write input args
    const Array<Ptr<IDLArg> >& inArgs = msg->GetInputArgs();
    IndexT argIndex;
    for (argIndex = 0; argIndex < inArgs.Size(); argIndex++)
    {
        this->WriteMessageArg(inArgs[argIndex], writer, true);
    }

    // write output args
    const Array<Ptr<IDLArg> >& outArgs = msg->GetOutputArgs();
    for (argIndex = 0; argIndex < outArgs.Size(); argIndex++)
    {
        this->WriteMessageArg(outArgs[argIndex], writer, false);
    }

    writer->WriteLine("};");
}

//------------------------------------------------------------------------------
/**
*/
void
IDLCodeGenerator::WriteMessageArg(IDLArg* arg, TextWriter* writer, bool isInputArg) const
{
    n_assert(0 != arg);
    n_assert(0 != writer);

    // build some string which are needed multiple times
    String argMemberName = arg->GetName();
    argMemberName.ToLower();
    String argTypeString;
    if ((arg->GetType().FindStringIndex("::") != InvalidIndex) &&
        (arg->GetType().FindStringIndex("*") == InvalidIndex))
    {
        // a complex type, pass by reference
        argTypeString.Append("const ");
        argTypeString.Append(arg->GetType());
        argTypeString.Append("&");
    }
    else
    {
        // a simple builtin type
        argTypeString = arg->GetType();
    }

    // write setter
    String str;
    str.Append("public:\n");
    str.Append("    void Set");
    str.Append(arg->GetName());
    str.Append("(");
    str.Append(argTypeString);
    str.Append(" val)\n");
    str.Append("    {\n");
    str.Append("        n_assert(!this->handled);\n");
    str.Append("        this->");
    str.Append(argMemberName);
    str.Append(" = val;\n");
    str.Append("    };\n");

    // write getter
    str.Append("    ");
    str.Append(argTypeString);
    str.Append(" Get");
    str.Append(arg->GetName());
    str.Append("() const\n");
    str.Append("    {\n");
    if (!isInputArg)
    {
        str.Append("        n_assert(this->handled);\n");
    }
    str.Append("        return this->");
    str.Append(argMemberName);
    str.Append(";\n");
    str.Append("    };\n");

    // write member
    str.Append("private:\n");
    str.Append("    ");
    str.Append(arg->GetType());
    str.Append(" ");
    str.Append(argMemberName);
    str.Append(";\n");
    
    // write setter, getter and member to TextWriter
    writer->WriteString(str);
}

//------------------------------------------------------------------------------
/**
*/
void
IDLCodeGenerator::WriteIncludeFooter(TextWriter* writer) const
{
    n_assert(0 != writer);
    writer->WriteLine("//------------------------------------------------------------------------------");
}

//------------------------------------------------------------------------------
/**
*/
void
IDLCodeGenerator::WriteSourceHeader(TextWriter* writer) const
{
    n_assert(0 != writer);
    writer->WriteLine("//------------------------------------------------------------------------------");
    writer->WriteLine("//  MACHINE GENERATED, DON'T EDIT!");
    writer->WriteLine("//------------------------------------------------------------------------------");
    
    URI headerUri = this->BuildHeaderUri();
    String headerFile = headerUri.AsString().ExtractFileName();
    writer->WriteLine("#include \"stdneb.h\"");
    if (!this->document->GetLibraries().IsEmpty())
    {
        writer->WriteLine("#include \"scripting/scriptserver.h\"");
        writer->WriteLine("#include \"scripting/arg.h\"");
    }
    writer->WriteFormatted("#include \"%s\"\n", headerFile.AsCharPtr());
    writer->WriteLine("");

    // write ImplementClass macros
    if (!this->document->GetLibraries().IsEmpty())
    {
        const Array<Ptr<IDLLibrary> >& libs = this->document->GetLibraries();
        IndexT libIndex;
        for (libIndex = 0; libIndex < libs.Size(); libIndex++)
        {
            const Ptr<IDLLibrary>& curLib = libs[libIndex];
            IndexT cmdIndex;
            for (cmdIndex = 0; cmdIndex < curLib->GetCommands().Size(); cmdIndex++)
            {
                const Ptr<IDLCommand>& cmd = curLib->GetCommands()[cmdIndex];
                writer->WriteFormatted("ImplementClass(Commands::%s, '%s', Scripting::Command);\n",
                    cmd->GetName().AsCharPtr(), cmd->GetFourCC().AsCharPtr());
            }
        }
    }
    if (!this->document->GetProtocols().IsEmpty())
    {
        const Array<Ptr<IDLProtocol> >& protocols = this->document->GetProtocols();
        IndexT protIndex;
        for (protIndex = 0; protIndex < protocols.Size(); protIndex++)
        {
            const Ptr<IDLProtocol>& curProt = protocols[protIndex];
            writer->WriteFormatted("namespace %s\n", curProt->GetNameSpace().AsCharPtr());
            writer->WriteLine("{");
            IndexT msgIndex;
            for (msgIndex = 0; msgIndex < curProt->GetMessages().Size(); msgIndex++)
            {
                const Ptr<IDLMessage>& msg = curProt->GetMessages()[msgIndex];
                writer->WriteFormatted("    ImplementClass(%s::%s, '%s', Messaging::Message);\n",
                    curProt->GetNameSpace().AsCharPtr(),
                    msg->GetName().AsCharPtr(),
                    msg->GetFourCC().AsCharPtr());
                writer->WriteFormatted("    ImplementMsgId(%s);\n", msg->GetName().AsCharPtr());
            }
            writer->WriteFormatted("} // %s\n", curProt->GetNameSpace().AsCharPtr());
        }
    }
    writer->WriteLine("");
}

//------------------------------------------------------------------------------
/**
*/
void
IDLCodeGenerator::WriteSourceFooter(TextWriter* writer) const
{
    n_assert(0 != writer);
    writer->WriteLine("//------------------------------------------------------------------------------");
}

//------------------------------------------------------------------------------
/**
*/
void
IDLCodeGenerator::WriteLibraryImplementations(TextWriter* writer) const
{
    n_assert(0 != writer);
    
    writer->WriteLine("namespace Commands");
    writer->WriteLine("{");

    // for each library definition...
    const Array<Ptr<IDLLibrary> >& libs = this->document->GetLibraries();
    IndexT libIndex;
    SizeT numLibs = libs.Size();
    for (libIndex = 0; libIndex < numLibs; libIndex++)
    {
        IDLLibrary* curLib = libs[libIndex];
        writer->WriteLine("//------------------------------------------------------------------------------");
        writer->WriteLine("/**");
        writer->WriteLine("*/");
        writer->WriteLine("void");
        writer->WriteFormatted("%s::Register()\n", curLib->GetName().AsCharPtr());
        writer->WriteLine("{");
        writer->WriteLine("    Scripting::ScriptServer* scriptServer = Scripting::ScriptServer::Instance();");
        IndexT cmdIndex;
        for (cmdIndex = 0; cmdIndex < curLib->GetCommands().Size(); cmdIndex++)
        {
            IDLCommand* cmd = curLib->GetCommands()[cmdIndex];
            String scriptCommandName = cmd->GetName();
            scriptCommandName.ToLower();
            writer->WriteFormatted("    scriptServer->RegisterCommand(\"%s\", %s::Create());\n", 
                scriptCommandName.AsCharPtr(), cmd->GetName().AsCharPtr());
        }
        writer->WriteLine("}");
        writer->WriteLine("");

        // write command implementations
        for (cmdIndex = 0; cmdIndex < curLib->GetCommands().Size(); cmdIndex++)
        {
            IDLCommand* cmd = curLib->GetCommands()[cmdIndex];
            this->WriteCommandImplementation(cmd, writer);
        }
    }
    writer->WriteLine("} // namespace Commands");
}

//------------------------------------------------------------------------------
/**
*/
void
IDLCodeGenerator::WriteCommandImplementation(IDLCommand* cmd, TextWriter* writer) const
{
    n_assert(0 != cmd);
    n_assert(0 != writer);

    // OnRegister()
    writer->WriteLine("//------------------------------------------------------------------------------");
    writer->WriteLine("/**");
    writer->WriteLine("*/");
    writer->WriteLine("void");
    writer->WriteFormatted("%s::OnRegister()\n", cmd->GetName().AsCharPtr());
    writer->WriteLine("{");
    writer->WriteLine("    Scripting::Command::OnRegister();");
    IndexT inArgIndex;
    for (inArgIndex = 0; inArgIndex < cmd->GetInputArgs().Size(); inArgIndex++)
    {
        IDLArg* arg = cmd->GetInputArgs()[inArgIndex];
        writer->WriteFormatted("    this->args.AddArg(\"%s\", %s);\n", 
            arg->GetName().AsCharPtr(), this->GetNebulaArgType(arg->GetType()).AsCharPtr());
    }
    IndexT outArgIndex;
    for (outArgIndex = 0; outArgIndex < cmd->GetOutputArgs().Size(); outArgIndex++)
    {
        IDLArg* arg = cmd->GetOutputArgs()[outArgIndex];
        writer->WriteFormatted("    this->results.AddArg(\"%s\", %s);\n",
            arg->GetName().AsCharPtr(), this->GetNebulaArgType(arg->GetType()).AsCharPtr());
    }
    writer->WriteLine("}");
    writer->WriteLine("");

    // GetHelp()
    writer->WriteLine("//------------------------------------------------------------------------------");
    writer->WriteLine("/**");
    writer->WriteLine("*/");
    writer->WriteLine("Util::String");
    writer->WriteFormatted("%s::GetHelp() const\n", cmd->GetName().AsCharPtr());
    writer->WriteLine("{");
    writer->WriteFormatted("    return \"%s\";\n", cmd->GetDesc().AsCharPtr());
    writer->WriteLine("}");

    // OnExecute()
    writer->WriteLine("//------------------------------------------------------------------------------");
    writer->WriteLine("/**");
    writer->WriteLine("*/");
    writer->WriteLine("bool");
    writer->WriteFormatted("%s::OnExecute()\n", cmd->GetName().AsCharPtr());
    writer->WriteLine("{");
    
    // marshal input args
    for (inArgIndex = 0; inArgIndex < cmd->GetInputArgs().Size(); inArgIndex++)
    {
        IDLArg* arg = cmd->GetInputArgs()[inArgIndex];
        writer->WriteFormatted("    %s %s = this->args.GetArgValue(%d).%s;\n",
            this->GetNebulaRefType(arg->GetType()).AsCharPtr(),
            arg->GetName().AsCharPtr(),
            inArgIndex,
            this->GetNebulaGetterMethod(arg->GetType()).AsCharPtr());
    }

    // generate callback method call
    String str;
    str.Append("    ");
    if (cmd->GetOutputArgs().Size() > 0)
    {
        str.Append(this->GetNebulaType(cmd->GetOutputArgs()[0]->GetType()));
        str.Append(" ");
        str.Append(cmd->GetOutputArgs()[0]->GetName());
        str.Append(" = ");
    }
    str.Append("this->Callback(");
    for (inArgIndex = 0; inArgIndex < cmd->GetInputArgs().Size(); inArgIndex++)
    {
        IDLArg* arg = cmd->GetInputArgs()[inArgIndex];
        str.Append(arg->GetName().AsCharPtr());
        if (inArgIndex < (cmd->GetInputArgs().Size() - 1))
        {
            str.Append(", ");
        }
    }
    str.Append(");");
    writer->WriteLine(str);

    // optionally set result
    if (cmd->GetOutputArgs().Size() > 0)
    {
        IDLArg* arg = cmd->GetOutputArgs()[0];
        writer->WriteFormatted("    this->results.ArgValue(0).%s(%s);\n",
            this->GetNebulaSetterMethod(arg->GetType()).AsCharPtr(),
            arg->GetName().AsCharPtr());
    }
    writer->WriteLine("    return true;");
    writer->WriteLine("}");
    writer->WriteLine("");

    // Callback() method
    writer->WriteLine("//------------------------------------------------------------------------------");
    writer->WriteLine("/**");
    writer->WriteLine("*/");
    writer->WriteLine(this->BuildCallbackPrototype(cmd, true));
    writer->WriteLine("{");
    writer->WriteLine(cmd->GetCode());
    writer->WriteLine("}");
    writer->WriteLine("");
}
 
} // namespace Tools