#pragma once
#ifndef LEGACY_NVX2STREAMREADER_H
#define LEGACY_NVX2STREAMREADER_H
//------------------------------------------------------------------------------
/**
    @class Legacy::Nvx2StreamReader
  
    A stream reader which reads legacy nvx2 binary mesh files.

    NOTE: this class exists purely for debugging and shouldn't be used in
    production code!

    (C) 2007 Radon Labs GmbH
*/    
#include "core/config.h"
#if NEBULA3_LEGACY_SUPPORT
#include "io/streamreader.h"
#include "coregraphics/vertexbuffer.h"
#include "coregraphics/indexbuffer.h"
#include "coregraphics/primitivegroup.h"
#include "coregraphics/base/vertexbufferbase.h"
#include "coregraphics/base/memoryvertexbufferloaderbase.h"
#include "coregraphics/base/indexbufferbase.h"
#include "coregraphics/base/memoryindexbufferloaderbase.h"

//------------------------------------------------------------------------------
namespace Legacy
{
class Nvx2StreamReader : public IO::StreamReader
{
    DeclareClass(Nvx2StreamReader);
public:
    /// contructor
    Nvx2StreamReader();
    /// destructor
    virtual ~Nvx2StreamReader();
    /// begin reading from the stream, read entire data
    virtual bool Open();
    /// end reading from the stream, destroys loaded objects
    virtual void Close();
    /// get vertex buffer
    const Ptr<Base::VertexBufferBase>& GetVertexBuffer() const;
    /// get index buffer
    const Ptr<Base::IndexBufferBase>& GetIndexBuffer() const;
    /// get primitive groups
    const Util::Array<CoreGraphics::PrimitiveGroup>& GetPrimitiveGroups() const;

    /// optional: set other vertex and index buffer and its loaders
    /// set vertex buffer type and loader
    void SetVertexBuffer(const Ptr<Base::VertexBufferBase>& vBuffer, const Ptr<Base::MemoryVertexBufferLoaderBase>& vBufferLoader);
    /// set vertex buffer type and loader
    void SetIndexBuffer(const Ptr<Base::IndexBufferBase>& iBuffer, const Ptr<Base::MemoryIndexBufferLoaderBase>& iBufferLoader);

private:
    /// read header data from stream
    void ReadHeaderData();
    /// read primitive groups from stream
    void ReadPrimitiveGroups();
    /// setup vertex components array
    void SetupVertexComponents();
    /// update primitive group bounding boxes
    void UpdateGroupBoundingBoxes();
    /// setup the vertex buffer object
    void SetupVertexBuffer();
    /// setup the index buffer object
    void SetupIndexBuffer();

    Ptr<Base::VertexBufferBase> vertexBuffer;
    Ptr<Base::IndexBufferBase> indexBuffer;
    Ptr<Base::MemoryVertexBufferLoaderBase> vertexBufferLoader;    
    Ptr<Base::MemoryIndexBufferLoaderBase> indexBufferLoader;

    Util::Array<CoreGraphics::PrimitiveGroup> primGroups;
    void* mapPtr;
    void* groupDataPtr;
    void* vertexDataPtr;
    void* indexDataPtr;

    SizeT groupDataSize;
    SizeT vertexDataSize;
    SizeT indexDataSize;
    
    uint numGroups;
    uint numVertices;
    uint vertexWidth;
    uint numIndices;
    uint numEdges;
    uint vertexComponentMask;
    Util::Array<CoreGraphics::VertexComponent> vertexComponents;   
};

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Base::VertexBufferBase>&
Nvx2StreamReader::GetVertexBuffer() const
{
    return this->vertexBuffer;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Base::IndexBufferBase>&
Nvx2StreamReader::GetIndexBuffer() const
{
    return this->indexBuffer;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<CoreGraphics::PrimitiveGroup>&
Nvx2StreamReader::GetPrimitiveGroups() const
{
    return this->primGroups;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
Nvx2StreamReader::SetVertexBuffer(const Ptr<Base::VertexBufferBase>& vBuffer, const Ptr<Base::MemoryVertexBufferLoaderBase>& vBufferLoader)
{
    this->vertexBuffer = vBuffer;
    this->vertexBufferLoader = vBufferLoader;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
Nvx2StreamReader::SetIndexBuffer(const Ptr<Base::IndexBufferBase>& iBuffer, const Ptr<Base::MemoryIndexBufferLoaderBase>& iBufferLoader)
{
    this->indexBuffer = iBuffer;
    this->indexBufferLoader = iBufferLoader;
}

} // namespace Legacy
//------------------------------------------------------------------------------
#endif // LEGACY_NVX2STREAMREADER_H
#endif // NEBULA3_LEGACY_SUPPORT