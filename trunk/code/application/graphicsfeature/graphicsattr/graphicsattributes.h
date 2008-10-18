#ifndef ATTR_GRAPHICSATTR_H
#define ATTR_GRAPHICSATTR_H
//------------------------------------------------------------------------------
/**
    @file graphicsattr/graphicsattributes.h    

    This is the central attribute registry for this feature. For more information
    on attributes, see Attr::Attribute.

    (C) 2007 Radon Labs GmbH
*/
#include "attr/attrid.h"
#include "attr/attributedefinition.h"

//------------------------------------------------------------------------------
namespace Attr
{
    // (simple) graphics property
    DeclareString(Graphics, 'GRFX', ReadWrite);        // name of the graphics resource
}; // namespace Attr
//------------------------------------------------------------------------------
#endif