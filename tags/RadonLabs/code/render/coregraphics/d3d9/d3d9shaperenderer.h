#pragma once
#ifndef DIRECT3D9_D3D9SHAPERENDERER_H
#define DIRECT3D9_D3D9SHAPERENDERER_H
//------------------------------------------------------------------------------
/**
    @class Direct3D9::D3D9ShapeRenderer
  
    D3D9 implementation of ShapeRenderer.
    
    (C) 2007 Radon Labs GmbH
*/    
#include "coregraphics/base/shaperendererbase.h"

//------------------------------------------------------------------------------
namespace Direct3D9
{
class D3D9ShapeRenderer : public Base::ShapeRendererBase
{
    DeclareClass(D3D9ShapeRenderer);
    DeclareSingleton(D3D9ShapeRenderer);
public:
    /// constructor
    D3D9ShapeRenderer();
    /// destructor
    virtual ~D3D9ShapeRenderer();
    
    /// open the shape renderer
    void Open();
    /// close the shape renderer
    void Close();
    
    /// draw a shape
    void DrawShape(const Math::matrix44& modelTransform, ShapeType shapeType, const Math::float4& color);
    /// draw primitives
    void DrawPrimitives(const Math::matrix44& modelTransform, CoreGraphics::PrimitiveTopology::Code topology, SizeT numPrimitives, float* vertices, SizeT vertexWidth, const Math::float4& color);
    /// draw indexed primitives
    void DrawIndexedPrimitives(const Math::matrix44& modelTransform, CoreGraphics::PrimitiveTopology::Code topology, SizeT numPrimitives, float* vertices, SizeT numVertices, SizeT vertexWidth, void* indices, CoreGraphics::IndexType::Code indexType, const Math::float4& color);

protected:
    ID3DXMesh* shapeMeshes[NumShapeTypes];
    Ptr<CoreGraphics::ShaderVariable> modelViewProj;
    Ptr<CoreGraphics::ShaderVariable> diffuseColor;
};

} // namespace Direct3D9
//------------------------------------------------------------------------------
#endif
