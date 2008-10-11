#pragma once
#ifndef MODELS_MANAGEDMODEL_H
#define MODELS_MANAGEDMODEL_H
//------------------------------------------------------------------------------
/**
    @class Models::ManagedModel
    
    A specialized managed resource for Models.
    
    (C) 2007 Radon Labs GmbH
*/
#include "resources/managedresource.h"
#include "models/model.h"

//------------------------------------------------------------------------------
namespace Models
{
class ManagedModel : public Resources::ManagedResource
{
    __DeclareClass(ManagedModel);
public:
    /// get contained model resource
    const Ptr<Model>& GetModel() const;
};

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Model>&
ManagedModel::GetModel() const
{
    return this->GetResource().downcast<Model>();
}

} // namespace Models
//------------------------------------------------------------------------------
#endif
