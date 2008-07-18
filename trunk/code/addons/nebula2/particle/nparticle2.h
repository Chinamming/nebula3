#ifndef N_PARTICLE2_H
#define N_PARTICLE2_H
//------------------------------------------------------------------------------
/**    
    @class nParticle2
    @ingroup Particle

    A single particle2 in a particle system. This is just a typedef

    (C) 2005 RadonLabs GmbH
*/

#include "math/float4.h"
#include "math/float2.h"

namespace Nebula2
{
struct nParticle2
{
	Math::float4 acc;
	Math::float4 velocity;               
	Math::float4 position;
	Math::float4 startPos;
	float   rotation;
    float   rotationVariation;
    float   sizeVariation;
    float   lifeTime;       
    float   oneDivMaxLifeTime;
	Math::float2 uvmin;
	Math::float2 uvmax;
    //TODO: BoundingSphere
};
};// namespace Nebula2
#endif

