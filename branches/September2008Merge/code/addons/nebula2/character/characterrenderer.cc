//------------------------------------------------------------------------------
//  CharacterRenderer.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "nebula2/character/characterrenderer.h"

namespace Char
{
#if __WIN32__
ImplementClass(Char::CharacterRenderer, 'CHRE', Direct3D9::D3D9CharacterRenderer);
#elif __XBOX360__
ImplementClass(Char::CharacterRenderer, 'CHRE', Xbox360::Xbox360CharacterRenderer);
#elif __WII__
ImplementClass(Char::CharacterRenderer, 'CHRE', Wii::WiiCharacterRenderer);
#else
#error "CharacterRenderer class not implemented on this platform!"
#endif

//------------------------------------------------------------------------------
/**
*/
CharacterRenderer::CharacterRenderer()
{
}

//------------------------------------------------------------------------------
/**
*/
CharacterRenderer::~CharacterRenderer()
{
}

} // namespace Char
