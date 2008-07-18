//------------------------------------------------------------------------------
//  modelwriter.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/save/modelwriter.h"

namespace Models
{
ImplementAbstractClass(Models::ModelWriter, 'MDLW', IO::StreamWriter);

//------------------------------------------------------------------------------
/**
*/
ModelWriter::~ModelWriter()
{
    // empty
}

}