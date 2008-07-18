//------------------------------------------------------------------------------
//  attr/attributes.h
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "physicsattr/physicsattributes.h"

namespace Attr
{
    DefineString(Physics, 'PHYS', ReadWrite);
    DefineBool(Collide, 'COLL', ReadWrite);
    DefineString(PhysicMaterial, 'PHMA', ReadWrite);
    DefineInt(TargetEntityId, 'TAEN', ReadWrite);
    //DefineString(NoGoArea, '', ReadWrite);
    //DefineMatrix44(NoGoAreaTransform, '', ReadWrite);

    // PhysicsProprty
    DefineFloat4(VelocityVector, 'VELV', ReadWrite); // the current absolute velocity vector

    // ActorPhysicsProperty
    DefineFloatWithDefault(RelVelocity, 'RVEL', ReadWrite, 1.0f);      // the currently set relative velocity (0..1)
    DefineFloatWithDefault(MaxVelocity, 'MVEL', ReadWrite, 5.0f);      // the maximum velocity
    DefineBool(Following, 'FOLL', ReadWrite);         // true if currently following somebody
    DefineBool(Moving, 'MOVG', ReadWrite);            // true if currently moving
};
