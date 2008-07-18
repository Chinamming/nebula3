#pragma once
#ifndef TIMING_GAMETIMESOURCE_H
#define TIMING_GAMETIMESOURCE_H
//------------------------------------------------------------------------------
/**
    @class Timing::TimingTimeSource
    
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
    DeclareClass(GameTimeSource);
    DeclareSingleton(GameTimeSource);

public:
    /// constructor
    GameTimeSource();
    /// destructor
    virtual ~GameTimeSource();
};
};
//------------------------------------------------------------------------------
#endif    

