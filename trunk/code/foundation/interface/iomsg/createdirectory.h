#pragma once
#ifndef INTERFACE_CREATEDIRECTORY_H
#define INTERFACE_CREATEDIRECTORY_H
//------------------------------------------------------------------------------
/**
    @class Interface::CreateDirectory
  
    IOInterface message to create a filesystem directory.
    
    (C) 2006 Radon Labs GmbH
*/
#include "interface/iomsg/iomessage.h"

//------------------------------------------------------------------------------
namespace Interface
{
class CreateDirectory : public IOMessage
{
    DeclareClass(CreateDirectory);
    DeclareMsgId;
};

} // namespace Interface
//------------------------------------------------------------------------------
#endif