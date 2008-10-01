#pragma once
#ifndef COREGRAPHICS_DEBUGTEXTRENDERER_H
#define COREGRAPHICS_DEBUGTEXTRENDERER_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::DebugTextRenderer
  
    A simple text renderer for drawing text on screen.
    Only for debug purposes.
    
    (C) 2008 Radon Labs GmbH
*/    
#if __WIN32__
#include "coregraphics/d3d9/d3d9debugtextrenderer.h"
namespace CoreGraphics
{
class DebugTextRenderer : public Direct3D9::D3D9DebugTextRenderer
{
    DeclareClass(DebugTextRenderer);
    DeclareSingleton(DebugTextRenderer);
public:
    /// constructor
    DebugTextRenderer();
    /// destructor
    virtual ~DebugTextRenderer();
};
} // namespace CoreGraphics
//#elif __XBOX360__
//#include "coregraphics/xbox360/xbox360DebugTextRenderer.h"
//namespace CoreGraphics
//{
//class DebugTextRenderer : public Xbox360::Xbox360DebugTextRenderer
//{
//    DeclareClass(DebugTextRenderer);
//    DeclareSingleton(DebugTextRenderer);
//public:
//    /// constructor
//    DebugTextRenderer();
//    /// destructor
//    virtual ~DebugTextRenderer();
//};
//} // namespace CoreGraphics
//#elif __WII__
//#include "coregraphics/wii/wiiDebugTextRenderer.h"
//namespace CoreGraphics
//{
//class DebugTextRenderer : public Wii::WiiDebugTextRenderer
//{
//    DeclareClass(DebugTextRenderer);
//    DeclareSingleton(DebugTextRenderer);
//public:
//    /// constructor
//    DebugTextRenderer();
//    /// destructor
//    virtual ~DebugTextRenderer();
//};
//} // namespace CoreGraphics
#else
#error "DebugTextRenderer class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
