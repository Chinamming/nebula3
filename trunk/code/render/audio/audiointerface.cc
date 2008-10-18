//------------------------------------------------------------------------------
//  audiointerface.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "audio/audiointerface.h"
#include "audio/audiohandler.h"

namespace Audio
{
__ImplementClass(Audio::AudioInterface, 'AUDI', Messaging::AsyncPort);
__ImplementInterfaceSingleton(Audio::AudioInterface);

//------------------------------------------------------------------------------
/**
*/
AudioInterface::AudioInterface()
{
    __ConstructSingleton;
    this->SetThreadCpuCoreId(System::Cpu::AudioThreadCore);
}    

//------------------------------------------------------------------------------
/**
*/
AudioInterface::~AudioInterface()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
AudioInterface::Open()
{
    // the audio device needs to be triggered from time to time, but we
    // also need to react immediately to messages
    this->SetBehaviour(WaitForMessageOrTimeOut);
    this->SetWaitTimeout(int(AudioHandler::AudioTriggerInterval * 1000.0));
    this->SetName("AudioInterface Thread");
    AsyncPort::Open();
}

//------------------------------------------------------------------------------
/**
*/
void
AudioInterface::OnCreateHandlers()
{
    Ptr<AudioHandler> audioHandler = AudioHandler::Create();
    this->AttachHandler(audioHandler.cast<Messaging::Handler>());
}

} // namespace Audio


