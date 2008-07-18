#pragma once
#ifndef INPUT_INPUTPRIORITY_H
#define INPUT_INPUTPRIORITY_H
//------------------------------------------------------------------------------
/**
    @class Input::InputPriority
    
    Input priorities for input handlers.
    
    (C) 2007 Radon Labs GmbH
*/    
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Input
{
class InputPriority
{
public:
    enum Code
    {
        Gui = 0,
        Game,
    };
};

} // namespace Input
//------------------------------------------------------------------------------
#endif
