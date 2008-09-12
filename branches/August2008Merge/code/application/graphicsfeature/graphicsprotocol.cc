//------------------------------------------------------------------------------
//  MACHINE GENERATED, DON'T EDIT!
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphicsprotocol.h"

namespace GraphicsFeature
{
    ImplementClass(GraphicsFeature::CameraDistance, 'MCAD', Messaging::Message);
    ImplementMsgId(CameraDistance);
    ImplementClass(GraphicsFeature::CameraFocus, 'CAFC', Messaging::Message);
    ImplementMsgId(CameraFocus);
    ImplementClass(GraphicsFeature::CameraOrbit, 'MCAO', Messaging::Message);
    ImplementMsgId(CameraOrbit);
    ImplementClass(GraphicsFeature::CameraReset, 'CARE', Messaging::Message);
    ImplementMsgId(CameraReset);
    ImplementClass(GraphicsFeature::GetGraphicsEntities, 'GGEN', Messaging::Message);
    ImplementMsgId(GetGraphicsEntities);
    ImplementClass(GraphicsFeature::InputFocus, 'INFC', Messaging::Message);
    ImplementMsgId(InputFocus);
    ImplementClass(GraphicsFeature::SetVisibleMsg, 'MGFV', Messaging::Message);
    ImplementMsgId(SetVisibleMsg);
} // GraphicsFeature

namespace Commands
{
} // namespace Commands
//------------------------------------------------------------------------------
