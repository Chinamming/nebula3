#ifndef MSG_MOVESTOP_H
#define MSG_MOVESTOP_H
//------------------------------------------------------------------------------
/**
    @class Messaging::MoveStop

    A MoveStop message. The expected behaviour is that an entity which
    receives this messages stops immediately.
    
    (C) 2007 Radon Labs GmbH
*/
#include "messaging/message.h"

//------------------------------------------------------------------------------
namespace BaseGameFeature
{
class MoveStop : public Messaging::Message
{
	DeclareClass(MoveStop);
    DeclareMsgId;
};

};
//------------------------------------------------------------------------------
#endif
