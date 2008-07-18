//------------------------------------------------------------------------------
//  ndynamicmesh.cc
//  (C) 2004 RadonLabs GmbH
//------------------------------------------------------------------------------

#include "stdneb.h"
#include "ndynamicmesh.h"
#include "resources/sharedresourceserver.h"
#include "resources/dynamicmeshresourceloader.h"
#include "coregraphics/mesh.h"
#include "coregraphics/renderdevice.h"

//------------------------------------------------------------------------------
/**
*/
namespace Nebula2
{
ImplementClass(nDynamicMesh, 'DYNM', Core::RefCounted);

nDynamicMesh::nDynamicMesh() :
    indexedRendering(true),
		primitiveType(CoreGraphics::PrimitiveTopology::TriangleList)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
nDynamicMesh::~nDynamicMesh()
{
	this->refMesh = 0;
}

//------------------------------------------------------------------------------
/**
    Return true if object is valid, otherwise Initialize() must be called
    to prepare the object for rendering. A dynamic mesh can become
    invalid anytime, thus make sure you check before each call to
    Begin()/End().
*/
bool
nDynamicMesh::IsValid() const
{
    if (this->refMesh.isvalid())
    {
		if (!this->refMesh->IsLoaded())
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
    Initialize the dynamic mesh. This will create or lookup a mesh
    which is shared with all other dynamic mesh objects with the
    same vertex components and usage flags.
    This method must be called whenever a call to IsValid() returns false.

    @param  primType            primitive type
    @param  vertexLayout        vertex component mask
    @param  indexed             true if indexed primitive rendering is intended
    @return                     true if initialized successful
*/
bool
nDynamicMesh::Initialize(const Util::Atom<Util::String>& resourceID,CoreGraphics::PrimitiveTopology::Code primType,const Util::Array<CoreGraphics::VertexComponent>& vertexComponents,
                        bool indexed)
{
    this->primitiveType = primType;
    this->indexedRendering = indexed;
	
	Ptr<CoreGraphics::Mesh> mesh = 0;
    if (!this->refMesh.isvalid())
    {
  
		Resources::ResourceId meshID(resourceID);
        n_assert(!Resources::SharedResourceServer::Instance()->HasSharedResource(meshID));
		mesh = Resources::SharedResourceServer::Instance()->CreateSharedResource(meshID, CoreGraphics::Mesh::RTTI,Resources::DynamicMeshResourceLoader::RTTI).downcast<CoreGraphics::Mesh>();
		
		n_assert(mesh.isvalid());
        n_assert(mesh);
        this->refMesh = mesh;
    }
    else
    {
        mesh = this->refMesh;
    }

    // initialize the mesh
    if (!mesh->IsLoaded())
    {
		Ptr<Resources::DynamicMeshResourceLoader> dynMeshLoader;
		dynMeshLoader = mesh->GetLoader().downcast<Resources::DynamicMeshResourceLoader>();
		dynMeshLoader->SetNumberOfVertices(VertexBufferSize);

		Ptr<CoreGraphics::VertexLayout> tmpVertexLayout = CoreGraphics::VertexLayout::Create();
		dynMeshLoader->SetVertexComponents(vertexComponents);
	
		//set the vertex buffers access mode and usage 
		dynMeshLoader->SetVertexAccessMode(CoreGraphics::VertexBuffer::AccessWrite);
		dynMeshLoader->SetVertexUsage(CoreGraphics::VertexBuffer::UsageDynamic);

		//calculate vertex width (numbytes/float)
		SizeT numComponentBytes = 0;
		SizeT i = 0;
		for(;i < vertexComponents.Size(); i++)
		{
			numComponentBytes += vertexComponents[i].GetByteSize();
		}
		
		SizeT vertexWidth = numComponentBytes/sizeof(float);
		dynMeshLoader->SetVertexWidth(vertexWidth);
		CoreGraphics::PrimitiveGroup primGroup;
		primGroup.SetBaseVertex(0);
		primGroup.SetNumVertices(VertexBufferSize);
        primGroup.SetPrimitiveTopology(primType);
				
        if (indexed)
        {
			//init index buffer
			dynMeshLoader->SetIndexBufferType(CoreGraphics::IndexType::Index16);
			dynMeshLoader->SetNumberOfIndices(IndexBufferSize);
			
			//set the index buffers access mode and usage 
			dynMeshLoader->SetIndexAccessMode(CoreGraphics::IndexBuffer::AccessWrite);
			dynMeshLoader->SetVertexUsage(CoreGraphics::VertexBuffer::UsageDynamic);

			primGroup.SetBaseIndex(0);
			primGroup.SetNumIndices(0);
	    }
        else
        {
			dynMeshLoader->SetNumberOfIndices(0);
        }
		Util::Array<CoreGraphics::PrimitiveGroup> primGroupArray;
		primGroupArray.Append(primGroup);
		mesh->SetPrimitiveGroups(primGroupArray); 
        mesh->Load();
    }
    return true;
}

//------------------------------------------------------------------------------
/**
    Begin indexed rendering to the dynamic mesh. The method returns pointers
    to the beginning of the vertex buffer and index buffer, and the
    maximum number of vertices and indices which can be written
    before Swap() or End() must be called.

    @param  vertexPointer   [out] will be filled with a pointer to the vertex buffer
    @param  indexPointer    [out] will be filled with a pointer to the index buffer
    @param  maxNumVertices  [out] max number of vertices before calling Swap() or End()
    @param  maxNumIndices   [out] max number of indices before calling Swap() or End()
*/
void
nDynamicMesh::BeginIndexed(float*& vertexPointer, 
                           ushort*& indexPointer, 
                           int& maxNumVertices, 
                           int& maxNumIndices)
{
    n_assert(this->indexedRendering);
	Ptr<CoreGraphics::Mesh> mesh = this->refMesh.get();
	mesh->ApplyPrimitives(0);
	vertexPointer  = (float*)mesh->GetVertexBuffer()->Map(CoreGraphics::VertexBuffer::MapWriteDiscard);
	indexPointer   = (ushort*)mesh->GetIndexBuffer()->Map(CoreGraphics::IndexBuffer::MapWriteDiscard);
	maxNumVertices = VertexBufferSize;
	maxNumIndices  = IndexBufferSize;
}


//------------------------------------------------------------------------------
/**
    Do an intermediate swap. Call this method when the max number of
    vertices or the max number of indices returned by Begin() have
    been written into the vertex and index buffers. The internal 
    dynamic mesh will be rendered, and render attributes will be returned.
    Note that the contents of the vertex and index buffer will be discarded,
    so everything must be overwritten!

    This method will unlock the global dynamic mesh, immediately render it
    through the RenderDevice, and lock it again.

    @param  numVertices     [in] number of vertices written to the vertex buffer
    @param  numIndices      [in] number of indices written to the index buffer
    @param  vertexPointer   [out] new vertex buffer pointer for writing new vertices
    @param  indexPointer    [out] new index buffer pointer for writing new indices
*/
void
nDynamicMesh::SwapIndexed(int numVertices, int numIndices, float*& vertexPointer, ushort*& indexPointer)
{
    n_assert(this->indexedRendering);
	Ptr<CoreGraphics::RenderDevice> renderDevice = CoreGraphics::RenderDevice::Instance();
	n_assert(renderDevice.isvalid());

    Ptr<CoreGraphics::Mesh> mesh = this->refMesh.get();
	mesh->GetVertexBuffer()->Unmap();
	mesh->GetIndexBuffer()->Unmap();

	//setup new primgroup
	CoreGraphics::PrimitiveGroup primGroup;
	primGroup.SetBaseVertex(0);
	primGroup.SetNumVertices(numVertices);
	primGroup.SetPrimitiveTopology(mesh->GetPrimitiveGroupAtIndex(0).GetPrimitiveTopology());
	primGroup.SetBaseIndex(0);
	primGroup.SetNumIndices(numIndices);
	
	Util::Array<CoreGraphics::PrimitiveGroup> primGroupArray;
	primGroupArray.Append(primGroup);
	mesh->SetPrimitiveGroups(primGroupArray); 
	renderDevice->SetPrimitiveGroup(mesh->GetPrimitiveGroupAtIndex(0));
		
	renderDevice->Draw();
	vertexPointer = (float*)mesh->GetVertexBuffer()->Map(CoreGraphics::VertexBuffer::MapWriteDiscard);
	indexPointer  = (ushort*)mesh->GetIndexBuffer()->Map(CoreGraphics::IndexBuffer::MapWriteDiscard);
}

//------------------------------------------------------------------------------
/**
    Finish indexed rendering. Call this method when no more dynamic geometry 
    needs to be rendered. This method will do a final DrawIndexed() call to
    the renderdevice with the remaining valid vertices and indices.

    @param  numVertices     number of valid vertices in the vertex buffer
    @param  numIndices      number of valid indices in the vertex buffer
*/
void
nDynamicMesh::EndIndexed(int numVertices, int numIndices)
{
    n_assert(this->indexedRendering);
	Ptr<CoreGraphics::RenderDevice> renderDevice = CoreGraphics::RenderDevice::Instance();

	Ptr<CoreGraphics::Mesh> mesh = this->refMesh.get();
    
	mesh->GetVertexBuffer()->Unmap();
	mesh->GetIndexBuffer()->Unmap();

	//setup new primgroup
	CoreGraphics::PrimitiveGroup primGroup;
	primGroup.SetBaseVertex(0);
	primGroup.SetNumVertices(numVertices);
	primGroup.SetPrimitiveTopology(mesh->GetPrimitiveGroupAtIndex(0).GetPrimitiveTopology());
	primGroup.SetBaseIndex(0);
	primGroup.SetNumIndices(numIndices);
		
	Util::Array<CoreGraphics::PrimitiveGroup> primGroupArray;
	primGroupArray.Append(primGroup);
	mesh->SetPrimitiveGroups(primGroupArray); 
	renderDevice->SetPrimitiveGroup(mesh->GetPrimitiveGroupAtIndex(0));

    renderDevice->Draw();
}

//------------------------------------------------------------------------------
/**
    Begin non-indexed rendering to the dynamic mesh. 

    @param  vertexPointer   [out] will be filled with a pointer to the vertex buffer
    @param  maxNumVertices  [out] max number of vertices before calling Swap() or End()
*/
void
nDynamicMesh::Begin(float*& vertexPointer, int& maxNumVertices)
{
	n_assert(!this->indexedRendering);
	Ptr<CoreGraphics::Mesh> mesh = this->refMesh.get();

	mesh->ApplyPrimitives(0);
	vertexPointer  = (float*)mesh->GetVertexBuffer()->Map(CoreGraphics::VertexBuffer::MapWriteDiscard);

	maxNumVertices = VertexBufferSize;
}

//------------------------------------------------------------------------------
/**
    Do an intermediate swap for non-indexed rendering.

    @param  numVertices     [in] number of vertices written to the vertex buffer
    @param  vertexPointer   [out] new vertex buffer pointer for writing new vertices
*/
void
nDynamicMesh::Swap(int numVertices, float*& vertexPointer)
{
    n_assert(!this->indexedRendering);
    Ptr<CoreGraphics::RenderDevice> renderDevice = CoreGraphics::RenderDevice::Instance();

    Ptr<CoreGraphics::Mesh> mesh = this->refMesh.get();
   	mesh->GetVertexBuffer()->Unmap();

    //setup new primgroup
	CoreGraphics::PrimitiveGroup primGroup;
	primGroup.SetBaseVertex(0);
	primGroup.SetNumVertices(numVertices);
	primGroup.SetPrimitiveTopology(mesh->GetPrimitiveGroupAtIndex(0).GetPrimitiveTopology());
	
	Util::Array<CoreGraphics::PrimitiveGroup> primGroupArray;
	primGroupArray.Append(primGroup);
	mesh->SetPrimitiveGroups(primGroupArray); 

	renderDevice->SetPrimitiveGroup(mesh->GetPrimitiveGroupAtIndex(0));
	renderDevice->Draw();

    vertexPointer  = (float*)mesh->GetVertexBuffer()->Map(CoreGraphics::VertexBuffer::MapWriteDiscard);
}

//------------------------------------------------------------------------------
/**
    Finish non-indexed rendering. 

    @param  numVertices     number of valid vertices in the vertex buffer
*/
void
nDynamicMesh::End(int numVertices)
{
    n_assert(!this->indexedRendering);

    Ptr<CoreGraphics::RenderDevice> renderDevice = CoreGraphics::RenderDevice::Instance();
    Ptr<CoreGraphics::Mesh> mesh = this->refMesh.get();
	mesh->GetVertexBuffer()->Unmap();
    if (0 != numVertices)
    {
		//setup new primgroup
		CoreGraphics::PrimitiveGroup primGroup;
		primGroup.SetBaseVertex(0);
		primGroup.SetNumVertices(numVertices);
		primGroup.SetPrimitiveTopology(mesh->GetPrimitiveGroupAtIndex(0).GetPrimitiveTopology());
		
        Util::Array<CoreGraphics::PrimitiveGroup> primGroupArray;
		primGroupArray.Append(primGroup);
		mesh->SetPrimitiveGroups(primGroupArray); 

        renderDevice->SetPrimitiveGroup(mesh->GetPrimitiveGroupAtIndex(0));
        renderDevice->Draw();
    }
}
};// namespace
