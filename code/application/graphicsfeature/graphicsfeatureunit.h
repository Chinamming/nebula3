#ifndef GRAPHICSFEATURE_GRAPHICSFEATUREUNIT_H
#define GRAPHICSFEATURE_GRAPHICSFEATUREUNIT_H
//------------------------------------------------------------------------------
/**
    @class GraphicsFeature::GraphicsFeatureUnitUnit
    
    The GraphicsFeatures provides everything for rendering graphicentities
    from different views in different stages.

    For the default use it creates one default view and one default stage which are
    used in the graphic and camera properties.    
    The BaseGameFeatureUnit uses this features to build up a default graphic world.

    The GraphicsFeatureUnit also offers visualization of debug shapes. Just
    attach your feature which should be considered in debug rendering.
    It also allows switching thru all debug renderings of each feature.

    Additonal to the rendering the graphicsfeature creates and triggers 
    the input server.

    (C) 2007 Radon Labs GmbH
*/
#include "game/featureunit.h"
#include "coregraphics/renderdevice.h"
#include "coregraphics/displaydevice.h"
#include "coregraphics/transformdevice.h"
#include "coregraphics/shaderserver.h"
#include "coregraphics/shaperenderer.h"
#include "coregraphics/vertexlayoutserver.h"
#include "resources/sharedresourceserver.h"
#include "resources/resourcemanager.h"
#include "models/modelserver.h"
#include "graphics/graphicsserver.h"
#include "graphics/stage.h"
#include "graphics/view.h"
#include "lighting/abstractlightentity.h"
#include "lighting/lightserver.h"
#include "lighting/shadowserver.h"
#include "frame/frameserver.h"
#include "input/inputserver.h"
#include "addons/nebula2/nebula2wrapper.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{

class GraphicsFeatureUnit : public Game::FeatureUnit    
{
    DeclareClass(GraphicsFeatureUnit);
    DeclareSingleton(GraphicsFeatureUnit);   

public:

    /// constructor
    GraphicsFeatureUnit();
    /// destructor
    virtual ~GraphicsFeatureUnit();

    /// called from GameServer::ActivateProperties()
    virtual void OnActivate();
    /// called from GameServer::DeactivateProperties()
    virtual void OnDeactivate();
           
    /// called from within GameServer::OnStart() after OnLoad when the complete world exist
    virtual void OnStart();  
    
    /// called on begin of frame
    virtual void OnBeginFrame();
    /// called in the middle of the feature trigger cycle
    virtual void OnFrame();        
    /// called at the end of the feature trigger cycle
    virtual void OnEndFrame();

    /// called when game debug visualization is on
    virtual void OnRenderDebug();

    /// attach new view to feature
    void AttachView(const Ptr<Graphics::View>& view);
    /// remove view to feature
    void RemoveView(const Ptr<Graphics::View>& view);
    
    /// attach new satge to feature
    void AttachStage(const Ptr<Graphics::Stage>& stage);
    /// remove stage to feature
    void RemoveStage(const Ptr<Graphics::Stage>& stage);

    /// create default stage and view
    void CreateDefaultGraphicsWorld();  

    /// cleanup default graphics world
    void CleanupDefaultGraphicsWorld();

    /// create a default light
    void CreateDefaultLight();

    /// get default stage
    const Ptr<Graphics::Stage>& GetDefaultStage() const;
    /// get default stage
    const Ptr<Graphics::View>& GetDefaultView() const;

    /// add render debug callback
    void AddRenderDebugCallback(const Ptr<Game::FeatureUnit>& feature, const Util::String& debugModeName);
    /// remove render debug callback
    void RemoveRenderDebugCallback(const Util::String& debugModeName);
    /// set debug mode enabeled or disabled
    void SetDebugModeEnabled(const Util::String& name, bool b);
    /// set next debug mode enabled
    void SetNextDebugMode();
    /// toggle renderdebug
    void ToggleRenderDebug();

protected:
    /// called to configure display device
    virtual void OnConfigureDisplayDevice();
    /// render current frame
    virtual void OnRenderFrame();
    /// render debug shapes of all features
    void RenderDebug();
    /// handle debug input keys
    void HandleDebugInputKeys();

    Util::Array<Ptr<Graphics::Stage> > activeStages;
    Util::Array<Ptr<Graphics::View> > activeViews;

    Ptr<Graphics::CameraEntity> defaultCamera;
    Ptr<Lighting::AbstractLightEntity> defaultLight;

    Ptr<CoreGraphics::RenderDevice> renderDevice;
    Ptr<CoreGraphics::DisplayDevice> displayDevice;
    Ptr<CoreGraphics::TransformDevice> transformDevice;
    Ptr<CoreGraphics::ShaderServer> shaderServer;
    Ptr<CoreGraphics::ShapeRenderer> shapeRenderer;
    Ptr<CoreGraphics::VertexLayoutServer> vertexLayoutServer;
    Ptr<Resources::SharedResourceServer> sharedResourceServer;
    Ptr<Resources::ResourceManager> resourceManager;
    Ptr<Models::ModelServer> modelServer;
    Ptr<Graphics::GraphicsServer> graphicsServer;
    Ptr<Lighting::LightServer> lightServer;
    Ptr<Lighting::ShadowServer> shadowServer;
    Ptr<Frame::FrameServer> frameServer; 
    Ptr<Input::InputServer> inputServer;
    Ptr<Nebula2::nAnimationServer> animationServer;    
    Ptr<Nebula2::CharacterServer> characterServer;
    Ptr<Nebula2::AnimTable> animTable;

    struct DebugEntry
    {
        Ptr<Game::FeatureUnit> feature;
        bool active;
    };
    bool renderActiveDebugModes;
    Util::Dictionary<Util::String, DebugEntry> debugCallbacks;
    uint activeDebugModeIndex;
};

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Graphics::Stage>&
GraphicsFeatureUnit::GetDefaultStage() const
{
    n_assert(Graphics::GraphicsServer::Instance()->HasStage(Util::StringAtom("DefaultStage")));
    return Graphics::GraphicsServer::Instance()->GetStageByName(Util::StringAtom("DefaultStage"));
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Graphics::View>&
GraphicsFeatureUnit::GetDefaultView() const
{
    n_assert(Graphics::GraphicsServer::Instance()->HasView(Util::StringAtom("DefaultView")));
    return Graphics::GraphicsServer::Instance()->GetViewByName(Util::StringAtom("DefaultView"));

}

//------------------------------------------------------------------------------
/**
*/
inline void 
GraphicsFeatureUnit::AddRenderDebugCallback(const Ptr<Game::FeatureUnit>& feature, const Util::String& debugModeName)
{
    DebugEntry newEntry;
    newEntry.feature = feature;
    newEntry.active = true;
    debugCallbacks.Add(debugModeName, newEntry);
}

//------------------------------------------------------------------------------
/**
*/
inline void 
GraphicsFeatureUnit::RemoveRenderDebugCallback(const Util::String& debugModeName)
{
    if (debugCallbacks.Contains(debugModeName))
    {
        debugCallbacks.Erase(debugModeName);
    }
}

//------------------------------------------------------------------------------
/**
*/
inline void 
GraphicsFeatureUnit::SetDebugModeEnabled(const Util::String& name, bool b)
{
    n_assert(this->debugCallbacks.Contains(name));
    this->debugCallbacks[name].active = b;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
GraphicsFeatureUnit::SetNextDebugMode()
{
    // first clear all debug Modeflags
    for (uint i = 0; i < this->debugCallbacks.Size(); i++)
    {
        this->debugCallbacks.ValueAtIndex(i).active = false;
    }

    this->activeDebugModeIndex++;
    if (activeDebugModeIndex > this->debugCallbacks.Size() - 1)
    {
        activeDebugModeIndex = 0;
    }
    this->debugCallbacks.ValueAtIndex(this->activeDebugModeIndex).active = true;    
}

//------------------------------------------------------------------------------
/**
*/
inline void 
GraphicsFeatureUnit::ToggleRenderDebug()
{
    this->renderActiveDebugModes = !this->renderActiveDebugModes;
}

}; // namespace GraphicsFeatureUnit
//------------------------------------------------------------------------------
#endif