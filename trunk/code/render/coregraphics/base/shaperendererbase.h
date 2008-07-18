#pragma once
#ifndef BASE_SHAPERENDERERBASE_H
#define BASE_SHAPERENDERERBASE_H
//------------------------------------------------------------------------------
/**
    @class Base::ShapeRendererBase
    
    Base class of ShapeRenderer, can render a number of shapes, mainly 
    for debug visualization.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "math/matrix44.h"
#include "math/float4.h"
#include "coregraphics/primitivetopology.h"
#include "coregraphics/indextype.h"
#include "coregraphics/shaderinstance.h"

//------------------------------------------------------------------------------
namespace Base
{
class ShapeRendererBase : public Core::RefCounted
{
    DeclareClass(ShapeRendererBase);
    DeclareSingleton(ShapeRendererBase);
public:
    /// shape type
    enum ShapeType
    {
        Box,
        Sphere,
        Cylinder,
        Torus,
        Teapot,

        NumShapeTypes,
    };

    /// constructor
    ShapeRendererBase();
    /// destructor
    virtual ~ShapeRendererBase();
    
    /// open the shape renderer
    void Open();
    /// close the shape renderer
    void Close();
    /// return true if open
    bool IsOpen() const;
    
    /// begin drawing shapes
    void Begin();
    /// draw a unit box
    void DrawShape(const Math::matrix44& modelTransform, ShapeType shapeType, const Math::float4& color);
    /// draw primitives
    void DrawPrimitives(const Math::matrix44& modelTransform, CoreGraphics::PrimitiveTopology::Code topology, SizeT numPrimitives, float* vertices, SizeT vertexWidth, const Math::float4& color);
    /// draw indexed primitives
    void DrawIndexedPrimitives(const Math::matrix44& modelTransform, CoreGraphics::PrimitiveTopology::Code topology, SizeT numPrimitives, float* vertices, SizeT numVertices, SizeT vertexWidth, void* indices, CoreGraphics::IndexType::Code indexType, const Math::float4& color);
    /// end drawing shapes
    void End();

protected:
    bool isOpen;
    bool inBegin;
    Ptr<CoreGraphics::ShaderInstance> shapeShader;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
ShapeRendererBase::IsOpen() const
{
    return this->isOpen;
}

} // namespace Base
//------------------------------------------------------------------------------
#endif

    