#pragma once
#ifndef WIN32_WIN32THREAD_H
#define WIN32_WIN32THREAD_H
//------------------------------------------------------------------------------
/**
    @class Win32::Win32Thread
    
    Win32 implementation of thread class.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "threading/win32/win32event.h"
#include "system/cpu.h"

//------------------------------------------------------------------------------
namespace Win32
{
class Win32Thread : public Core::RefCounted
{
    DeclareClass(Win32Thread);
public:
    /// thread priorities
    enum Priority
    {
        Low,
        Normal,
        High,
    };
    /// constructor
    Win32Thread();
    /// destructor
    virtual ~Win32Thread();
    /// set the thread priority
    void SetPriority(Priority p);
    /// get the thread priority
    Priority GetPriority() const;
    /// set cpu core on which the thread should be running
    void SetCoreId(System::Cpu::CoreId coreId);
    /// get the cpu core on which the thread should be running
    System::Cpu::CoreId GetCoreId() const;
    /// set stack size in bytes (default is 4 KByte)
    void SetStackSize(unsigned int s);
    /// get stack size
    unsigned int GetStackSize() const;
    /// set thread name
    void SetName(const Util::String& n);
    /// get thread name
    const Util::String& GetName() const;
    /// start executing the thread code, returns when thread has actually started
    void Start();
    /// request threading code to stop, returns when thread has actually finished
    void Stop();
    /// return true if thread has been started
    bool IsRunning() const;
    /// obtain name of thread from within thread code
    static const char* GetMyThreadName();

protected:
    /// override this method if your thread loop needs a wakeup call before stopping
    virtual void EmitWakeupSignal();
    /// this method runs in the thread context
    virtual void DoWork();
    /// check if stop is requested, call from DoWork() to see if the thread proc should quit
    bool ThreadStopRequested() const;

private:
    /// internal thread proc helper function
    static DWORD WINAPI ThreadProc(LPVOID self);

    HANDLE threadHandle;
    Win32Event stopRequestEvent;
    bool running;
    Priority priority;
    unsigned int stackSize;
    Util::String name;
    System::Cpu::CoreId coreId;
    ThreadLocal static const char* ThreadName;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
Win32Thread::IsRunning() const
{
    return (0 != this->threadHandle);
}

//------------------------------------------------------------------------------
/**
*/
inline void
Win32Thread::SetPriority(Priority p)
{
    this->priority = p;
}

//------------------------------------------------------------------------------
/**
*/
inline Win32Thread::Priority
Win32Thread::GetPriority() const
{
    return this->priority;
}

//------------------------------------------------------------------------------
/**
    If the derived DoWork() method is running in a loop it must regularly
    check if the process wants the thread to terminate by calling
    ThreadStopRequested() and simply return if the result is true. This
    will cause the thread to shut down.
*/
inline bool
Win32Thread::ThreadStopRequested() const
{
    return this->stopRequestEvent.Peek();
}

//------------------------------------------------------------------------------
/**
    Set the thread's name. To obtain the current thread's name from anywhere
    in the thread's execution context, call the static method
    Thread::GetMyThreadName().
*/
inline void
Win32Thread::SetName(const Util::String& n)
{
    n_assert(n.IsValid());
    this->name = n;
}

//------------------------------------------------------------------------------
/**
    Get the thread's name. This is the vanilla method which
    returns the name member. To obtain the current thread's name from anywhere
    in the thread's execution context, call the static method
    Thread::GetMyThreadName().
*/
inline const Util::String&
Win32Thread::GetName() const
{
    return this->name;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Win32Thread::SetCoreId(System::Cpu::CoreId id)
{
    this->coreId = id;
}

//------------------------------------------------------------------------------
/**
*/
inline System::Cpu::CoreId
Win32Thread::GetCoreId() const
{
    return this->coreId;
}

}; // namespace Win32
//------------------------------------------------------------------------------
#endif
