//------------------------------------------------------------------------------
//  uifeature/uifeatureunit.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "uifeature/uifeatureunit.h"
#include "basegametiming/systemtimesource.h"

namespace UIFeature
{
__ImplementClass(UIFeatureUnit, 'UIFU' , Game::FeatureUnit);
__ImplementSingleton(UIFeatureUnit);

//------------------------------------------------------------------------------
/**
*/
UIFeatureUnit::UIFeatureUnit()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
UIFeatureUnit::~UIFeatureUnit()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
UIFeatureUnit::OnActivate()
{
    FeatureUnit::OnActivate();
    
	this->uiServer = UI::UIServer::Create();
    this->uiServer->Open();
	
	this->uiRenderer = UI::UIRenderer::Create();
}

//------------------------------------------------------------------------------
/**
*/
void
UIFeatureUnit::OnDeactivate()
{
    this->uiServer->Close();
    this->uiServer = 0;

	this->uiRenderer = 0;

    FeatureUnit::OnDeactivate();    
}

//------------------------------------------------------------------------------
/**
*/
void
UIFeatureUnit::OnRenderDebug()
{
	this->uiServer->RenderDebug();

	FeatureUnit::OnRenderDebug();
}

//------------------------------------------------------------------------------
/**
*/
void
UIFeatureUnit::OnFrame()
{    
    // update time in physics system
    Timing::Time time = BaseGameFeature::SystemTimeSource::Instance()->GetTime();
	this->uiServer->SetTime(time);
	
	Timing::Time frameTime = BaseGameFeature::SystemTimeSource::Instance()->GetFrameTime();
	this->uiServer->SetFrameTime(frameTime);

    // trigger physics system, steps physics world etc.
    this->uiServer->Trigger();
	this->uiServer->Render();

	// now render the stuff	
	this->uiRenderer->OnFrame();
}

}; // namespace Game
