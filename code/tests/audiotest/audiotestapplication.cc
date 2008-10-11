//------------------------------------------------------------------------------
//  audiotestapplication.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "audiotestapplication.h"
#include "audio/audiointerface.h"
#include "input/inputserver.h"
#include "input/keyboard.h"
#include "debugrender/debugrender.h"
#include "debugrender/debugshaperenderer.h"
#include "math/quaternion.h"

namespace Test
{
using namespace CoreAudio;
using namespace Audio;
using namespace Debug;
using namespace Http;
using namespace Resources;
using namespace Input;
using namespace Math;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
AudioTestApplication::AudioTestApplication()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
AudioTestApplication::~AudioTestApplication()
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
AudioTestApplication::Open()
{
    n_assert(!this->IsOpen());
    if (ViewerApplication::Open())
    {
        // setup the audio system
        this->audioInterface = AudioInterface::Create();
        this->audioInterface->Open();
        this->audioServer = AudioServer::Create();
        this->audioServer->Open();
        this->audioPlayer = AudioPlayer::Create();

        // first dump some information about the available audio adapters to debug out
        Array<AudioAdapterInfo> audioAdapters = this->audioServer->GetAdapterInfos();
        IndexT i;
        for (i = 0; i < audioAdapters.Size(); i++)
        {
            const AudioAdapterInfo& info = audioAdapters[i];
            n_printf("--- Audio Adapter:\n");
            n_printf("Adapter Id:      %s\n", info.GetAdapterId().AsCharPtr());
            n_printf("Display Name:    %s\n", info.GetDisplayName().AsCharPtr());
            n_printf("Default Adapter: %s\n", info.IsDefaultAdapter() ? "yes" : "no");
        }
    
        // load sound banks
        this->audioServer->LoadSoundBank(ResourceId("music"), true);
        this->audioServer->LoadSoundBank(ResourceId("effects"), false);

        // setup a few audio emitters
        Ptr<AudioEmitter> em0 = AudioEmitter::Create();
        em0->Setup();
        em0->SetTransform(matrix44::translation(10.0f, 0.0f, 0.0f));
        em0->Play(CueId("music1"));
        this->audioEmitters.Append(em0);

        Ptr<AudioEmitter> em1 = AudioEmitter::Create();
        em1->Setup();
        em1->SetTransform(matrix44::translation(-10.0f, 0.0f, 0.0f));
        em1->Play(CueId("music2"));
        this->audioEmitters.Append(em1);

        Ptr<AudioEmitter> em2 = AudioEmitter::Create();
        em2->Setup();
        em2->SetTransform(matrix44::translation(0.0f, 0.0f, 10.0f));
        em2->Play(CueId("music3"));
        this->audioEmitters.Append(em2);
        
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
AudioTestApplication::Close()
{
    n_assert(this->IsOpen());

    IndexT i;
    for (i = 0; i < this->audioEmitters.Size(); i++)
    {
        this->audioEmitters[i]->Discard();
    }
    this->audioEmitters.Clear();
    this->audioPlayer = 0;
    this->audioServer->Close();
    this->audioServer = 0;
    this->audioInterface->Close();
    this->audioInterface = 0;
    ViewerApplication::Close();
}

//------------------------------------------------------------------------------
/**
*/
void
AudioTestApplication::OnProcessInput()
{
    const Ptr<Keyboard>& kbd = InputServer::Instance()->GetDefaultKeyboard();
    if (kbd->KeyDown(Key::Key1))
    {
        this->audioPlayer->PlayCue(CueId("effect1"));
    }
    if (kbd->KeyDown(Key::Key2))
    {
        this->audioPlayer->PlayCue(CueId("effect2"));
    }
    if (kbd->KeyDown(Key::Key3))
    {
        this->audioPlayer->PlayCue(CueId("effect3"));
    }
    if (kbd->KeyDown(Key::Key4))
    {
        this->audioPlayer->PlayCue(CueId("effect4"));
    }
    if (kbd->KeyDown(Key::Key5))
    {
        this->audioPlayer->PlayCue(CueId("effect5"));
    }
    if (kbd->KeyDown(Key::Key6))
    {
        this->audioPlayer->PlayCue(CueId("music1"));
    }
    if (kbd->KeyDown(Key::Key7))
    {
        this->audioPlayer->PlayCue(CueId("music2"));
    }
    if (kbd->KeyDown(Key::Key8))
    {
        this->audioPlayer->PlayCue(CueId("music3"));
    }
    ViewerApplication::OnProcessInput();
}

//------------------------------------------------------------------------------
/**
*/
void
AudioTestApplication::OnUpdateFrame()
{
    // display help text
    _debug_text("Press keys [1..8] to play sound effects!", float2(0.0f, 0.0f), float4(1.0f, 1.0f, 0.0f, 1.0f));

    // update audio listener
    AudioListener::Instance()->SetTransform(this->mayaCameraUtil.GetCameraTransform());

    // call AudioServer
    AudioServer::Instance()->OnFrame();

    // render visualizations for the audio emitters
    IndexT i;
    for (i = 0; i < this->audioEmitters.Size(); i++)
    {
        DebugShapeRenderer::Instance()->DrawSphere(this->audioEmitters[i]->GetTransform(), float4(1.0f, 0.0f, 0.0f, 0.5f));
    }    

    /*
    const Array<Ptr<Cue>>& activeCues = this->audioDevice->GetActiveCues();
    point curPos(-2.0f, 0.0f, 0.0f);
    vector distVec(0.5f, 0.0f, 0.0f);
    IndexT i;
    for (i = 0; i < activeCues.Size(); i++)
    {
        const CueInfo& cueInfo = activeCues[i]->GetCueInfo();
        float4 color;
        if (cueInfo.GetCueId() == CueId("effect1"))
        {
            color.set(1.0f, 0.0f, 0.0f, 0.5f);
        }
        else if (cueInfo.GetCueId() == CueId("effect2"))
        {
            color.set(1.0f, 1.0f, 0.0f, 0.5f);
        }
        else if (cueInfo.GetCueId() == CueId("effect3"))
        {
            color.set(0.0f, 1.0f, 0.0f, 0.5f);
        }
        else if (cueInfo.GetCueId() == CueId("effect4"))
        {
            color.set(0.0f, 1.0f, 1.0f, 0.5f);
        }
        else if (cueInfo.GetCueId() == CueId("effect5"))
        {
            color.set(0.0f, 0.0f, 1.0f, 0.5f);
        }
        else if (cueInfo.GetCueId() == CueId("music1"))
        {
            color.set(1.0f, 0.5f, 0.0f, 0.5f);
        }
        else if (cueInfo.GetCueId() == CueId("music2"))
        {
            color.set(1.0f, 0.0f, 1.0f, 0.5f);
        }
        else if (cueInfo.GetCueId() == CueId("music3"))
        {
            color.set(0.5f, 1.0f, 0.0f, 0.5f);
        }

        // render a debug shape for each active cue
        matrix44 m = matrix44::affinetransformation(0.5f, vector::nullvec(), quaternion::identity(), curPos);
        DebugShapeRenderer::Instance()->DrawBox(m, color);
        curPos += distVec;
    }
    */

    // call parent class
    ViewerApplication::OnUpdateFrame();
}

} // namespace Test
