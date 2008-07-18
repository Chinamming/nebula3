//------------------------------------------------------------------------------
//  nparticleserver_main.cc
//  (C) 2003 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "nparticleserver2.h"

#include <stdlib.h>
#include <time.h> 

//#include "kernel/ntimeserver.h" TODO: EQ in Nebula3?
namespace Nebula2
{
//nNebulaClass(nParticleServer2, "nroot");
nParticleServer2* nParticleServer2::Singleton = 0;

//------------------------------------------------------------------------------
/**
*/
nParticleServer2::nParticleServer2() :
    floatRandomPool(FloatRandomCount),
    intRandomPool(IntRandomCount),
    globalAccel(0.0f, -1.0f, 0.0f,0.0f),
    time(0.0),
    isOpen(false),
    particlesEnabled(true)
{
    n_assert(0 == Singleton);
    Singleton = this;

//    srand((unsigned int) ::time(NULL));

    // fill the random number pools
    unsigned int i;
	for (i = 0; i < this->intRandomPool.Size(); i++)
    {
        this->intRandomPool[i] = rand();
	}
    for (i = 0; i < this->floatRandomPool.Size();)
    {
        // get a random number between -1.0f and 1.0f, organized
        // into 3 normalized vector components, and one w component
		Math::float4 v(Math::n_rand(-1.0f, 1.0f), Math::n_rand(-1.0f, 1.0f), Math::n_rand(-1.0f, 1.0f),Math::n_rand(-1.0f, 1.0f));
		//float w = ;
		v = Math::float4::normalize(v);
        this->floatRandomPool[i++] = v.x();
        this->floatRandomPool[i++] = v.y();
        this->floatRandomPool[i++] = v.z();
		this->floatRandomPool[i++] = v.w();
    }
}

//------------------------------------------------------------------------------
/**
*/
nParticleServer2::~nParticleServer2()
{
    if (this->IsOpen())
    {
        this->Close();
    }
    n_assert(Singleton);
    Singleton = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
nParticleServer2::Open()
{
    n_assert(!this->IsOpen());
    this->isOpen = true;
}

//------------------------------------------------------------------------------
/**
*/
void
nParticleServer2::Close()
{
    n_assert(this->IsOpen());
    
    // cleanup existing emitters
    unsigned int i;
    for (i = 0; i < this->emitters.Size(); i++)
    {
        n_delete(this->emitters[i]);
    }
    this->emitters.Clear();
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
nParticle2Emitter*
nParticleServer2::NewParticleEmitter()
{
    n_assert(this->IsOpen());
    nParticle2Emitter* particleEmitter = n_new(nParticle2Emitter);
    this->emitters.Append(particleEmitter);
    n_printf("nParticleServer2: particle emitter created!\n");
    return particleEmitter;
}

//------------------------------------------------------------------------------
/**
*/
void nParticleServer2::Trigger()
{
    n_assert(this->IsOpen());
    int i;
    int num = this->emitters.Size();
    for (i = 0; i < num; i++)
    {
        this->emitters[i]->Update(float(this->time));
    }
}

//------------------------------------------------------------------------------
/**
*/
void nParticleServer2::DeleteParticleEmitter(nParticle2Emitter* emitter)
{
    n_assert(this->IsOpen());
    n_assert(0 != emitter);
    int index = this->emitters.FindIndex(emitter);
    if (-1 != index)
    {
        n_delete(this->emitters[index]);
        this->emitters.EraseIndex(index);
    }
}
};// namespace Nebula2