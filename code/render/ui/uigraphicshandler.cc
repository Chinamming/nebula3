//------------------------------------------------------------------------------
//  uigraphicshandler.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "ui/uigraphicshandler.h"
#include "internalgraphics/internalgraphicsserver.h"
#include "ui/uirenderprotocol.h"

namespace CoreUI
{
__ImplementClass(UIGraphicsHandler, 'UIGH', Messaging::Handler);

using namespace IO;
using namespace Util;
using namespace Messaging;
using namespace CoreGraphics;
using namespace InternalGraphics;

//------------------------------------------------------------------------------
/**
*/
UIGraphicsHandler::UIGraphicsHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
UIGraphicsHandler::~UIGraphicsHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
UIGraphicsHandler::HandleMessage(const Ptr<Messaging::Message>& msg)
{
    n_assert(msg.isvalid());
    if (msg->CheckId(RenderUIElements::Id))
    {
        this->OnRenderUIElement(msg.cast<RenderUIElements>());
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
UIGraphicsHandler::OnRenderUIElement(const Ptr<RenderUIElements>& msg)
{
    //TextRenderer::Instance()->DeleteTextElementsByThreadId(msg->GetThreadId());
    //TextRenderer::Instance()->AddTextElements(msg->GetTextElements());
}
} // namespace Debug
