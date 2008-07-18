//------------------------------------------------------------------------------
//  xmlmodelwriter.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/save/xmlmodelwriter.h"

namespace Models
{
ImplementClass(Models::XmlModelWriter, 'XMMW', Models::ModelWriter);

using namespace Util;
using namespace IO;
using namespace Attr;

//------------------------------------------------------------------------------
/**
*/
XmlModelWriter::XmlModelWriter() :
    isInBeginModel(false),
    isInBeginModelNode(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
XmlModelWriter::~XmlModelWriter()
{
    if (this->IsOpen())
    {
        this->Close();
    }
}

//------------------------------------------------------------------------------
/**
*/
String
XmlModelWriter::GetFileExtension() const
{
    return "xml";
}

//------------------------------------------------------------------------------
/**
*/
bool
XmlModelWriter::Open()
{
    n_assert(!this->isOpen);
    n_assert(this->stream.isvalid());
    n_assert(this->stream->CanWrite());

    this->isInBeginModel = false;
    this->isInBeginModelNode = false;

    // open a proxy XmlWriter 
    this->xmlWriter = XmlWriter::Create();
    this->xmlWriter->SetStream(this->stream);
    if (this->xmlWriter->Open())
    {
        // write root element
        this->xmlWriter->BeginNode("Nebula3");
        this->isOpen = true;
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
XmlModelWriter::Close()
{
    n_assert(this->isOpen);
    n_assert(!this->isInBeginModel);
    n_assert(!this->isInBeginModelNode);
    this->xmlWriter->EndNode();
    this->xmlWriter->Close();
    this->xmlWriter = 0;
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
bool
XmlModelWriter::BeginModel(const Ptr<Model>& model)
{
    n_assert(this->isOpen);
    n_assert(!this->isInBeginModel);
    n_assert(!this->isInBeginModelNode);
    this->isInBeginModel = true;    
    this->xmlWriter->BeginNode("Model");
    this->xmlWriter->SetString("name", model->GetResourceId().Value());
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
XmlModelWriter::EndModel()
{
    n_assert(this->isOpen);
    n_assert(this->isInBeginModel);
    n_assert(!this->isInBeginModelNode);
    this->xmlWriter->EndNode();
    this->isInBeginModel = false;
}

//------------------------------------------------------------------------------
/**
*/
void
XmlModelWriter::WriteModelAttrs(const AttributeContainer& attrs)
{
    n_assert(this->isOpen);
    n_assert(this->isInBeginModel);
    n_assert(!this->isInBeginModelNode);
    IndexT i;
    for (i = 0; i < attrs.GetAttrs().Size(); i++)
    {
        const Attribute& attr = attrs.GetAttrs().ValueAtIndex(i);
        this->xmlWriter->BeginNode("Attr");
        this->xmlWriter->SetString("name", attr.GetName());
        this->xmlWriter->SetString("value", attr.ValueAsString());
        this->xmlWriter->EndNode();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
XmlModelWriter::BeginModelNode(const Ptr<ModelNode>& modelNode)
{
    n_assert(this->isOpen);
    n_assert(this->isInBeginModel);
    n_assert(!this->isInBeginModelNode);
    this->isInBeginModelNode = true;
    this->xmlWriter->BeginNode("Node");
    this->xmlWriter->SetString("name", modelNode->GetName().Value());
    this->xmlWriter->SetString("class", modelNode->GetRtti()->GetName());
    if (modelNode->HasParent())
    {
        this->xmlWriter->SetString("parent", modelNode->GetParent()->GetName().Value());
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
XmlModelWriter::EndModelNode()
{
    n_assert(this->isOpen);
    n_assert(this->isInBeginModel);
    n_assert(this->isInBeginModelNode);
    this->xmlWriter->EndNode();
    this->isInBeginModelNode = false;
}

//------------------------------------------------------------------------------
/**
*/
void
XmlModelWriter::WriteModelNodeAttrs(const AttributeContainer& attrs)
{
    n_assert(this->isOpen);
    n_assert(this->isInBeginModel);
    n_assert(this->isInBeginModelNode);
    IndexT i;
    for (i = 0; i < attrs.GetAttrs().Size(); i++)
    {
        const Attribute& attr = attrs.GetAttrs().ValueAtIndex(i);
        this->xmlWriter->BeginNode("Attr");
        this->xmlWriter->SetString("name", attr.GetName());
        this->xmlWriter->SetString("value", attr.ValueAsString());
        this->xmlWriter->EndNode();
    }
}

} // namespace Models