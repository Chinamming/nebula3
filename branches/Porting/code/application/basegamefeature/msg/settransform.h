#ifndef MSG_SETTRANSFORM_H
#define MSG_SETTRANSFORM_H
//------------------------------------------------------------------------------
/**
    @class Messaging::SetTransform
    
    Set the complete transform of a entity, including the  physics tranform.
    Use not per Frame, the physics won't like it!
    
    (C) 2007 Radon Labs GmbH
*/
#include "msg/updatetransform.h"

//------------------------------------------------------------------------------
namespace BaseGameFeature
{
class SetTransform : public UpdateTransform
{
	DeclareClass(SetTransform);
    DeclareMsgId;
};

}; // namespace BaseGameFeature
//------------------------------------------------------------------------------
#endif
