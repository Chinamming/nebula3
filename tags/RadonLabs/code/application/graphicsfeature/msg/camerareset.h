#ifndef MSG_CAMERARESET_H
#define MSG_CAMERARESET_H
//------------------------------------------------------------------------------
/**
    @class GraphicsFeature::CameraReset
    
    A camera reset message.
    
    (C) 2007 Radon Labs GmbH
*/
#include "messaging/message.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class CameraReset : public Messaging::Message
{
    DeclareClass(CameraReset);
    DeclareMsgId;
};

};
//------------------------------------------------------------------------------
#endif
