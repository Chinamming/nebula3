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
#if (__WIN32__ || __XBOX360__)
#include "nebula2/character/win360/d3d9characterrenderer.h"
namespace Char
{
class CharacterRenderer : public Win360::D3D9CharacterRenderer
{
    __DeclareClass(CharacterRenderer);
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
    __DeclareClass(CharacterRenderer);
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
