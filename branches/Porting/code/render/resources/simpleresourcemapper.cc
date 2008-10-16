//------------------------------------------------------------------------------
//  simpleresourcemapper.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "resources/simpleresourcemapper.h"
#include "resources/sharedresourceserver.h"

namespace Resources
{
__ImplementClass(Resources::SimpleResourceMapper, 'SRSM', Resources::ResourceMapper);

using namespace Core;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
SimpleResourceMapper::SimpleResourceMapper() :
    resourceClass(0),
    resLoaderClass(0),
    managedResourceClass(0),
    placeholderResource(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
SimpleResourceMapper::~SimpleResourceMapper()
{
    n_assert(!this->IsAttachedToResourceManager());
}

//------------------------------------------------------------------------------
/**
*/
const Rtti&
SimpleResourceMapper::GetResourceType() const
{
    n_assert2(0 != this->resourceClass, "SimpleResourceMapper::SetResourceType() method not called!");
    return *(this->resourceClass);
}

//------------------------------------------------------------------------------
/**
*/
void
SimpleResourceMapper::OnAttachToResourceManager()
{
    n_assert2(0 != this->resourceClass, "SimpleResourceMapper::SetResourceClass() not called!");
    n_assert2(0 != this->managedResourceClass, "SimpleResourceMapper::SetManagedResourceClass() not called!");
    n_assert2(0 != this->resLoaderClass, "SimpleResourceMapper::SetResourceLoaderClass() not called!");
    n_assert2(this->placeholderResourceId.IsValid(), "SimpleResourceMapper::SetPlaceholderResourceId() not called!"); 

    // call parent class
    ResourceMapper::OnAttachToResourceManager();
    n_assert(this->managedResources.IsEmpty());
    n_assert(this->pendingResources.IsEmpty());

    // load placeholder resource
    this->placeholderResource = (Resource*) this->resourceClass->Create();
    this->placeholderResource->SetResourceId(this->placeholderResourceId);
    this->placeholderResource->SetLoader((ResourceLoader*)this->resLoaderClass->Create());
    this->placeholderResource->SetAsyncEnabled(false);
    this->placeholderResource->Load();
    if (!this->placeholderResource->IsLoaded())
    {
        n_error("SimpleResourceMapper::OnAttachToServer(): could not not load placeholder resource '%s' of class '%s'!",
            this->placeholderResourceId.Value().AsCharPtr(), this->resourceClass->GetName().AsCharPtr());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
SimpleResourceMapper::OnRemoveFromResourceManager()
{
    n_assert(this->IsAttachedToResourceManager());

    // discard all remaining resources
    while (this->managedResources.Size() > 0)
    {
        this->OnDiscardManagedResource(this->managedResources.ValueAtIndex(this->managedResources.Size() - 1));
    }
    n_assert(this->pendingResources.IsEmpty());

    // unload the placeholder resource
    this->placeholderResource->Unload();
    this->placeholderResource->SetLoader(0);
    this->placeholderResource = 0;

    // finally call parent class
    ResourceMapper::OnRemoveFromResourceManager();
}

//------------------------------------------------------------------------------
/**
    This method is called by the ResourceManager when a new
    ManagedResource must be created. The resource manager will only call this
    method if this is the first request for the resource name.
*/
Ptr<ManagedResource>
SimpleResourceMapper::OnCreateManagedResource(const Rtti& resType, const ResourceId& resId)
{
    n_assert(0 != this->managedResourceClass);

    // ResourceManager should call this method only when 
    // a managed resource with the same resource id doesn't exist yet:
    n_assert(!this->managedResources.Contains(resId));

    // for the same reason the resource shouldn't be pending already:
    n_assert(!this->pendingResources.Contains(resId));

    // create a new managed resource, and set the placeholder resource on it
    Ptr<ManagedResource> managedResource = (ManagedResource*) this->managedResourceClass->Create();
    managedResource->IncrClientCount();
    managedResource->SetResourceId(resId);
    managedResource->SetResourceType(&this->GetResourceType());
    managedResource->SetPlaceholder(this->placeholderResource);
    this->managedResources.Add(resId, managedResource);

    // check if the resource already exists as shared resource
    SharedResourceServer* sharedServ = SharedResourceServer::Instance();
    Ptr<Resource> resource = sharedServ->CreateSharedResource(resId, *(this->resourceClass), *(this->resLoaderClass));
    if (resource->IsLoaded())
    {
        // the resource is already loaded... (must have been somebody else...)
        managedResource->SetResource(resource);
    }
    else if (resource->IsPending())
    {
        // the resource is already pending... (must have been somebody else...)
        this->pendingResources.Add(resId, resource);
    }
    else
    {
        // initiate an synchrnouse or asynchronous load (depending on setting)
        resource->SetAsyncEnabled(this->asyncEnabled);
        resource->Load();
        if (resource->IsLoaded() || resource->LoadFailed())
        {
            // synchronous loading succeeded (or failed)
            managedResource->SetResource(resource); 
        }
        else if (resource->IsPending())
        {
            // asynchronous load initiayed and pending
            this->pendingResources.Add(resId, resource);
        }
    }
    return managedResource;
}

//------------------------------------------------------------------------------
/**
    This method will be called by the ResourceManager whenever a ManagedResource
    should be discarded. 
*/
void
SimpleResourceMapper::OnDiscardManagedResource(const Ptr<ManagedResource>& managedResource)
{
    n_assert(managedResource->GetClientCount() > 0);
    n_assert(this->managedResources.Contains(managedResource->GetResourceId()));
    n_assert(managedResource->GetResourceType() == &this->GetResourceType());
    n_assert(managedResource->IsInstanceOf(*(this->managedResourceClass)));

    managedResource->DecrClientCount();
    if (managedResource->GetClientCount() == 0)
    {
        // unregister the shared resource
        SharedResourceServer::Instance()->UnregisterSharedResource(managedResource->GetResourceId());
        
        // drop pending resource
        if (this->pendingResources.Contains(managedResource->GetResourceId()))
        {
            this->pendingResources.Erase(managedResource->GetResourceId());
        }

        // clear the contained resource
        managedResource->Clear();

        // finally drop the managed resource itself, this must be the last
        // call in this method, since the managedResource pointer
        // may become invalid at this point
        this->managedResources.Erase(managedResource->GetResourceId());
    }
}

//------------------------------------------------------------------------------
/**
    This method will go through all ManagedResources and reset their
    render statistics. It will also check whether pending resources
    have finished loading, and update the associated managed resources 
    accordingly.
*/
void
SimpleResourceMapper::OnPrepare()
{
    // first reset render statistics
    IndexT resIndex;
    for (resIndex = 0; resIndex < this->managedResources.Size(); resIndex++)
    {
        this->managedResources.ValueAtIndex(resIndex)->ClearRenderStats();
    }

    // now check pending resources...
    IndexT pendingIndex;
    for (pendingIndex = 0; pendingIndex < this->pendingResources.Size();)
    {
        Ptr<Resource>& resource = this->pendingResources.ValueAtIndex(pendingIndex);
        n_assert(resource->IsPending());

        // try load...
        resource->Load();
        if (resource->IsLoaded() || resource->LoadFailed())
        {
            // resource has finished loading, or failed to load
            this->managedResources[resource->GetResourceId()]->SetResource(resource);            
            this->pendingResources.EraseAtIndex(pendingIndex);
        }
        else
        {
            // still pending...
            n_assert(resource->IsPending());
            pendingIndex++;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
SimpleResourceMapper::OnUpdate()
{
    // @todo: do we need to do anything here?
}

//------------------------------------------------------------------------------
/**
*/
SizeT
SimpleResourceMapper::GetNumPendingResources() const
{
    return this->pendingResources.Size();
}

} // namespace Resources

