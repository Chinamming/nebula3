#ifndef GRAPHICS_ANIMTABLE_H
#define GRAPHICS_ANIMTABLE_H
//------------------------------------------------------------------------------
/**
    @class Graphics::AnimTable

    Creates a mapping from abstract animation names to actual anim clip names.
    
    (C) 2005 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "util/array.h"
#include "nmetadata.h"
#include "util/dictionary.h"
#include "timing/time.h"

//------------------------------------------------------------------------------
namespace Nebula2
{
class CharEntity;

class AnimTable : public Core::RefCounted
{
	__DeclareClass(AnimTable);
    __DeclareSingleton(AnimTable);
public:
    /// constructor
    AnimTable();
    /// destructor
    virtual ~AnimTable();
    /// set uri
    void SetUri(const Util::String& n);
    /// get uri
    const Util::String& GetUri() const;
    /// open the table
    bool Open();
    /// close the table
    void Close();
    /// return true if open
    bool IsOpen() const;
    /// Check if the specified animation exists
    bool ExistsAnimation(const Util::String& cat, const Util::String& animName) const;    
    /// get animation name for maping name
    const Util::String& GetMappedAnimName(const Util::String& cat, const Util::String& animName) const;

private:
    /// load the anim.xml
    bool LoadAnimTable();
    
    Util::String uri;
    Util::Dictionary<Util::String, Util::Dictionary<Util::String, Util::String> > animMapping;    
    bool isOpen;
};

//------------------------------------------------------------------------------
/**
*/
inline
bool
AnimTable::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
AnimTable::SetUri(const Util::String& n)
{
    this->uri = n;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String&
AnimTable::GetUri() const
{
    return this->uri;
}

};
//------------------------------------------------------------------------------
#endif    