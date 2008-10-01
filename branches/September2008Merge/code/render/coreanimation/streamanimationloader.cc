//------------------------------------------------------------------------------
//  streamanimationloader.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coreanimation/streamanimationloader.h"
#include "coreanimation/animresource.h"
#include "system/byteorder.h"

namespace CoreAnimation
{
ImplementClass(CoreAnimation::StreamAnimationLoader, 'SANL', Resources::StreamResourceLoader);

using namespace IO;
using namespace Util;
using namespace System;
using namespace Math;

// legacy NAX2 file format structs
#if NEBULA3_LEGACY_SUPPORT
#pragma pack(push, 1)
struct Nax2Header
{
    uint magic;
    SizeT numGroups;
    SizeT numKeys;
};

struct Nax2Group
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

struct Nax2Curve
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
#endif

//------------------------------------------------------------------------------
/**
*/
bool
StreamAnimationLoader::SetupResourceFromStream(const Ptr<Stream>& stream)
{
    n_assert(stream.isvalid());
    n_assert(this->resource.isvalid());
    String fileExt = this->resource->GetResourceId().Value().GetFileExtension();
    #if NEBULA3_LEGACY_SUPPORT
    if (fileExt == "nax2")
    {
        return this->SetupFromNax2(stream);
    }
    else
    #endif
    if (fileExt == "nax3")
    {
        n_error("FIXME!");
        return false;
    }
    else if (fileExt == "nanim3")
    {
        n_error("FIXME!");
        return false;
    }
    else
    {
        n_error("CoreAnimation::StreamAnimationLoader: unrecognized file extension in '%s'\n", this->resource->GetResourceId().Value().AsCharPtr());
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
/**
    When loading legacy NAX2 files, clip names must be provided by the
    caller because they are not part of NAX2 files!
*/
#if NEBULA3_LEGACY_SUPPORT
void
StreamAnimationLoader::SetNax2ClipNames(const Array<String>& clipNames)
{
    this->nax2ClipNames = clipNames;
}
#endif

//------------------------------------------------------------------------------
/**
    Setup the animation resource from a legacy Nebula2 binary
    animation file. 
    FIXME: NAX2 files don't contain clip names! Need to feed clip names
    from some external source...
*/
#if NEBULA3_LEGACY_SUPPORT
bool
StreamAnimationLoader::SetupFromNax2(const Ptr<Stream>& stream)
{
    const Ptr<AnimResource>& anim = this->resource.downcast<AnimResource>();
    n_assert(!anim->IsLoaded());
    uchar* ptr = (uchar*) stream->Map();

    // read header
    ByteOrder byteOrder(ByteOrder::LittleEndian, ByteOrder::Host);
    Nax2Header* naxHeader = (Nax2Header*) ptr;
    byteOrder.Convert<uint>(naxHeader->magic);
    byteOrder.Convert<SizeT>(naxHeader->numGroups);
    byteOrder.Convert<SizeT>(naxHeader->numKeys);
    ptr += sizeof(Nax2Header);

    // check magic value
    if (FourCC(naxHeader->magic) != 'NAX3')
    {
        n_error("nMemoryAnimation::SetupFromNax2(): '%s' has obsolete file format!", stream->GetURI().AsString().AsCharPtr());        
        return false;
    }
    n_assert(0 != naxHeader->numGroups);

    // setup animation clips
    n_assert(this->nax2ClipNames.Size() == naxHeader->numGroups);
    anim->BeginSetupClips(naxHeader->numGroups);
    IndexT clipIndex;
    for (clipIndex = 0; clipIndex < naxHeader->numGroups; clipIndex++)
    {
        Nax2Group* naxGroup = (Nax2Group*) ptr;
        
        // endian conversion
        byteOrder.Convert<SizeT>(naxGroup->numCurves);
        byteOrder.Convert<IndexT>(naxGroup->startKey);
        byteOrder.Convert<SizeT>(naxGroup->numKeys);
        byteOrder.Convert<SizeT>(naxGroup->keyStride);
        byteOrder.Convert<float>(naxGroup->keyTime);
        byteOrder.Convert<float>(naxGroup->fadeInFrames);
        byteOrder.Convert<int>(naxGroup->loopType);
        
        // setup anim clip object
        AnimClip& clip = anim->Clip(clipIndex);
        clip.SetNumCurves(naxGroup->numCurves);
        clip.SetNumKeys(naxGroup->numKeys);
        clip.SetKeyStride(naxGroup->keyStride);
        clip.SetKeyDuration(Timing::SecondsToTicks(naxGroup->keyTime));
        if (0 == naxGroup->loopType)
        {
            // Clamp
            clip.SetPreInfinityType(InfinityType::Constant);
            clip.SetPostInfinityType(InfinityType::Constant);
        }
        else
        {
            clip.SetPreInfinityType(InfinityType::Cycle);
            clip.SetPostInfinityType(InfinityType::Cycle);
        }
        ptr += sizeof(Nax2Group);
    }

    // setup clip curves
    for (clipIndex = 0; clipIndex < naxHeader->numGroups; clipIndex++)
    {
        AnimClip& clip = anim->Clip(clipIndex);
        IndexT curveIndex;
        for (curveIndex = 0; curveIndex < clip.GetNumCurves(); curveIndex++)
        {
            Nax2Curve* naxCurve = (Nax2Curve*) ptr;
            
            // endian conversion
            byteOrder.Convert<int>(naxCurve->ipolType);
            byteOrder.Convert<int>(naxCurve->firstKeyIndex);
            byteOrder.Convert<int>(naxCurve->isAnim);
            byteOrder.Convert<float>(naxCurve->keyX);
            byteOrder.Convert<float>(naxCurve->keyY);
            byteOrder.Convert<float>(naxCurve->keyZ);
            byteOrder.Convert<float>(naxCurve->keyW);

            AnimCurve& animCurve = clip.CurveByIndex(curveIndex);
            animCurve.SetFirstKeyIndex(naxCurve->firstKeyIndex);
            animCurve.SetStatic(-1 == naxCurve->firstKeyIndex);
            animCurve.SetStaticKey(float4(naxCurve->keyX, naxCurve->keyY,naxCurve->keyZ, naxCurve->keyW));
            animCurve.SetActive(0 != naxCurve->isAnim);
            
            // this is a hack, Nebula2 files usually always have translation,
            // rotation and scale (in that order)
            int type = curveIndex % 3;
            switch (type)
            {
                case 0:
                    animCurve.SetCurveType(CurveType::Translation);
                    break;
                case 1:
                    n_assert(2 == naxCurve->ipolType);   // nAnimation::IpolType::Quat
                    animCurve.SetCurveType(CurveType::Rotation);
                    break;
                case 2:
                    animCurve.SetCurveType(CurveType::Scale);
                    break;
            }

            // advance to next NAX2 curve
            ptr += sizeof(Nax2Curve);
        }
    }
    anim->EndSetupClips();

    // finally load keys (endian convert if necessary)
    const Ptr<AnimKeyBuffer> animKeyBuffer = anim->SetupKeyBuffer(naxHeader->numKeys);
    void* keyPtr = animKeyBuffer->Map();
    Memory::Copy(ptr, keyPtr, animKeyBuffer->GetByteSize());

    #pragma warning(push)   
    #pragma warning(disable : 4127) // expression is constant
    if (ByteOrder::Host != ByteOrder::LittleEndian)
    {
        ByteOrder byteOrder(ByteOrder::LittleEndian, ByteOrder::Host);
        float* floatKeyPtr = (float*) keyPtr;
        IndexT keyIndex;
        SizeT numKeys = animKeyBuffer->GetNumKeys();
        for (keyIndex = 0; keyIndex < numKeys; keyIndex++)
        {
            byteOrder.Convert<float>(floatKeyPtr[0]);
            byteOrder.Convert<float>(floatKeyPtr[1]);
            byteOrder.Convert<float>(floatKeyPtr[2]);
            byteOrder.Convert<float>(floatKeyPtr[3]);
            floatKeyPtr += 4;
        }
    }
    #pragma warning(pop)
    animKeyBuffer->Unmap();

    // done!
    return true;
}
#endif

} // namespace CoreAnimation
