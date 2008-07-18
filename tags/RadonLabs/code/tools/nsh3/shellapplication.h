#pragma once
#ifndef TOOLS_SHELLAPPLICATION_H
#define TOOLS_SHELLAPPLICATION_H
//------------------------------------------------------------------------------
/**
    @class Tools::ShellApplication
    
    Application class for Nebula3's command shell. Takes commands from
    stdin and evaluates them through the script server.
    
    (C) 2006 Radon Labs GmbH
*/
#include "app/consoleapplication.h"

//------------------------------------------------------------------------------
namespace Tools
{
class ShellApplication : public App::ConsoleApplication
{
public:
    /// constructor
    ShellApplication();
    /// destructor
    virtual ~ShellApplication();
    /// run the application, return when user wants to exit
    virtual void Run();
};

} // namespace App
//------------------------------------------------------------------------------
#endif    