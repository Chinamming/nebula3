#pragma once
#ifndef RESOURCES_DYNAMICMESHRESOURCELOADER_H
#define RESOURCES_DYNAMICMESHRESOURCELOADER_H
//------------------------------------------------------------------------------
/**
    @class Resources::DynamicMeshResourceLoader
    
    A resource loader for dynamic meshes. 
    
    (C) 2008 Radon Labs GmbH
*/

#include "resources/resourceloader.h"
#include "coregraphics/memoryindexbufferloader.h"
#include "coregraphics/memoryvertexbufferloader.h"
#include "util/array.h"

namespace CoreGraphics
{
	class Mesh;
}

namespace Resources
{

class DynamicMeshResourceLoader : public ResourceLoader
{
	DeclareClass(DynamicMeshResourceLoader);
public:
    /// constructor
    DynamicMeshResourceLoader();
    /// destructor
    virtual ~DynamicMeshResourceLoader();

	/// called when the resource loader is attached to its resource
    virtual void OnAttachToResource(const Ptr<Resource>& res);
    /// called when the resource loader is removed from its resource
    virtual void OnRemoveFromResource();

	/// called by resource when a load is requested
    virtual bool OnLoadRequested();

	/// sets number of indices, if 0 no index buffer allocated
	void SetNumberOfIndices(SizeT num);
	/// sets number of vertices
	void SetNumberOfVertices(SizeT num);
	
	/// gets the number of vertices
	SizeT GetNumberOfIndices() const;
	/// gets the number of indices
	SizeT GetNumberOfVertices() const;

	/// sets index data	
	void SetIndexData(void* data);
	/// sets vertex data
	void SetVertexData(void* data);
	/// sets the usage of the index buffer
	void SetIndexUsage(CoreGraphics::IndexBuffer::Usage usage);
	/// sets the usage of the vertex buffer
	void SetVertexUsage(CoreGraphics::VertexBuffer::Usage usage);
	/// sets the access mode of the index buffer
	void SetIndexAccessMode(CoreGraphics::IndexBuffer::Access accessMode);
	/// sets the access mode of the vertex buffer
	void SetVertexAccessMode(CoreGraphics::VertexBuffer::Access accessMode);
	/// sets the indexbuffer type
	void SetIndexBufferType(CoreGraphics::IndexType::Code type);
	/// sets the vertex buffer components
	void SetVertexComponents(const Util::Array<CoreGraphics::VertexComponent>& components);
	/// sets the width of a vertex
	void SetVertexWidth(int vertexWidth);
private:
	/// used for loading/intiliazing the vertex buffer
	Ptr<CoreGraphics::MemoryVertexBufferLoader> vertexBufferLoader;
	/// used for loading/intiliazing the index buffer
	Ptr<CoreGraphics::MemoryIndexBufferLoader> indexBufferLoader;
	
	SizeT numIndices;
	SizeT numVertices;
	void* vertexData;
	void* indexData;
	uint vertexWidth;
	CoreGraphics::IndexBuffer::Usage indexUsage;
	CoreGraphics::VertexBuffer::Usage vertexUsage;
	CoreGraphics::IndexBuffer::Access indexAccessMode;
	CoreGraphics::VertexBuffer::Access vertexAccessMode;
	CoreGraphics::IndexType::Code indexBufferType;
	Util::Array<CoreGraphics::VertexComponent> vertexComponents;

	Ptr<CoreGraphics::Mesh> refMesh;
};




//------------------------------------------------------------------------------
/**
*/
inline void 
DynamicMeshResourceLoader::SetNumberOfIndices(SizeT num)
{
	this->numIndices = num;
}
//------------------------------------------------------------------------------
/**
*/
inline void 
DynamicMeshResourceLoader::SetNumberOfVertices(SizeT num)
{
	this->numVertices = num;
}
//------------------------------------------------------------------------------
/**
*/
inline void 
DynamicMeshResourceLoader::SetIndexData(void* data)
{
	n_assert(data);
	this->indexData = data;
}
//------------------------------------------------------------------------------
/**
*/
inline void 
DynamicMeshResourceLoader::SetVertexData(void* data)
{
	n_assert(data);
	this->vertexData;
}
//------------------------------------------------------------------------------
/**
*/
inline void 
DynamicMeshResourceLoader::SetIndexUsage(CoreGraphics::IndexBuffer::Usage usage)
{
	this->indexUsage = usage;
}
//------------------------------------------------------------------------------
/**
*/
inline void DynamicMeshResourceLoader::SetVertexUsage(CoreGraphics::VertexBuffer::Usage usage)
{
	this->vertexUsage = usage;
}
//------------------------------------------------------------------------------
/**
*/
inline void 
DynamicMeshResourceLoader::SetIndexAccessMode(CoreGraphics::IndexBuffer::Access accessMode)
{
	this->indexAccessMode = accessMode;
}
//------------------------------------------------------------------------------
/**
*/
inline void 
DynamicMeshResourceLoader::SetVertexAccessMode(CoreGraphics::VertexBuffer::Access accessMode)
{
	this->vertexAccessMode = accessMode;
}
//------------------------------------------------------------------------------
/**
*/
inline void 
DynamicMeshResourceLoader::SetIndexBufferType(CoreGraphics::IndexType::Code type)
{
}
//------------------------------------------------------------------------------
/**
*/
inline void 
DynamicMeshResourceLoader::SetVertexComponents(const Util::Array<CoreGraphics::VertexComponent>& components)
{
	this->vertexComponents = components;
}
//------------------------------------------------------------------------------
/**
*/
inline void 
DynamicMeshResourceLoader::SetVertexWidth(int width)
{
	this->vertexWidth = width;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT 
DynamicMeshResourceLoader::GetNumberOfIndices() const
{
	return numIndices;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT 
DynamicMeshResourceLoader::GetNumberOfVertices() const
{
	return numVertices;
}


};// namespace Resources



#endif