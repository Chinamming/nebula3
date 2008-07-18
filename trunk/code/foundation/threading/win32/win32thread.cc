//------------------------------------------------------------------------------
//  win32thread.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "threading/win32/win32thread.h"

namespace Win32
{
ImplementClass(Win32::Win32Thread, 'THRD', Core::RefCounted);

__declspec(thread) const char* Win32Thread::ThreadName = 0; 

//------------------------------------------------------------------------------
/**
*/
Win32Thread::Win32Thread() :
    threadHandle(0),
    running(false),
    priority(Normal),
    stackSize(4096),
    coreId(System::Cpu::UndefinedCoreId)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Win32Thread::~Win32Thread()
{
    if (this->IsRunning())
    {
        this->Stop();
    }
}

//------------------------------------------------------------------------------
/**
    Start the thread, this creates a Win32 thread and calls the static
    ThreadProc, which in turn calls the virtual DoWork() class of this object.
    The method returns immediately without waiting for the thread to start.
*/
void
Win32Thread::Start()
{
    n_assert(!this->IsRunning());
    n_assert(0 == this->threadHandle);
    this->threadHandle = CreateThread(NULL,             // lpThreadAttributes
                                      this->stackSize,  // dwStackSize
                                      ThreadProc,       // lpStartAddress
                                      (LPVOID) this,    // lpParameter
                                      0,                // dwCreationFlags
                                      NULL);            // lpThreadId
    n_assert(NULL != this->threadHandle);

    // apply thread priority
    switch (this->priority)
    {
        case Low:   
            SetThreadPriority(this->threadHandle, THREAD_PRIORITY_BELOW_NORMAL);
            break;

        case Normal:
            SetThreadPriority(this->threadHandle, THREAD_PRIORITY_NORMAL);
            break;

        case High:
            SetThreadPriority(this->threadHandle, THREAD_PRIORITY_ABOVE_NORMAL);
            break;
    }
}

//------------------------------------------------------------------------------
/**
    This method is called by Thread::Stop() after setting the 
    stopRequest event and before waiting for the thread to stop. If your
    thread runs a loop and waits for jobs it may need an extra wakeup
    signal to stop waiting and check for the ThreadStopRequested() event. In
    this case, override this method and signal your event object.
*/
void
Win32Thread::EmitWakeupSignal()
{
    // empty, override in subclass!
}

//------------------------------------------------------------------------------
/**
    This stops the thread by signalling the stopRequestEvent and waits for the
    thread to actually quit. If the thread code runs in a loop it should use the 
    IsStopRequested() method to see if the thread object wants it to shutdown. 
    If so DoWork() should simply return.
*/
void
Win32Thread::Stop()
{
    n_assert(this->IsRunning());
    n_assert(0 != this->threadHandle);

    // signal the thread to stop
    this->stopRequestEvent.Signal();

    // call the wakeup-thread method, may be derived in a subclass
    // if the threads needs to be woken up, it is important that this
    // method is called AFTER the stopRequestEvent is signalled!
    this->EmitWakeupSignal();

    // wait for the thread to terminate
    WaitForSingleObject(this->threadHandle, INFINITE);
    CloseHandle(this->threadHandle);
    this->threadHandle = 0;
}

//------------------------------------------------------------------------------
/**
    This method should be derived in a Thread subclass and contains the
    actual code which is run in the thread. The method must not call
    C-Lib functions under Win32. To terminate the thread, just return from
    this function. If DoWork() runs in an infinite loop, call ThreadStopRequested()
    to check whether the Thread object wants the thread code to quit.
*/
void
Win32Thread::DoWork()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Internal static helper method. This is called by CreateThread() and
    simply calls the virtual DoWork() method on the thread object.
*/
DWORD WINAPI
Win32Thread::ThreadProc(LPVOID self)
{
    n_assert(0 != self);
    Win32Thread* threadObj = (Win32Thread*) self;
    ThreadName = threadObj->GetName().AsCharPtr();
    threadObj->DoWork();
    return 0;
}

//------------------------------------------------------------------------------
/**
    Static method to obtain the current thread name from anywhere
    in the thread's code.
*/
const char*
Win32Thread::GetMyThreadName()
{
    return ThreadName;
}

};