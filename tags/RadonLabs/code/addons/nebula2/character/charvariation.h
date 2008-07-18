#ifndef N_CHARVARIATION_H
#define N_CHARVARIATION_H
//------------------------------------------------------------------------------
/**
    @class CharVariation
    @ingroup Character

    @brief 

    (C) 2007 RadonLabs GmbH
*/
#include "core/types.h"
#include "ncharjoint.h"

namespace Nebula2
{
//------------------------------------------------------------------------------
class CharVariation
{
public:
    /// constructor
    CharVariation();
    /// destructor
    ~CharVariation();

    Util::String name;
    IndexT animGroupIndex;
    Util::Array<nCharJoint> joints;
};

//------------------------------------------------------------------------------
/**
*/
inline
CharVariation::CharVariation()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
CharVariation::~CharVariation()
{
    // empty
}
}; // namespace Nebula2
//------------------------------------------------------------------------------
#endif
