//------------------------------------------------------------------------------
//  interfacehandler.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "interface/iointerfacehandler.h"

namespace Interface
{
#if __WII__
ImplementClass(Interface::IOInterfaceHandler, 'IOIH', Wii::WiiIOInterfaceHandler);
ImplementSingleton(Interface::IOInterfaceHandler);
#else
ImplementClass(Interface::IOInterfaceHandler, 'IOIH', Base::IOInterfaceHandlerBase);
ImplementSingleton(Interface::IOInterfaceHandler);
#endif

//------------------------------------------------------------------------------
/**
*/
IOInterfaceHandler::IOInterfaceHandler()
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
IOInterfaceHandler::~IOInterfaceHandler()
{
    DestructSingleton;
}

} // namespace Interface
