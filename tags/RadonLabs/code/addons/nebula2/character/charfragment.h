#ifndef N_CHARFRAGMENT_H
#define N_CHARFRAGMENT_H
//------------------------------------------------------------------------------
/**
    @class CharFragment
    @ingroup Character

    @brief 

    (C) 2007 RadonLabs GmbH
*/
#include "core/types.h"
#include "ncharjointpalette.h"

namespace Nebula2
{
//------------------------------------------------------------------------------
class CharFragment
{
public:
    /// constructor
    CharFragment();
    /// destructor
    ~CharFragment();

    /// set mesh group index
    void SetMeshGroupIndex(int i);
    /// get mesh group index
    int GetMeshGroupIndex() const;
    /// get embedded joint palette
    nCharJointPalette& GetJointPalette();

private:
    int primGroupIndex;
    nCharJointPalette jointPalette;
};

//------------------------------------------------------------------------------
/**
*/
inline
CharFragment::CharFragment():
    primGroupIndex(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
CharFragment::~CharFragment()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
void
CharFragment::SetMeshGroupIndex(int i)
{
    this->primGroupIndex = i;
}

//------------------------------------------------------------------------------
/**
*/
inline
int
CharFragment::GetMeshGroupIndex() const
{
    return this->primGroupIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline
nCharJointPalette&
CharFragment::GetJointPalette()
{
    return this->jointPalette;
}
}; // namespace Nebula2
//------------------------------------------------------------------------------
#endif
