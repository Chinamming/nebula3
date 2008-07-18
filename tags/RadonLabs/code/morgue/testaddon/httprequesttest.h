#ifndef TEST_HTTPREQUESTTEST_H
#define TEST_HTTPREQUESTTEST_H
//------------------------------------------------------------------------------
/**
    @class Test::HttpRequestTest
  
    Test HttpRequest functionality.
    
    (C) 2006 Radon Labs GmbH
*/    
#include "test/testcase.h"

//------------------------------------------------------------------------------
namespace Test
{
class HttpRequestTest : public TestCase
{
    DeclareClass(HttpRequestTest);
public:
    /// run the test
    virtual void Run();
};

}; // namespace Test
//------------------------------------------------------------------------------
#endif        
    