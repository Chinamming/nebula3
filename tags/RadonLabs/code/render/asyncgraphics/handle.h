#pragma once
#ifndef ASYNCGRAPHICS_HANDLE_H
#define ASYNCGRAPHICS_HANDLE_H
//------------------------------------------------------------------------------
/**
    @class AsyncGraphics::Handle
    
    A private Handle typedef to safely pass data between a server and
    a client thread. Don't assume anything about the content of the
    Handle!
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace AsyncGraphics
{
typedef void* Handle;
}
//------------------------------------------------------------------------------
#endif
    