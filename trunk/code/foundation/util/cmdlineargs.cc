//------------------------------------------------------------------------------
//  util/cmdlineargs.cc
//  (C) 2005 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "util/cmdlineargs.h"

namespace Util
{
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
CmdLineArgs::CmdLineArgs()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
CmdLineArgs::CmdLineArgs(const String& l)
{
    this->args = l.Tokenize(" \t\n=", '"');
}

//------------------------------------------------------------------------------
/**
*/
CmdLineArgs::CmdLineArgs(int argc, const char** argv)
{
    int i;
    for (i = 0; i < argc; i++)
    {
        this->args.Append(argv[i]);
    }
}

//------------------------------------------------------------------------------
/**
    Put out a missing argument error.
*/
void
CmdLineArgs::Error(const String& argName) const
{
    n_error("CmdLine: arg '%s' not found!", argName.AsCharPtr());
}

//------------------------------------------------------------------------------
/**
    Returns the command name.
*/
const String&
CmdLineArgs::GetCmdName() const
{
    n_assert(this->args.Size() > 0);
    return this->args[0];
}

//------------------------------------------------------------------------------
/**
    Returns true if argument exists.
*/
bool
CmdLineArgs::HasArg(const String& name) const
{
    IndexT index = this->args.FindIndex(name);
    return (index != InvalidIndex);
}

//------------------------------------------------------------------------------
/**
    Returns index of argument. Throws an error if argument not found.
*/
int
CmdLineArgs::FindArgIndex(const String& name) const
{
    n_assert(name.IsValid());
    IndexT i = this->args.FindIndex(name);
    if (InvalidIndex == i)
    {
        this->Error(name);
    }
    return i;
}

//------------------------------------------------------------------------------
/**
*/
const String&
CmdLineArgs::GetString(const String& name, const String& defaultValue) const
{
    if (this->HasArg(name))
    {
        return this->args[this->FindArgIndex(name) + 1];
    }
    else
    {
        return defaultValue;
    }
}

//------------------------------------------------------------------------------
/**
*/
int
CmdLineArgs::GetInt(const String& name, int defaultValue) const
{
    if (this->HasArg(name))
    {
        return this->args[this->FindArgIndex(name) + 1].AsInt();
    }
    else
    {
        return defaultValue;
    }
}

//------------------------------------------------------------------------------
/**
*/
float
CmdLineArgs::GetFloat(const String& name, float defaultValue) const
{
    if (this->HasArg(name))
    {
        return this->args[this->FindArgIndex(name) + 1].AsFloat();
    }
    else
    {
        return defaultValue;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
CmdLineArgs::GetBool(const String& name) const
{
    return this->HasArg(name);
}

//------------------------------------------------------------------------------
/**
*/
float4
CmdLineArgs::GetFloat4(const String& name, const float4& defaultValue) const
{
    if (this->HasArg(name))
    {
        return this->args[this->FindArgIndex(name) + 1].AsFloat4();
    }
    else
    {
        return defaultValue;
    }
}

//------------------------------------------------------------------------------
/**
*/
matrix44
CmdLineArgs::GetMatrix44(const String& name, const matrix44& defaultValue) const
{
    if (this->HasArg(name))
    {
        return this->args[this->FindArgIndex(name) + 1].AsMatrix44();
    }
    else
    {
        return defaultValue;
    }
}

//------------------------------------------------------------------------------
/**
*/
SizeT
CmdLineArgs::GetNumArgs() const
{
    n_assert(this->args.Size() > 0);
    return this->args.Size() - 1;
}

//------------------------------------------------------------------------------
/**
*/
const String&
CmdLineArgs::GetStringAtIndex(IndexT index) const
{
    return this->args[index + 1];
}

//------------------------------------------------------------------------------
/**
*/
int
CmdLineArgs::GetIntAtIndex(IndexT index) const
{
    return this->args[index + 1].AsInt();
}

//------------------------------------------------------------------------------
/**
*/
float
CmdLineArgs::GetFloatAtIndex(IndexT index) const
{
    return this->args[index + 1].AsFloat();
}

//------------------------------------------------------------------------------
/**
*/
bool
CmdLineArgs::GetBoolAtIndex(IndexT index) const
{
    return this->args[index + 1].AsBool();
}

//------------------------------------------------------------------------------
/**
*/
float4
CmdLineArgs::GetFloat4AtIndex(IndexT index) const
{
    return this->args[index + 1].AsFloat4();
}

//------------------------------------------------------------------------------
/**
*/
matrix44
CmdLineArgs::GetMatrix44AtIndex(IndexT index) const
{
    return this->args[index + 1].AsMatrix44();
}

} // namespace Util
