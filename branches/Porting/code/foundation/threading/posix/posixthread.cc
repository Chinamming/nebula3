//------------------------------------------------------------------------------
//  posixthread.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "threading/posix/posixthread.h"

namespace Posix
{
ImplementClass(Posix::PosixThread, 'THRD', Core::RefCounted);

__thread const char* PosixThread::ThreadName = 0; 

//------------------------------------------------------------------------------
/**
*/
PosixThread::PosixThread() :
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
PosixThread::~PosixThread()
{
    if (this->IsRunning())
    {
        this->Stop();
    }
}

//------------------------------------------------------------------------------
/**
    Start the thread, this creates a Posix thread and calls the static
    ThreadProc, which in turn calls the virtual DoWork() class of this object.
    The method returns immediately without waiting for the thread to start.
*/
void
PosixThread::Start()
{
    n_assert(!this->IsRunning());
    n_assert(0 == this->threadHandle);
    pthread_attr_t threadAttributes;
    pthread_attr_init(&threadAttributes);
    pthread_attr_setstacksize(&threadAttributes, PTHREAD_STACK_MIN + this->stackSize);
    // XXX: Consider dealing with the thread priority here.
    int r = pthread_create(&(this->threadHandle), &threadAttributes, ThreadProc, (void *) this);
    n_assert(0 == r);
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
PosixThread::EmitWakeupSignal()
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
PosixThread::Stop()
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
    pthread_join(this->threadHandle, NULL);
    this->threadHandle = 0;
}

//------------------------------------------------------------------------------
/**
    This method should be derived in a Thread subclass and contains the
    actual code which is run in the thread. The method must not call
    C-Lib functions under Posix. To terminate the thread, just return from
    this function. If DoWork() runs in an infinite loop, call ThreadStopRequested()
    to check whether the Thread object wants the thread code to quit.
*/
void
PosixThread::DoWork()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Internal static helper method. This is called by CreateThread() and
    simply calls the virtual DoWork() method on the thread object.
*/
void*
PosixThread::ThreadProc(void* self)
{
    n_assert(0 != self);
    PosixThread* threadObj = (PosixThread*) self;
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
PosixThread::GetMyThreadName()
{
    return ThreadName;
}

};