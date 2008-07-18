//------------------------------------------------------------------------------
//  createdirectory.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "interface/iomsg/createdirectory.h"

namespace Interface
{
ImplementClass(Interface::CreateDirectory, 'CDIR', Interface::IOMessage);
ImplementMsgId(CreateDirectory);
} // namespace IO
