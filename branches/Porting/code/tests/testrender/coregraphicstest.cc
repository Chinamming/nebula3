//------------------------------------------------------------------------------
//  coregraphicstest.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphicstest.h"
#include "io/filestream.h"
#include "resources/simpleresourcemapper.h"
#include "coregraphics/streamtextureloader.h"
#include "coregraphics/streammeshloader.h"
#include "resources/managedtexture.h"
#include "resources/managedmesh.h"

namespace Test
{
ImplementClass(Test::CoreGraphicsTest, 'cgtt', Test::TestCase);

using namespace Util;
using namespace CoreGraphics;
using namespace IO;
using namespace Interface;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
bool
CoreGraphicsTest::SetupRuntime()
{
    // setup foundation layer runtime
    this->ioServer = IoServer::Create();
    this->ioServer->SetAssign(Assign("tex", "home:work/testdata"));
    this->ioServer->SetAssign(Assign("msh", "home:work/testdata"));
    this->ioServer->SetAssign(Assign("shd", "home:work/testdata/shaders"));
    this->ioServer->SetAssign(Assign("mdl", "home:work/testdata"));
    this->ioServer->RegisterUriScheme("file", FileStream::RTTI);
    this->ioInterface = IOInterface::Create();
    this->ioInterface->Open();

    // setup the display device
    this->displayDevice = DisplayDevice::Create();
    this->displayDevice->SetFullscreen(false);
    this->displayDevice->SetDisplayMode(DisplayMode(0, 0, 64, 64));
    this->displayDevice->SetWindowTitle("Nebula3 Test Window");
    if (this->displayDevice->Open())
    {
        this->renderDevice = RenderDevice::Create();
        if (!this->renderDevice->Open())
        {
            this->displayDevice->Close();
            this->renderDevice = 0;
            this->displayDevice = 0;
            return false;
        }
    }

    // setup the shader server
    this->shaderServer = ShaderServer::Create();
    if (!this->shaderServer->Open())
    {
        return false;
    }

    // setup the resource server
    this->resServer = SharedResourceServer::Create();
    if (!this->resServer->Open())
    {
        return false;
    }

    // setup the resource manager
    this->resManager = ResourceManager::Create();
    this->resManager->Open();

    Ptr<SimpleResourceMapper> texMapper = SimpleResourceMapper::Create();
    texMapper->SetPlaceholderResourceId(ResourceId("tex:placeholder.dds"));
    texMapper->SetResourceClass(Texture::RTTI);
    texMapper->SetResourceLoaderClass(StreamTextureLoader::RTTI);
    texMapper->SetManagedResourceClass(ManagedTexture::RTTI);
    this->resManager->AttachMapper(texMapper.upcast<ResourceMapper>());

    Ptr<SimpleResourceMapper> meshMapper = SimpleResourceMapper::Create();
    meshMapper->SetPlaceholderResourceId(ResourceId("msh:placeholder.nvx2"));
    meshMapper->SetResourceClass(Mesh::RTTI);
    meshMapper->SetResourceLoaderClass(StreamMeshLoader::RTTI);
    meshMapper->SetManagedResourceClass(ManagedMesh::RTTI);
    this->resManager->AttachMapper(meshMapper.upcast<ResourceMapper>());

    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
CoreGraphicsTest::ShutdownRuntime()
{
    this->shaderServer = 0;
    this->resManager = 0;
    this->resServer = 0;
    this->renderDevice = 0;
    this->displayDevice = 0;
    this->ioInterface = 0;
    this->ioServer = 0;
}

}