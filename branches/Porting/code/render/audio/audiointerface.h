#pragma once
#ifndef AUDIO_AUDIOINTERFACE_H
#define AUDIO_AUDIOINTERFACE_H
//------------------------------------------------------------------------------
/**
    @class Audio::AudioInterface
  
    Implements the interface to the CoreAudio threaded subsystem. Apps
    don't call directly into AudioInterface, instead use the provided 
    front-end classes in the Audio subsystem.
    
    (C) 2008 Radon Labs GmbH
*/    
#include "messaging/asyncport.h"
#include "core/singleton.h"

//------------------------------------------------------------------------------
namespace Audio
{
class AudioInterface : public Messaging::AsyncPort
{
    __DeclareClass(AudioInterface);
    __DeclareInterfaceSingleton(AudioInterface);
public:
    /// constructor
    AudioInterface();
    /// destructor
    virtual ~AudioInterface();
    /// open the interface object
    virtual void Open();

protected:
    /// derive in subclass, create and attach handlers from here
    virtual void OnCreateHandlers();
};

} // namespace Audio
//------------------------------------------------------------------------------
#endif
