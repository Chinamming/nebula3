//------------------------------------------------------------------------------
//  console.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "io/console.h"
#if __WIN32__
#include "io/win32/win32consolehandler.h"
#elif __XBOX360__
#include "io/xbox360/xbox360consolehandler.h"
#elif __WII__
#include "io/wii/wiiconsolehandler.h"
#endif

namespace IO
{
ImplementClass(IO::Console, 'CNSL', Core::RefCounted);
ImplementSingleton(IO::Console);

using namespace Core;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
Console::Console() :
    isOpen(false)
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
Console::~Console()
{
    if (this->IsOpen())
    {
        this->Close();
    }
    DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
Console::Open()
{
    n_assert(!this->IsOpen());

    // create default console handlers
    #if __WIN32__
    Ptr<ConsoleHandler> consoleHandler = Win32::Win32ConsoleHandler::Create();
    this->AttachHandler(consoleHandler);    
    #elif __XBOX360__
    Ptr<ConsoleHandler> consoleHandler = Xbox360::Xbox360ConsoleHandler::Create();
    this->AttachHandler(consoleHandler);
    #elif __WII__
    Ptr<ConsoleHandler> consoleHandler = Wii::WiiConsoleHandler::Create();
    this->AttachHandler(consoleHandler);
    #endif

    this->isOpen = true;
}

//------------------------------------------------------------------------------
/**
*/
void
Console::Close()
{
    n_assert(this->IsOpen());
    
    // cleanup console handlers
    while (this->consoleHandlers.Size() > 0)
    {
        this->RemoveHandler(this->consoleHandlers[0]);
    }
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
void
Console::Update()
{
    IndexT i;
    for (i = 0; i < this->consoleHandlers.Size(); i++)
    {
        this->consoleHandlers[i]->Update();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
Console::AttachHandler(const Ptr<ConsoleHandler>& h)
{
    n_assert(h.isvalid());
    h->Open();
    this->consoleHandlers.Append(h);
}

//------------------------------------------------------------------------------
/**
*/
void
Console::RemoveHandler(const Ptr<ConsoleHandler>& h)
{
    n_assert(h.isvalid());
    IndexT index = this->consoleHandlers.FindIndex(h);
    n_assert(InvalidIndex != index);
    this->consoleHandlers[index]->Close();
    this->consoleHandlers.EraseIndex(index);
}

//------------------------------------------------------------------------------
/**
*/
bool
Console::HasInput() const
{
    n_assert(this->IsOpen());
    IndexT i;
    for (i = 0; i < this->consoleHandlers.Size(); i++)
    {
        if (this->consoleHandlers[i]->HasInput())
        {
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
String
Console::GetInput() const
{
    n_assert(this->IsOpen());
    IndexT i;
    for (i = 0; i < this->consoleHandlers.Size(); i++)
    {
        if (this->consoleHandlers[i]->HasInput())
        {
            return this->consoleHandlers[i]->GetInput();
        }
    }
    return "";
}

//------------------------------------------------------------------------------
/**
*/
void
Console::Print(const char* fmt, ...)
{
    n_assert(this->IsOpen());
    va_list argList;
    va_start(argList, fmt);
    this->Print(fmt, argList);
    va_end(argList);
}

//------------------------------------------------------------------------------
/**
*/
void
Console::Print(const char* fmt, va_list argList)
{
    n_assert(this->IsOpen());
    String str;
    str.FormatArgList(fmt, argList);
    IndexT i;
    for (i = 0; i < this->consoleHandlers.Size(); i++)
    {
        this->consoleHandlers[i]->Print(str);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
Console::Print(const String& str)
{
    n_assert(this->IsOpen());
    IndexT i;
    for (i = 0; i < this->consoleHandlers.Size(); i++)
    {
        this->consoleHandlers[i]->Print(str);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
Console::Error(const char* fmt, ...)
{
    n_assert(this->IsOpen());
    va_list argList;
    va_start(argList, fmt);
    this->Error(fmt, argList);
    va_end(argList);
}

//------------------------------------------------------------------------------
/**
*/
void
Console::Error(const char* fmt, va_list argList)
{
    n_assert(this->IsOpen());
    String str;
    str.FormatArgList(fmt, argList);
    IndexT i;
    for (i = 0; i < this->consoleHandlers.Size(); i++)
    {
        this->consoleHandlers[i]->Error(str);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
Console::DebugOut(const char* fmt, ...)
{
    n_assert(this->IsOpen());
    va_list argList;
    va_start(argList, fmt);
    this->DebugOut(fmt, argList);
    va_end(argList);
}

//------------------------------------------------------------------------------
/**
*/
void
Console::DebugOut(const char* fmt, va_list argList)
{
    n_assert(this->IsOpen());
    String str;
    str.FormatArgList(fmt, argList);
    IndexT i;
    for (i = 0; i < this->consoleHandlers.Size(); i++)
    {
        this->consoleHandlers[i]->DebugOut(str);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
Console::Warning(const char* fmt, ...)
{
    n_assert(this->IsOpen());
    va_list argList;
    va_start(argList, fmt);
    this->Warning(fmt, argList);
    va_end(argList);
}

//------------------------------------------------------------------------------
/**
*/
void
Console::Warning(const char* fmt, va_list argList)
{
    n_assert(this->IsOpen());
    String str;
    str.FormatArgList(fmt, argList);
    IndexT i;
    for (i = 0; i < this->consoleHandlers.Size(); i++)
    {
        this->consoleHandlers[i]->Warning(str);
    }
}

}
