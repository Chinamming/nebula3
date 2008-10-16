#pragma once
#ifndef COREANIMATION_STREAMANIMATIONLOADER_H
#define COREANIMATION_STREAMANIMATIONLOADER_H
//------------------------------------------------------------------------------
/** 
    @class CoreAnimation::StreamAnimationLoader
    
    Initialize a CoreAnimation::AnimResource from the content of a stream.
    
    (C) 2008 Radon Labs GmbH
*/
#include "resources/streamresourceloader.h"

//------------------------------------------------------------------------------
namespace CoreAnimation
{
class StreamAnimationLoader : public Resources::StreamResourceLoader
{
    __DeclareClass(StreamAnimationLoader);
public:
    #if NEBULA3_LEGACY_SUPPORT
    /// when loading from legacy NAX2 file format, caller needs to provide clip names!
    void SetNax2ClipNames(const Util::Array<Util::String>& clipNames);
    #endif

protected:
    /// setup the AnimResource object from a stream
    virtual bool SetupResourceFromStream(const Ptr<IO::Stream>& stream);

    #if NEBULA3_LEGACY_SUPPORT
    /// setup the AnimResource from a legacy NAX2 stream
    bool SetupFromNax2(const Ptr<IO::Stream>& stream);
    
    Util::Array<Util::String> nax2ClipNames;
    #endif
};

} // namespace CoreAnimation
//------------------------------------------------------------------------------
#endif
