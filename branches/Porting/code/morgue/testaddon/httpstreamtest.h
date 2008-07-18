#ifndef TEST_HTTPSTREAMTEST_H
#define TEST_HTTPSTREAMTEST_H
//------------------------------------------------------------------------------
/**
    @class Test::HttpStreamTest
    
    Test HttpStream functionality.
    
    (C) 2006 Radon Labs GmbH
*/
#include "test/testcase.h"

//------------------------------------------------------------------------------
namespace Test
{
class HttpStreamTest : public TestCase
{
    DeclareClass(HttpStreamTest);
public:
    /// run the test
    virtual void Run();
};

}; // namespace Test
//------------------------------------------------------------------------------
#endif        

