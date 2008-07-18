//------------------------------------------------------------------------------
//  idlarg.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "tools/idlcompiler/idlarg.h"

namespace Tools
{
ImplementClass(Tools::IDLArg, 'ILAG', Core::RefCounted);

using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
IDLArg::IDLArg()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
IDLArg::Parse(XmlReader* reader)
{
    n_assert(0 != reader);
    this->name = reader->GetString("name");
    this->type = reader->GetString("type");
    this->defaultValue = reader->GetOptString("default", "");
    if (reader->SetToFirstChild("Encode"))
    {
        this->encodeCode = reader->GetContent();
        reader->SetToParent();
    }
    if (reader->SetToFirstChild("Decode"))
    {
        this->decodeCode = reader->GetContent();
        reader->SetToParent();
    }
    return true;
}

} // namespace Tools
