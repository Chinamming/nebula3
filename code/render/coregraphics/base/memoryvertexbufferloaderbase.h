#pragma once
#ifndef BASE_MEMORYVERTEXBUFFERLOADERBASE_H
#define BASE_MEMORYVERTEXBUFFERLOADERBASE_H
//------------------------------------------------------------------------------
/**
    @class Base::MemoryVertexBufferLoaderBase
    
    Base resource loader class for initializing an vertex buffer from
    data in memory.
    
    (C) 2007 Radon Labs GmbH
*/
#include "resources/resourceloader.h"
#include "coregraphics/vertexcomponent.h"
#include "coregraphics/vertexbuffer.h"

//------------------------------------------------------------------------------
namespace Base
{
class MemoryVertexBufferLoaderBase : public Resources::ResourceLoader
{
    DeclareClass(MemoryVertexBufferLoaderBase);
public:
    /// constructor
    MemoryVertexBufferLoaderBase();
    /// setup vertex buffer data, must remain valid until OnLoadRequested() is called!
    void Setup(const Util::Array<CoreGraphics::VertexComponent>& vertexComponents, SizeT numVertices, void* ptr, SizeT numBytes);
	/// setup a empty vertex buffer
	void Setup(const Util::Array<CoreGraphics::VertexComponent>& vertexComponents, SizeT numVertices,SizeT numBytes,
				CoreGraphics::VertexBuffer::Usage usage, CoreGraphics::VertexBuffer::Access access);
	/// setup a vertex buffer, vertex buffer data, must remain valid until OnLoadRequested() is called!
	void Setup(const Util::Array<CoreGraphics::VertexComponent>& vertexComponents, SizeT numVertices, void* ptr,SizeT numBytes,
				CoreGraphics::VertexBuffer::Usage usage, CoreGraphics::VertexBuffer::Access access);
protected:
    Util::Array<CoreGraphics::VertexComponent> vertexComponents;
    SizeT numVertices;
    void* vertexDataPtr;
    SizeT vertexDataSize;
	CoreGraphics::VertexBuffer::Usage vertexBufferUsage;
	CoreGraphics::VertexBuffer::Access accessMode;
};

} // namespace Base
//------------------------------------------------------------------------------
#endif
