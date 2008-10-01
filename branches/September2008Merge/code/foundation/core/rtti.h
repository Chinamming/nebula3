#pragma once
#ifndef CORE_RTTI_H
#define CORE_RTTI_H
//------------------------------------------------------------------------------
/**
    @class Core::Rtti

    Nebula3's runtime type information for one class. Every class derived
    from Core::RefCounted should use the macros DeclareClass and ImplementClass
    to properly initialize the runtime type information for the class. This
    will also automatically register the class with the Core::Factory object
    to implement object construction from class name string or fourcc code.

    (C) 2006 RadonLabs GmbH
*/
#include "core/types.h"
#include "core/factory.h"
#include "core/sysfunc.h"
#include "util/string.h"
#include "util/fourcc.h"
#include "memory/memorypool.h"

//------------------------------------------------------------------------------
namespace Core
{
class RefCounted;
class Rtti
{
public:
    /// define a creation callback function prototype
    typedef RefCounted* (*Creator)();

    /// constructor
    Rtti(const char* className, Util::FourCC fcc, Creator creatorFunc, const Core::Rtti* parentClass, SizeT instSize);
    /// equality operator
    bool operator==(const Rtti& rhs) const;
    /// inequality operator
    bool operator!=(const Rtti& rhs) const;
    /// get class name
    const Util::String& GetName() const;
    /// get four character code of class
    Util::FourCC GetFourCC() const;
    /// get pointer to parent class
    const Core::Rtti* GetParent() const;
    /// get instance size in bytes
    SizeT GetInstanceSize() const;
    /// create an object of this class
    RefCounted* Create() const;
    /// return true if this rtti is equal or derived from to other rtti
    bool IsDerivedFrom(const Rtti& other) const;
    /// return true if this rtti is equal or derived from to other rtti, by string
    bool IsDerivedFrom(const Util::String& otherClassName) const;
    /// return true if this rtti is equal or derived from to other rtti, by fourcc
    bool IsDerivedFrom(const Util::FourCC& otherClassFourCC) const;
    /// allocate instance memory block (called by class new operator)
    void* AllocInstanceMemory();
    /// free instance memory block (called by class delete operator)
    void FreeInstanceMemory(void* ptr);

private:
    Util::String name;
    const Core::Rtti* parent;
    const Util::FourCC fourCC;
    const Creator creator;
    SizeT instanceSize;
    #if NEBULA3_REFCOUNTED_MEMORYPOOLS
    Memory::MemoryPool memoryPool;
    #endif
};

//------------------------------------------------------------------------------
/**
*/
inline bool
Rtti::operator==(const Rtti& rhs) const
{
    return this == &rhs;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
Rtti::operator!=(const Rtti& rhs) const
{
    return this != &rhs;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String&
Rtti::GetName() const
{
    return this->name;
}

//------------------------------------------------------------------------------
/**
*/
inline Util::FourCC
Rtti::GetFourCC() const
{
    return this->fourCC;
}

//------------------------------------------------------------------------------
/**
*/
inline const Core::Rtti*
Rtti::GetParent() const
{
    return this->parent;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
Rtti::GetInstanceSize() const
{
    return this->instanceSize;
}

}  // namespace Core

//------------------------------------------------------------------------------
/**
    Declaration macro. Put this into the class declaration.
*/
#define DeclareClass(type) \
public: \
    void* operator new(size_t size) \
    { \
        n_assert(size == RTTI.GetInstanceSize()); \
        return RTTI.AllocInstanceMemory(); \
    }; \
    void operator delete(void* p) \
    { \
        RTTI.FreeInstanceMemory(p); \
    }; \
    static Core::Rtti RTTI; \
    static Core::RefCounted* FactoryCreator(); \
    static type* Create(); \
    static bool RegisterWithFactory(); \
    virtual Core::Rtti* GetRtti() const; \
private:

#define DeclareAbstractClass(class_name) \
public: \
    static Core::Rtti RTTI; \
    virtual Core::Rtti* GetRtti() const; \
private:

//------------------------------------------------------------------------------
/**
    Register a class with the factory. This is only necessary for classes
    which can create objects by name or fourcc.
*/
#ifdef RegisterClass
#undef RegisterClass
#endif
#define RegisterClass(type) \
    static const bool type##_registered = type::RegisterWithFactory(); \

//------------------------------------------------------------------------------
/**
    Implementation macro. Put this into the source file.
*/
#if NEBULA3_DEBUG
#define ImplementClass(type, fourcc, baseType) \
    Core::Rtti type::RTTI(#type, fourcc, type::FactoryCreator, &baseType::RTTI, sizeof(type)); \
    Core::Rtti* type::GetRtti() const { return &this->RTTI; } \
    Core::RefCounted* type::FactoryCreator() { return type::Create(); } \
    type* type::Create() \
    { \
        RefCounted::criticalSection.Enter(); \
        RefCounted::isInCreate = true; \
        type* newObject = n_new(type); \
        RefCounted::isInCreate = false; \
        RefCounted::criticalSection.Leave(); \
        return newObject; \
    }\
    bool type::RegisterWithFactory() \
    { \
        Core::SysFunc::Setup(); \
        if (!Core::Factory::Instance()->ClassExists(#type)) \
        { \
            Core::Factory::Instance()->Register(&type::RTTI, #type, fourcc); \
        } \
        return true; \
    }
#else
#define ImplementClass(type, fourcc, baseType) \
    Core::Rtti type::RTTI(#type, fourcc, type::FactoryCreator, &baseType::RTTI, sizeof(type)); \
    Core::Rtti* type::GetRtti() const { return &this->RTTI; } \
    Core::RefCounted* type::FactoryCreator() { return type::Create(); } \
    type* type::Create() \
    { \
        return n_new(type); \
    }\
    bool type::RegisterWithFactory() \
    { \
        Core::SysFunc::Setup(); \
        if (!Core::Factory::Instance()->ClassExists(#type)) \
        { \
            Core::Factory::Instance()->Register(&type::RTTI, #type, fourcc); \
        } \
        return true; \
    }
#endif

#define ImplementAbstractClass(type, fourcc, baseType) \
    Core::Rtti type::RTTI(#type, fourcc, 0, &baseType::RTTI, 0); \
    Core::Rtti* type::GetRtti() const { return &this->RTTI; }

//------------------------------------------------------------------------------
/**
    Type implementation of topmost type in inheritance hierarchy (source file).
*/
#if NEBULA3_DEBUG
#define ImplementRootClass(type, fourcc) \
    Core::Rtti type::RTTI(#type, fourcc, type::FactoryCreator, 0, sizeof(type)); \
    Core::Rtti* type::GetRtti() const { return &this->RTTI; } \
    Core::RefCounted* type::FactoryCreator() { return type::Create(); } \
    type* type::Create() \
    { \
        RefCounted::criticalSection.Enter(); \
        RefCounted::isInCreate = true; \
        type* newObject = n_new(type); \
        RefCounted::isInCreate = false; \
        RefCounted::criticalSection.Leave(); \
        return newObject; \
    }\
    bool type::RegisterWithFactory() \
    { \
        if (!Core::Factory::Instance()->ClassExists(#type)) \
        { \
            Core::Factory::Instance()->Register(&type::RTTI, #type, fourcc); \
        } \
        return true; \
    }
#else
#define ImplementRootClass(type, fourcc) \
    Core::Rtti type::RTTI(#type, fourcc, type::FactoryCreator, 0, sizeof(type)); \
    Core::Rtti* type::GetRtti() const { return &this->RTTI; } \
    Core::RefCounted* type::FactoryCreator() { return type::Create(); } \
    type* type::Create() \
    { \
        return n_new(type); \
    }\
    bool type::RegisterWithFactory() \
    { \
        if (!Core::Factory::Instance()->ClassExists(#type)) \
        { \
            Core::Factory::Instance()->Register(&type::RTTI, #type, fourcc); \
        } \
        return true; \
    }
#endif
//------------------------------------------------------------------------------
#endif