#pragma once
#ifndef THREADING_SAFEQUEUE_H
#define THREADING_SAFEQUEUE_H
//------------------------------------------------------------------------------
/**
    @class Threading::SafeQueue
    
    Thread-safe version of Util::Queue.
    
    (C) 2007 Radon Labs GmbH
*/
#include "util/queue.h"
#include "threading/criticalsection.h"
#include "threading/event.h"

//------------------------------------------------------------------------------
namespace Threading
{
template<class TYPE> class SafeQueue : protected Util::Queue<TYPE>
{
public:
    /// constructor
    SafeQueue();
    /// copy constructor
    SafeQueue(const SafeQueue<TYPE>& rhs);

    /// assignment operator
    void operator=(const SafeQueue<TYPE>& rhs);
    /// returns number of elements in the queue
    SizeT Size() const;
    /// return true if queue is empty
    bool IsEmpty() const;
    /// remove all elements from the queue
    void Clear();
    /// add element to the back of the queue
    void Enqueue(const TYPE& e);
    /// remove the element from the front of the queue
    TYPE Dequeue();
    /// dequeue all events (only requires one lock)
    Util::Array<TYPE> DequeueAll();
    /// access to element at front of queue without removing it
    TYPE Peek() const;
    /// wait until queue contains at least one element
    void Wait();
    /// signal the internal event, so that Wait() will return
    void Signal();
    /// erase all matching elements
    void EraseMatchingElements(const TYPE& e);

protected:
    CriticalSection criticalSection;
    Event enqueueEvent;
};

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
SafeQueue<TYPE>::SafeQueue()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
SafeQueue<TYPE>::SafeQueue(const SafeQueue<TYPE>& rhs)
{
    this->criticalSection.Enter();
    this->queueArray = rhs.queueArray;
    this->criticalSection.Leave();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
SafeQueue<TYPE>::operator=(const SafeQueue<TYPE>& rhs)
{
    this->criticalSection.Enter();
    this->queueArray = rhs.queueArray;
    this->criticalSection.Leave();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
SafeQueue<TYPE>::Clear()
{
    this->criticalSection.Enter();
    this->queueArray.Clear();
    this->criticalSection.Leave();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> SizeT
SafeQueue<TYPE>::Size() const
{
    return this->queueArray.Size();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
SafeQueue<TYPE>::IsEmpty() const
{
    return this->queueArray.IsEmpty();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
SafeQueue<TYPE>::Enqueue(const TYPE& e)
{
    this->criticalSection.Enter();
    this->queueArray.Append(e);
    this->criticalSection.Leave();
    this->enqueueEvent.Signal();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> TYPE
SafeQueue<TYPE>::Dequeue()
{
    this->criticalSection.Enter();
    TYPE e = this->queueArray.Front();
    this->queueArray.EraseIndex(0);
    this->criticalSection.Leave();
    return e;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> Util::Array<TYPE>
SafeQueue<TYPE>::DequeueAll()
{
    this->criticalSection.Enter();
    Util::Array<TYPE> res = this->queueArray;
    this->queueArray.Clear();
    this->criticalSection.Leave();
    return res;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> TYPE
SafeQueue<TYPE>::Peek() const
{
    this->criticalSection.Enter();
    TYPE e = this->queueArray.Front();
    this->criticalSection.Leave();
    return e;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
SafeQueue<TYPE>::Wait()
{
    if (this->queueArray.Size() == 0)
    {
        this->enqueueEvent.Wait();
    }
}

//------------------------------------------------------------------------------
/**
    This signals the internal event object, on which Wait() may be waiting.
    This method may be useful to wake up a thread waiting for events
    when it should stop.
*/
template<class TYPE> void
SafeQueue<TYPE>::Signal()
{
    this->enqueueEvent.Signal();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
SafeQueue<TYPE>::EraseMatchingElements(const TYPE& e)
{
    this->criticalSection.Enter();
    Util::Array<TYPE>::Iterator iter; 
    for (iter = this->queueArray.Begin(); iter != this->queueArray.End();)
    {
        if ((*iter) == e)
        {
            iter = this->queueArray.Erase(iter);
        }
        else
        {
            iter++;
        }
    }
    this->criticalSection.Leave();
}

} // namespace Threading
//------------------------------------------------------------------------------
#endif

    