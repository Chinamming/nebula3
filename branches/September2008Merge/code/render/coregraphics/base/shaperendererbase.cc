//------------------------------------------------------------------------------
//  shaperendererbase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/base/shaperendererbase.h"
#include "coregraphics/shaderserver.h"
#include "resources/resourceid.h"
#include "threading/threadid.h"

namespace Base
{
__ImplementClass(Base::ShapeRendererBase, 'SRBS', Core::RefCounted);
__ImplementSingleton(Base::ShapeRendererBase);

using namespace Threading;
using namespace CoreGraphics;
using namespace Resources;
using namespace Math;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
ShapeRendererBase::ShapeRendererBase() :
    isOpen(false)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
ShapeRendererBase::~ShapeRendererBase()
{
    n_assert(!this->isOpen);
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeRendererBase::Open()
{
    n_assert(!this->isOpen);
    n_assert(!this->shapeShader.isvalid());
    n_assert(this->shapes.IsEmpty());
    this->isOpen = true;

    // create shape shader instance
    this->shapeShader = ShaderServer::Instance()->CreateShaderInstance(ResourceId("shd:shape"));
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeRendererBase::Close()
{
    n_assert(this->isOpen);
    n_assert(this->shapeShader.isvalid());
    this->isOpen = false;
    this->shapeShader->Discard();
    this->shapeShader = 0;
    this->shapes.Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeRendererBase::DeleteShapesByThreadId(Threading::ThreadId threadId)
{
    n_assert(this->IsOpen());
    IndexT i;
    for (i = this->shapes.Size() - 1; i != InvalidIndex; i--)
    {
        ThreadId shapeThreadId = this->shapes[i].GetThreadId();
        n_assert(shapeThreadId != InvalidThreadId);
        if (shapeThreadId == threadId)
        {
            this->shapes.EraseIndex(i);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeRendererBase::AddShape(const Shape& shape)
{
    n_assert(this->IsOpen());
    this->shapes.Append(shape);
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeRendererBase::AddShapes(const Array<Shape>& shapeArray)
{
    n_assert(this->IsOpen());
    this->shapes.AppendArray(shapeArray);
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeRendererBase::DrawShapes()
{
    // override in subclass!
    n_error("ShapeRendererBase::DrawShapes() called!");
}

} // namespace Base
