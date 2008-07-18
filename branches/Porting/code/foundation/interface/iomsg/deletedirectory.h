#pragma once
#ifndef INTERFACE_DELETEDIRECTORY_H
#define INTERFACE_DELETEDIRECTORY_H
//------------------------------------------------------------------------------
/**
    @class Interface::DeleteDirectory
  
    IOInterface message to delete a directory.
    
    (C) 2006 Radon Labs GmbH
*/
#include "interface/iomsg/iomessage.h"

//------------------------------------------------------------------------------
namespace Interface
{
class DeleteDirectory : public IOMessage
{
    DeclareClass(DeleteDirectory);
    DeclareMsgId;
};

} // namespace Interface
//------------------------------------------------------------------------------
#endif