//------------------------------------------------------------------------------
//  xmlmodelreader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/load/xmlmodelreader.h"

namespace Models
{
ImplementClass(Models::XmlModelReader, 'XMMR', Models::ModelReader);

using namespace Core;
using namespace Util;
using namespace IO;
using namespace Attr;

//------------------------------------------------------------------------------
/**
*/
XmlModelReader::XmlModelReader() :
    isSetToModel(false),
    isSetToModelNode(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
XmlModelReader::~XmlModelReader()
{
    /*if (this->IsOpen())
    {
        this->Close();
    }*/
}

//------------------------------------------------------------------------------
/**
*/
bool 
XmlModelReader::FillModel()
{
   return false; 
}


//------------------------------------------------------------------------------
/**
*/
bool
XmlModelReader::Open()
{
    n_assert(!this->isOpen);
    n_assert(this->stream.isvalid());
    n_assert(this->stream->CanRead());

    this->isSetToModel = false;
    this->isSetToModelNode = false;

    // open a proxy xml reader and position on root node
    this->xmlReader = XmlReader::Create();
    this->xmlReader->SetStream(this->stream);
    if (this->xmlReader->Open())
    {
        if (this->xmlReader->HasNode("/Nebula3/Model"))
        {
            this->xmlReader->SetToNode("/Nebula3");
            this->isOpen = true;
            return true; 
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
XmlModelReader::Close()
{
    n_assert(this->isOpen);
    this->xmlReader->Close();
    this->xmlReader = 0;
    this->isOpen = false;
}

////------------------------------------------------------------------------------
///**
//*/
//bool
//XmlModelReader::SetToFirstModel()
//{
//    n_assert(this->isOpen);
//    n_assert(!this->isSetToModel);
//    n_assert(!this->isSetToModelNode);
//    bool success = this->xmlReader->SetToFirstChild("Model");
//    if (success)
//    {
//        this->isSetToModel = true;
//        this->isSetToModelNode = false;
//        this->ReadModelData();
//    }
//    else
//    {
//        this->isSetToModel = false;
//        this->isSetToModelNode = false;
//    }
//    return success;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//bool
//XmlModelReader::SetToNextModel()
//{
//    n_assert(this->isOpen);
//    n_assert(this->isSetToModel);
//    n_assert(!this->isSetToModelNode);
//    bool success = this->xmlReader->SetToNextChild("Model");
//    if (success)
//    {
//        this->isSetToModel = true;
//        this->isSetToModelNode = false;
//        this->ReadModelData();
//    }
//    else
//    {
//        this->isSetToModel = false;
//        this->isSetToModelNode = false;
//    }
//    return success;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//bool
//XmlModelReader::SetToFirstModelNode()
//{
//    n_assert(this->isOpen);
//    n_assert(this->isSetToModel);
//    n_assert(!this->isSetToModelNode);
//    bool success = this->xmlReader->SetToFirstChild("Node");
//    if (success)
//    {
//        this->isSetToModelNode = true;
//        this->ReadModelNodeData();
//    }
//    else
//    {
//        this->isSetToModelNode = false;
//    }
//    return success;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//bool
//XmlModelReader::SetToNextModelNode()
//{
//    n_assert(this->isOpen);
//    n_assert(this->isSetToModel);
//    n_assert(this->isSetToModelNode);
//    bool success = this->xmlReader->SetToNextChild("Node");
//    if (success)
//    {
//        this->isSetToModelNode = true;
//        this->ReadModelNodeData();
//    }
//    else
//    {
//        this->isSetToModelNode = false;
//    }
//    return success;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//void
//XmlModelReader::ReadModelData()
//{
//    n_assert(this->xmlReader->HasAttr("name"));
//    this->modelResId = this->xmlReader->GetString("name");
//    this->ReadAttrs(this->modelAttrs);        
//}
//
////------------------------------------------------------------------------------
///**
//*/
//void
//XmlModelReader::ReadModelNodeData()
//{
//    n_assert(this->xmlReader->HasAttr("name"));
//    n_assert(this->xmlReader->HasAttr("class"));
//    this->modelNodeName = this->xmlReader->GetString("name");
//    if (this->xmlReader->HasAttr("parent"))
//    {
//        this->modelNodeParentName = this->xmlReader->GetString("parent");
//    }
//    else
//    {
//        this->modelNodeParentName.Clear();
//    }
//    String className = this->xmlReader->GetString("class");
//    if (Factory::Instance()->ClassExists(className))
//    {
//        this->modelNodeRtti = Factory::Instance()->GetClassRtti(className);
//    }
//    else
//    {
//        n_error("XmlModelReader: invalid class name '%s' in file '%s'!", 
//            className.AsCharPtr(),
//            this->stream->GetURI().AsString().AsCharPtr());
//    }
//    this->ReadAttrs(this->modelNodeAttrs);        
//}
//
////------------------------------------------------------------------------------
///**
//*/
//void
//XmlModelReader::ReadAttrs(AttributeContainer& attrs)
//{
//    n_assert(this->isSetToModel || this->isSetToModelNode);
//    attrs.Clear();
//    if (this->xmlReader->SetToFirstChild("Attr")) do
//    {
//        String name = this->xmlReader->GetString("name");
//        String value = this->xmlReader->GetString("value");
//        if (AttrId::IsValidName(name))
//        {
//            Attribute attr(name);
//            attr.SetValueFromString(value);        
//            if (!attrs.HasAttr(attr.GetAttrId()))
//            {
//                attrs.SetAttr(attr);
//            }
//            else
//            {
//                n_error("XmlModelReader: duplicate attribute '%s' in file '%s'!",
//                    name.AsCharPtr(), this->stream->GetURI().AsString().AsCharPtr());
//            }
//        }
//        else
//        {
//            n_error("XmlModelReader: invalid attribute name '%s' in file '%s'!",
//                name.AsCharPtr(), this->stream->GetURI().AsString().AsCharPtr());
//        }
//    }
//    while (this->xmlReader->SetToNextChild("Attr"));
//}

} // namespace Models
