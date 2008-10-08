#pragma once
#ifndef TIMING_GAMETIMESOURCE_H
#define TIMING_GAMETIMESOURCE_H
//------------------------------------------------------------------------------
/**
    @class BaseGameFeature::TimingTimeSource
    
    Provides timing information for the Timing logic.
    
    (C) 2007 Radon Labs GmbH
*/
#include "basegametiming/timesource.h"
#include "core/singleton.h"

//------------------------------------------------------------------------------
namespace BaseGameFeature
{
class GameTimeSource : public TimeSource
{
    __DeclareClass(GameTimeSource);
    __DeclareSingleton(GameTimeSource);

public:
    /// constructor
    GameTimeSource();
    /// destructor
    virtual ~GameTimeSource();
};
};
//------------------------------------------------------------------------------
#endif    

