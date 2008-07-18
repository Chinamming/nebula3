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

ImplementClass(Nebula2::nMemoryAnimation, 'MEMA', Nebula2::nAnimation);

namespace Nebula2
{

using namespace Math;
// structs for faster NAX loading
#pragma pack(push, 1)
struct NaxHeader
{
    int magic;
    int numGroups;
    int numKeys;
};

struct NaxGroup
{
    int numCurves;
    int startKey;
    int numKeys;
    int keyStride;
    float keyTime;
    float fadeInFrames;
    int loopType;
    char metaData[256];
};
struct NaxCurve
{
    int ipolType;
    int firstKeyIndex;
    int isAnim;
    Math::float4 collapsedKey;
};
#pragma pack(pop)

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
*/
bool
nMemoryAnimation::LoadResource()
{
    n_assert(!this->IsLoaded());

    bool success = false;
    Util::String filename = this->GetResourceId().Value().AsCharPtr();
    if (filename.CheckFileExtension("nanim2"))
    {
        n_error("Loading of nanim2 file not supported! Use nax2 binary files!");
    }
    else if (filename.CheckFileExtension("nax2"))
    {
        success = this->LoadNax2(filename.AsCharPtr());
    }
    if (success)
    {
        this->SetState(Resources::Resource::Loaded);
    }
    return success;
}

//------------------------------------------------------------------------------
/**
*/
void
nMemoryAnimation::UnloadResource()
{
    if (this->IsLoaded())
    {
        nAnimation::UnloadResource();
        this->keyArray.SetSize(0);
        this->SetState(Resources::Resource::Initial);
    }
}

//------------------------------------------------------------------------------
/**
    Loads animation data from a binary nax2 file.

    - 30-Jun-04 floh    fixed assertion bug when number of keys in a curve is 0
    - 22-Jun-07 floh    optimized loading (fewer file accesses), now does only
                        do 4 reads instead of several thousands on big files
*/
bool
nMemoryAnimation::LoadNax2(const char* filename)
{
    n_assert(!this->IsLoaded());

    // TODO: use extra streamreader?
    Ptr<IO::Stream> stream = IO::IoServer::Instance()->CreateStream(filename);
    stream->SetAccessMode(IO::Stream::ReadAccess);
    
    // open the file
    if (!stream->Open())
    {
        n_error("nMemoryAnimation::LoadNax2(): Could not open file %s!", filename);        
        return false;
    }

    // read header
    NaxHeader naxHeader;
    stream->Read(&naxHeader, sizeof(naxHeader));
    if (naxHeader.magic != 'NAX3')
    {
        n_error("nMemoryAnimation::LoadNax2(): File %s has obsolete file format!", filename);
        stream->Close();        
        return false;
    }
    if (0 == naxHeader.numGroups)
    {
        n_error("nMemoryAnimation::LoadNax2(): File %s has no groups! Invalid Export ?", filename);
        stream->Close();        
        return false;
    }
    this->SetNumGroups(naxHeader.numGroups);
    this->keyArray.SetSize(naxHeader.numKeys);

    // read groups
    SizeT groupDataSize = sizeof(NaxGroup) * naxHeader.numGroups;
    NaxGroup* groupData = (NaxGroup*) Memory::Alloc(groupDataSize);
    stream->Read(groupData, groupDataSize);
    IndexT groupIndex = 0;
    SizeT numCurves = 0;
    for (groupIndex = 0; groupIndex < (uint)naxHeader.numGroups; groupIndex++)
    {
        const NaxGroup& naxGroup = groupData[groupIndex];
        Group& group = this->GetGroupAt(groupIndex);
        group.SetNumCurves(naxGroup.numCurves);
        group.SetStartKey(naxGroup.startKey);
        group.SetNumKeys(naxGroup.numKeys);
        group.SetKeyStride(naxGroup.keyStride);
        group.SetKeyTime(naxGroup.keyTime);
        group.SetFadeInFrames(naxGroup.fadeInFrames);
        group.SetLoopType((Group::LoopType) naxGroup.loopType);
        if (!group.SetMetaData(naxGroup.metaData))
        {
            n_error("MetaData error:\n String: %s\n Name: %s\n File: %s", naxGroup.metaData, "", this->GetResourceId().Value().AsCharPtr());
        }
        numCurves += group.GetNumCurves();
    }
    Memory::Free(groupData);
    groupData = 0;

    // read curves
    SizeT curveDataSize = sizeof(NaxCurve) * numCurves;
    NaxCurve* curveData = (NaxCurve*) Memory::Alloc(curveDataSize);
    stream->Read(curveData, curveDataSize);
    IndexT dataCurveIndex = 0;
    for (groupIndex = 0; groupIndex < (uint)naxHeader.numGroups; groupIndex++)
    {
        Group& group = this->GetGroupAt(groupIndex);
        IndexT curveIndex;
        for (curveIndex = 0; curveIndex < (uint)group.GetNumCurves(); curveIndex++, dataCurveIndex++)
        {
            const NaxCurve& naxCurve = curveData[dataCurveIndex];
            Curve& curve = group.GetCurveAt(curveIndex);
            curve.SetIpolType((Curve::IpolType) naxCurve.ipolType);
            curve.SetFirstKeyIndex(naxCurve.firstKeyIndex);
            curve.SetIsAnimated(naxCurve.isAnim);
            curve.SetConstValue(naxCurve.collapsedKey);
        }
    }
    Memory::Free(curveData);
    curveData = 0;

    // read keys
    if (naxHeader.numKeys > 0)
    {
        int keyArraySize = naxHeader.numKeys * sizeof(Math::float4);
        stream->Read(&(this->keyArray[0]), keyArraySize);
    }

    // cleanup
    stream->Close();    
    return true;
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
    int animCount = 0;
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