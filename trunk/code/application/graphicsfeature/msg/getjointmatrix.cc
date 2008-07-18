//------------------------------------------------------------------------------
//  msg/getjointmatrix.cc
//  (C) 2005 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "msg/getjointmatrix.h"

namespace GraphicsFeature
{
ImplementClass(GraphicsFeature::GetJointMatrix, 'GJOM', Messaging::Message);
ImplementMsgId(GetJointMatrix);

//------------------------------------------------------------------------------
/**
*/
GetJointMatrix::GetJointMatrix() : 
	isJointMatrixSet(false),
	localJointMatrixOnly(false)
{
    // empty
}

}; // namespace Msg