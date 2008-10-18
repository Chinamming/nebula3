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
#include "coregraphics/shape.h"
#include "threading/threadid.h"

//------------------------------------------------------------------------------
namespace Base
{
class ShapeRendererBase : public Core::RefCounted
{
    __DeclareClass(ShapeRendererBase);
    __DeclareSingleton(ShapeRendererBase);
public:
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

    /// delete shapes of given thread id
    void DeleteShapesByThreadId(Threading::ThreadId threadId);
    /// add a shape for deferred rendering (can be called from outside render loop)
    void AddShape(const CoreGraphics::Shape& shape);
    /// add multiple shapes
    void AddShapes(const Util::Array<CoreGraphics::Shape>& shapeArray);
    /// draw deferred shapes and clear deferred stack, must be called inside render loop
    void DrawShapes();

protected:
    bool isOpen;
    Ptr<CoreGraphics::ShaderInstance> shapeShader;
    Util::Array<CoreGraphics::Shape> shapes;
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

    