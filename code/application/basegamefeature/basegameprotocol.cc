//------------------------------------------------------------------------------
//  MACHINE GENERATED, DON'T EDIT!
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "basegameprotocol.h"

namespace BaseGameFeature
{
    ImplementClass(BaseGameFeature::MoveDirection, 'MOVD', Messaging::Message);
    ImplementMsgId(MoveDirection);
    ImplementClass(BaseGameFeature::MoveFollow, 'MOVF', Messaging::Message);
    ImplementMsgId(MoveFollow);
    ImplementClass(BaseGameFeature::MoveGoto, 'MOVG', Messaging::Message);
    ImplementMsgId(MoveGoto);
    ImplementClass(BaseGameFeature::MoveRotate, 'MMRO', Messaging::Message);
    ImplementMsgId(MoveRotate);
    ImplementClass(BaseGameFeature::MoveSetVelocity, 'MMSV', Messaging::Message);
    ImplementMsgId(MoveSetVelocity);
    ImplementClass(BaseGameFeature::MoveStop, 'MOVS', Messaging::Message);
    ImplementMsgId(MoveStop);
    ImplementClass(BaseGameFeature::MoveTurn, 'MOVT', Messaging::Message);
    ImplementMsgId(MoveTurn);
    ImplementClass(BaseGameFeature::UpdateTransform, 'UPTR', Messaging::Message);
    ImplementMsgId(UpdateTransform);
    ImplementClass(BaseGameFeature::SetTransform, 'MOST', Messaging::Message);
    ImplementMsgId(SetTransform);
} // BaseGameFeature

namespace Commands
{
} // namespace Commands
//------------------------------------------------------------------------------
