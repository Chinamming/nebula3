#pragma once
#ifndef RENDER_RENDER_H
#define RENDER_RENDER_H
//------------------------------------------------------------------------------
/**
    @file render.h
    
    Add all class headers for classes which need dynamic object creation
    by class name or class fourcc here.
    
    (C) 2007 Radon Labs GmbH
*/
#include "models/modelnode.h"

namespace Models
{
    __RegisterClass(ModelNode);
};
#endif
