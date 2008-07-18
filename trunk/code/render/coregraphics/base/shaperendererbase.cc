//------------------------------------------------------------------------------
//  shaperendererbase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/base/shaperendererbase.h"
#include "coregraphics/shaderserver.h"
#include "resources/resourceid.h"

namespace Base
{
ImplementClass(Base::ShapeRendererBase, 'SRBS', Core::RefCounted);
ImplementSingleton(Base::ShapeRendererBase);

using namespace CoreGraphics;
using namespace Resources;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
ShapeRendererBase::ShapeRendererBase() :
    isOpen(false),
    inBegin(false)
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
ShapeRendererBase::~ShapeRendererBase()
{
    n_assert(!this->isOpen);
    n_assert(!this->inBegin);
    DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeRendererBase::Open()
{
    n_assert(!this->isOpen);
    n_assert(!this->inBegin);
    n_assert(!this->shapeShader.isvalid());
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
    n_assert(!this->inBegin);
    n_assert(this->shapeShader.isvalid());
    this->isOpen = false;
    this->shapeShader->Discard();
    this->shapeShader = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeRendererBase::Begin()
{
    n_assert(this->isOpen);
    n_assert(!this->inBegin);
    this->inBegin = true;

    // begin rendering through shape shader
    this->shapeShader->Begin();
    this->shapeShader->BeginPass(0);
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeRendererBase::DrawShape(const matrix44& modelTransform, ShapeType shapeType, const Math::float4& color)
{
    n_assert(this->inBegin);
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeRendererBase::DrawPrimitives(const matrix44& modelTransform, PrimitiveTopology::Code topology, SizeT numPrimitives, float* vertices, SizeT vertexWidth, const Math::float4& color)
{
    n_assert(this->inBegin);
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeRendererBase::DrawIndexedPrimitives(const matrix44& modelTransform, PrimitiveTopology::Code topology, SizeT numPrimitives, float* vertices, SizeT numVertices, SizeT vertexWidth, void* indices, IndexType::Code indexType, const Math::float4& color)
{
    n_assert(this->inBegin);
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeRendererBase::End()
{
    n_assert(this->inBegin);
    this->inBegin = false;
    this->shapeShader->EndPass();
    this->shapeShader->End();
}

} // namespace Base
