#pragma once
#ifndef MATH_MATRIX_H
#define MATH_MATRIX_H
//------------------------------------------------------------------------------
/**
    @file math/matrix.h

    Frontend header for matrix classes.

    (C) 2006 Radon Labs GmbH
*/
#include "core/types.h"

#include "math/_matrix33.h"
#include "math/_matrix44.h"
namespace Math
{
    typedef _matrix33 matrix33;
    typedef _matrix44 matrix44;
};
//-------------------------------------------------------------------
#endif
