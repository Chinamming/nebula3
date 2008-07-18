//------------------------------------------------------------------------------
//  graphicsservertest.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphicsservertest.h"
#include "graphics/graphicsserver.h"
#include "graphics/stage.h"
#include "graphics/view.h"
#include "graphics/simplestagebuilder.h"
#include "graphics/cameraentity.h"
#include "graphics/modelentity.h"
#include "lighting/spotlightentity.h"

namespace Test
{
ImplementClass(Test::GraphicsServerTest, 'GSTT', Test::CoreGraphicsTest);

using namespace CoreGraphics;
using namespace Util;
using namespace Graphics;
using namespace Lighting;
using namespace Math;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
void
GraphicsServerTest::Run()
{
    if (this->SetupRuntime())
    {
        // create a graphics server
        Ptr<GraphicsServer> graphicsServer = GraphicsServer::Create();
        graphicsServer->Open();
        this->Verify(graphicsServer->IsOpen());

        // create and setup a stage
        StringAtom stageName("TestStage");
        Ptr<StageBuilder> stageBuilder = SimpleStageBuilder::Create();
        Ptr<Stage> stage = graphicsServer->CreateStage(StringAtom("TestStage"), stageBuilder);
        this->Verify(stage->GetName() == stageName);
        this->Verify(stage->GetStageBuilder() == stageBuilder);
        this->Verify(stage->GetRootCell().isvalid());

        // add some entities to the stage
        Ptr<CameraEntity> cameraEntity = CameraEntity::Create();
        Ptr<SpotLightEntity> lightEntity = SpotLightEntity::Create();
        Ptr<ModelEntity> modelEntity = ModelEntity::Create(); 
        this->Verify(cameraEntity->GetType() == GraphicsEntity::CameraType);
        this->Verify(lightEntity->GetType() == GraphicsEntity::LightType);
        this->Verify(modelEntity->GetType() == GraphicsEntity::ModelType);
        this->Verify(cameraEntity->IsActive());
        this->Verify(lightEntity->IsActive());
        this->Verify(modelEntity->IsActive());
        this->Verify(!cameraEntity->IsAttachedToStage());
        this->Verify(!lightEntity->IsAttachedToStage());
        this->Verify(!modelEntity->IsAttachedToStage());

        // FIXME: SETUP ENTITIES
        cameraEntity->SetTransform(matrix44::translation(0.0f, 0.0f, -5.0f));
        cameraEntity->SetupPerspectiveFov(n_deg2rad(60.0f), 4.0 / 3.0, 0.01f, 1000.0f);

        modelEntity->SetTransform(matrix44::translation(0.0f, 0.0f, 1.0f));
        modelEntity->SetResourceId(ResourceId("models:examples/tiger.n2"));
        
        stage->AttachEntity(cameraEntity.upcast<GraphicsEntity>());
        stage->AttachEntity(lightEntity.upcast<GraphicsEntity>());
        stage->AttachEntity(modelEntity.upcast<GraphicsEntity>());
        this->Verify(cameraEntity->IsAttachedToStage());
        this->Verify(lightEntity->IsAttachedToStage());
        this->Verify(modelEntity->IsAttachedToStage());
        this->Verify(cameraEntity->GetStage() == stage);
        this->Verify(lightEntity->GetStage() == stage);
        this->Verify(modelEntity->GetStage() == stage);
        this->Verify(cameraEntity->IsAttachedToCell());
        this->Verify(lightEntity->IsAttachedToCell());
        this->Verify(modelEntity->IsAttachedToCell());
        this->Verify(cameraEntity->GetCell() == stage->GetRootCell());
        this->Verify(lightEntity->GetCell() == stage->GetRootCell());
        this->Verify(modelEntity->GetCell() == stage->GetRootCell());

        // create and setup a view
        StringAtom viewName("TestView");
        Ptr<View> view = graphicsServer->CreateView(View::RTTI, viewName);
        view->SetStage(stage);
        view->SetCameraEntity(cameraEntity);
        this->Verify(view->IsAttachedToServer());
        this->Verify(view->GetName() == viewName);
        this->Verify(view->GetStage() == stage);
        this->Verify(view->GetCameraEntity() == cameraEntity);


        // FIXME: perform rendering...

        // cleanup and shutdown
        graphicsServer->Close();
        this->Verify(!graphicsServer->IsOpen());
        graphicsServer = 0;

        this->ShutdownRuntime();
    }
}

}