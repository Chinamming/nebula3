//------------------------------------------------------------------------------
//  MACHINE GENERATED, DON'T EDIT!
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "basegameprotocol.h"

namespace BaseGameFeature
{
    __ImplementClass(BaseGameFeature::MoveDirection, 'MOVD', Messaging::Message);
    ImplementMsgId(MoveDirection);
    __ImplementClass(BaseGameFeature::MoveFollow, 'MOVF', Messaging::Message);
    ImplementMsgId(MoveFollow);
    __ImplementClass(BaseGameFeature::MoveGoto, 'MOVG', Messaging::Message);
    ImplementMsgId(MoveGoto);
    __ImplementClass(BaseGameFeature::MoveRotate, 'MMRO', Messaging::Message);
    ImplementMsgId(MoveRotate);
    __ImplementClass(BaseGameFeature::MoveSetVelocity, 'MMSV', Messaging::Message);
    ImplementMsgId(MoveSetVelocity);
    __ImplementClass(BaseGameFeature::MoveStop, 'MOVS', Messaging::Message);
    ImplementMsgId(MoveStop);
    __ImplementClass(BaseGameFeature::MoveTurn, 'MOVT', Messaging::Message);
    ImplementMsgId(MoveTurn);
    __ImplementClass(BaseGameFeature::UpdateTransform, 'UPTR', Messaging::Message);
    ImplementMsgId(UpdateTransform);
    __ImplementClass(BaseGameFeature::SetTransform, 'MOST', Messaging::Message);
    ImplementMsgId(SetTransform);
} // BaseGameFeature

namespace Commands
{
} // namespace Commands
//------------------------------------------------------------------------------
