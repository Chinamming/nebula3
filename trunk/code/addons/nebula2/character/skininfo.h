#ifndef N_SKININFO_H
#define N_SKININFO_H
//------------------------------------------------------------------------------
/**
    @class SkinInfo
    @ingroup Character

    @brief 

    (C) 2007 RadonLabs GmbH
*/
#include "core/types.h"
#include "models/nodes/transformnode.h"

namespace Nebula2
{
//------------------------------------------------------------------------------
class SkinInfo
{
public:
    /// constructor
    SkinInfo();
    /// destructor
    ~SkinInfo();

    Ptr<Models::TransformNode> skinNode;
    Util::String name;
    Util::String category;
};

//------------------------------------------------------------------------------
/**
*/
inline
SkinInfo::SkinInfo()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
SkinInfo::~SkinInfo()
{
    // empty
}
}; // namespace Nebula2
//------------------------------------------------------------------------------
#endif
