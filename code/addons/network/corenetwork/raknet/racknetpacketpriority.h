#pragma once
#ifndef RAKNET_RACKNETPACKETPRIORITY_H
#define RAKNET_RACKNETPACKETPRIORITY_H

#include "extlibs/raknet/source/packetpriority.h"
namespace RakNet
{
class RackNetPacketPriority
{
public:
    // not really necessary this wrapping?!
    enum Priority
    {
        LOW = LOW_PRIORITY,
        MEDIUM = MEDIUM_PRIORITY,
        HIGH = HIGH_PRIORITY
    };
}
}
#endif // RAKNET_RACKNETPACKETPRIORITY_H