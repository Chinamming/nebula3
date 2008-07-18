//------------------------------------------------------------------------------
//  d3d9shaprenderer.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/d3d9/d3d9shaperenderer.h"
#include "coregraphics/d3d9/d3d9renderdevice.h"
#include "coregraphics/d3d9/d3d9types.h"
#include "coregraphics/transformdevice.h"

namespace Direct3D9
{
ImplementClass(Direct3D9::D3D9ShapeRenderer, 'D9SR', Base::ShapeRendererBase);

using namespace Math;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
D3D9ShapeRenderer::D3D9ShapeRenderer()
{
    Memory::Clear(this->shapeMeshes, sizeof(this->shapeMeshes));
}

//------------------------------------------------------------------------------
/**
*/
D3D9ShapeRenderer::~D3D9ShapeRenderer()
{
    n_assert(!this->IsOpen());
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9ShapeRenderer::Open()
{
    n_assert(!this->IsOpen());

    // call parent class
    ShapeRendererBase::Open();

    // create D3DX shapes
    HRESULT hr;
    IDirect3DDevice9* d3d9Dev = D3D9RenderDevice::Instance()->GetDirect3DDevice();
    n_assert(0 != d3d9Dev);
    hr = D3DXCreateBox(d3d9Dev, 1.0f, 1.0f, 1.0f, &this->shapeMeshes[Box], NULL);
    n_assert(SUCCEEDED(hr));
    hr = D3DXCreateSphere(d3d9Dev, 1.0f, 10, 5, &this->shapeMeshes[Sphere], NULL);
    n_assert(SUCCEEDED(hr));
    hr = D3DXCreateCylinder(d3d9Dev, 1.0f, 1.0f, 1.0f, 10, 1, &this->shapeMeshes[Cylinder], NULL);
    n_assert(SUCCEEDED(hr));
    hr = D3DXCreateTorus(d3d9Dev, 1.0f, 1.0f, 10, 10, &this->shapeMeshes[Torus], NULL);
    n_assert(SUCCEEDED(hr));
    hr = D3DXCreateTeapot(d3d9Dev, &this->shapeMeshes[Teapot], NULL);
    n_assert(SUCCEEDED(hr));   
    
    // lookup ModelViewProjection shader variable
    this->modelViewProj = this->shapeShader->GetVariableBySemantic(ShaderVariable::Semantic("ModelViewProjection"));
    this->diffuseColor  = this->shapeShader->GetVariableBySemantic(ShaderVariable::Semantic("MatDiffuse"));
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9ShapeRenderer::Close()
{
    n_assert(this->IsOpen());

    this->diffuseColor = 0;
    this->modelViewProj = 0;

    // release D3DX shapes
    IndexT i;
    for (i = 0; i < NumShapeTypes; i++)
    {
        n_assert(0 != this->shapeMeshes[i]);
        this->shapeMeshes[i]->Release();
        this->shapeMeshes[i] = 0;
    }

    // call parent class
    ShapeRendererBase::Close();
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9ShapeRenderer::DrawShape(const matrix44& modelTransform, ShapeType shapeType, const float4& color)
{
    n_assert(this->inBegin);
    n_assert(0 != this->shapeMeshes[shapeType]);
    n_assert(shapeType < NumShapeTypes);
    HRESULT hr;

    // resolve model-view-projection matrix and update shader
    TransformDevice* transDev = TransformDevice::Instance();
    transDev->SetModelTransform(modelTransform);
    this->modelViewProj->SetMatrix(transDev->GetModelViewProjTransform());
    this->diffuseColor->SetVector(color);
    this->shapeShader->Commit();

    // draw shape
    hr = this->shapeMeshes[shapeType]->DrawSubset(0);
    n_assert(SUCCEEDED(hr));
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9ShapeRenderer::DrawPrimitives(const matrix44& modelTransform, 
                                  PrimitiveTopology::Code topology,
                                  SizeT numPrimitives,
                                  float* vertices,
                                  SizeT vertexWidth,
                                  const Math::float4& color)
{
    n_assert(this->inBegin);
    n_assert(0 != vertices);
    HRESULT hr;

    // resolve model-view-projection matrix and update shader
    TransformDevice* transDev = TransformDevice::Instance();
    transDev->SetModelTransform(modelTransform);
    this->modelViewProj->SetMatrix(transDev->GetModelViewProjTransform());
    this->diffuseColor->SetVector(color);
    this->shapeShader->Commit();

    // draw primitives
    D3DPRIMITIVETYPE d3d9PrimType = D3D9Types::AsD3D9PrimitiveType(topology);
    IDirect3DDevice9* d3d9Dev = D3D9RenderDevice::Instance()->GetDirect3DDevice();
    hr = d3d9Dev->DrawPrimitiveUP(d3d9PrimType,                     // PrimitiveType
                                  numPrimitives,                    // PrimitiveCount
                                  vertices,                         // pVertexStreamZeroData
                                  vertexWidth * sizeof(float));     // VertexStreamZeroStride
    n_assert(SUCCEEDED(hr));
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9ShapeRenderer::DrawIndexedPrimitives(const matrix44& modelTransform,
                                         PrimitiveTopology::Code topology,
                                         SizeT numPrimitives,
                                         float* vertices,
                                         SizeT numVertices,
                                         SizeT vertexWidth,
                                         void* indices,
                                         IndexType::Code indexType,
                                         const float4& color)
{
    n_assert(this->inBegin);
    n_assert(0 != vertices);
    n_assert(0 != indices);
    HRESULT hr;

    // resolve model-view-projection matrix and update shader
    TransformDevice* transDev = TransformDevice::Instance();
    transDev->SetModelTransform(modelTransform);
    this->modelViewProj->SetMatrix(transDev->GetModelViewProjTransform());
    this->diffuseColor->SetVector(color);
    this->shapeShader->Commit();

    // draw indexed primitives
    D3DPRIMITIVETYPE d3d9PrimType = D3D9Types::AsD3D9PrimitiveType(topology);
    D3DFORMAT d3d9IndexType = (IndexType::Index16 == indexType) ? D3DFMT_INDEX16 : D3DFMT_INDEX32;
    IDirect3DDevice9* d3d9Dev = D3D9RenderDevice::Instance()->GetDirect3DDevice();
    hr = d3d9Dev->DrawIndexedPrimitiveUP(d3d9PrimType,      // PrimitiveType
                                         0,                 // MinVertexIndex
                                         numVertices,       // NumVertices
                                         numPrimitives,     // NumPrimitives
                                         indices,           // pIndexData
                                         d3d9IndexType,     // IndexDataFormat
                                         vertices,          // pVertexStreamZeroData
                                         vertexWidth * sizeof(float));  // VertexStreamZeroStride
    n_assert(SUCCEEDED(hr));
}

} // namespace Direct3D9
