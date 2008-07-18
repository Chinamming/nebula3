//------------------------------------------------------------------------------
//  memoryindexbufferloaderbase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/base/memoryindexbufferloaderbase.h"

namespace Base
{
ImplementClass(Base::MemoryIndexBufferLoaderBase, 'MILB', Resources::ResourceLoader);

using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
MemoryIndexBufferLoaderBase::MemoryIndexBufferLoaderBase() :
    indexType(IndexType::None),
    numIndices(0),
    indexDataPtr(0),
    indexDataSize(0)
{
	
    // empty
}

//------------------------------------------------------------------------------
/**
    Setup all information needed to initialize the IndexBuffer resource.
    The data must remain valid until OnLoadRequested() is called (which
    will invaliate the data).

*/
void
MemoryIndexBufferLoaderBase::Setup(IndexType::Code type, SizeT num, void* ptr, SizeT numBytes)
{
    n_assert((type == IndexType::Index16) || (type == IndexType::Index32));
    n_assert(num > 0);
    n_assert(numBytes == (IndexType::SizeOf(type) * num));
    n_assert(0 != ptr);
    this->indexType = type;
    this->numIndices = num;
    this->indexDataPtr = ptr;
    this->indexDataSize = numBytes;

	//use default options
	this->indexBufferUsage = CoreGraphics::IndexBuffer::UsageImmutable;
	this->accessMode = CoreGraphics::IndexBuffer::AccessNone;
}
//------------------------------------------------------------------------------
/**
    Setup all information needed to initialize a empty IndexBuffer resource.
*/

void 
MemoryIndexBufferLoaderBase::Setup(IndexType::Code type, SizeT num,  SizeT numBytes, 
				CoreGraphics::IndexBuffer::Usage usage, CoreGraphics::IndexBuffer::Access access )
{
	n_assert((type == IndexType::Index16) || (type == IndexType::Index32));
    n_assert(num > 0);
    n_assert(numBytes == (IndexType::SizeOf(type) * num));
	this->indexType = type;
    this->numIndices = num;
	this->indexDataSize = numBytes;

	//start as empty indexbuffer
    this->indexDataPtr = 0;
		
	this->indexBufferUsage = usage;
	this->accessMode = accessMode;
	

}

//------------------------------------------------------------------------------
/**
    Setup all information needed to initialize a IndexBuffer resource.
*/

void 
MemoryIndexBufferLoaderBase::Setup(IndexType::Code type, SizeT num, void* ptr, SizeT numBytes, 
				CoreGraphics::IndexBuffer::Usage usage, CoreGraphics::IndexBuffer::Access access )
{
	n_assert((type == IndexType::Index16) || (type == IndexType::Index32));
    n_assert(num > 0);
    n_assert(numBytes == (IndexType::SizeOf(type) * num));
    n_assert(0 != ptr);
	
	this->indexType = type;
    this->numIndices = num;

    this->indexDataPtr = ptr;
    this->indexDataSize = numBytes;
		
	this->indexBufferUsage = usage;
	this->accessMode = accessMode;
	

}

}; // namespace Base
