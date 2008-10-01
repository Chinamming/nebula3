#pragma once
#ifndef COREGRAPHICS_DISPLAYMODE_H
#define COREGRAPHICS_DISPLAYMODE_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::DisplayMode
  
    Describe a fullscreen display mode or window dimensions.
      
    (C) 2006 Radon Labs GmbH
*/    
#include "core/types.h"
#include "coregraphics/pixelformat.h"

//------------------------------------------------------------------------------
namespace CoreGraphics
{
class DisplayMode
{
public:
    /// default constructor
    DisplayMode();
    /// windowed mode constructor
    DisplayMode(uint x, uint y, uint w, uint h);
    /// fullscreen constructor
    DisplayMode(uint w, uint h, PixelFormat::Code p);
    /// generic constructor
    DisplayMode(uint x, uint y, uint w, uint h, PixelFormat::Code p);
    /// equality operator
    bool operator==(const DisplayMode& rhs) const;
    /// inequality operator
    bool operator!=(const DisplayMode& rhs) const;
    /// set x position
    void SetXPos(uint x);
    /// get x position
    uint GetXPos() const;
    /// set y position
    void SetYPos(uint y);
    /// get y position
    uint GetYPos() const;
    /// set width
    void SetWidth(uint w);
    /// get width
    uint GetWidth() const;
    /// set height
    void SetHeight(uint h);
    /// get height
    uint GetHeight() const;
    /// set pixel format
    void SetPixelFormat(PixelFormat::Code p);
    /// get pixel format
    PixelFormat::Code GetPixelFormat() const;
    /// get aspect ratio, this is simply width / height
    float GetAspectRatio() const;

private:
    uint xPos;
    uint yPos;
    uint width;
    uint height;
    PixelFormat::Code pixelFormat;
};

//------------------------------------------------------------------------------
/**
*/
inline
DisplayMode::DisplayMode() :
    xPos(0),
    yPos(0),
    width(1024),
    height(768),
    pixelFormat(PixelFormat::X8R8G8B8)
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This constructor is suitable for windowed modes.
*/
inline
DisplayMode::DisplayMode(uint x, uint y, uint w, uint h) :
    xPos(x),
    yPos(y),
    width(w),
    height(h),
    pixelFormat(PixelFormat::InvalidPixelFormat)
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This constructor is suitable for fullscreen modes.
*/
inline
DisplayMode::DisplayMode(uint w, uint h, PixelFormat::Code p) :
    xPos(0),
    yPos(0),
    width(w),
    height(h),
    pixelFormat(p)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
DisplayMode::DisplayMode(uint x, uint y, uint w, uint h, PixelFormat::Code p) :
    xPos(x),
    yPos(y),
    width(w),
    height(h),
    pixelFormat(p)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline bool
DisplayMode::operator==(const DisplayMode& rhs) const
{
    return ((this->xPos == rhs.xPos) &&
            (this->yPos == rhs.yPos) &&
            (this->width == rhs.width) &&
            (this->height == rhs.height) &&
            (this->pixelFormat == rhs.pixelFormat));
}

//------------------------------------------------------------------------------
/**
*/
inline bool
DisplayMode::operator!=(const DisplayMode& rhs) const
{
    return !(*this == rhs); 
}

//------------------------------------------------------------------------------
/**
*/
inline void
DisplayMode::SetXPos(uint x)
{
    this->xPos = x;
}

//------------------------------------------------------------------------------
/**
*/
inline uint
DisplayMode::GetXPos() const
{
    return this->xPos;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DisplayMode::SetYPos(uint y)
{
    this->yPos = y;
}

//------------------------------------------------------------------------------
/**
*/
inline uint
DisplayMode::GetYPos() const
{
    return this->yPos;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DisplayMode::SetWidth(uint w)
{
    this->width = w;
}

//------------------------------------------------------------------------------
/**
*/
inline uint
DisplayMode::GetWidth() const
{
    return this->width;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DisplayMode::SetHeight(uint h)
{
    this->height = h;
}

//------------------------------------------------------------------------------
/**
*/
inline uint
DisplayMode::GetHeight() const
{
    return this->height;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DisplayMode::SetPixelFormat(PixelFormat::Code p)
{
    this->pixelFormat = p;
}

//------------------------------------------------------------------------------
/**
*/
inline PixelFormat::Code
DisplayMode::GetPixelFormat() const
{
    return this->pixelFormat;
}

//------------------------------------------------------------------------------
/**
*/
inline float
DisplayMode::GetAspectRatio() const
{
    n_assert(this->height > 0);
    return float(this->width) / float(this->height);
}

} // namespace CoreGraphics
//------------------------------------------------------------------------------
#endif

