//------------------------------------------------------------------------------
//  deletedirectory.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "interface/iomsg/deletedirectory.h"

namespace Interface
{
ImplementClass(Interface::DeleteDirectory, 'DDIR', Interface::IOMessage);
ImplementMsgId(DeleteDirectory);
} // namespace IO
