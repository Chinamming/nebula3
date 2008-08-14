#ifndef N_DYNAMICMESH_H
#define N_DYNAMICMESH_H
//------------------------------------------------------------------------------
/**
    @class nDynamicMesh
    @ingroup Particle

    Helper class for rendering dynamic geometry, simplifies writing
    to a dynamic mesh.

    The first thing you should do to use a @ref nDynamicMesh is that initialization  
    of the dynamic mesh object. This can be done by calling nDynamicMesh::Initialize().
    @code
	Util::Array<CoreGraphics::VertexComponent> vertexComponents;
	vertexComponents.Append(CoreGraphics::VertexComponent(CoreGraphics::VertexComponent::Position,0,CoreGraphics::VertexComponent::Float4));
	vertexComponents.Append(CoreGraphics::VertexComponent(CoreGraphics::VertexComponent::Normal,0,CoreGraphics::VertexComponent::Float4));
	vertexComponents.Append(CoreGraphics::VertexComponent(CoreGraphics::VertexComponent::TexCoord,0,CoreGraphics::VertexComponent::Float2));
	vertexComponents.Append(CoreGraphics::VertexComponent(CoreGraphics::VertexComponent::TexCoord,1,CoreGraphics::VertexComponent::Float2));
	vertexComponents.Append(CoreGraphics::VertexComponent(CoreGraphics::VertexComponent::Color,0,CoreGraphics::VertexComponent::Float4));

    dynMesh.Initialize("part_system_resource_2",
                       CoreGraphics::PrimitiveTopology::Code::TriangleList, 
                       vertexComponente, 
                       false  
                       );
    @endcode

    Before filling the mesh, nDynamicMesh::Begin() or nDynamicMesh::BeginIndexed() 
    should be called to lock the vertex (and index buffer if you use indexed
    dynamic mesh, the case nDynamicMesh::BeginIndexed() is used).
    @code
    float* dstVertices = 0; // Retrived vertex buffer pointer.
    int maxVertices    = 0; // Max number of vertices which retrived from created(or shared) mesh

    dynMesh.Begin(dstVertices, maxVertices);
    @endcode

    Now, you can fill vertices via retrieved vertex buffer pointer.
    @code
    dstVertices[curIndex++] = curPosition.x;
    dstVertices[curIndex++] = curPosition.y;
    dstVertices[curIndex++] = curPosition.z;
    ...
    @endcode

    During you fill vertices, you should check that the mesh is full. 
    If it is, you should throw away aleady filled vertices by calling 
    nDynamicMesh::Swap()(or nDynamicMesh::SwapIndexed() for indexed vertices)
    @code
    dynMesh.Swap(curVertex, dstVertices);
    @endcode

    After you fill all vertices, call nDynamicMesh::End() to render the mesh.
    @code
    dynMesh.End(curVertex);
    @endcode

    See ParticleEmitter::Render() for complete source code of this section.
    (C) 2008 RadonLabs GmbH
*/

#include "core/refcounted.h"

#include "coregraphics/primitivetopology.h"
#include "util/string.h"
#include "coregraphics/mesh.h"


//------------------------------------------------------------------------------
namespace Nebula2
{
class nDynamicMesh : public Core::RefCounted
{
	DeclareClass(nDynamicMesh);
	
public:
    /// constructor
    nDynamicMesh();
    /// destructor
    ~nDynamicMesh();
    /// initialize the dynamic mesh
	bool Initialize(const Util::Atom<Util::String>& resourceID,CoreGraphics::PrimitiveTopology::Code primType, const Util::Array<CoreGraphics::VertexComponent>& vertexComponents, bool indexedRendering);
    /// if this returns false, call Initialize()
    bool IsValid() const;
    /// begin indexed rendering
    void BeginIndexed(float*& vertexPointer, ushort*& indexPointer, int& maxNumVertices, int& maxNumIndices);
    /// do an intermediate swap for indexed rendering
    void SwapIndexed(int numValidVertices, int numValidIndices, float*& vertexPointer, ushort*& indexPointer);
    /// end indexed rendering
    void EndIndexed(int numValidVertices, int numValidIndices);
    /// begin non-indexed rendering
    void Begin(float*& vertexPointer, int& maxNumVertices);
    /// do an intermediate swap for non-indexed rendering
    void Swap(int numValidVertices, float*& vertexPointer);
    /// end non-indexed rendering
    void End(int numValidVertices);

protected:
    enum
    {
        IndexBufferSize = 300,                     // number of vertices
        VertexBufferSize  = 3 * IndexBufferSize,    // number of indices
    };
    bool indexedRendering;
	Ptr<CoreGraphics::Mesh> refMesh;
	CoreGraphics::PrimitiveTopology::Code primitiveType;
};

};// namespace Nebula2
//------------------------------------------------------------------------------
#endif

