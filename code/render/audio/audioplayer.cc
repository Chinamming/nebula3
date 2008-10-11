//------------------------------------------------------------------------------
//  audioplayer.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "audio/audioplayer.h"
#include "audio/audioprotocol.h"
#include "audio/audiointerface.h"

namespace Audio
{
__ImplementClass(Audio::AudioPlayer, 'APLR', Core::RefCounted);
__ImplementSingleton(Audio::AudioPlayer);

using namespace CoreAudio;
using namespace Math;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
*/
AudioPlayer::AudioPlayer()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
AudioPlayer::~AudioPlayer()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
AudioPlayer::PlayCue(const CueId& cueId)
{
    Ptr<Audio::PlayCue> msg = Audio::PlayCue::Create();
    msg->SetCueId(cueId);
    msg->SetEnable3D(false);
    AudioInterface::Instance()->Send(msg.cast<Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
AudioPlayer::PlayCue3D(const CueId& cueId, const matrix44& transform, const vector& velocity)
{
    Ptr<Audio::PlayCue> msg = Audio::PlayCue::Create();
    msg->SetCueId(cueId);
    msg->SetEnable3D(true);
    msg->SetTransform(transform);
    msg->SetVelocity(velocity);
    AudioInterface::Instance()->Send(msg.cast<Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
AudioPlayer::StopCue(const CueId& cueId)
{
    Ptr<Audio::StopCueInstances> msg = Audio::StopCueInstances::Create();
    msg->SetCueId(cueId);
    AudioInterface::Instance()->Send(msg.cast<Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
AudioPlayer::PlayWave(const WaveId& waveId, SizeT loopCount, float volume, float pitch)
{
    Ptr<Audio::PlayWave> msg = Audio::PlayWave::Create();
    msg->SetWaveId(waveId);
    msg->SetStartPlaying(true);
    msg->SetLoopCount(loopCount);
    msg->SetVolume(volume);
    msg->SetPitch(pitch);
    msg->SetEnable3D(false);
    AudioInterface::Instance()->Send(msg.cast<Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
AudioPlayer::PlayWave3D(const WaveId& waveId, SizeT loopCount, float volume, float pitch, const matrix44& transform, const vector& velocity)
{
    Ptr<Audio::PlayWave> msg = Audio::PlayWave::Create();
    msg->SetWaveId(waveId);
    msg->SetStartPlaying(true);
    msg->SetLoopCount(loopCount);
    msg->SetVolume(volume);
    msg->SetPitch(pitch);
    msg->SetEnable3D(true);
    msg->SetTransform(transform);
    msg->SetVelocity(velocity);
    AudioInterface::Instance()->Send(msg.cast<Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
AudioPlayer::StopWave(const WaveId& waveId)
{
    Ptr<Audio::StopWaveInstances> msg = Audio::StopWaveInstances::Create();
    msg->SetWaveId(waveId);
    AudioInterface::Instance()->Send(msg.cast<Message>());
}

} // namespace Audio