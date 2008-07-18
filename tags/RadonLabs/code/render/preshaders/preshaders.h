#pragma once
#ifndef PRESHADERS_PRESHADERS_H
#define PRESHADERS_PRESHADERS_H
//------------------------------------------------------------------------------
/**
    @file preshaders/preshaders.h
    
    Includes all pre-shader headers and registers class names.
    
    (C) 2007 Radon Labs GmbH
*/
#include "preshaders/boxfilterkernel.h"
#include "preshaders/gaussianblur5x5filterkernel.h"

namespace PreShaders
{
    RegisterClass(BoxFilterKernel);
    RegisterClass(GaussianBlur5x5FilterKernel);
};
//------------------------------------------------------------------------------
#endif

    