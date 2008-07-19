#pragma once
#ifndef INTERFACE_DELETEFILE_H
#define INTERFACE_DELETEFILE_H
//------------------------------------------------------------------------------
/**
    @class Interface::DeleteFile
    
    IOInterface message to asynchronously delete a file.
    
    (C) 2006 Radon Labs GmbH
*/
#include "interface/iomsg/iomessage.h"

//------------------------------------------------------------------------------
namespace Interface
{
class DeleteFile : public IOMessage
{
    DeclareClass(DeleteFile);
    DeclareMsgId;
};

} // namespace IO
//------------------------------------------------------------------------------
#endif
    