#pragma once
#ifndef MODELS_MODELSERVER_H
#define MODELS_MODELSERVER_H
//------------------------------------------------------------------------------
/**
    @class Models::ModelServer
  
    The ModelServer loads and creates shared Model objects.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "models/model.h"
#include "resources/resourceid.h"
#include "models/managedmodel.h"
#include "resources/resourcemapper.h"
#include "io/uri.h"

//------------------------------------------------------------------------------
namespace Models
{
class ModelServer : public Core::RefCounted
{
    __DeclareClass(ModelServer);
    __DeclareSingleton(ModelServer);
public:
    /// constructor
    ModelServer();
    /// destructor
    virtual ~ModelServer();

    /// set resource mapper for creating models
    void SetModelResourceMapper(const Ptr<Resources::ResourceMapper>& mapper);
    /// get resource mapper
    const Ptr<Resources::ResourceMapper>& GetModelResourceMapper() const;

    /// open the model server
    void Open();
    /// close the model server
    void Close();
    /// return true if model server is open
    bool IsOpen() const;

    /// check if a managed model exists
    bool HasManagedModel(const Resources::ResourceId& resId) const;
    /// load a managed Model from URI
    Ptr<ManagedModel> LoadManagedModel(const Resources::ResourceId& resId);
    /// lookup an existing model
    const Ptr<ManagedModel>& LookupManagedModel(const Resources::ResourceId& resId) const;
    /// discard a managed model
    void DiscardManagedModel(const Ptr<ManagedModel>& managedModel) const;

    /// save a Model to URI using the provided model saver
    bool SaveModel(const Ptr<Model>& model, const IO::URI& uri, const Core::Rtti& saverClass);
    
private:
    friend class Model;

    /// helper method to recursively clone model nodes
    void RecurseCloneModelNode(const Ptr<Model>& srcModel, const Ptr<Model>& dstModel, const Ptr<ModelNode>& srcNode);

    Ptr<VisResolver> visResolver;
    Ptr<Resources::ResourceMapper> modelResourceMapper;
    bool isOpen;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
ModelServer::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ModelServer::SetModelResourceMapper(const Ptr<Resources::ResourceMapper>& mapper)
{
    this->modelResourceMapper = mapper;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Resources::ResourceMapper>&
ModelServer::GetModelResourceMapper() const
{
    return this->modelResourceMapper;
}

} // namespace Models
//------------------------------------------------------------------------------
#endif
