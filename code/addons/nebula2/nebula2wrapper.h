#ifndef N_NEBULA2WRAPPER_H
#define N_NEBULA2WRAPPER_H
//------------------------------------------------------------------------------
/**
    @file addons/nebula2/nebula2wrapper.h

    Just a typedef wrapper for Nebula2 Legacy support.

    (C) 2007 RadonLabs GmbH
*/

#include "character/ncharjointpalette.h"
#include "character/ncharacter2.h"
#include "character/ncharacter3set.h"
#include "character/ncharskeleton.h"
#include "character/charfragment.h"
#include "character/charvariation.h"
#include "character/characterserver.h"
#include "anim/managedanimation.h"
#include "anim/nanimationserver.h"
#include "anim/nanimclip.h"
#include "anim/nmemoryanimation.h"
#include "character/skininfo.h"
#include "anim/animtable.h"

//#include "addons/nebula2/particle/envelopecurve.h"
//#include "addons/nebula2/particle/vector3envelopecurve.h"
///nebula2/particle/ndynamicmesh.h"
//#include "addons/nebula2/particle/nparticle2.h"
//#include "addons/nebula2/particle/nparticle2emitter.h"
//#include "addons/nebula2/particle/particleemitterbase.h"
//#include "addons/nebula2/particle/nparticleserver2.h"



namespace Char
{
typedef Nebula2::nCharacter2 Character;
typedef Nebula2::nCharacter3Set CharacterSet;
typedef Nebula2::nCharJointPalette CharJointPalette;
typedef Nebula2::nCharSkeleton CharSkeleton;
typedef Nebula2::CharFragment CharFragment ;
typedef Nebula2::CharVariation CharVariation;
typedef Nebula2::nCharJoint CharJoint;
typedef Nebula2::SkinInfo SkinInfo;
typedef Nebula2::CharacterServer CharacterServer;
};

namespace Anim
{
typedef Nebula2::ManagedAnimation ManagedAnimation;
typedef Nebula2::nAnimation Animation;
typedef Nebula2::nMemoryAnimation MemoryAnimation;
typedef Nebula2::nAnimationServer AnimationServer;
typedef Nebula2::nAnimClipScheduler AnimClipScheduler;
typedef Nebula2::nAnimClip AnimClip;
typedef Nebula2::AnimTable AnimTable;
};

/*
namespace ParticleSystem
{
typedef Nebula2::EnvelopeCurve EnvelopeCurve;
typedef Nebula2::Vector3EnvelopeCurve Vector3EnvelopeCurve;
};
namespace CoreGraphics
{
typedef Nebula2::nDynamicMesh DynamicMesh;
};
*/
//------------------------------------------------------------------------------
#endif
