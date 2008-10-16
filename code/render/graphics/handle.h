#pragma once
#ifndef GRAPHICS_HANDLE_H
#define GRAPHICS_HANDLE_H
//------------------------------------------------------------------------------
/**
    @class Graphics::Handle
    
    A private Handle typedef to safely pass data between a server and
    a client thread. Don't assume anything about the content of the
    Handle!
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Graphics
{
typedef void* Handle;
}
//------------------------------------------------------------------------------
#endif
    