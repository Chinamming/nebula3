#ifndef TEST_MATHSSETEST_H
#define TEST_MATHSSETEST_H
//------------------------------------------------------------------------------
/**
    @class Test::MathSSETest
    
    Test the SSE lowlevel math lib library.
    
    (C) 2006 Radon Labs GmbH
*/
#include "test/testcase.h"

//------------------------------------------------------------------------------
namespace Test
{
class MathSSETest : public TestCase
{
    DeclareClass(MathSSETest);
public:
    /// run the test
    virtual void Run();
};

} // namespace Test
//------------------------------------------------------------------------------
#endif
