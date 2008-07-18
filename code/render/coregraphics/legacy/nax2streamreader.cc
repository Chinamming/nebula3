//------------------------------------------------------------------------------
//  Nax2StreamReader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/legacy/nax2streamreader.h"
#include "system/byteorder.h"

#if NEBULA3_LEGACY_SUPPORT
namespace Legacy
{
ImplementClass(Legacy::Nax2StreamReader, 'N2AR', IO::StreamReader);

using namespace Util;
using namespace Math;
using namespace Resources;
using namespace System;
using namespace Anim;

// structs for faster NAX loading
#pragma pack(push, 1)
struct NaxHeader
{
    uint magic;
    SizeT numGroups;
    SizeT numKeys;
};

struct NaxGroup
{
    SizeT numCurves;
    IndexT startKey;
    SizeT numKeys;
    SizeT keyStride;
    float keyTime;
    float fadeInFrames;
    int loopType;
    char metaData[512];
};
struct NaxCurve
{
    int ipolType;
    int firstKeyIndex;
    int isAnim;
    float keyX;
    float keyY;
    float keyZ;
    float keyW;
};
#pragma pack(pop)

//------------------------------------------------------------------------------
/**
*/
Nax2StreamReader::Nax2StreamReader()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Nax2StreamReader::~Nax2StreamReader()
{
    if (this->IsOpen())
    {
        this->Close();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
Nax2StreamReader::Open()
{
    n_assert(this->stream->CanBeMapped());
    if (StreamReader::Open())
    {
        // TODO: read as memory or stream animation
        // read data
        this->ReadAnimationData();
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
Nax2StreamReader::Close()
{
    StreamReader::Close();
}

//------------------------------------------------------------------------------
/**
    This reads the nvx2 header data and checks whether the file is 
    actually an nvx2 file through the magic number. All header data
    will be read into member variables, and pointers to the
    start of the group-, vertex- and index-data will be setup.
*/
bool
Nax2StreamReader::ReadAnimationData()
{
    n_assert(this->animation.isvalid());

    n_assert(!this->animation->IsLoaded());

    String filename = this->animation->GetResourceId().Value().AsCharPtr();
    // read header
    NaxHeader naxHeader;
    stream->Read(&naxHeader, sizeof(naxHeader));
    FourCC magic = ByteOrder::ConvertUInt(ByteOrder::LittleEndian, ByteOrder::Host, naxHeader.magic);
    if (magic != 'NAX3')
    {
        n_error("nMemoryAnimation::LoadNax2(): File %s has obsolete file format!", filename);        
        return false;
    }

    uint numGroups = ByteOrder::ConvertUInt(ByteOrder::LittleEndian, ByteOrder::Host, naxHeader.numGroups);
    if (0 == numGroups)
    {
        n_error("nMemoryAnimation::LoadNax2(): File %s has no groups! Invalid Export ?", filename);        
        return false;
    }
    this->animation->SetNumGroups(numGroups);
    SizeT numKeys = ByteOrder::ConvertUInt(ByteOrder::LittleEndian, ByteOrder::Host, naxHeader.numKeys);
    this->keyArray.SetSize(numKeys);

    // read groups
    SizeT groupDataSize = sizeof(NaxGroup) * numGroups;
    NaxGroup* groupData = (NaxGroup*) Memory::Alloc(groupDataSize);
    stream->Read(groupData, groupDataSize);
    IndexT groupIndex = 0;
    SizeT numCurves = 0;
    for (groupIndex = 0; groupIndex < numGroups; groupIndex++)
    {
        const NaxGroup& naxGroup = groupData[groupIndex];
        Animation::Group& group = this->animation->GetGroupAt(groupIndex);
        group.SetNumCurves(ByteOrder::ConvertUInt(ByteOrder::LittleEndian, ByteOrder::Host, naxGroup.numCurves));
        group.SetStartKey(ByteOrder::ConvertUInt(ByteOrder::LittleEndian, ByteOrder::Host, naxGroup.startKey));
        group.SetNumKeys(ByteOrder::ConvertUInt(ByteOrder::LittleEndian, ByteOrder::Host, naxGroup.numKeys));
        group.SetKeyStride(ByteOrder::ConvertUInt(ByteOrder::LittleEndian, ByteOrder::Host, naxGroup.keyStride));
        group.SetKeyTime(ByteOrder::ConvertFloat(ByteOrder::LittleEndian, ByteOrder::Host, naxGroup.keyTime));
        group.SetFadeInFrames(ByteOrder::ConvertFloat(ByteOrder::LittleEndian, ByteOrder::Host, naxGroup.fadeInFrames));
        group.SetLoopType((Animation::Group::LoopType) ByteOrder::ConvertInt(ByteOrder::LittleEndian, ByteOrder::Host, naxGroup.loopType));
        if (!group.SetMetaData(naxGroup.metaData))
        {
            n_error("MetaData error:\n String: %s\n Name: %s\n File: %s", naxGroup.metaData, "", filename);
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
    for (groupIndex = 0; groupIndex < numGroups; groupIndex++)
    {
        Animation::Group& group = this->animation->GetGroupAt(groupIndex);
        IndexT curveIndex;
        for (curveIndex = 0; curveIndex < (uint)group.GetNumCurves(); curveIndex++, dataCurveIndex++)
        {
            const NaxCurve& naxCurve = curveData[dataCurveIndex];
            Animation::Curve& curve = group.GetCurveAt(curveIndex);
            curve.SetIpolType((Animation::Curve::IpolType) naxCurve.ipolType);
            curve.SetFirstKeyIndex(naxCurve.firstKeyIndex);
            curve.SetIsAnimated(naxCurve.isAnim);
            curve.SetConstValue(float4(naxCurve.keyX, naxCurve.keyY, naxCurve.keyZ, naxCurve.keyW));
        }
    }
    Memory::Free(curveData);
    curveData = 0;

    // read keys
    if (numKeys > 0)
    {
        int keyArraySize = numKeys * sizeof(float4);
        stream->Read(&(this->keyArray[0]), keyArraySize);
    }

    if (this->animation->IsA(MemoryAnimation::RTTI))
    {
        const Ptr<MemoryAnimation>& memAni = this->animation.downcast<MemoryAnimation>();
        memAni->SetKeyArray(this->keyArray);
    }

    return true;
}

} // namespace Legacy

#endif // NEBULA3_LEGACY_SUPPORT
