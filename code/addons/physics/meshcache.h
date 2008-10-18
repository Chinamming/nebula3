#ifndef PHYSICS_MESHCACHE_H
#define PHYSICS_MESHCACHE_H
//------------------------------------------------------------------------------
/**
    @class Physics::MeshCache
    
    A cache for loaded meshes to prevent redundant loading of collide meshes.
    
    (C) 2006 Radon Labs GmbH
*/    
#include "core/refcounted.h"
#include "util/dictionary.h"
#include "physics/physicsmesh.h"

//------------------------------------------------------------------------------
namespace Physics
{
class MeshCache : public Core::RefCounted
{
    __DeclareClass(MeshCache);
public:
    /// constructor
    MeshCache();
    /// destructor
    virtual ~MeshCache();
    /// get instance pointer
    static MeshCache* Instance();
    /// open the mesh cache
    void Open();
    /// close the mesh cache (unloads all cached meshes)
    void Close();
    /// return true if currently open
    bool IsOpen() const;
    /// create a mesh, or re-use existing mesh
    Ptr<PhysicsMesh> NewMesh(const Util::String& filename);

private:
    static MeshCache* Singleton;
    bool isOpen;
    Util::Dictionary<Util::String,Ptr<PhysicsMesh> > meshes;
};

//------------------------------------------------------------------------------
/**
*/
inline
MeshCache*
MeshCache::Instance()
{
    n_assert(0 != Singleton);
    return Singleton;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
MeshCache::IsOpen() const
{
    return this->isOpen;
}

} // namespace Physics
//------------------------------------------------------------------------------
#endif
