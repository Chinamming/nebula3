//------------------------------------------------------------------------------
//  CharacterRenderer.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "nebula2/character/characterrenderer.h"

namespace Char
{
#if (__WIN32__ || __XBOX360__)
__ImplementClass(Char::CharacterRenderer, 'CHRE', Win360::D3D9CharacterRenderer);
#elif __WII__
__ImplementClass(Char::CharacterRenderer, 'CHRE', Wii::WiiCharacterRenderer);
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
