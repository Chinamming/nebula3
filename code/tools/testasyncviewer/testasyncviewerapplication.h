#pragma once
#ifndef TOOLS_TESTASYNCVIEWERAPPLICATION_H
#define TOOLS_TESTASYNCVIEWERAPPLICATION_H
//------------------------------------------------------------------------------
/**
    @class Tools::TestAsyncViewerApplication
    
    Application class for testing asynchronous rendering.
    
    (C) 2008 Radon Labs GmbH
*/
#include "apprender/asyncviewerapplication.h"

//------------------------------------------------------------------------------
namespace Tools
{
class TestAsyncViewerApplication : public App::AsyncViewerApplication
{
public:
    /// constructor
    TestAsyncViewerApplication();
    /// destructor
    virtual ~TestAsyncViewerApplication();
    /// open the application
    virtual bool Open();
    /// update world 
    virtual void OnUpdateFrame();
    /// process input
    virtual void OnProcessInput();
};

} // namespace Tools
//------------------------------------------------------------------------------
#endif
