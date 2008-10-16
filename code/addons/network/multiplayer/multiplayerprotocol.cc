//------------------------------------------------------------------------------
//  MACHINE GENERATED, DON'T EDIT!
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "multiplayerprotocol.h"

namespace Multiplayer
{
    __ImplementClass(Multiplayer::Connect, 'conc', Messaging::Message);
    ImplementMsgId(Connect);
    __ImplementClass(Multiplayer::Disconnect, 'dcon', Messaging::Message);
    ImplementMsgId(Disconnect);
    __ImplementClass(Multiplayer::ConnectionSucceeded, 'cosu', Messaging::Message);
    ImplementMsgId(ConnectionSucceeded);
    __ImplementClass(Multiplayer::ConnectionLost, 'colo', Messaging::Message);
    ImplementMsgId(ConnectionLost);
    __ImplementClass(Multiplayer::ConnectionFailed, 'cofa', Messaging::Message);
    ImplementMsgId(ConnectionFailed);
    __ImplementClass(Multiplayer::Chatmsg, 'chms', Messaging::Message);
    ImplementMsgId(Chatmsg);
    __ImplementClass(Multiplayer::IsClientConnected, 'clco', Messaging::Message);
    ImplementMsgId(IsClientConnected);
    __ImplementClass(Multiplayer::LoadLevel, 'lolv', Messaging::Message);
    ImplementMsgId(LoadLevel);
    __ImplementClass(Multiplayer::LevelLoaded, 'lvld', Messaging::Message);
    ImplementMsgId(LevelLoaded);
    __ImplementClass(Multiplayer::SynchronizationFinished, 'syfi', Messaging::Message);
    ImplementMsgId(SynchronizationFinished);
    __ImplementClass(Multiplayer::CreateEntity, 'cren', Messaging::Message);
    ImplementMsgId(CreateEntity);
    __ImplementClass(Multiplayer::DoesEntityExist, 'dene', Messaging::Message);
    ImplementMsgId(DoesEntityExist);
    __ImplementClass(Multiplayer::EntityExists, 'enex', Messaging::Message);
    ImplementMsgId(EntityExists);
    __ImplementClass(Multiplayer::GuidNetIdTable, 'guit', Messaging::Message);
    ImplementMsgId(GuidNetIdTable);
    __ImplementClass(Multiplayer::SetTransformOfEntity, 'stoe', Messaging::Message);
    ImplementMsgId(SetTransformOfEntity);
    __ImplementClass(Multiplayer::UpdateTransformOfEntity, 'upte', Messaging::Message);
    ImplementMsgId(UpdateTransformOfEntity);
    __ImplementClass(Multiplayer::CreateClientSideEntity, 'crce', Messaging::Message);
    ImplementMsgId(CreateClientSideEntity);
    __ImplementClass(Multiplayer::DeleteClientSideEntity, 'dcse', Messaging::Message);
    ImplementMsgId(DeleteClientSideEntity);
} // Multiplayer

namespace Commands
{
} // namespace Commands
//------------------------------------------------------------------------------
