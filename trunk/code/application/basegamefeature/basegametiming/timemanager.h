#ifndef TIMING_TIMEMANAGER_H
#define TIMING_TIMEMANAGER_H
//------------------------------------------------------------------------------
/**
    @class BaseGameFeature::TimeManager

    Singleton object which manages the current game time. These are
    the standard time source objects provided by Mangalore:

    SystemTimeSource    - timing for low level Mangalore subsystems
    GameTimeSource      - timing for the game logic
    CameraTimeSource    - extra time source for camera handling
    GuiTimeSource       - time source for user interface stuff

    (C) 2005 Radon Labs GmbH 
*/
#include "game/manager.h"
#include "timing/timer.h"
#include "basegametiming/timesource.h"
#include "core/singleton.h"

//------------------------------------------------------------------------------
namespace BaseGameFeature
{
class TimeManager : public Game::Manager
{
	__DeclareClass(TimeManager);
    __DeclareSingleton(TimeManager);

public:
    /// constructor
    TimeManager();
    /// destructor
    virtual ~TimeManager();

    /// called when attached to game server
    virtual void OnActivate();
    /// called when removed from game server
    virtual void OnDeactivate();
    /// called after loading game state
    virtual void OnLoad();
    /// called before saving game state
    virtual void OnSave();
    /// on frame 
    virtual void OnFrame();
    /// reset all time sources
    void ResetAll();
    /// pause all time sources
    void PauseAll();
    /// continue all time sources
    void ContinueAll();

    /// set the time factor
    void SetTimeFactor(Timing::Time timeFactor);
    /// get the time factor
    Timing::Time GetTimeFactor() const;

    /// render a debug visualization 
    virtual void OnRenderDebug();

    /// attach a time source
    void AttachTimeSource(const Ptr<TimeSource>& timeSource);
    /// remove a time source
    void RemoveTimeSource(const Ptr<TimeSource>& timeSource);
    /// get number of time source
    int GetNumTimeSources() const;
    /// get pointer to time source by index
    const Ptr<TimeSource>& GetTimeSourceByIndex(int index) const;
    /// get pointer to time source by class name
    Ptr<TimeSource> GetTimeSourceByClassName(const Util::String& n) const;

protected:
    /// update the time manager
    void Update();

    Timing::Time timeFactor;
    Timing::Timer timer;
    Timing::Time time;
    Timing::Time frameTime;
    Util::Array<Ptr<TimeSource> > timeSourceArray;
};

}; // namespace Timing
//------------------------------------------------------------------------------
#endif