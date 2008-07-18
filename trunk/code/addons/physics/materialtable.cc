//------------------------------------------------------------------------------
//  physics/materialtable.cc
//  (C) 2003 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "physics/materialtable.h"
#include "io/xmlreader.h"

namespace Physics
{
Util::Array<struct MaterialTable::Material> MaterialTable::materials;
Util::Array<Util::Array<struct MaterialTable::Interaction> > MaterialTable::interactions;
Util::String MaterialTable::invalidTypeString = "InvalidMaterial";

//------------------------------------------------------------------------------
/**
    Materials used in ODE
    
    Unit defintions are:
    Density:                t/m^3
    Force (MegaNewton):     t*m/s^2
    Momentum:               t*m/s
    Moment of intertia:     t*m^2
    Moment of momentum:     t*m^2/s
    Moment of force:        N*m
    Pressure (MegaPascal):  MN/m^2
    Weight (MegaNewton):    t*m/s^2
    Work (MegaJoule):       MN*m 
    Energy (MegaJoule):     MN*m

    Note: I'm not really sure that this is right... ;-)
*/      
void 
MaterialTable::Setup()
{
    /// Soil:Stone:Asphalt:Metal:Wood:HollowWood:Glass:Ice:Gold:Flesh:Bone
    /// create some default materials with densities and interactions
    AddMaterialType("HollowWood",   0.6f);
    AddMaterialType("Wood",         0.8f);
    AddMaterialType("Ice",          0.919f);
    AddMaterialType("Bone",         0.85f);
    AddMaterialType("Water",        1.0f);
    AddMaterialType("Character",    1.0f);
    AddMaterialType("Flesh",        1.0f);
    AddMaterialType("Asphalt",      1.4f);
    AddMaterialType("Glass",        2.579f);
    AddMaterialType("Stone",        2.691f);
    AddMaterialType("Metal",        7.8f);
    AddMaterialType("Soil",         10.0f);
    AddMaterialType("Gold",         19.32f);
    AddMaterialType("Probe",         0.1f);

    uint characterIndex = StringToMaterialType("Character");
    for (uint i = 0; i < materials.Size(); i++)
    {
        for (uint j = 0; j < materials.Size(); j++)
        {
            // set special friction and bouncyness for character
            if (characterIndex == i
                || characterIndex == j)
            {
                SetFriction(i, j, 1.5f);
                SetBouncyness(i, j, 0.0f);
            }
            else
            {
                SetFriction(i, j, 8.0f);
                SetBouncyness(i, j, 0.5f);
            }            
        }
    }
};

//------------------------------------------------------------------------------
/**
*/
const String& 
MaterialTable::MaterialTypeToString(MaterialType t)
{
    if (-1 == t)
    {
        return invalidTypeString;
    }
    else
    {
        n_assert(t >= 0 && (uint)t < materials.Size());
        return materials[t].name;
    }
}

//------------------------------------------------------------------------------
/**
*/
MaterialType 
MaterialTable::StringToMaterialType(const String& str)
{
    for (uint i = 0; i < materials.Size(); ++i)
    {
        if (materials[i].name == str)
        {
            return i;
        }
    }

    return -1;
}

//------------------------------------------------------------------------------
/**
*/
float
MaterialTable::GetDensity(MaterialType t)
{
    n_assert(t >= 0 && (uint)t < materials.Size());
    return materials[t].density;
}

//------------------------------------------------------------------------------
/**
*/
float
MaterialTable::GetFriction(MaterialType t0, MaterialType t1)
{
    n_assert(t0 >= 0 && (uint)t0 < materials.Size());
    n_assert(t1 >= 0 && (uint)t1 < materials.Size());
    return interactions[t0][t1].friction;
}

//------------------------------------------------------------------------------
/**
*/
float 
MaterialTable::GetBounce(MaterialType t0, MaterialType t1)
{
    n_assert(t0 >= 0 && (uint)t0 < materials.Size());
    n_assert(t1 >= 0 && (uint)t1 < materials.Size());
    return interactions[t0][t1].bouncyness;
}

//------------------------------------------------------------------------------
/**
*/
const Util::String&
MaterialTable::GetCollisionSound(MaterialType t0, MaterialType t1)
{
    n_assert(t0 >= 0 && (uint)t0 < materials.Size());
    n_assert(t1 >= 0 && (uint)t1 < materials.Size());
    return interactions[t0][t1].collSound;
}

//------------------------------------------------------------------------------
/**
*/
void 
MaterialTable::AddMaterialType(const Util::String& matName, float density)
{
    n_assert(StringToMaterialType(matName) == InvalidMaterial);
    Material newMat;
    newMat.name = matName;
    newMat.density = density;
    materials.Append(newMat);
    
    // add new interactions row
    Interaction newInteraction;
    Array<Interaction> interactionRow;
    if (interactions.Size() > 0 )
    {
        interactionRow = interactions[interactions.Size()-1];   
    }
    interactions.Append(interactionRow);

    // go thru all interactions and add our new material
    uint i;
    for (i = 0; i < interactions.Size(); i++)
    {
        interactions[i].Append(newInteraction);        
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
MaterialTable::SetDensity(MaterialType type, float density)
{
    n_assert(type != -1 && (uint)type < materials.Size());

    materials[type].density = density;
}

//------------------------------------------------------------------------------
/**
*/
void 
MaterialTable::SetFriction(MaterialType t0, MaterialType t1, float friction)
{
    n_assert(t0 != -1 && (uint)t0 < materials.Size());
    n_assert(t1 != -1 && (uint)t1 < materials.Size());

    interactions[t0][t1].friction = friction;
}

//------------------------------------------------------------------------------
/**
*/
void 
MaterialTable::SetBouncyness(MaterialType t0, MaterialType t1, float bouncyness)
{
    n_assert(t0 != -1 && (uint)t0 < materials.Size());
    n_assert(t1 != -1 && (uint)t1 < materials.Size());

    interactions[t0][t1].bouncyness = bouncyness;
}

} // namespace Physics