//------------------------------------------------------------------------------
//  deletefile.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "interface/iomsg/deletefile.h"

namespace Interface
{
ImplementClass(Interface::DeleteFile, 'DFIL', Interface::IOMessage);
ImplementMsgId(DeleteFile);
} // namespace IO
