//------------------------------------------------------------------------------
//  nmemoryanimation_main.cc
//  (C) 2003 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "nmemoryanimation.h"
#include "io/ioserver.h"
#include "io/streamreader.h"
#include "math/quaternion.h"
#include "memory/memory.h"
#include "system/byteorder.h"

__ImplementClass(Nebula2::nMemoryAnimation, 'MEMA', Nebula2::nAnimation);

namespace Nebula2
{

using namespace Math;

//------------------------------------------------------------------------------
/**
*/
nMemoryAnimation::nMemoryAnimation()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
nMemoryAnimation::~nMemoryAnimation()
{
    if (this->IsLoaded())
    {
        this->Unload();
    }
}

//------------------------------------------------------------------------------
/**
    Samples the current values for a number of curves in the given
    animation group. The sampled values will be written to a client provided
    Math::float4 array.

    @param  time                a point in time
    @param  groupIndex          index of animation group to sample from
    @param  firstCurveIndex     group-relative curve index of first curve to sample
    @param  numCurves           number of curves to sample
    @param  dstKeyArray         pointer to Math::float4 array with numCurves element which
                                will be filled with the results
    - 18-Oct-2004   floh    Fixed collapsed curve check (now checks if start key
                            is -1, instead of the curveIpolType). The curve ipol type
                            MUST be set to something sane even for collapsed curves,
                            because it's used at various places for deviding whether
                            Math::quaternion-interpolation must be used instead of simple
                            linear interpolation!!
*/
void
nMemoryAnimation::SampleCurves(Timing::Time time, int groupIndex, int firstCurveIndex, int numCurves, Math::float4* dstKeyArray)
{
    // convert the time into 2 global key indices and an inbetween value
    const Group& group = this->GetGroupAt(groupIndex);
    int keyIndex[2];
    Timing::Time inbetween;
    group.TimeToIndex(time, keyIndex[0], keyIndex[1], inbetween);
	Math::float4* srcKeys = 0;

	if(this->keyArray.Size() != 0)
	{
		srcKeys = &(this->keyArray[0]);
	}
		

    int i;
    static Math::quaternion q0;
    static Math::quaternion q1;
    static Math::quaternion q;
    for (i = 0; i < numCurves; i++)
    {
       Curve& curve = group.GetCurveAt(i + firstCurveIndex);

       if (curve.GetFirstKeyIndex() == -1)
       {
           // a collapsed curve
           dstKeyArray[i] = curve.GetConstValue();
       }
       else
       {
           int curveFirstKeyIndex = curve.GetFirstKeyIndex();
           switch (curve.GetIpolType())
           {
               case Curve::Step:
               {
                   int index0 = curveFirstKeyIndex + keyIndex[0];
                   dstKeyArray[i] = srcKeys[index0];
                   index0 = curve.GetFirstKeyIndex();
               }
               break;

               case Curve::Quat:
               {
                   // do some dirty pointer arithmetics to speed up processing
                   int index0 = curveFirstKeyIndex + keyIndex[0];
                   int index1 = curveFirstKeyIndex + keyIndex[1];
                   q = Math::quaternion::slerp(*(quaternion*)&(srcKeys[index0]), *(quaternion*)&(srcKeys[index1]), (float)inbetween);
                   dstKeyArray[i] = *(Math::float4*)&q;
               }
               break;

               case Curve::Linear:
               {
                   int curveFirstKeyIndex = curve.GetFirstKeyIndex();
                   int index0 = curveFirstKeyIndex + keyIndex[0];
                   int index1 = curveFirstKeyIndex + keyIndex[1];
                   const Math::float4& v0 = srcKeys[index0];
                   const Math::float4& v1 = srcKeys[index1];
                   dstKeyArray[i] = v0 + ((v1 - v0) * (float)inbetween);
               }
               break;
            
               default:
                   n_error("nMemoryAnimation::SampleCurves(): invalid curveIpolType %d!", curve.GetIpolType());
                   break;
           }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
int
nMemoryAnimation::GetByteSize()
{
    return this->keyArray.Size() * sizeof(Math::float4);
}
}; // namespace Nebula2