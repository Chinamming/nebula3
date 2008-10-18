//------------------------------------------------------------------------------
//  debuggraphicshandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "debugrender/debuggraphicshandler.h"
#include "coregraphics/textrenderer.h"
#include "io/memorystream.h"
#include "internalgraphics/internalgraphicsserver.h"

namespace Debug
{
__ImplementClass(Debug::DebugGraphicsHandler, 'DGRH', Messaging::Handler);

using namespace IO;
using namespace Util;
using namespace Messaging;
using namespace CoreGraphics;
using namespace InternalGraphics;

//------------------------------------------------------------------------------
/**
*/
DebugGraphicsHandler::DebugGraphicsHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DebugGraphicsHandler::~DebugGraphicsHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
DebugGraphicsHandler::HandleMessage(const Ptr<Message>& msg)
{
    n_assert(msg.isvalid());
    if (msg->CheckId(RenderDebugText::Id))
    {
        this->OnRenderDebugText(msg.cast<RenderDebugText>());
    }
    else if (msg->CheckId(RenderDebugShapes::Id))
    {
        this->OnRenderDebugShapes(msg.cast<RenderDebugShapes>());
    }
    else if (msg->CheckId(Debug::RenderDebugView::Id))
    {
        this->OnRenderDebugView(msg.cast<Debug::RenderDebugView>());
    }
    else
    {
        // unknown message
        return false;
    }
    // fallthrough: message was handled
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
DebugGraphicsHandler::OnRenderDebugText(const Ptr<RenderDebugText>& msg)
{
    TextRenderer::Instance()->DeleteTextElementsByThreadId(msg->GetThreadId());
    TextRenderer::Instance()->AddTextElements(msg->GetTextElements());
}

//------------------------------------------------------------------------------
/**
*/
void
DebugGraphicsHandler::OnRenderDebugShapes(const Ptr<RenderDebugShapes>& msg)
{
    ShapeRenderer::Instance()->DeleteShapesByThreadId(msg->GetThreadId());
    ShapeRenderer::Instance()->AddShapes(msg->GetShapes());
}

//------------------------------------------------------------------------------
/**
*/
void
DebugGraphicsHandler::OnRenderDebugView(const Ptr<Debug::RenderDebugView>& msg)
{
    InternalGraphicsServer::Instance()->SetRenderDebug(msg->GetEnableDebugRendering());
}

} // namespace Debug
