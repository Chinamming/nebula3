//------------------------------------------------------------------------------
//  modelserver.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/modelserver.h"
#include "io/ioserver.h"
#include "io/stream.h"
#include "resources/resourcemanager.h"
#include "resources/simpleresourcemapper.h"
#include "models/load/streammodelloader.h"
#include "models/visresolver.h"

namespace Models
{
ImplementClass(Models::ModelServer, 'MDLS', Core::RefCounted);
ImplementSingleton(Models::ModelServer);

using namespace Core;
using namespace Util;
using namespace IO;
using namespace Attr;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
ModelServer::ModelServer() :
    isOpen(false)
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
ModelServer::~ModelServer()
{
    if (this->IsOpen())
    {
        this->Close();
    }
    DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
ModelServer::Open()
{
    n_assert(!this->IsOpen())
    this->isOpen = true;

    // create the visibility resolver singleton
    this->visResolver = VisResolver::Create();
    this->visResolver->Open();

    // setup a SimpleResourceMapper if no external mapper is set
    if (!this->modelResourceMapper.isvalid())
    {
        Ptr<SimpleResourceMapper> resMapper = SimpleResourceMapper::Create();
        resMapper->SetPlaceholderResourceId(ResourceId("mdl:system/placeholder.n2"));   // FIXME: n3!
        resMapper->SetResourceClass(Model::RTTI);
        resMapper->SetResourceLoaderClass(StreamModelLoader::RTTI);
        resMapper->SetManagedResourceClass(ManagedModel::RTTI);
        ResourceManager::Instance()->AttachMapper(resMapper.upcast<ResourceMapper>());
    }
    else
    {
        n_assert(this->modelResourceMapper->GetResourceType() == Model::RTTI);
        ResourceManager::Instance()->AttachMapper(this->modelResourceMapper);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
ModelServer::Close()
{
    n_assert(this->IsOpen());
 
    // detach our model resource mapper
    ResourceManager::Instance()->RemoveMapper(Model::RTTI);
    this->isOpen = false;

    // release the visibility resolver singleton
    this->visResolver->Close();
    this->visResolver = 0;
}

//------------------------------------------------------------------------------
/**
*/
bool
ModelServer::HasManagedModel(const ResourceId& resId) const
{
    return ResourceManager::Instance()->HasManagedResource(resId);
}

//------------------------------------------------------------------------------
/**
*/
Ptr<ManagedModel>
ModelServer::LoadManagedModel(const ResourceId& resId)
{
    return ResourceManager::Instance()->CreateManagedResource(Model::RTTI, resId).downcast<ManagedModel>();
}

//------------------------------------------------------------------------------
/**
*/
const Ptr<ManagedModel>&
ModelServer::LookupManagedModel(const ResourceId& resId) const
{
    n_assert(this->HasManagedModel(resId));
    return ResourceManager::Instance()->LookupManagedResource(resId).downcast<ManagedModel>();
}

//------------------------------------------------------------------------------
/**
*/
void
ModelServer::DiscardManagedModel(const Ptr<ManagedModel>& managedModel) const
{
    ResourceManager::Instance()->DiscardManagedResource(managedModel.upcast<ManagedResource>());
}

//------------------------------------------------------------------------------
/**
*/
bool
ModelServer::SaveModel(const Ptr<Model>& model, const URI& uri, const Rtti& saverClass)
{
    n_error("IMPLEMENT ME!");
    return false;
}

} // namespace Models