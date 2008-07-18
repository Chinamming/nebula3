//------------------------------------------------------------------------------
//  graphics/animtable.cc
//  (C) 2005 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "animtable.h"
#include "io/xmlreader.h"

namespace Nebula2
{
ImplementClass(Nebula2::AnimTable, 'ANTA', Core::RefCounted);
ImplementSingleton(Nebula2::AnimTable);

using namespace Util;
using namespace Timing;

//------------------------------------------------------------------------------
/**
*/
AnimTable::AnimTable() :
    isOpen(false)
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
AnimTable::~AnimTable()
{
    DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
bool
AnimTable::Open()
{
    n_assert(!this->IsOpen());

    this->isOpen = this->LoadAnimTable();

    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
bool
AnimTable::LoadAnimTable()
{
    //// create an xml spreadsheet object
    //nXmlSpreadSheet xmlSpreadSheet;
    //xmlSpreadSheet.SetFilename(this->filename);
    //if (xmlSpreadSheet.Open())
    //{
    //    n_assert(xmlSpreadSheet.NumTables() >= 1);

    //    const nXmlTable& xmlTable = xmlSpreadSheet.TableAt(0);
    //    n_assert(xmlTable.NumColumns() >= 2);
    //    n_assert(xmlTable.NumRows() >= 2);

    //    // build index mapper tables
    //    this->BuildIndexMapperTables(xmlTable);

    //    // build anim info table
    //    this->BuildAnimInfoTable(xmlTable);

    //    xmlSpreadSheet.Close();
    //    return true;
    //}

    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
AnimTable::Close()
{
    n_assert(this->IsOpen());
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/** 
    Check if the specified animation exists
*/
bool
AnimTable::ExistsAnimation(const Util::String& cat, const String& animName) const
{    
    if (this->animMapping.Contains(cat))
    {
        return this->animMapping[cat].Contains(animName);
    }
    return false;
}

//------------------------------------------------------------------------------
/** 
    Return mapped animation name of given category and animName
*/
const Util::String& 
AnimTable::GetMappedAnimName(const Util::String& cat, const Util::String& animName) const
{
    if (this->ExistsAnimation(cat, animName))
    {
        return this->animMapping[cat][animName];
    }
    return animName;
}

}; // namespace Graphics