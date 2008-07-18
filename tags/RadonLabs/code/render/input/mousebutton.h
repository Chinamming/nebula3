#pragma once
#ifndef INPUT_MOUSEBUTTON_H
#define INPUT_MOUSEBUTTON_H
//------------------------------------------------------------------------------
/**
    @class Input::MouseButton
    
    Mouse button codes and conversion from/to string.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/types.h"
#include "util/string.h"

//------------------------------------------------------------------------------
namespace Input
{
class MouseButton
{
public:
    /// code enums
    enum Code
    {
        LeftButton,
        RightButton,
        MiddleButton,

        NumMouseButtons,
        InvalidMouseButton,
    };
    
    /// convert to string
    static Util::String ToString(Code code);
    /// convert from string
    static Code FromString(const Util::String& str);
};

} // namespace Input
//------------------------------------------------------------------------------
#endif
    