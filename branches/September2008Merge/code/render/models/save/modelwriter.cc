//------------------------------------------------------------------------------
//  modelwriter.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/save/modelwriter.h"

namespace Models
{
__ImplementAbstractClass(Models::ModelWriter, 'MDLW', IO::StreamWriter);

//------------------------------------------------------------------------------
/**
*/
ModelWriter::~ModelWriter()
{
    // empty
}

}