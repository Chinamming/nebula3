#ifndef N_PARTICLESERVER2_H
#define N_PARTICLESERVER2_H
//------------------------------------------------------------------------------
/**
    @class nParticleServer2
    @ingroup Particle

    Particle subsystem server. Holds an array of all particles and all particle
    emitters in the world. Take care when updating them, the rendering is a
    function of the emitters.
    
    (C) 2005 RadonLabs GmbH
*/
//#include "kernel/nroot.h"
#include "core/refcounted.h"
#include "nparticle2.h"
#include "nparticle2emitter.h"

#include "util/array.h"
#include "util/fixedarray.h"
#include "timing/time.h"
/*
#include "util/narray.h"
#include "util/nringbuffer.h"//What is the equalation to this in nebula3
#include "util/nfixedarray.h"
*/


//------------------------------------------------------------------------------

namespace Nebula2
{
class nParticle2Emitter;

class nParticleServer2 : public Core::RefCounted
{
private:
    enum 
    {
        MaxParticles = 30000,       // maximum number of particles in the world
        FloatRandomCount = 32768,   // number of floats in the float random pool
        IntRandomCount = 512,       // number of ints in the int random pool
    };
public:
    /// constructor
    nParticleServer2();
    /// destructor
    virtual ~nParticleServer2();
    /// get server instance
    static nParticleServer2* Instance();
    /// open the particle server
    void Open();
    /// close the particle server
    void Close();
    /// return true if currently open
    bool IsOpen() const;
    /// update all active particle emitters
    void Trigger();
    /// debug: enable/disable particle updates and rendering
    void SetParticlesEnabled(bool b);
    /// debug: get particle update/rendering state
    bool AreParticlesEnabled() const;

    /// set current time
	void SetTime(Timing::Time t);
    /// get current time
    Timing::Time GetTime() const;
    /// create a new particle emitter
    nParticle2Emitter* NewParticleEmitter();
    /// delete the given emitter
    void DeleteParticleEmitter(nParticle2Emitter* emitter);
    /// set global force attribute
	void SetGlobalAccel(const Math::float4& accel);
    /// get global force attribute
    const Math::float4& GetGlobalAccel() const;
    /// get a random int from the int random pool
    int PseudoRandomInt(int key);
    /// get a random float from the float random pool
    float PseudoRandomFloat(int key);
    /// pseudo random vector
    Math::float4 PseudoRandomVector3(int key);

private:
    static nParticleServer2* Singleton;

    bool isOpen;
    bool particlesEnabled;
	Util::Array<nParticle2Emitter*> emitters;
	Util::FixedArray<float> floatRandomPool;
	Util::FixedArray<int> intRandomPool;
    Math::float4 globalAccel;
	Timing::Time time;
};

//------------------------------------------------------------------------------
/**
*/
inline
nParticleServer2*
nParticleServer2::Instance()
{
    n_assert(Singleton);
    return Singleton;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
nParticleServer2::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nParticleServer2::SetTime(Timing::Time t)
{
    this->time = t;
}

//------------------------------------------------------------------------------
/**
*/
inline
Timing::Time
nParticleServer2::GetTime() const
{
    return this->time;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nParticleServer2::SetGlobalAccel(const Math::float4& accel)
{
    this->globalAccel = accel;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::float4&
nParticleServer2::GetGlobalAccel() const
{
    return this->globalAccel;
}

//------------------------------------------------------------------------------
/**
*/
inline
int
nParticleServer2::PseudoRandomInt(int key)
{
    // force key into valid range
    key %= (IntRandomCount-1);
    n_assert(key >= 0);
    return this->intRandomPool[key];
};

//------------------------------------------------------------------------------
/**
*/
inline
float
nParticleServer2::PseudoRandomFloat(int key)
{
    // force key into valid range
    key %= (FloatRandomCount-1);
    n_assert(key >= 0);
    return this->floatRandomPool[key];
};

//------------------------------------------------------------------------------
/**
*/
inline
Math::float4
nParticleServer2::PseudoRandomVector3(int key)
{
    // align to start of random normalized 3d vector
    key *= 4;             
    int k0 = key % (FloatRandomCount - 1);
    n_assert((k0 >= 0) && ((k0 + 2) < FloatRandomCount));
	return Math::float4(this->floatRandomPool[k0], this->floatRandomPool[k0+1], this->floatRandomPool[k0+2],0);
};

//------------------------------------------------------------------------------
/**
*/
inline void
nParticleServer2::SetParticlesEnabled(bool b)
{
    this->particlesEnabled = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
nParticleServer2::AreParticlesEnabled() const
{
    return this->particlesEnabled;
}
};// namespace Nebula2
//------------------------------------------------------------------------------

#endif    

