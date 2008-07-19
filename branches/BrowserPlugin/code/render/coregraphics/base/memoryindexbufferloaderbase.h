#pragma once
#ifndef BASE_MEMORYINDEXBUFFERLOADERBASE_H
#define BASE_MEMORYINDEXBUFFERLOADERBASE_H
//------------------------------------------------------------------------------
/**
    @class Base::MemoryIndexBufferLoaderBase
    
    Base resource loader class for initializing an index buffer from
    data in memory.
    
    (C) 2007 Radon Labs GmbH
*/
#include "resources/resourceloader.h"
#include "coregraphics/indexbuffer.h"
#include "coregraphics/indextype.h"

//------------------------------------------------------------------------------
namespace Base
{
class MemoryIndexBufferLoaderBase : public Resources::ResourceLoader
{
    DeclareClass(MemoryIndexBufferLoaderBase);
public:
    /// constructor
    MemoryIndexBufferLoaderBase();
    /// setup index buffer data, must remain valid until OnLoadRequested() is called!
    void Setup(CoreGraphics::IndexType::Code indexType, SizeT numIndices, void* ptr, SizeT numBytes);
	/// setup a empty index buffer
	void Setup(CoreGraphics::IndexType::Code indexType, SizeT numIndices,SizeT numBytes, 
		CoreGraphics::IndexBuffer::Usage usage, CoreGraphics::IndexBuffer::Access access);
	/// setup index buffer data, must remain valid until OnLoadRequested() is called!
	void Setup(CoreGraphics::IndexType::Code type, SizeT num, void* ptr, SizeT numBytes, 
				CoreGraphics::IndexBuffer::Usage usage, CoreGraphics::IndexBuffer::Access access); 
	

protected:
    CoreGraphics::IndexType::Code indexType;
    SizeT numIndices;
    void* indexDataPtr;
    SizeT indexDataSize;
	CoreGraphics::IndexBuffer::Usage indexBufferUsage;
	CoreGraphics::IndexBuffer::Access accessMode;
};
        
} // namespace CoreGraphics
//------------------------------------------------------------------------------
#endif
