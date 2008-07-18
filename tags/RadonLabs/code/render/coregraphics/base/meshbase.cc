//------------------------------------------------------------------------------
//  meshbase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/base/meshbase.h"
#include "coregraphics/renderdevice.h"

namespace Base
{
ImplementClass(Base::MeshBase, 'MSHB', Resources::Resource);

using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
MeshBase::MeshBase()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
MeshBase::~MeshBase()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
MeshBase::Unload()
{
    if (this->vertexBuffer.isvalid())
    {
        this->vertexBuffer->Unload();
        this->vertexBuffer = 0;
    }
    if (this->indexBuffer.isvalid())
    {
        this->indexBuffer->Unload();
        this->indexBuffer = 0;
    }
    Resource::Unload();
}

//------------------------------------------------------------------------------
/**
*/
void 
MeshBase::ApplyPrimitives(IndexT primGroupIndex)
{
    if (this->vertexBuffer.isvalid())
    {
        RenderDevice::Instance()->SetVertexBuffer(this->vertexBuffer);   
    }
    if (this->indexBuffer.isvalid())
    {
        RenderDevice::Instance()->SetIndexBuffer(this->indexBuffer);
    }
    RenderDevice::Instance()->SetPrimitiveGroup(GetPrimitiveGroupAtIndex(primGroupIndex));
}

} // namespace Base
