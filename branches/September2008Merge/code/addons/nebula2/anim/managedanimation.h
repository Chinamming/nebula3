#pragma once
#ifndef N_MANAGEDANIMATION_H
#define N_MANAGEDANIMATION_H
//------------------------------------------------------------------------------
/**
    @class Models::ManagedModel
    
    A specialized managed resource for Models.
    
    (C) 2007 Radon Labs GmbH
*/
#include "resources/managedresource.h"
#include "nanimation.h"

//------------------------------------------------------------------------------
namespace Nebula2
{
class ManagedAnimation : public Resources::ManagedResource
{
    __DeclareClass(ManagedAnimation);
public:
    /// get contained model resource
    const Ptr<nAnimation>& GetAnimation() const;
};

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<nAnimation>&
ManagedAnimation::GetAnimation() const
{
    return this->GetResource().downcast<nAnimation>();
}

} // namespace Models
//------------------------------------------------------------------------------
#endif
