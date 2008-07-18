#ifndef N_METADATA_H
#define N_METADATA_H
//------------------------------------------------------------------------------
/**
    @class nAnimKey
    @ingroup Util

    Holds metadata from animation clips etc.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/types.h"
#include "util/variant.h"
#include "util/dictionary.h"

namespace Nebula2
{
//------------------------------------------------------------------------------
class nMetaData
{
public:
    /// default constructor
    nMetaData();
    /// initialize
    bool Init(const Util::String& dataString);
    /// get data by name
    const Util::Variant& GetData(const Util::String& name) const;
    /// return true if data exists by name
    bool HasData(const Util::String& name) const;
    /// set data
    void SetData(const Util::String& name, const Util::Variant& data);
    /// get content concatenated to string
    Util::String AsString() const;

private:    
    Util::Dictionary<Util::String, Util::Variant> dictionary; 
};

//------------------------------------------------------------------------------
/**
*/
inline
nMetaData::nMetaData()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline bool
nMetaData::Init(const Util::String &dataString)
{
    bool ok = true;

    Util::Array<Util::String> dataSetTokens = dataString.Tokenize(";");
    uint i;
    for (i = 0; i < dataSetTokens.Size(); i++)
    {
        Util::Array<Util::String> values = dataSetTokens[i].Tokenize("=");
        Util::String name;
        Util::Variant data;
        if (values.Size() == 2) 
        {
            name = values[0];
            data.SetString(values[1]);
        }
        else if (values.Size() == 3)
        {
            name = values[1];            
            values[0].ToLower();
            Util::Variant::Type type = Util::Variant::StringToType(values[0]);
            switch (type)
            {
                case Util::Variant::Int:       data.SetInt(values[2].AsInt());           break;
                case Util::Variant::Float:     data.SetFloat(values[2].AsFloat());       break;
                case Util::Variant::Bool:      data.SetBool(values[2].AsBool());         break;                
                case Util::Variant::Float4:    data.SetFloat4(values[2].AsFloat4());    break;
                case Util::Variant::Matrix44:  data.SetMatrix44(values[2].AsMatrix44()); break;
                case Util::Variant::String:    data.SetString(values[2]);                break;
                default:                       data.SetString(values[2]);                break;
            }
        }
        else
        {
            ok = false;
            //n_error("MetaData Error: %s has not 2 or 3 parts ([type=]name=data)", dataSetTokens[i].Get());
        }

        this->SetData(name, data); 
    }
    return ok;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Variant&
nMetaData::GetData(const Util::String &name) const
{
    int index = this->dictionary.FindIndex(name);
    if (index == -1) n_error("MetaData %s does not exist!", name.AsCharPtr());
    return this->dictionary.ValueAtIndex(index);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
nMetaData::HasData(const Util::String &name) const
{
    return this->dictionary.Contains(name);    
}

//------------------------------------------------------------------------------
/**
*/
inline void
nMetaData::SetData(const Util::String& name, const Util::Variant& data)
{
    IndexT index = this->dictionary.FindIndex(name);
    if (index != InvalidIndex)
    {
        this->dictionary.ValueAtIndex(index) = data;
    }
    else
    {
        this->dictionary.Add(name, data); 
    }
}

//------------------------------------------------------------------------------
/**
*/
inline Util::String
nMetaData::AsString() const
{
    Util::String res;
    IndexT i;
    for (i = 0; i < this->dictionary.Size(); i++)
    {
        const Util::String& name = this->dictionary.KeyAtIndex(i);
        const Util::Variant& var = this->dictionary.ValueAtIndex(i);        
        res.Append(Util::Variant::TypeToString(var.GetType()));
        res.Append("=");
        res.Append(name);
        res.Append("=");
        res.Append(var.GetString());
        res.Append(";");
    }
    return res;
}

}; // namespace Nebula2

//------------------------------------------------------------------------------
#endif
