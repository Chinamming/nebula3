#include "stdneb.h"
#include "dynamicmeshresourceloader.h"
#include "coregraphics/mesh.h"


namespace Resources
{
ImplementClass(Resources::DynamicMeshResourceLoader, 'DYRL', Resources::ResourceLoader);

//------------------------------------------------------------------------------
/**
*/
DynamicMeshResourceLoader::DynamicMeshResourceLoader():
	indexData(0),
	vertexData(0),
	numVertices(0),
	numIndices(0),
	vertexWidth(0)

{
	
}

//------------------------------------------------------------------------------
/**
*/
DynamicMeshResourceLoader::~DynamicMeshResourceLoader()
{
	this->refMesh = 0;
}

//------------------------------------------------------------------------------
/**
*/
void 
DynamicMeshResourceLoader::OnAttachToResource(const Ptr<Resource>& res)
{
	n_assert(res->IsA(CoreGraphics::Mesh::RTTI));
	refMesh = res.downcast<CoreGraphics::Mesh>();
	
}

//------------------------------------------------------------------------------
/**
*/
void 
DynamicMeshResourceLoader::OnRemoveFromResource()
{
}


//------------------------------------------------------------------------------
/**
*/
bool 
DynamicMeshResourceLoader::OnLoadRequested()
{
	n_assert(this->numVertices > 0);
	n_assert(this->vertexWidth > 0);
	
	//check if a index buffer should be initialized
	if(0 > this->numIndices)
	{
		this->indexBufferLoader = CoreGraphics::MemoryIndexBufferLoader::Create();
		n_assert(this->indexBufferLoader.isvalid());
		//check if index buffer should be initialized empty
		if(0 == this->indexData)
		{
			this->indexBufferLoader->Setup(this->indexBufferType,this->numIndices,
											this->indexBufferType * numVertices,this->indexUsage,
											this->indexAccessMode);
		}
		else
		{
			this->indexBufferLoader->Setup(this->indexBufferType,this->numIndices,this->indexData,
											this->indexBufferType * numVertices,this->indexUsage,
											this->indexAccessMode);
		}
		//setup index buffer
		if (!this->refMesh->GetIndexBuffer().isvalid())
		{
			this->refMesh->SetIndexBuffer(CoreGraphics::IndexBuffer::Create());
		}
		this->refMesh->GetIndexBuffer()->SetLoader(indexBufferLoader.upcast<Resources::ResourceLoader>());
		this->refMesh->GetIndexBuffer()->Load();
		this->refMesh->GetIndexBuffer()->SetLoader(0);
		this->indexBufferLoader = 0;
		n_assert(this->refMesh->GetIndexBuffer()->GetState() == CoreGraphics::IndexBuffer::Loaded);
	
	
	}
	

	
	//setup vertex buffer
	this->vertexBufferLoader = CoreGraphics::MemoryVertexBufferLoader::Create();
	n_assert(this->vertexBufferLoader.isvalid());
	if(0 == vertexData)
	{
		this->vertexBufferLoader->Setup(this->vertexComponents,this->numVertices,
										this->numVertices*this->vertexWidth*sizeof(float),
										this->vertexUsage,this->vertexAccessMode);
				
	}
	else
	{
		this->vertexBufferLoader->Setup(this->vertexComponents,this->numVertices,this->vertexData,
											this->numVertices*this->vertexWidth*sizeof(float),
											this->vertexUsage,this->vertexAccessMode);
		
	}
	//load this meshs vertex buffer
	this->refMesh->SetVertexBuffer(CoreGraphics::VertexBuffer::Create());
	this->refMesh->GetVertexBuffer()->SetLoader(vertexBufferLoader.upcast<Resources::ResourceLoader>());
	this->refMesh->GetVertexBuffer()->Load();
	this->refMesh->GetVertexBuffer()->SetLoader(0);
	this->vertexBufferLoader = 0;
    n_assert(this->refMesh->vertexBuffer->GetState() == CoreGraphics::VertexBuffer::Loaded);
	this->SetState(Resource::Loaded);
	
	return true;
}

};// namespace Resources