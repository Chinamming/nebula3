//------------------------------------------------------------------------------
//  n2modelloadertest.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "n2modelloadertest.h"
#include "models/modelserver.h"
#include "timing/timer.h"
#include "coregraphics/shaderserver.h"

namespace Test
{
ImplementClass(Test::N2ModelLoaderTest, 'N2MT', Test::CoreGraphicsTest);

using namespace IO;
using namespace Util;
using namespace Resources;
using namespace Models;
using namespace Timing;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
void
N2ModelLoaderTest::Run()
{
    if (this->SetupRuntime())
    {
        ResourceManager* resManager = ResourceManager::Instance();
        IoServer* ioServer = IoServer::Instance();

        // needed to backward compatibility
        ioServer->SetAssign(Assign("meshes", "msh:"));
        ioServer->SetAssign(Assign("textures", "tex:"));
        ShaderServer::Instance()->SetShaderParamBindMode(ShaderServer::BindByName);

        Ptr<ModelServer> modelServer = ModelServer::Create();
        modelServer->Open();
        
        // asynchronously load a legacy Nebula2 object file
        Ptr<ManagedModel> managedModel = modelServer->LoadManagedModel(ResourceId("mdl:tiger.n2"));

        // idle until everything is loaded (or timeout is reached)        
        Timer timer;
        timer.Start();
        const Time timeOut = 5.0;
        const Time startTime = timer.GetTime();
        bool timedOut = false;
        bool allLoaded = false;
        do
        {
            // trigger the resource manager (this is usually done per-frame)
            Timing::Sleep(0.01);
            resManager->Prepare();
            resManager->Update();
            allLoaded = (managedModel->GetState() == Resource::Loaded);
            timedOut = timer.GetTime() > (startTime + timeOut);
        }
        while (!(allLoaded || timedOut));
        this->Verify(allLoaded);

        if (!timedOut)
        {
            managedModel->GetModel()->LoadResources();

            // wait until model resource are loaded
            timedOut = false;
            allLoaded = false;
            do
            {
                Timing::Sleep(0.01);
                resManager->Prepare();
                resManager->Update();
                allLoaded = (managedModel->GetModel()->GetResourceState() == Resource::Loaded);
                timedOut = timer.GetTime() > (startTime + timeOut);            
            }    
            while (!(allLoaded || timedOut));
            this->Verify(allLoaded);
        }

        // discard everything
        modelServer->DiscardManagedModel(managedModel);
        modelServer->Close();
        modelServer = 0;
        this->ShutdownRuntime();
    }
}

} // namespace Test