//------------------------------------------------------------------------------
//  threadtest.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "threadtest.h"
#include "io/console.h"

namespace Test
{
ImplementClass(Test::ThreadTest, 'THRT', Test::TestCase);

ImplementClass(Test::TestSingle, 'TSTS', Core::RefCounted);
ImplementSingleton(Test::TestSingle);

ImplementClass(Test::MyThread, 'MYTD', Threading::Thread);

bool ThreadTest::InstanceCheck = true;
Threading::Event ThreadTest::threadEvent;

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
void
MyThread::DoWork()
{
    Ptr<IO::Console> con;
    ThreadTest::InstanceCheck = TestSingle::HasInstance();

    if (!IO::Console::HasInstance())
    {
        con = IO::Console::Create();
        con->Open();
    }
	
    if (!ThreadTest::InstanceCheck)
    {
        n_printf("no test single available \n");
        Ptr<Test::TestSingle> ptr = TestSingle::Create();
        n_printf("thread single address 0x%x \n",TestSingle::Instance());
    }
    else
    {
        n_printf("already available\n");
    }

    ThreadTest::threadEvent.Signal();    
}


//------------------------------------------------------------------------------
/**
*/
TestSingle::TestSingle()
{
    ConstructSingleton;     
}

//------------------------------------------------------------------------------
/**
*/
TestSingle::~TestSingle()
{
}

//------------------------------------------------------------------------------
/**
*/
void
ThreadTest::Run()
{
    this->mythread = MyThread::Create();
    this->single = TestSingle::Create();

    this->single = TestSingle::Instance();
    n_printf("main address 0x%x \n",TestSingle::Instance());
    this->mythread->Start();
    
    threadEvent.Wait();
    this->Verify(!InstanceCheck);    
}

} // namespace Test