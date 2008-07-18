//------------------------------------------------------------------------------
//  definition.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "attr/attributedefinitionbase.h"

namespace Attr
{
Util::HashTable<Util::String, const AttributeDefinitionBase*>* AttributeDefinitionBase::NameRegistry = 0;
Util::Dictionary<Util::FourCC, const AttributeDefinitionBase*>* AttributeDefinitionBase::FourCCRegistry = 0;
Util::Array<const AttributeDefinitionBase*>* AttributeDefinitionBase::DynamicAttributes = 0;
   
//------------------------------------------------------------------------------
/**
*/
AttributeDefinitionBase::AttributeDefinitionBase() :
    accessMode(ReadOnly),
    isDynamic(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
AttributeDefinitionBase::AttributeDefinitionBase(const Util::String& n, const Util::FourCC& fcc, AccessMode m, bool dyn) :
    name(n),
    fourCC(fcc),
    accessMode(m),
    isDynamic(dyn)
{
    this->Register();
}

//------------------------------------------------------------------------------
/**
*/
AttributeDefinitionBase::AttributeDefinitionBase(const Util::String& n, const Util::FourCC& fcc, AccessMode m, bool defVal, bool dyn) :
    name(n),
    fourCC(fcc),
    accessMode(m),
    isDynamic(dyn),
    defaultValue(defVal)
{
    this->Register();
}

//------------------------------------------------------------------------------
/**
*/
AttributeDefinitionBase::AttributeDefinitionBase(const Util::String& n, const Util::FourCC& fcc, AccessMode m, int defVal, bool dyn) :
    name(n),
    fourCC(fcc),
    accessMode(m),
    isDynamic(dyn),
    defaultValue(defVal)
{
    this->Register();
}

//------------------------------------------------------------------------------
/**
*/
AttributeDefinitionBase::AttributeDefinitionBase(const Util::String& n, const Util::FourCC& fcc, AccessMode m, float defVal, bool dyn) :
    name(n),
    fourCC(fcc),
    accessMode(m),
    isDynamic(dyn),
    defaultValue(defVal)
{
    this->Register();
}

//------------------------------------------------------------------------------
/**
*/
AttributeDefinitionBase::AttributeDefinitionBase(const Util::String& n, const Util::FourCC& fcc, AccessMode m, const Util::String& defVal, bool dyn) :
    name(n),
    fourCC(fcc),
    accessMode(m),
    isDynamic(dyn),
    defaultValue(defVal)
{
    this->Register();
}

//------------------------------------------------------------------------------
/**
*/
    AttributeDefinitionBase::AttributeDefinitionBase(const Util::String& n, const Util::FourCC& fcc, AccessMode m, const Math::float4& defVal, bool dyn) :
    name(n),
    fourCC(fcc),
    accessMode(m),
    isDynamic(dyn),
    defaultValue(defVal)
{
    this->Register();
}

//------------------------------------------------------------------------------
/**
*/
AttributeDefinitionBase::AttributeDefinitionBase(const Util::String& n, const Util::FourCC& fcc, AccessMode m, const Math::matrix44& defVal, bool dyn) :
    name(n),
    fourCC(fcc),
    accessMode(m),
    isDynamic(dyn),
    defaultValue(defVal)
{
    this->Register();
}

//------------------------------------------------------------------------------
/**
*/
AttributeDefinitionBase::AttributeDefinitionBase(const Util::String& n, const Util::FourCC& fcc, AccessMode m, const Util::Blob& defVal, bool dyn) :
    name(n),
    fourCC(fcc),
    accessMode(m),
    isDynamic(dyn),
    defaultValue(defVal)
{
    this->Register();
}

//------------------------------------------------------------------------------
/**
*/
AttributeDefinitionBase::AttributeDefinitionBase(const Util::String& n, const Util::FourCC& fcc, AccessMode m, const Util::Guid& defVal, bool dyn) :
    name(n),
    fourCC(fcc),
    accessMode(m),
    isDynamic(dyn),
    defaultValue(defVal)
{
    this->Register();
}

//------------------------------------------------------------------------------
/**
*/
AttributeDefinitionBase::~AttributeDefinitionBase()
{
    this->Unregister();
}

//------------------------------------------------------------------------------
/**
    Check if name registry exists and create if necessary.
*/
void
AttributeDefinitionBase::CheckCreateNameRegistry()
{
    if (0 == NameRegistry)
    {
        NameRegistry = new Util::HashTable<Util::String, const AttributeDefinitionBase*>(4096);
    }
}
//------------------------------------------------------------------------------
/**
    Check if fourcc registry exists and create if necessary.
*/
void
AttributeDefinitionBase::CheckCreateFourCCRegistry()
{
    if (0 == FourCCRegistry)
    {
        FourCCRegistry = new Util::Dictionary<Util::FourCC, const AttributeDefinitionBase*>;
    }
}

//------------------------------------------------------------------------------
/**
    Check if dynamic attrs array exists and create if necessary.
*/
void
AttributeDefinitionBase::CheckCreateDynamicAttributesArray()
{
    if (0 == DynamicAttributes)
    {
        DynamicAttributes = new Util::Array<const AttributeDefinitionBase*>;
    }
}


//------------------------------------------------------------------------------
/**
    Register this static attribute definition in the name and fourcc registries.
    Since the order of initialization is not defined for static 
    objects we need to use pointers and creation-on-demand for the registry
    objects.
*/
void
AttributeDefinitionBase::Register()
{
    n_assert(this->name.IsValid());
    this->CheckCreateNameRegistry();
    this->CheckCreateFourCCRegistry();
	if (NameRegistry->Contains(this->name))
    {
        n_error("Attribute '%s' already registered!", this->name.AsCharPtr());
    }
    NameRegistry->Add(this->name, this);
    
    if (this->IsDynamic())
    {
        CheckCreateDynamicAttributesArray();
        DynamicAttributes->Append(this);
    }
    else
    {
        // only non-dynamic attributes has a valid fourcc code
        n_assert(this->fourCC.IsValid());
        FourCCRegistry->Add(this->fourCC, this);
    }
}

//------------------------------------------------------------------------------
/**
    Unregister this attribute definition from the registries.
*/
void
AttributeDefinitionBase::Unregister()
{
    n_assert((0 != NameRegistry) && (0 != FourCCRegistry));
    NameRegistry->Erase(this->name);
    FourCCRegistry->Erase(this->fourCC);
    if (this->IsDynamic())
    {
        n_assert(0 != DynamicAttributes);
        IndexT index = DynamicAttributes->FindIndex(this);
        n_assert(InvalidIndex != index);
        DynamicAttributes->EraseIndex(index);
    }
}

//------------------------------------------------------------------------------
/**
    Cleanup the name registry!
*/
void
AttributeDefinitionBase::Destroy()
{
    // first clear the dynamic attributes
    ClearDynamicAttributes();

    if (NameRegistry != 0)
    {
        // cleanup name registry
        delete NameRegistry;
        NameRegistry = 0;
    }
    if (FourCCRegistry != 0)
    {
        // cleanup fourcc registry
        delete FourCCRegistry;
        FourCCRegistry = 0;
    }
    if (DynamicAttributes != 0)
    {
        delete DynamicAttributes;
        DynamicAttributes = 0;
    }
}

//------------------------------------------------------------------------------
/**
    This method registers a new dynamic attribut.
*/
void
AttributeDefinitionBase::RegisterDynamicAttribute(const Util::String &name, const Util::FourCC& fourCC, Attr::ValueType valueType, Attr::AccessMode accessMode)
{
    if (FindByName(name))
    {
        n_error("RegisterDynamicAttribute: attribute '%s' already exists!\n", name.AsCharPtr());
    }
    AttributeDefinitionBase* dynAttr;
    switch (valueType)
    {
        case VoidType:
            dynAttr = n_new(AttributeDefinitionBase(name, fourCC, accessMode, true));
            break;

        case IntType:
            dynAttr = n_new(AttributeDefinitionBase(name, fourCC, accessMode, 0, true));
            break;

        case FloatType:
            dynAttr = n_new(AttributeDefinitionBase(name, fourCC, accessMode, 0.0f, true));
            break;

        case BoolType:
            dynAttr = n_new(AttributeDefinitionBase(name, fourCC, accessMode, false, true));
            break;

        case Float4Type:
            {
                const static Math::float4 nullVec;
                dynAttr = n_new(AttributeDefinitionBase(name, fourCC, accessMode, nullVec, true));
                break;
            }
            break;

        case Matrix44Type:
            {
                const static Math::matrix44 identity;
                dynAttr = n_new(AttributeDefinitionBase(name, fourCC, accessMode, identity, true));
                break;
            }
            break;

        case BlobType:
            {
                const static Util::Blob blob;
                dynAttr = n_new(AttributeDefinitionBase(name, fourCC, accessMode, blob, true));
                break;
            }
            break;

        case GuidType:
            {
                const static Util::Guid guid;
                dynAttr = n_new(AttributeDefinitionBase(name, fourCC, accessMode, guid, true));
                break;
            }
            break;

        case StringType:
            {
                const static Util::String str;
                dynAttr = n_new(AttributeDefinitionBase(name, fourCC, accessMode, str, true));
            }
            break;

        default:
            n_error("RegisterDynamicAttribute(): invalid value type!");
            break;
    }
}

//------------------------------------------------------------------------------
/**
    This clears all dynamic attributes.
*/
void
AttributeDefinitionBase::ClearDynamicAttributes()
{
    if (0 != DynamicAttributes)
    {
        while (!DynamicAttributes->IsEmpty())
        {
            const AttributeDefinitionBase* cur = DynamicAttributes->Front();
            delete cur;
        }
    }
}


}