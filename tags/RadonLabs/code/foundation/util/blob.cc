//------------------------------------------------------------------------------
//  blob.cc
//  (C) 2006 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "util/blob.h"

namespace Util
{
Memory::Heap* Blob::DataHeap = 0;
Memory::Heap* Blob::ObjectHeap = 0;
}