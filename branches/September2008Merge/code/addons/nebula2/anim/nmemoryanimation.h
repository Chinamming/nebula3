#ifndef N_MEMORYANIMATION_H
#define N_MEMORYANIMATION_H
//------------------------------------------------------------------------------
/**
    @class nMemoryAnimation
    @ingroup Anim2

    @brief An nMemoryAnimation object holds its entire animation data in
    memory.  Since all the animation is in memory all the time, the animation
    can be shared between many client objects. The disadvantage is of
    course the memory footprint.

    See the parent class nAnimation for more info.

    (C) 2003 RadonLabs GmbH
*/
#include "nanimation.h"

namespace Nebula2
{
//------------------------------------------------------------------------------
class nMemoryAnimation : public nAnimation
{
    __DeclareClass(nMemoryAnimation);
public:
    /// constructor
    nMemoryAnimation();
    /// destructor
    virtual ~nMemoryAnimation();
    /// sample value of given curve at given time
    virtual void SampleCurves(Timing::Time time, int groupIndex, int firstCurveIndex, int numCurves, Math::float4* keyArray);
    /// get an estimated byte size of the resource data (for memory statistics)
    virtual int GetByteSize();
    /// gets the keyArray
    const Util::FixedArray<Math::float4>& GetKeyArray() const;
    /// set key array
    void SetKeyArray(const Util::FixedArray<Math::float4>& kArray);

protected:

    Util::FixedArray<Math::float4> keyArray;
};

//------------------------------------------------------------------------------
/**
*/
inline
const Util::FixedArray<Math::float4>&
nMemoryAnimation::GetKeyArray() const
{
    return this->keyArray;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
nMemoryAnimation::SetKeyArray(const Util::FixedArray<Math::float4>& kArray)
{
    this->keyArray = kArray;
}
}; // namespace Nebula2
//------------------------------------------------------------------------------
#endif
