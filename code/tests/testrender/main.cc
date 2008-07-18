//------------------------------------------------------------------------------
//  testrender/main.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "foundation.h"
#include "render.h"
#include "core/coreserver.h"
#include "core/sysfunc.h"
#include "testbase/testrunner.h"
#include "loadmeshtest.h"
#include "loadtexturetest.h"
#include "modelloadsavetest.h"
#include "shadertest.h"
#include "simplemeshmappertest.h"
#include "simpletexturemappertest.h"
#include "graphicsservertest.h"
#include "n2modelloadertest.h"

using namespace Core;
using namespace Test;

void
__cdecl main()
{
    // create Nebula3 runtime
    Ptr<CoreServer> coreServer = CoreServer::Create();
    coreServer->SetAppName("Nebula3 Render Tests");
    coreServer->Open();

    n_printf("NEBULA3 RENDER TESTS\n");
    n_printf("====================\n");

    // setup and run test runner
    Ptr<TestRunner> testRunner = TestRunner::Create();
    testRunner->AttachTestCase(N2ModelLoaderTest::Create());
	testRunner->AttachTestCase(GraphicsServerTest::Create());
    testRunner->AttachTestCase(LoadMeshTest::Create());
    testRunner->AttachTestCase(LoadTextureTest::Create());
    testRunner->AttachTestCase(ModelLoadSaveTest::Create());
    testRunner->AttachTestCase(ShaderTest::Create());
    testRunner->AttachTestCase(SimpleMeshMapperTest::Create());
    testRunner->AttachTestCase(SimpleTextureMapperTest::Create());
    testRunner->Run(); 

    coreServer->Close();
    testRunner = 0;
    coreServer = 0;
    
    Core::SysFunc::Exit(0);
}
