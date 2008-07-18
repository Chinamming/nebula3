//------------------------------------------------------------------------------
//  physicsmesh.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "physics/physicsmesh.h"
#include "coregraphics/legacy/nvx2streamreader.h"
#include "io/ioserver.h"
#include "coregraphics/cpumemoryvertexbufferloader.h"
#include "coregraphics/cpumemoryindexbufferloader.h"

using namespace Math;

namespace Physics
{
ImplementClass(Physics::PhysicsMesh, 'PMSH', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
PhysicsMesh::PhysicsMesh() :
    isLoaded(false),
    inAppend(false)
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
    n_assert(!this->vertexBuffer.isvalid());
    n_assert(!this->indexBuffer.isvalid());

    // create a mesh loader
    Ptr<Legacy::Nvx2StreamReader> meshLoader = Legacy::Nvx2StreamReader::Create();

    // create and set main memory cpu accessable vertex and index buffers
    this->vertexBuffer = CoreGraphics::CPUVertexBuffer::Create();
    this->indexBuffer = CoreGraphics::CPUIndexBuffer::Create();
    // set index depth, Opcodes ICE uses 32 bit indices, not our 16bit
    this->indexBuffer->SetIndexBufferDepth(CoreGraphics::IndexType::Index32);

    Ptr<CoreGraphics::CPUMemoryVertexBufferLoader> vertexBufferLoader = CoreGraphics::CPUMemoryVertexBufferLoader::Create();    
    Ptr<CoreGraphics::CPUMemoryIndexBufferLoader> indexBufferLoader = CoreGraphics::CPUMemoryIndexBufferLoader::Create();

    meshLoader->SetVertexBuffer(vertexBuffer.upcast<Base::VertexBufferBase>(), vertexBufferLoader.upcast<Base::MemoryVertexBufferLoaderBase>());
    meshLoader->SetIndexBuffer(indexBuffer.upcast<Base::IndexBufferBase>(), indexBufferLoader.upcast<Base::MemoryIndexBufferLoaderBase>());
   
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
    
    // copy over mesh groups
    this->meshGroups = meshLoader->GetPrimitiveGroups();
    this->UpdateGroupBoundingBoxes(this->GetVertexPointer(), this->GetIndexPointer());
    this->vertexBuffer->Unmap();
    this->indexBuffer->Unmap();

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
    n_assert(this->isLoaded);
    FreeBuffer();
    this->isLoaded = false;
}

//------------------------------------------------------------------------------
/**
*/
void
PhysicsMesh::FreeBuffer()
{
    n_assert(this->vertexBuffer.isvalid());
    n_assert(this->indexBuffer.isvalid());  
    this->vertexBuffer->Unload();
    this->vertexBuffer = 0;
    this->indexBuffer->Unload();
    this->indexBuffer = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
PhysicsMesh::BeginAppendMesh(int numVertices, int numIndices, int vertexWidth)
{
    n_assert(!this->isLoaded);
    n_assert(!this->inAppend);
    n_assert(!this->vertexBuffer.isvalid());
    n_assert(!this->indexBuffer.isvalid());
    this->inAppend = true;

    this->vertexBuffer = CoreGraphics::CPUVertexBuffer::Create();
    this->indexBuffer = CoreGraphics::CPUIndexBuffer::Create();

    Ptr<CoreGraphics::CPUMemoryVertexBufferLoader> vertexBufferLoader = CoreGraphics::CPUMemoryVertexBufferLoader::Create();    
    Ptr<CoreGraphics::CPUMemoryIndexBufferLoader> indexBufferLoader = CoreGraphics::CPUMemoryIndexBufferLoader::Create();

    CoreGraphics::VertexComponent vertexLayout(CoreGraphics::VertexComponent::Position,0, CoreGraphics::VertexComponent::Float3); 
    Util::Array<CoreGraphics::VertexComponent> vertexComps;
    vertexComps.Append(vertexLayout);
    vertexBufferLoader->Setup(vertexComps, numVertices, this->vertexBuffer->Map(Base::ResourceBase::MapWrite), numVertices * 3 * sizeof(float));
    vertexBufferLoader->OnLoadRequested();

    indexBufferLoader->Setup(CoreGraphics::IndexType::Index32, numIndices, this->indexBuffer->Map(Base::ResourceBase::MapWrite), numIndices * sizeof(uint));
    indexBufferLoader->OnLoadRequested();     

    // allocate vertex and index buffer
    vertexIndex = 0;
    indicesIndex = 0;

    // create one meshgroup
    CoreGraphics::PrimitiveGroup mGroup;
    mGroup.SetBaseIndex(0);
    mGroup.SetBaseVertex(0);    
    this->meshGroups.Append(mGroup);
}

//------------------------------------------------------------------------------
/**
*/
void
PhysicsMesh::EndAppendMesh()
{
    n_assert(!this->isLoaded);
    n_assert(this->inAppend);

    n_assert(this->vertexIndex == this->vertexBuffer->GetNumVertices());
    n_assert(this->indicesIndex == this->indexBuffer->GetNumIndices());

    this->vertexBuffer->Unmap();
    this->indexBuffer->Unmap();

    vertexIndex = 0;
    indicesIndex = 0;

    this->inAppend = false;
    this->isLoaded = true;
}

//------------------------------------------------------------------------------
/**
*/
bool
PhysicsMesh::AppendMesh(const Ptr<PhysicsMesh>& mesh, const Math::matrix44& transform)
{
    // copy vertex, and transform position
    int vertexIndex;
    float* srcMeshPtr = mesh->GetVertexPointer();
    int vertexWidth = this->vertexBuffer->GetVertexLayout()->GetVertexByteSize();
    float* destMeshPtr = this->GetVertexPointer() + this->vertexIndex * vertexWidth;
    for (vertexIndex = 0; vertexIndex < mesh->GetNumVertices() * vertexWidth; vertexIndex += vertexWidth)
    {
        Math::vector position(srcMeshPtr[vertexIndex], srcMeshPtr[vertexIndex + 1], srcMeshPtr[vertexIndex + 2]);
        position = vector::transform(position, transform);

        destMeshPtr[vertexIndex] = position.x();
        destMeshPtr[vertexIndex + 1] = position.y();
        destMeshPtr[vertexIndex + 2] = position.z();
        // copy rest of vertex
        int i;
        for(i = 3; i < vertexWidth; i++)
        {
            destMeshPtr[vertexIndex + i] =  srcMeshPtr[vertexIndex + i];
        }
    }
    this->vertexIndex += mesh->GetNumVertices();

    // copy indices with offset
    int i;
    int* srcIndexPtr = mesh->GetIndexPointer();
    int* destIndexPtr = this->GetIndexPointer() + this->indicesIndex;
    int vertexOffset = this->meshGroups[0].GetNumVertices();
    for (i = 0; i < mesh->GetNumIndices(); i ++)
    {
        destIndexPtr[i] = srcIndexPtr[i] + vertexOffset;    
    }
    this->indicesIndex += mesh->GetNumIndices();

    this->meshGroups[0].SetNumIndices(this->meshGroups[0].GetNumIndices() + mesh->GetNumIndices());
    this->meshGroups[0].SetNumVertices(this->meshGroups[0].GetNumVertices() + mesh->GetNumVertices());
    this->UpdateGroupBoundingBoxes(this->GetVertexPointer(), this->GetIndexPointer());

    return true;
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
    float* ptr = this->GetVertexPointer() + firstVertex * this->vertexBuffer->GetVertexLayout()->GetVertexByteSize();
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
            int vertexWidth = this->vertexBuffer->GetVertexLayout()->GetVertexByteSize() / 4;
            float* vertexPointer = vertexBufferData + (indexPointer[i] * vertexWidth);
            groupBox.extend(point(vertexPointer[0], vertexPointer[1], vertexPointer[2]));
        }
        group.SetBoundingBox(groupBox);
    }
}


} // namespace Physics