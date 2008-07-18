#pragma once
#ifndef LEGACY_NAX2STREAMREADER_H
#define LEGACY_NAX2STREAMREADER_H
//------------------------------------------------------------------------------
/**
    @class Legacy::Nax2StreamReader
  
    A stream reader which reads legacy nvx2 binary mesh files.

    NOTE: this class exists purely for debugging and shouldn't be used in
    production code!

    (C) 2007 Radon Labs GmbH
*/    
#include "core/config.h"
#if NEBULA3_LEGACY_SUPPORT
#include "io/streamreader.h"
#include "nebula2/nebula2wrapper.h"

//------------------------------------------------------------------------------
namespace Legacy
{
class Nax2StreamReader : public IO::StreamReader
{
    DeclareClass(Nax2StreamReader);
public:
    /// contructor
    Nax2StreamReader();
    /// destructor
    virtual ~Nax2StreamReader();
    /// begin reading from the stream, read entire data
    virtual bool Open();
    /// end reading from the stream, destroys loaded objects
    virtual void Close();
    /// set animation to write data into
    void SetAnimation(const Ptr<Anim::Animation>& anim);
    
private:
    /// read animation data from stream
    bool ReadAnimationData();

    Ptr<Anim::Animation> animation;
    Util::FixedArray<Math::float4> keyArray;
};

//------------------------------------------------------------------------------
/**
*/
inline
void 
Nax2StreamReader::SetAnimation(const Ptr<Anim::Animation>& anim)
{
    this->animation = anim;
}

} // namespace Legacy
//------------------------------------------------------------------------------
#endif // LEGACY_Nax2StreamReader_H
#endif // NEBULA3_LEGACY_SUPPORT