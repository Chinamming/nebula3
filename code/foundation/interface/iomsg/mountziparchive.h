#pragma once
#ifndef INTERFACE_MOUNTZIPARCHIVE_H
#define INTERFACE_MOUNTZIPARCHIVE_H
//------------------------------------------------------------------------------
/**
    @class Interface::MountZipArchive
    
    IOInterface message to mount a zip archive.
    
    (C) 2007 Radon Labs GmbH
*/
#include "interface/iomsg/iomessage.h"

//------------------------------------------------------------------------------
namespace Interface
{
class MountZipArchive : public IOMessage
{
    DeclareClass(MountZipArchive);
    DeclareMsgId;
};

} // namespace IO
//------------------------------------------------------------------------------
#endif
    