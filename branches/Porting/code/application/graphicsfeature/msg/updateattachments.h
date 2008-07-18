#ifndef MSG_UPDATEATTACHMENTS_H
#define MSG_UPDATEATTACHMENTS_H
//------------------------------------------------------------------------------
/**
    @class Messaging::UpdateAttachments

    Update a character's attachments.
    
    (C) 2006 Radon Labs GmbH
*/
#include "messaging/message.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class UpdateAttachments : public Messaging::Message
{
    DeclareClass(UpdateAttachments);
    DeclareMsgId;

public:
    /// constructor
    UpdateAttachments();
};

//------------------------------------------------------------------------------
/**
*/

}; // namespace Msg
//------------------------------------------------------------------------------
#endif