//------------------------------------------------------------------------------
//  physicsmesh.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "physics/physicsmesh.h"
#include "coregraphics/legacy/nvx2streamreader.h"
#include "io/ioserver.h"

using namespace Math;

namespace Physics
{
__ImplementClass(Physics::PhysicsMesh, 'PMSH', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
PhysicsMesh::PhysicsMesh() :
    numVertices(0),
    numIndices(0),
    vertexByteSize(0),
    vertexNumFloats(0),
    vertexData(0),
    indexData(0),
    isLoaded(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
PhysicsMesh::~PhysicsMesh()
{
    if (this->IsLoaded())
    {
        this->Unload();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
PhysicsMesh::Load()
{
    n_assert(!this->isLoaded);
    n_assert(this->filename.IsValid());
    n_assert(0 == this->vertexData);
    n_assert(0 == this->indexData);

    // create a mesh loader
    Ptr<Legacy::Nvx2StreamReader> meshLoader = Legacy::Nvx2StreamReader::Create();
    meshLoader->SetRawMode(true);   
    if (this->filename.CheckFileExtension("n3d2"))
    {
        n_error("n3d2 files not supported!");
    }
    else if (this->filename.CheckFileExtension("nvx2"))
    {
        meshLoader->SetStream(IO::IoServer::Instance()->CreateStream(this->filename));    
    }
    else
    {
        n_error("Physics::PhysicsMesh::Load(): invalid file extension in filename '%s'!", this->filename.AsCharPtr());
    }

    // load mesh data    
    if (!meshLoader->Open())
    {
        n_error("Physics::PhysicsMesh:Load()(): Failed to open mesh file '%s'!", this->filename.AsCharPtr());
        return false;
    }
    this->isLoaded = true;
    
    // get loaded mesh data
    this->meshGroups = meshLoader->GetPrimitiveGroups();
    this->numVertices = meshLoader->GetNumVertices();
    this->numIndices = meshLoader->GetNumIndices();
    this->vertexByteSize = meshLoader->GetVertexWidth() * sizeof(float);
    this->vertexNumFloats = meshLoader->GetVertexWidth();

    // copy over vertices
    this->vertexData = (float*) Memory::Alloc(Memory::ResourceHeap, this->numVertices * this->vertexByteSize);
    Memory::Copy(meshLoader->GetVertexData(), this->vertexData, this->numVertices * this->vertexByteSize);

    // copy over indices, need to expand to 32 bit
    ushort* srcIndexData = meshLoader->GetIndexData();
    this->indexData = (int*) Memory::Alloc(Memory::ResourceHeap, this->numIndices * sizeof(int));
    IndexT i;
    for (i = 0; i < this->numIndices; i++)
    {
        this->indexData[i] = (int) srcIndexData[i];
    }
    this->meshGroups = meshLoader->GetPrimitiveGroups();
    this->UpdateGroupBoundingBoxes(this->GetVertexPointer(), this->GetIndexPointer());

    // cleanup mesh loader
    meshLoader->Close();
    
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
PhysicsMesh::Unload()
{
    if (this->GetRefCount() == 1)
    {
        n_assert(this->isLoaded);
        n_assert(0 != this->vertexData);
        n_assert(0 != this->indexData);
        Memory::Free(Memory::ResourceHeap, this->vertexData);
        this->vertexData = 0;
        Memory::Free(Memory::ResourceHeap, this->indexData);
        this->indexData = 0;
        this->isLoaded = false;
    }
}

//------------------------------------------------------------------------------
/**
*/
int
PhysicsMesh::GetGroupNumVertices(int groupIndex) const
{
    n_assert(this->isLoaded);
    return this->meshGroups[groupIndex].GetNumVertices();
}

//------------------------------------------------------------------------------
/**
*/
int
PhysicsMesh::GetGroupNumIndices(int groupIndex) const
{
    n_assert(this->isLoaded);
    return this->meshGroups[groupIndex].GetNumIndices();
}

//------------------------------------------------------------------------------
/**
*/
float*
PhysicsMesh::GetGroupVertexPointer(int groupIndex) const
{
    n_assert(this->isLoaded);
    int firstVertex = this->meshGroups[groupIndex].GetBaseVertex();
    float* ptr = this->GetVertexPointer() + firstVertex * this->vertexNumFloats;
    return ptr;
}

//------------------------------------------------------------------------------
/**
*/
int*
PhysicsMesh::GetGroupIndexPointer(int groupIndex) const
{
    n_assert(this->isLoaded);
    int firstIndex = this->meshGroups[groupIndex].GetBaseIndex();
    int* ptr = this->GetIndexPointer() + firstIndex;
    return ptr;
}

//------------------------------------------------------------------------------
/**
    Update the group bounding boxes. This is a slow operation (since the
    vertex buffer must read). It should only be called once after loading.
    NOTE that the vertex and index buffer must be locked while calling
    this method!
*/
void
PhysicsMesh::UpdateGroupBoundingBoxes(float* vertexBufferData, int* indexBufferData)
{
    bbox groupBox;
	int numGroups = this->meshGroups.Size();
    int groupIndex;
    for (groupIndex = 0; groupIndex < numGroups; groupIndex++)
    {
        groupBox.begin_extend();
        CoreGraphics::PrimitiveGroup& group = this->meshGroups[groupIndex];
        int* indexPointer = indexBufferData + group.GetBaseIndex();
        uint i;
        for (i = 0; i < group.GetNumIndices(); i++)
        {
            float* vertexPointer = vertexBufferData + (indexPointer[i] * this->vertexNumFloats);
            groupBox.extend(point(vertexPointer[0], vertexPointer[1], vertexPointer[2]));
        }
        group.SetBoundingBox(groupBox);
    }
}

} // namespace Physics