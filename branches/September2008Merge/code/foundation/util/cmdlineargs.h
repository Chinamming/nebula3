#pragma once
//------------------------------------------------------------------------------
/**
    @class Util::CmdLineArgs
    
    A universal cmd line argument parser. The command line string
    must have the form 

    cmd arg0[=]value0 arg1[=]value1 arg2[=]value2
    
    (C) 2006 Radon Labs GmbH
*/
#include "util/array.h"
#include "util/string.h"

//------------------------------------------------------------------------------
namespace Util
{
class CmdLineArgs
{
public:
    /// default constructor
    CmdLineArgs();
    /// construct from Win32-style command string
    CmdLineArgs(const String& cmdLine);
    /// construct from posix style command string
    CmdLineArgs(int argc, const char** argv);
    /// get the command name
    const String& GetCmdName() const;
    /// return true if arg exists
    bool HasArg(const String& arg) const;
    /// get string value
    const String& GetString(const String& name, const String& defaultValue = "") const;
    /// get int value
    int GetInt(const String& name, int defaultValue = 0) const;
    /// get float value
    float GetFloat(const String& name, float defaultValue = 0.0f) const;
    /// get bool value
    bool GetBool(const String& name) const;
    /// get float4 value 
    Math::float4 GetFloat4(const String& name, const Math::float4& defaultValue = Math::float4()) const;
    /// get matrix44 value
    Math::matrix44 GetMatrix44(const String& name, const Math::matrix44& defaultValue = Math::matrix44()) const;

    /// get number of arguments (exluding command name)
    SizeT GetNumArgs() const;
    /// get string argument at index
    const String& GetStringAtIndex(IndexT index) const;
    /// get int value at index
    int GetIntAtIndex(IndexT index) const;
    /// get float value at index
    float GetFloatAtIndex(IndexT index) const;
    /// get bool value at index
    bool GetBoolAtIndex(IndexT index) const;
    /// get float4 value at index
    Math::float4 GetFloat4AtIndex(IndexT index) const;
    /// get matrix44 value at index
    Math::matrix44 GetMatrix44AtIndex(IndexT index) const;

private:
    /// parse the command line string
    void Parse();
    /// put out an error
    void Error(const String& argName) const;
    /// find index of argument, fails hard with error msg if not found
    int FindArgIndex(const String& argName) const;

    Array<String> args;
};

} // namespace Util
//------------------------------------------------------------------------------
