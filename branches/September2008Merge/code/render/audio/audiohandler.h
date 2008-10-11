#pragma once
#ifndef AUDIO_AUDIOHANDLER_H
#define AUDIO_AUDIOHANDLER_H
//------------------------------------------------------------------------------
/**
    @class Audio::AudioHandler
    
    Message handler for the Audio subsystem. Runs in the Audio thread.
    
    (C) 2008 Radon Labs GmbH
*/
#include "messaging/handler.h"
#include "messaging/message.h"
#include "audio/audioprotocol.h"
#include "io/console.h"
#include "io/ioserver.h"
#include "http/httpserverproxy.h"
#include "timing/timer.h"

//------------------------------------------------------------------------------
namespace Audio
{
class AudioHandler : public Messaging::Handler
{
    __DeclareClass(AudioHandler);
public:
    /// the audio trigger interval in seconds
    static const Timing::Time AudioTriggerInterval;

    /// constructor
    AudioHandler();
    /// destructor
    virtual ~AudioHandler();

    /// open the handler
    virtual void Open();
    /// close the handler
    virtual void Close();
    /// handle a message, return true if handled
    virtual bool HandleMessage(const Ptr<Messaging::Message>& msg);
    /// do per-frame work (trigger the audio device)
    virtual void DoWork();

private:
    /// setup the audio runtime
    void SetupAudioRuntime(IndexT adapterIndex, const IO::URI& globalSettingsURI);
    /// shutdown the audio runtime
    void ShutdownAudioRuntime();
    /// process SetupAudio message
    void OnSetupAudio(const Ptr<Audio::SetupAudio>& msg);
    /// process GetAudioAdapterInfos message
    void OnGetAudioAdapterInfos(const Ptr<Audio::GetAudioAdapterInfos>& msg);
    /// process LoadSoundBank message
    void OnLoadSoundBank(const Ptr<Audio::LoadSoundBank>& msg);
    /// process UnloadSoundBank message
    void OnUnloadSoundBank(const Ptr<Audio::UnloadSoundBank>& msg);
    /// process PlayCue message
    void OnPlayCue(const Ptr<Audio::PlayCue>& msg);
    /// process UpdateCue message
    void OnUpdateCue(const Ptr<Audio::UpdateCue>& msg);
    /// process StopCue message
    void OnStopCue(const Ptr<Audio::StopCue>& msg);
    /// process StopCueInstances message
    void OnStopCueInstances(const Ptr<Audio::StopCueInstances>& msg);
    /// process PlayWave message
    void OnPlayWave(const Ptr<Audio::PlayWave>& msg);
    /// process StopWave message
    void OnStopWaveInstances(const Ptr<Audio::StopWaveInstances>& msg);
    /// process UpdateListener message
    void OnUpdateListener(const Ptr<Audio::UpdateListener>& msg);

    bool isAudioRuntimeValid;
    Ptr<IO::Console> ioConsole;
    Ptr<IO::IoServer> ioServer;
    Ptr<CoreAudio::AudioDevice> audioDevice;
    Ptr<Http::HttpServerProxy> httpServerProxy;
    Timing::Timer timer;
    Timing::Time time;
};

} // namespace Audio
//------------------------------------------------------------------------------
#endif
