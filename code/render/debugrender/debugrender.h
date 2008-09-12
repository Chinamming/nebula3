#pragma once
#ifndef DEBUG_DEBUGRENDER_H
#define DEBUG_DEBUGRENDER_H
//------------------------------------------------------------------------------
/**
    Macros for debug rendering.

    (C) 2008 Radon Labs GmbH
*/

// debug text render macro
#include "graphics/graphicsprotocol.h"
#include "graphics/graphicsinterface.h"

#define _debug_text(txt, pos, color) { Ptr<Graphics::RenderDebugText> msg = Graphics::RenderDebugText::Create(); \
                                       msg->SetText(txt);\
                                       msg->SetPosition(pos);\
                                       msg->SetColor(color);\
                                       Graphics::GraphicsInterface::Instance()->Send(msg.cast<Messaging::Message>()); }

#endif
