//------------------------------------------------------------------------------
//  interfacehandler.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "interface/iointerfacehandler.h"

namespace Interface
{
#if __WII__
__ImplementClass(Interface::IOInterfaceHandler, 'IOIH', Wii::WiiIOInterfaceHandler);
__ImplementSingleton(Interface::IOInterfaceHandler);
#else
__ImplementClass(Interface::IOInterfaceHandler, 'IOIH', Base::IOInterfaceHandlerBase);
__ImplementSingleton(Interface::IOInterfaceHandler);
#endif

//------------------------------------------------------------------------------
/**
*/
IOInterfaceHandler::IOInterfaceHandler()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
IOInterfaceHandler::~IOInterfaceHandler()
{
    __DestructSingleton;
}

} // namespace Interface
