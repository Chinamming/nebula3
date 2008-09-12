#pragma once
#ifndef CHAR_CHARACTERRENDERER_H
#define CHAR_CHARACTERRENDERER_H
//------------------------------------------------------------------------------
/**
    @class Char::CharacterRenderer
  
    Platform dependent character renderer. Implements
    skinning specialized for platform hardware.
    
    !!! FIXME FIXME FIXME !!!
    This shouldn't go into Char, but instead into some
    future Character subsystem. Also Char should never never
    know about the Models namespace!!!

    (C) 2008 Radon Labs GmbH
*/    
#if __WIN32__
#include "nebula2/character/d3d9/d3d9characterrenderer.h"
namespace Char
{
class CharacterRenderer : public Direct3D9::D3D9CharacterRenderer
{
    DeclareClass(CharacterRenderer);
public:
    /// constructor
    CharacterRenderer();
    /// destructor
    virtual ~CharacterRenderer();
};
} // namespace Char
#elif __XBOX360__
#include "nebula2/character/xbox360/xbox360characterrenderer.h"
namespace Char
{
class CharacterRenderer : public Xbox360::Xbox360CharacterRenderer
{
    DeclareClass(CharacterRenderer);
public:
    /// constructor
    CharacterRenderer();
    /// destructor
    virtual ~CharacterRenderer();
};
} // namespace Char
#elif __WII__
#include "nebula2/character/wii/wiicharacterrenderer.h"
namespace Char
{
class CharacterRenderer : public Wii::WiiCharacterRenderer
{
    DeclareClass(CharacterRenderer);
public:
    /// constructor
    CharacterRenderer();
    /// destructor
    virtual ~CharacterRenderer();
};
} // namespace Char
#else
#error "CharacterRenderer class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
