//------------------------------------------------------------------------------
//  memoryvertexbufferloaderbase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/base/memoryvertexbufferloaderbase.h"

namespace Base
{
ImplementClass(Base::MemoryVertexBufferLoaderBase, 'MVLB', Resources::ResourceLoader);

//------------------------------------------------------------------------------
/**
*/
MemoryVertexBufferLoaderBase::MemoryVertexBufferLoaderBase() :
    numVertices(0),
    vertexDataPtr(0),
    vertexDataSize(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Setup all information needed to initialize the VertexBuffer resource.
    The data must remain valid until OnLoadRequested() is called (which
    will invaliate the data).
*/
void
MemoryVertexBufferLoaderBase::Setup(const Util::Array<CoreGraphics::VertexComponent>& components, SizeT num, void* ptr, SizeT numBytes)
{
    n_assert(num > 0);
    n_assert(0 != ptr);
    this->vertexComponents = components;
    this->numVertices = num;
	this->vertexDataPtr = ptr;
	this->vertexDataSize = numBytes;
	//set default settings
	this->vertexBufferUsage = CoreGraphics::VertexBuffer::UsageImmutable;
	this->accessMode = CoreGraphics::VertexBuffer::AccessNone;
	
}



//------------------------------------------------------------------------------
/**
    Setup all information needed to initialize a empty VertexBuffer resource.
*/

void 
MemoryVertexBufferLoaderBase::Setup(const Util::Array<CoreGraphics::VertexComponent>& vertexComponents, SizeT num,SizeT numBytes,
				CoreGraphics::VertexBuffer::Usage usage, CoreGraphics::VertexBuffer::Access access)
{
	n_assert(num > 0);
	
	
	this->vertexComponents = vertexComponents;
	this->numVertices = num;
	//start as empty vertexbuffer
	this->vertexDataPtr = 0;
    this->vertexDataSize = numBytes;
	
	
	this->vertexBufferUsage = usage;
	this->accessMode = access;
	

}

//------------------------------------------------------------------------------
/**
    Setup all information needed to initialize the VertexBuffer resource.
    The data must remain valid until OnLoadRequested() is called (which
    will invaliate the data).
*/
void
MemoryVertexBufferLoaderBase::Setup(const Util::Array<CoreGraphics::VertexComponent>& components, SizeT num, void* ptr, SizeT numBytes,
									CoreGraphics::VertexBuffer::Usage usage, CoreGraphics::VertexBuffer::Access access)
{
    n_assert(num > 0);
    n_assert(0 != ptr);
    this->vertexComponents = components;
    this->numVertices = num;
	this->vertexDataPtr = ptr;
	this->vertexDataSize = numBytes;
		
	this->vertexBufferUsage = usage;
	this->accessMode = access;
	
}

}; // namespace Base