#ifndef ATTR_PHYSICSATTR_H
#define ATTR_PHYSICSATTR_H
//------------------------------------------------------------------------------
/**
    @file physicsattr/physicsattributes.h

    This is the central attribute registry for thisfeature. For more information
    on attributes, see Attr::Attribute.

    (C) 2007 Radon Labs GmbH
*/
#include "attr/attrid.h"
#include "physicsattr/physicsattributes.h"

//------------------------------------------------------------------------------
namespace Attr
{   
    DeclareString(Physics, 'PHYS', ReadWrite);
    DeclareBool(Collide, 'COLL', ReadWrite);
    DeclareString(PhysicMaterial, 'PHMA', ReadWrite);
    DeclareInt(TargetEntityId, 'TAEN', ReadWrite);
    //DeclareString(NoGoArea, '', ReadWrite);
    //DeclareMatrix44(NoGoAreaTransform, '', ReadWrite);
    
    // PhysicsProprty
    DeclareFloat4(VelocityVector, 'VELV', ReadWrite); // the current absolute velocity vector

    // ActorPhysicsProperty
    DeclareFloat(RelVelocity, 'RVEL', ReadWrite);      // the currently set relative velocity (0..1)
    DeclareFloat(MaxVelocity, 'MVEL', ReadWrite);      // the maximum velocity
    DeclareBool(Following, 'FOLL', ReadWrite);         // true if currently following somebody
    DeclareBool(Moving, 'MOVG', ReadWrite);            // true if currently moving

}; // namespace Attr
//------------------------------------------------------------------------------
#endif