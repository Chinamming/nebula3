#ifndef N_ANIMCLIP_H
#define N_ANIMCLIP_H
//------------------------------------------------------------------------------
/**
    @class nAnimClip
    @ingroup Anim2

    @brief An animation clip bundles several animation curves into a unit
    and associates them with a weight value.

    (C) 2003 RadonLabs GmbH
*/
#include "nanimation.h"

namespace Nebula2
{
//------------------------------------------------------------------------------
class nAnimClip
{
public:
    /// default constructor
    nAnimClip();
    /// constructor
    nAnimClip(const Util::String& clipName, int animGroupIndex, int numCurves);
    /// get the name of the clip
    const Util::String& GetClipName() const;
    /// get animation group index
    int GetAnimGroupIndex() const;
    /// get number of curves in the clip's anim group
    int GetNumCurves() const;

private:
    Util::String clipName;
    int animGroupIndex;
    int numCurves;
};

//------------------------------------------------------------------------------
/**
*/
inline
nAnimClip::nAnimClip() :
    animGroupIndex(0),
    numCurves(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
nAnimClip::nAnimClip(const Util::String& name, int animGroupIndex, int numCurves) :
   clipName(name),
   animGroupIndex(animGroupIndex),
   numCurves(numCurves)
{
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String&
nAnimClip::GetClipName() const
{
    return this->clipName;
}

//------------------------------------------------------------------------------
/**
*/
inline
int
nAnimClip::GetAnimGroupIndex() const
{
    return this->animGroupIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline
int
nAnimClip::GetNumCurves() const
{
    return this->numCurves;
}
}; // namespace Nebula2
//------------------------------------------------------------------------------
#endif
