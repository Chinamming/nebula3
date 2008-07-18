//------------------------------------------------------------------------------
//  mountziparchive.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "interface/iomsg/mountziparchive.h"

namespace Interface
{
ImplementClass(Interface::MountZipArchive, 'MZPA', Interface::IOMessage);
ImplementMsgId(MountZipArchive);
} // namespace IO
