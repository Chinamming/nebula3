//------------------------------------------------------------------------------
//  MACHINE GENERATED, DON'T EDIT!
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "audioprotocol.h"

namespace Audio
{
    __ImplementClass(Audio::SetupAudio, 'saud', Messaging::Message);
    ImplementMsgId(SetupAudio);
    __ImplementClass(Audio::GetAudioAdapterInfos, 'gaad', Messaging::Message);
    ImplementMsgId(GetAudioAdapterInfos);
    __ImplementClass(Audio::LoadSoundBank, 'lsbk', Messaging::Message);
    ImplementMsgId(LoadSoundBank);
    __ImplementClass(Audio::UnloadSoundBank, 'ulsk', Messaging::Message);
    ImplementMsgId(UnloadSoundBank);
    __ImplementClass(Audio::PlayCue, 'pcue', Messaging::Message);
    ImplementMsgId(PlayCue);
    __ImplementClass(Audio::UpdateCue, 'ucue', Messaging::Message);
    ImplementMsgId(UpdateCue);
    __ImplementClass(Audio::StopCue, 'scue', Messaging::Message);
    ImplementMsgId(StopCue);
    __ImplementClass(Audio::StopCueInstances, 'scui', Messaging::Message);
    ImplementMsgId(StopCueInstances);
    __ImplementClass(Audio::PlayWave, 'pwav', Messaging::Message);
    ImplementMsgId(PlayWave);
    __ImplementClass(Audio::StopWaveInstances, 'swav', Messaging::Message);
    ImplementMsgId(StopWaveInstances);
    __ImplementClass(Audio::UpdateListener, 'updl', Messaging::Message);
    ImplementMsgId(UpdateListener);
} // Audio

namespace Commands
{
} // namespace Commands
//------------------------------------------------------------------------------
