//------------------------------------------------------------------------------
//  DebugTextRenderer.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/debugtextrenderer.h"

namespace CoreGraphics
{
#if __WIN32__
ImplementClass(CoreGraphics::DebugTextRenderer, 'DTXR', Direct3D9::D3D9DebugTextRenderer);
//#elif __XBOX360__
//ImplementClass(CoreGraphics::DebugTextRenderer, 'DTXR', Xbox360::Xbox360DebugTextRenderer);
//#elif __WII__
//ImplementClass(CoreGraphics::DebugTextRenderer, 'DTXR', Wii::WiiDebugTextRenderer);
#else
#error "DebugTextRenderer class not implemented on this platform!"
#endif

ImplementSingleton(CoreGraphics::DebugTextRenderer);

//------------------------------------------------------------------------------
/**
*/
DebugTextRenderer::DebugTextRenderer()
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
DebugTextRenderer::~DebugTextRenderer()
{
    DestructSingleton;
}

} // namespace CoreGraphics
