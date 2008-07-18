//------------------------------------------------------------------------------
//  viewproxy.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "asyncgraphics/viewproxy.h"
#include "asyncgraphics/protocol.h"
#include "asyncgraphics/asyncgraphicsinterface.h"
#include "graphics/view.h"

namespace AsyncGraphics
{
ImplementClass(AsyncGraphics::ViewProxy, 'AGVP', Core::RefCounted);

using namespace Messaging;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
ViewProxy::ViewProxy() :
    viewClass(&Graphics::View::RTTI),
    isDefaultView(false),
    viewHandle(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ViewProxy::~ViewProxy()
{
    n_assert(!this->IsValid());
}

//------------------------------------------------------------------------------
/**
*/
void
ViewProxy::Setup()
{
    n_assert(0 != this->viewClass);
    n_assert(!this->IsValid());
    n_assert(this->name.Value().IsValid());
    n_assert(this->stageName.Value().IsValid());
    n_assert(this->frameShaderName.Value().IsValid());

    // send a CreateGraphicsView message and wait for completion
    Ptr<AsyncGraphics::CreateGraphicsView> msg = AsyncGraphics::CreateGraphicsView::Create();
    msg->SetViewClass(this->viewClass);
    msg->SetName(this->name);
    msg->SetStageName(this->stageName);
    msg->SetFrameShaderName(this->frameShaderName);
    msg->SetDefaultView(this->isDefaultView);
    AsyncGraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    n_assert(msg->Handled());
    this->viewHandle = msg->GetResult();
    n_assert(0 != this->viewHandle);
}

//------------------------------------------------------------------------------
/**
*/
void
ViewProxy::Discard()
{
    n_assert(this->IsValid());

    // send a DiscardGraphicsView message and wait for completion
    Ptr<AsyncGraphics::DiscardGraphicsView> msg = AsyncGraphics::DiscardGraphicsView::Create();
    msg->SetViewHandle(this->viewHandle);
    AsyncGraphicsInterface::Instance()->SendWait(msg.cast<Message>());
    this->viewHandle = 0;
}

} // namespace AsyncGraphics
