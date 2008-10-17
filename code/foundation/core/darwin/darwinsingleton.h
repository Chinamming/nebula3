#pragma once
#ifndef CORE_DARWINSINGLETON_H
#define CORE_DARWINSINGLETON_H
//------------------------------------------------------------------------------
/**
    @file core/posix/posixsingleton.h

    Provides helper macros to implement singleton objects:
    
    - __DeclareSingleton      put this into class declaration
    - __ImplementSingleton    put this into the implemention file
    - __ConstructSingleton    put this into the constructor
    - __DestructSingleton     put this into the destructor

    Get a pointer to a singleton object using the static Instance() method:

    Core::Server* coreServer = Core::Server::Instance();
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/types.h"
#include <pthread.h>

//------------------------------------------------------------------------------
#define __DeclareSingleton(type) \
public: \
    static type * Instance() {\
        pthread_once(&singletonKeyOnce, &AllocSingletonKey); \
        void* singleton = pthread_getspecific(singletonKey); \
        n_assert(0 != singleton); \
        return static_cast<type*>(singleton); \
    }; \
    static bool HasInstance() { \
        pthread_once(&singletonKeyOnce, &AllocSingletonKey); \
        void* singleton = pthread_getspecific(singletonKey); \
        return 0 != singleton; \
    }; \
private: \
    static pthread_key_t singletonKey; \
    static pthread_once_t singletonKeyOnce; \
    static void AllocSingletonKey() {\
        pthread_key_create(&singletonKey, NULL); \
    };

#define __DeclareInterfaceSingleton(type) \
public: \
    static type * Singleton; \
    static type * Instance() { n_assert(0 != Singleton); return Singleton; }; \
    static bool HasInstance() { return 0 != Singleton; }; \
private:

#define __ImplementSingleton(type) \
    pthread_key_t type::singletonKey; \
    pthread_once_t type::singletonKeyOnce = PTHREAD_ONCE_INIT;

#define __ImplementInterfaceSingleton(type) \
    type * type::Singleton = 0;

#define __ConstructSingleton \
    pthread_once(&singletonKeyOnce, &AllocSingletonKey); \
    void* singleton = pthread_getspecific(singletonKey); \
    n_assert(0 == singleton); \
    pthread_setspecific(singletonKey, this);

#define __ConstructInterfaceSingleton \
    n_assert(0 == Singleton); Singleton = this;

#define __DestructSingleton \
    pthread_once(&singletonKeyOnce, &AllocSingletonKey); \
    void* singleton = pthread_getspecific(singletonKey); \
    n_assert(singleton); \
    pthread_setspecific(singletonKey, 0);

#define __DestructInterfaceSingleton \
    n_assert(Singleton); Singleton = 0;
//------------------------------------------------------------------------------
#endif

