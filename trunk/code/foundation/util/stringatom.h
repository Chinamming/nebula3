#pragma once
#ifndef UTIL_STRINGATOM_H
#define UTIL_STRINGATOM_H
//------------------------------------------------------------------------------
/**
    @class Util::StringAtom
    
    A shortcut typedef for Atom<String>.
    
    (C) 2007 Radon Labs GmbH
*/
#include "util/string.h"
#include "util/atom.h"

//------------------------------------------------------------------------------
namespace Util
{
typedef Atom<String> StringAtom;
}
//------------------------------------------------------------------------------
#endif
