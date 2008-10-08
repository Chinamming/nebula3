//------------------------------------------------------------------------------
//  iointerface.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "interface/iointerface.h"
#include "interface/iointerfacehandler.h"

namespace Interface
{
__ImplementClass(Interface::IOInterface, 'IIOF', Messaging::AsyncPort);
__ImplementInterfaceSingleton(Interface::IOInterface);

using namespace IO;
using namespace Messaging;

//------------------------------------------------------------------------------
/**
*/
IOInterface::IOInterface()
{
    __ConstructSingleton;
    this->SetThreadCpuCoreId(System::Cpu::IoThreadCore);
}

//------------------------------------------------------------------------------
/**
*/
IOInterface::~IOInterface()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
IOInterface::Open()
{
    this->SetName("IOInterface Thread");
    AsyncPort::Open();
}

//------------------------------------------------------------------------------
/**
*/
void
IOInterface::OnCreateHandlers()
{
    Ptr<IOInterfaceHandler> msgHandler = IOInterfaceHandler::Create();
    this->AttachHandler(msgHandler.upcast<Messaging::Handler>());
}

} // namespace IO