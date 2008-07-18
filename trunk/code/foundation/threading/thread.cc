//------------------------------------------------------------------------------
//  thread.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "threading/thread.h"

namespace Threading
{
#if __WIN32__
ImplementClass(Threading::Thread, 'TRED', Win32::Win32Thread);
#elif __XBOX360__
ImplementClass(Threading::Thread, 'TRED', Xbox360::Xbox360Thread);
#elif __WII__
ImplementClass(Threading::Thread, 'TRED', Wii::WiiThread);
#else
#error "Thread class not implemented on this platform!"
#endif
}
