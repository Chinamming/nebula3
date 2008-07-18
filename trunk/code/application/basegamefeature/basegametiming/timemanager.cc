//------------------------------------------------------------------------------
//  timing/timemanager.cc
//  (C) 2005 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "basegametiming/timemanager.h"
#include "addons/db/dbserver.h"
#include "addons/db/reader.h"
#include "addons/db/writer.h"
#include "math/scalar.h"
#include "timing/time.h"

#ifdef _DEBUG
#include "io/console.h"
#endif

namespace BaseGameFeature
{
ImplementClass(TimeManager, 'TIMA', Game::Manager);
ImplementSingleton(TimeManager);

using namespace Timing;

//------------------------------------------------------------------------------
/**
*/
TimeManager::TimeManager() :
    time(0.0),
    timeFactor(1.0)
{    
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
TimeManager::~TimeManager()
{
    DestructSingleton;
}

//------------------------------------------------------------------------------
/**
    Activate the time manager. This will create all the standard time
    sources for Mangalore.
*/
void
TimeManager::OnActivate()
{
    // start system timer
    this->timer.Start();
    // get current global time
    this->time = this->timer.GetTime();
    this->frameTime = 0;

    // create standard time sources...
    Manager::OnActivate();
}

//------------------------------------------------------------------------------
/**
    Deactivate the time manager. 
*/
void
TimeManager::OnDeactivate()
{
    // cleanup time sources...
    while (this->timeSourceArray.Size() > 0)
    {
        this->RemoveTimeSource(this->timeSourceArray[0]);
    }
        
    // stop system timer
    this->timer.Stop();
    Manager::OnDeactivate();
}

//------------------------------------------------------------------------------
/**
    Update our time sources
*/
void
TimeManager::OnFrame()
{
    this->Update();
}

//------------------------------------------------------------------------------
/**
    Attach a time source to the time manager. This will invoke
    OnActivate() on the time source.
*/
void
TimeManager::AttachTimeSource(const Ptr<TimeSource>& timeSource)
{
    n_assert(timeSource);
    n_assert(!this->timeSourceArray.Find(timeSource));
    timeSource->OnActivate();
    this->timeSourceArray.Append(timeSource);
}

//------------------------------------------------------------------------------
/**
    Remove a time source from the time manager. This will invoke OnDeactivate()
    on the time source.
*/
void
TimeManager::RemoveTimeSource(const Ptr<TimeSource>& timeSource)
{
    n_assert(timeSource);
    IndexT index = this->timeSourceArray.FindIndex(timeSource);
    n_assert(InvalidIndex != index);
    this->timeSourceArray[index]->OnDeactivate();
    this->timeSourceArray.EraseIndex(index);
}

//------------------------------------------------------------------------------
/**
    Returns number of time sources attached to the time manager.
*/
int
TimeManager::GetNumTimeSources() const
{
    return this->timeSourceArray.Size();
}

//------------------------------------------------------------------------------
/**
    Gets pointer to time source object by index.
*/
const Ptr<TimeSource>& 
TimeManager::GetTimeSourceByIndex(int index) const
{
    return this->timeSourceArray[index];
}

//------------------------------------------------------------------------------
/**
    Get pointer to time source object by class name, returns 0 if not found.
*/
Ptr<TimeSource> 
TimeManager::GetTimeSourceByClassName(const Util::String& n) const
{
    n_assert(n.IsValid());
    uint i;
    for (i = 0; i < this->timeSourceArray.Size(); i++)
    {
        if (this->timeSourceArray[i]->GetClassName() == n)
        {
            return this->timeSourceArray[i];
        }
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    Checks whether the TimeSources table exists in the database, if
    yes invokes OnLoad() on all time sources...
*/
void
TimeManager::OnLoad()
{
    Db::Database* db = Db::DbServer::Instance()->GetGameDatabase();
    if (db->HasTable("TimeSources"))
    {
        // create a db reader...
        Ptr<Db::Reader> dbReader = Db::Reader::Create();
        dbReader->SetDatabase(db);
        dbReader->SetTableName("TimeSources");
        if (dbReader->Open())
        {   
            int rowIndex;
            int numRows = dbReader->GetNumRows();
            for (rowIndex = 0; rowIndex < numRows; rowIndex++)
            {
                dbReader->SetToRow(rowIndex);
                TimeSource* timeSource = this->GetTimeSourceByClassName(dbReader->GetString(Attr::TimeSourceId));
                if (timeSource)
                {
                    timeSource->OnLoad(dbReader);
                }
            }
            dbReader->Close();
        }
    }
}

//------------------------------------------------------------------------------
/**
    Ask all time sources to save their status to the database.
*/
void
TimeManager::OnSave()
{
    Ptr<Db::Writer> dbWriter = Db::Writer::Create();
    dbWriter->SetDatabase(Db::DbServer::Instance()->GetGameDatabase());
    dbWriter->SetTableName("TimeSources");
    dbWriter->AddColumn(Db::Column(Attr::TimeSourceId, Db::Column::Primary));
    dbWriter->AddColumn(Db::Column(Attr::TimeSourceTime));
    dbWriter->AddColumn(Db::Column(Attr::TimeSourceFactor));
    
    if (dbWriter->Open())
    {
        uint i;
        for (i = 0; i < this->timeSourceArray.Size(); i++)
        {
            this->timeSourceArray[i]->OnSave(dbWriter);
        }
        dbWriter->Close();
    }
}

//------------------------------------------------------------------------------
/**
    Reset all time sources. This is usually called at the beginning
    of an application state.
*/
void
TimeManager::ResetAll()
{
    uint i;
    for (i = 0; i < this->timeSourceArray.Size(); i++)
    {
        this->timeSourceArray[i]->Reset();
    }
}

//------------------------------------------------------------------------------
/**
    Pause all time sources. NOTE: there's an independent pause counter inside each
    time source, a pause just increments the counter, a continue decrements
    it, when the pause counter is != 0 it means, pause is activated.
*/
void
TimeManager::PauseAll()
{
    uint i;
    for (i = 0; i < this->timeSourceArray.Size(); i++)
    {
        this->timeSourceArray[i]->Pause();
    }
}

//------------------------------------------------------------------------------
/**
    Unpause all time sources.
*/
void
TimeManager::ContinueAll()
{
    uint i;
    for (i = 0; i < this->timeSourceArray.Size(); i++)
    {
        this->timeSourceArray[i]->Continue();
    }
}

//------------------------------------------------------------------------------
/**
    Update all time sources. This method is called very early in the frame
    by the current application state handler. This will get the current frame time
    and call UpdateTime() on all attached time sources.

    FIXME:
    * properly handle time exceptions!!!
*/
void
TimeManager::Update()
{
    // compute the current frame time
    Time curTime = this->timer.GetTime();
    this->frameTime = curTime - this->time;
    
    // apply time factor
    this->frameTime *= this->GetTimeFactor();
    
    // clamp frame time
    if (this->frameTime <= 0.0)
    {
        this->frameTime = 0.0001;
        this->time = curTime - this->frameTime;
    }
    else if (this->frameTime > 0.25)
    {
        this->frameTime = 0.25;
        this->time = curTime - this->frameTime;
    }
    this->time = this->time + this->frameTime;

    // update all time sources
    uint i;
    for (i = 0; i < this->timeSourceArray.Size(); i++)
    {
        this->timeSourceArray[i]->UpdateTime(this->frameTime);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
TimeManager::SetTimeFactor(Time f)
{
    n_assert(f > 0);
    this->timeFactor = f;
}

//------------------------------------------------------------------------------
/**
*/
Time
TimeManager::GetTimeFactor() const
{
    return this->timeFactor;
}

//------------------------------------------------------------------------------
/**
    show time factor if not 1.0
*/
void
TimeManager::OnRenderDebug()
{
    #ifdef _DEBUG
    if (this->GetTimeFactor() != 1.0)
    {
        IO::Console::Instance()->Print("Time Factor is %i%%.", (float)this->GetTimeFactor() * 100.f); 
    }
    #endif
    
    // call parrent
    Game::Manager::OnRenderDebug();
}

}; // namespace Game
