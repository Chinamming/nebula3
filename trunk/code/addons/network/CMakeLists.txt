#-------------------------------------------------------------------------------
# addons/network
#-------------------------------------------------------------------------------
INCLUDE_DIRECTORIES(
    ${Nebula3_SOURCE_DIR}/
    ${Nebula3_SOURCE_DIR}/foundation/
    ${Nebula3_SOURCE_DIR}/extlibs/raknet/source/
    ${Nebula3_SOURCE_DIR}/addons/
    ${Nebula3_SOURCE_DIR}/addons/network/
)
SET(addons_network_corenetwork_src
    corenetwork/ipaddress.cc
    corenetwork/ipaddress.h
    corenetwork/netclient.cc
    corenetwork/netclient.h
    corenetwork/netconfig.h
    corenetwork/netserver.cc
    corenetwork/netserver.h
    corenetwork/netstream.cc
    corenetwork/netstream.h
    corenetwork/netstreamfactory.h
    corenetwork/packetid.h
    corenetwork/packetpriority.h
)
SET(addons_network_corenetwork_base_src
    corenetwork/base/netclientbase.cc
    corenetwork/base/netclientbase.h
    corenetwork/base/netserverbase.cc
    corenetwork/base/netserverbase.h
)
SET(addons_network_corenetwork_raknet_src
    corenetwork/raknet/raknetclient.cc
    corenetwork/raknet/raknetclient.h
    corenetwork/raknet/raknetipaddress.cc
    corenetwork/raknet/raknetipaddress.h
    corenetwork/raknet/raknetpacketid.h
    corenetwork/raknet/raknetpacketpriority.h
    corenetwork/raknet/raknetserver.cc
    corenetwork/raknet/raknetserver.h
    corenetwork/raknet/raknetstream.cc
    corenetwork/raknet/raknetstream.h
    corenetwork/raknet/raknetstreamfactory.cc
    corenetwork/raknet/raknetstreamfactory.h
)
SET(addons_network_multiplayer_src
    multiplayer/defaultclienthandler.cc
    multiplayer/defaultclienthandler.h
    multiplayer/defaultserverhandler.cc
    multiplayer/defaultserverhandler.h
    multiplayer/multiplayermanager.cc
    multiplayer/multiplayermanager.h
    multiplayer/multiplayerprotocol.cc
    multiplayer/multiplayerprotocol.h
    multiplayer/netmessagehandler.cc
    multiplayer/netmessagehandler.h
)
SET(addons_network_pch_src
    ../stdneb.cc
    ../stdneb.h
)
SET(addons_network_SRCS
    ${addons_network_corenetwork_src}
    ${addons_network_corenetwork_base_src}
    ${addons_network_corenetwork_raknet_src}
    ${addons_network_multiplayer_src}
    ${addons_network_pch_src}
)
SOURCE_GROUP(corenetwork FILES ${addons_network_corenetwork_src})
SOURCE_GROUP(corenetwork\\base FILES ${addons_network_corenetwork_base_src})
SOURCE_GROUP(corenetwork\\raknet FILES ${addons_network_corenetwork_raknet_src})
SOURCE_GROUP(multiplayer FILES ${addons_network_multiplayer_src})
SOURCE_GROUP(precompiled FILES ${addons_network_pch_src})

ADD_LIBRARY(network STATIC ${addons_network_SRCS})
N_SET_COMPILER_DEFAULTS()
IF(N_USE_PRECOMPILED_HEADERS)
    N_SET_PCH(network stdneb.h stdneb.cc ../)
ENDIF()
ADD_DEPENDENCIES(network foundation)
