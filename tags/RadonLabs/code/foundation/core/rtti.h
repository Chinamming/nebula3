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
#include "memory/heap.h"

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
    Rtti(const char* className, Util::FourCC fcc, Creator creatorFunc, const Core::Rtti* parentClass);
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
    /// create an object of this class
    RefCounted* Create() const;
    /// return true if this rtti is equal or derived from to other rtti
    bool IsDerivedFrom(const Rtti& other) const;
    /// return true if this rtti is equal or derived from to other rtti, by string
    bool IsDerivedFrom(const Util::String& otherClassName) const;
    /// return true if this rtti is equal or derived from to other rtti, by fourcc
    bool IsDerivedFrom(const Util::FourCC& otherClassFourCC) const;

private:
    Util::String name;
    const Core::Rtti* parent;
    const Util::FourCC fourCC;
    const Creator creator;
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

}  // namespace Core

//------------------------------------------------------------------------------
/**
    Declaration macro. Put this into the class declaration.
*/
#define DeclareClass(type) \
public: \
    void* operator new(size_t size) \
    { \
        return Memory::Alloc(size); \
    }; \
    void operator delete(void* p) \
    { \
        Memory::Free(p); \
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
    Core::Rtti type::RTTI(#type, fourcc, type::FactoryCreator, &baseType::RTTI); \
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
    Core::Rtti type::RTTI(#type, fourcc, type::FactoryCreator, &baseType::RTTI); \
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
    Core::Rtti type::RTTI(#type, fourcc, 0, &baseType::RTTI); \
    Core::Rtti* type::GetRtti() const { return &this->RTTI; }

//------------------------------------------------------------------------------
/**
    Type implementation of topmost type in inheritance hierarchy (source file).
*/
#if NEBULA3_DEBUG
#define ImplementRootClass(type, fourcc) \
    Core::Rtti type::RTTI(#type, fourcc, type::FactoryCreator, 0); \
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
    Core::Rtti type::RTTI(#type, fourcc, type::FactoryCreator, 0); \
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