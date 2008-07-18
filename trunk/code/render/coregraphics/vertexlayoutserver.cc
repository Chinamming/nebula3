//------------------------------------------------------------------------------
//  vertexlayoutserver.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/vertexlayoutserver.h"
#include "coregraphics/vertexlayout.h"

namespace CoreGraphics
{
ImplementClass(CoreGraphics::VertexLayoutServer, 'VXSV', Core::RefCounted);
ImplementSingleton(CoreGraphics::VertexLayoutServer);

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
VertexLayoutServer::VertexLayoutServer() :
    isOpen(false)
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
VertexLayoutServer::~VertexLayoutServer()
{
    DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
VertexLayoutServer::Open()
{
    n_assert(!this->isOpen);
    n_assert(this->vertexLayouts.IsEmpty());
    this->isOpen = true;
}

//------------------------------------------------------------------------------
/**
*/
void
VertexLayoutServer::Close()
{
    n_assert(this->isOpen);
    this->isOpen = false;
    IndexT i;
    for (i = 0; i < this->vertexLayouts.Size(); i++)
    {
        this->vertexLayouts.ValueAtIndex(i)->Discard();
    }
    this->vertexLayouts.Clear();
}

//------------------------------------------------------------------------------
/**
*/
Ptr<VertexLayout>
VertexLayoutServer::CreateSharedVertexLayout(const Util::Array<VertexComponent>& vertexComponents)
{
    n_assert(this->IsOpen());
    n_assert(vertexComponents.Size() > 0);

    // get sharing signature from vertex components
    StringAtom signature = VertexLayout::BuildSignature(vertexComponents);
    if (this->vertexLayouts.Contains(signature))
    {
        // return existing instance
        return this->vertexLayouts[signature];
    }
    else
    {
        // create new instance
        Ptr<VertexLayout> newVertexLayout = VertexLayout::Create();
        newVertexLayout->Setup(vertexComponents);
        this->vertexLayouts.Add(signature, newVertexLayout);
        return newVertexLayout;
    }
}

} // namespace CoreGraphics
