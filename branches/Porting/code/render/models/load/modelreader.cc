//------------------------------------------------------------------------------
//  modelreader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/load/modelreader.h"

namespace Models
{
ImplementAbstractClass(Models::ModelReader, 'MDLR', IO::StreamReader);

using namespace Core;
using namespace Util;
using namespace Attr;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
ModelReader::~ModelReader()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
ModelReader::SetModelResId(const ResourceId& resId)
{
    this->modelResId = resId;
}

//------------------------------------------------------------------------------
/**
*/
const ResourceId&
ModelReader::GetModelResId() const
{
    return this->modelResId;
}
//
////------------------------------------------------------------------------------
///**
//*/
//const AttributeContainer&
//ModelReader::GetModelAttrs() const
//{
//    return this->modelAttrs;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//const StringAtom&
//ModelReader::GetModelNodeName() const
//{
//    return this->modelNodeName;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//const Rtti&
//ModelReader::GetModelNodeClassRtti() const
//{
//    n_assert(0 != this->modelNodeRtti);
//    return *(this->modelNodeRtti);
//}
//
////------------------------------------------------------------------------------
///**
//*/
//const StringAtom&
//ModelReader::GetModelNodeParentName() const
//{
//    return this->modelNodeParentName;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//const AttributeContainer&
//ModelReader::GetModelNodeAttrs() const
//{
//    return this->modelNodeAttrs;
//}

} // namespace Models