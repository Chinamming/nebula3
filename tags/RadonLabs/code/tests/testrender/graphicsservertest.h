#pragma once
#ifndef TEST_GRAPHICSSERVERTEST_H
#define TEST_GRAPHICSSERVERTEST_H
//------------------------------------------------------------------------------
/**
    @class Test::GraphicsServerTest
    
    Test graphics server functionality.
    
    (C) 2007 Radon Labs GmbH
*/
#include "coregraphicstest.h"

//------------------------------------------------------------------------------
namespace Test
{
class GraphicsServerTest : public CoreGraphicsTest
{
    DeclareClass(GraphicsServerTest);
public:
    /// run the test
    virtual void Run();
};    

} // namespace Test
//------------------------------------------------------------------------------
#endif
    