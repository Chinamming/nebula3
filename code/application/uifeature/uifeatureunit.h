#ifndef UIFEATURE_UIFEATUREUNIT_H
#define UIFEATURE_UIFEATUREUNIT_H
//------------------------------------------------------------------------------
/**
    @class Game::UIFeatureUnit
    
    The UIFeatureUnit deliveres a Server and the needed Factory Stuff, to create
	and handle a complete UserInterface

	// TODO more desc

    (C) 2008 Radon Labs GmbH
*/
#include "game/featureunit.h"
#include "uifeature/uiserver.h"
#include "ui/uirenderer.h"

//------------------------------------------------------------------------------
namespace UIFeature
{

class UIFeatureUnit : public Game::FeatureUnit    
{
    __DeclareClass(UIFeatureUnit);
    __DeclareSingleton(UIFeatureUnit);   

public:
    /// constructor
    UIFeatureUnit();
    /// destructor
    virtual ~UIFeatureUnit();

    /// called from GameServer::ActivateProperties()
    virtual void OnActivate();
    /// called from GameServer::DeactivateProperties()
    virtual void OnDeactivate();
    
    /// called in the middle of the feature trigger cycle
    virtual void OnFrame();   

	/// called when game debug visualization is on
	virtual void OnRenderDebug();

protected:
    Ptr<UI::UIServer> uiServer;
	Ptr<UI::UIRenderer> uiRenderer;
};

}; // namespace PhysicsFeature
//------------------------------------------------------------------------------
#endif