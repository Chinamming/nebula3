#ifndef TEST_HTTPCONNECTIONTEST_H
#define TEST_HTTPCONNECTIONTEST_H
//------------------------------------------------------------------------------
/**
    @class Test::HttpConnectionTest
    
    Test HttpConnection class functionality.
    
    (C) 2006 Radon Labs GmbH
*/
#include "test/testcase.h"

//------------------------------------------------------------------------------
namespace Test
{
class HttpConnectionTest : public TestCase
{
    DeclareClass(HttpConnectionTest);
public:
    /// run the test
    virtual void Run();
};

}; // namespace Test
//------------------------------------------------------------------------------
#endif        
    