//------------------------------------------------------------------------------
//  nvx2streamreader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/legacy/nvx2streamreader.h"
#include "coregraphics/memoryvertexbufferloader.h"
#include "coregraphics/memoryindexbufferloader.h"
#include "system/byteorder.h"

#if NEBULA3_LEGACY_SUPPORT
namespace Legacy
{
ImplementClass(Legacy::Nvx2StreamReader, 'N2SR', IO::StreamReader);

using namespace CoreGraphics;
using namespace Util;
using namespace Math;
using namespace Resources;
using namespace System;

//------------------------------------------------------------------------------
/**
*/
Nvx2StreamReader::Nvx2StreamReader() :
    mapPtr(0),
    groupDataPtr(0),
    vertexDataPtr(0),
    indexDataPtr(0),
    groupDataSize(0),
    vertexDataSize(0),
    indexDataSize(0),
    numGroups(0),
    numVertices(0),
    vertexWidth(0),
    numIndices(0),
    numEdges(0),
    vertexComponentMask(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Nvx2StreamReader::~Nvx2StreamReader()
{
    if (this->IsOpen())
    {
        this->Close();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
Nvx2StreamReader::Open()
{
    n_assert(0 == this->primGroups.Size());
    n_assert(this->stream->CanBeMapped());
    n_assert(this->vertexComponents.IsEmpty());
    n_assert(0 == this->mapPtr);
    if (StreamReader::Open())
    {
        // map the stream to memory
        this->mapPtr = this->stream->Map();
        n_assert(0 != this->mapPtr);

        // read data
        this->ReadHeaderData();
        this->ReadPrimitiveGroups();
        this->SetupVertexComponents();
        this->SetupVertexBuffer();
        this->SetupIndexBuffer();
        this->UpdateGroupBoundingBoxes();

        stream->Unmap();
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
Nvx2StreamReader::Close()
{
    this->mapPtr = 0;
    this->groupDataPtr = 0;
    this->vertexDataPtr = 0;
    this->indexDataPtr = 0;
    this->vertexBuffer = 0;
    this->indexBuffer = 0;
    this->primGroups.Clear();
    this->vertexComponents.Clear();
    StreamReader::Close();
}

//------------------------------------------------------------------------------
/**
    This reads the nvx2 header data and checks whether the file is 
    actually an nvx2 file through the magic number. All header data
    will be read into member variables, and pointers to the
    start of the group-, vertex- and index-data will be setup.
*/
void
Nvx2StreamReader::ReadHeaderData()
{
    n_assert(0 != this->mapPtr);
    uint* headerPtr = (uint*) this->mapPtr;

    // check nvx2 magic number
    FourCC magic = ByteOrder::ConvertUInt(ByteOrder::LittleEndian, ByteOrder::Host, *headerPtr++);
    if (magic != FourCC('NVX2'))
    {
        // not a nvx2 file, break hard
        n_error("Nvx2StreamReader: '%s' is not a nvx2 file!", this->stream->GetURI().AsString().AsCharPtr());
    }
    this->numGroups = ByteOrder::ConvertUInt(ByteOrder::LittleEndian, ByteOrder::Host, *headerPtr++);
    this->numVertices = ByteOrder::ConvertUInt(ByteOrder::LittleEndian, ByteOrder::Host,*headerPtr++);
    this->vertexWidth = ByteOrder::ConvertUInt(ByteOrder::LittleEndian, ByteOrder::Host,*headerPtr++);
    this->numIndices = ByteOrder::ConvertUInt(ByteOrder::LittleEndian, ByteOrder::Host,*headerPtr++) * 3;
    this->numEdges = ByteOrder::ConvertUInt(ByteOrder::LittleEndian, ByteOrder::Host,*headerPtr++);
    this->vertexComponentMask = ByteOrder::ConvertUInt(ByteOrder::LittleEndian, ByteOrder::Host,*headerPtr++);
    this->groupDataSize = 6 * sizeof(uint) * this->numGroups;
    this->vertexDataSize = this->numVertices * this->vertexWidth * sizeof(float);
    this->indexDataSize = this->numIndices * sizeof(ushort);
    this->groupDataPtr = headerPtr;
    this->vertexDataPtr = ((uchar*)this->groupDataPtr) + this->groupDataSize;
    this->indexDataPtr = ((uchar*)this->vertexDataPtr) + this->vertexDataSize;
}

//------------------------------------------------------------------------------
/**
*/
void
Nvx2StreamReader::ReadPrimitiveGroups()
{
    n_assert(this->primGroups.IsEmpty());
    n_assert(this->numGroups > 0);
    n_assert(0 != this->groupDataPtr);
    uint* ptr = (uint*) this->groupDataPtr;
    IndexT i;
    for (i = 0; i < this->numGroups; i++)
    {
        PrimitiveGroup primGroup;
        primGroup.SetBaseVertex(ByteOrder::ConvertUInt(ByteOrder::LittleEndian, ByteOrder::Host, *ptr++));
        primGroup.SetNumVertices(ByteOrder::ConvertUInt(ByteOrder::LittleEndian, ByteOrder::Host, *ptr++));
        primGroup.SetBaseIndex(ByteOrder::ConvertUInt(ByteOrder::LittleEndian, ByteOrder::Host, *ptr++) * 3);   // firstTriangle
        primGroup.SetNumIndices(ByteOrder::ConvertUInt(ByteOrder::LittleEndian, ByteOrder::Host, *ptr++) * 3);   // numTriangles
        ptr += 2; // skip firstEdge and numEdges
        primGroup.SetPrimitiveTopology(PrimitiveTopology::TriangleList);
        this->primGroups.Append(primGroup);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
Nvx2StreamReader::SetupVertexComponents()
{
    n_assert(this->vertexComponents.IsEmpty());

    IndexT i;
    for (i = 0; i < 11; i++)
    {
        VertexComponent::SemanticName sem;
        VertexComponent::Format fmt;
        IndexT index = 0;
        if (vertexComponentMask & (1<<i))
        {
            switch (i)
            {
                case 0:     sem = VertexComponent::Position;     fmt=VertexComponent::Float3; break;
                case 1:     sem = VertexComponent::Normal;       fmt=VertexComponent::Float3; break;
                case 2:     sem = VertexComponent::TexCoord;     fmt=VertexComponent::Float2; index = 0; break;
                case 3:     sem = VertexComponent::TexCoord;     fmt=VertexComponent::Float2; index = 1; break;
                case 4:     sem = VertexComponent::TexCoord;     fmt=VertexComponent::Float2; index = 2; break;
                case 5:     sem = VertexComponent::TexCoord;     fmt=VertexComponent::Float2; index = 3; break;
                case 6:     sem = VertexComponent::Color;        fmt=VertexComponent::Float4; break;
                case 7:     sem = VertexComponent::Tangent;      fmt=VertexComponent::Float3; break;
                case 8:     sem = VertexComponent::Binormal;     fmt=VertexComponent::Float3; break;
                case 9:     sem = VertexComponent::SkinWeights;  fmt=VertexComponent::Float4; break;
                case 10:    sem = VertexComponent::SkinJIndices; fmt=VertexComponent::Float4; break;
                default:
                    n_error("Can't happen!");
                    sem = VertexComponent::Position;
                    fmt = VertexComponent::Float3;
                    break;
            }
            this->vertexComponents.Append(VertexComponent(sem, index, fmt));
        }
    }
}

//------------------------------------------------------------------------------
/**
    Since nvx2 files don't contain any bounding box information
    we need to compute per-primitive-group bounding boxes
    manually by walking the triangle indices. This may be inefficient
    with large meshes.
*/
void
Nvx2StreamReader::UpdateGroupBoundingBoxes()
{
    n_assert(0 != this->vertexDataPtr);
    n_assert(0 != this->indexDataPtr);
    n_assert(this->primGroups.Size() > 0);

    float* vertexPtr = (float*) this->vertexDataPtr;
    ushort* indexPtr = (ushort*) this->indexDataPtr;
    IndexT groupIndex;
    for (groupIndex = 0; groupIndex < this->primGroups.Size(); groupIndex++)
    {
        PrimitiveGroup& group = this->primGroups[groupIndex];        
        bbox box;
        box.begin_extend();
        point p;
        IndexT ii;
        for (ii = 0; ii < group.GetNumIndices(); ii++)
        {
            float* curVertexPtr = vertexPtr + (indexPtr[ii + group.GetBaseIndex()] * this->vertexWidth);
            p.set(curVertexPtr[0], curVertexPtr[1], curVertexPtr[2]);
            box.extend(p);
        }
        group.SetBoundingBox(box);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
Nvx2StreamReader::SetupVertexBuffer()
{
    n_assert(0 != this->vertexDataPtr);
    n_assert(this->vertexDataSize > 0);
    n_assert(this->numVertices > 0);    
    n_assert(this->vertexComponents.Size() > 0);

    // endian convert the vertex buffer
    #pragma warning(push)   
    #pragma warning(disable : 4127)  // expression is constant
    if (ByteOrder::Host != ByteOrder::LittleEndian)
    {
        IndexT i;
        SizeT num = this->vertexDataSize / sizeof(float);
        float* ptr = (float*) this->vertexDataPtr;
        for (i = 0; i < num; i++)
        {
            ptr[i] = ByteOrder::ConvertFloat(ByteOrder::LittleEndian, ByteOrder::Host, ptr[i]);
        }
    }
    #pragma warning(pop)

    // setup new vertex buffer
    if (!this->vertexBuffer.isvalid())
    {
        this->vertexBuffer = VertexBuffer::Create();
    }
    if (!this->vertexBufferLoader.isvalid())
    {
        this->vertexBufferLoader = MemoryVertexBufferLoader::Create();
    }
    this->vertexBufferLoader->Setup(this->vertexComponents, this->numVertices, this->vertexDataPtr, this->vertexDataSize);
    this->vertexBuffer->SetLoader(this->vertexBufferLoader.upcast<ResourceLoader>());
    this->vertexBuffer->Load();
    this->vertexBuffer->SetLoader(0);
    this->vertexBufferLoader = 0;
    n_assert(this->vertexBuffer->GetState() == VertexBuffer::Loaded);
}

//------------------------------------------------------------------------------
/**
*/
void
Nvx2StreamReader::SetupIndexBuffer()
{
    n_assert(0 != this->indexDataPtr);
    n_assert(this->indexDataSize > 0);
    n_assert(this->numIndices > 0);
    
    // endian convert the index buffer
    #pragma warning(push)   
    #pragma warning(disable : 4127) // expression is constant
    if (ByteOrder::Host != ByteOrder::LittleEndian)
    {
        IndexT i;
        ushort* ptr = (ushort*) this->indexDataPtr;
        for (i = 0; i < this->numIndices; i++)
        {
            ptr[i] = ByteOrder::ConvertUShort(ByteOrder::LittleEndian, ByteOrder::Host, ptr[i]);
        }
    }
    #pragma warning(pop)

    // setup a new index buffer
    if (!this->indexBuffer.isvalid())
    {
        this->indexBuffer = IndexBuffer::Create();
    }
    if (!this->indexBufferLoader.isvalid())
    {
        this->indexBufferLoader = MemoryIndexBufferLoader::Create();
    }
    this->indexBufferLoader->Setup(IndexType::Index16, this->numIndices, this->indexDataPtr, this->indexDataSize);
    this->indexBuffer->SetLoader(this->indexBufferLoader.upcast<ResourceLoader>());
    this->indexBuffer->Load();
    this->indexBuffer->SetLoader(0);
    this->indexBufferLoader = 0;
    n_assert(this->indexBuffer->GetState() == IndexBuffer::Loaded);
}

} // namespace Legacy

#endif // NEBULA3_LEGACY_SUPPORT