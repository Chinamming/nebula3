//------------------------------------------------------------------------------
//  rtti.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "core/rtti.h"
#include "core/refcounted.h"
#include "core/sysfunc.h"

namespace Core
{
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
Rtti::Rtti(const char* className, FourCC fcc, Creator creatorFunc, const Rtti* parentClass) :
    fourCC(fcc),
    parent(parentClass),
    creator(creatorFunc)
{
    n_assert(0 != className);
    n_assert(fourCC.IsValid() != 0);
    n_assert(parentClass != this);

    // make sure String, etc... is working correctly
    Core::SysFunc::Setup();
    this->name = className;
    if (!Factory::Instance()->ClassExists(this->name))
    {
        Factory::Instance()->Register(this, this->name, fcc);
    }
}

//------------------------------------------------------------------------------
/**
*/
RefCounted*
Rtti::Create() const
{
    if (0 == this->creator)
    {
        n_error("Rtti::Create(): Trying to create instance of abstract class '%s'!", this->name.AsCharPtr());
        return 0;
    }
    else
    {
        return this->creator();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
Rtti::IsDerivedFrom(const Rtti& other) const
{
    const Rtti* cur;
    for (cur = this; cur != 0; cur = cur->GetParent())
    {
        if (cur == &other)
        {
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
Rtti::IsDerivedFrom(const Util::String& otherClassName) const
{
    const Rtti* cur;
    for (cur = this; cur != 0; cur = cur->GetParent())
    {
        if (cur->name == otherClassName)
        {
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
Rtti::IsDerivedFrom(const Util::FourCC& otherClassFourCC) const
{
    const Rtti* cur;
    for (cur = this; cur != 0; cur = cur->GetParent())
    {
        if (cur->fourCC == otherClassFourCC)
        {
            return true;
        }
    }
    return false;
}

} // namespace Core
