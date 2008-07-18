//------------------------------------------------------------------------------
//  binarymodelreader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/load/binarymodelreader.h"

namespace Models
{
ImplementClass(Models::BinaryModelReader, 'BNMR', Models::ModelReader);

using namespace Core;
using namespace Util;
using namespace IO;
using namespace Attr;

//------------------------------------------------------------------------------
/**
*/
BinaryModelReader::BinaryModelReader() :
    isSetToModel(false),
    isSetToModelNode(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
BinaryModelReader::~BinaryModelReader()
{
    if (this->IsOpen())
    {
        this->Close();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
BinaryModelReader::Open()
{
    n_assert(!this->isOpen);
    n_assert(this->stream.isvalid());
    n_assert(this->stream->CanRead());

    this->isSetToModel = false;
    this->isSetToModelNode = false;

    // open a proxy binary reader and position on root node
    this->binaryReader = BinaryReader::Create();
    this->binaryReader->SetStream(this->stream);
    if (this->binaryReader->Open())
    {
        FourCC magic(this->binaryReader->ReadUInt());
        if (magic == FourCC('>NB3'))
        {
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
BinaryModelReader::Close()
{
    n_assert(this->isOpen);
    this->binaryReader->Close();
    this->binaryReader = 0;
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
bool
BinaryModelReader::FillModel()
{
    return false;
}
//
////------------------------------------------------------------------------------
///**
//*/
//bool
//BinaryModelReader::SetToFirstModel()
//{
//    n_assert(this->isOpen);
//    n_assert(!this->isSetToModel);
//    n_assert(!this->isSetToModelNode);
//    
//    FourCC tag(this->binaryReader->ReadUInt());
//    if (tag == FourCC('>MDL'))
//    {
//        // a valid model block
//        this->isSetToModel = true;
//        this->isSetToModelNode = false;
//        this->ReadModelData();
//        return true;
//    }
//    else if (tag == FourCC('<NB3'))
//    {
//        // end of file reached
//        this->isSetToModel = false;
//        this->isSetToModelNode = false;
//        return false;
//    }
//    else
//    {
//        n_error("BinaryModelReader: unexpected tag '%s' in file '%s' (file corrupt).", 
//            tag.AsString().AsCharPtr(), this->stream->GetURI().AsString().AsCharPtr());
//        return false;
//    }
//}
//
////------------------------------------------------------------------------------
///**
//*/
//bool
//BinaryModelReader::SetToNextModel()
//{
//    n_assert(this->isOpen);
//    n_assert(this->isSetToModel);
//    n_assert(!this->isSetToModelNode);
//
//    // read next Model tag, or end-of-file tag
//    FourCC tag(this->binaryReader->ReadUInt());
//    if (tag == FourCC('>MDL'))
//    {
//        // a valid model block
//        this->isSetToModel = true;
//        this->isSetToModelNode = false;
//        this->ReadModelData();
//        return true;
//    }
//    else if (tag == FourCC('<NB3'))
//    {
//        // end of file reached
//        this->isSetToModel = false;
//        this->isSetToModelNode = false;
//        return false;
//    }
//    else
//    {
//        n_error("BinaryModelReader: unexpected tag '%s' in file '%s' (file corrupt).", 
//            tag.AsString().AsCharPtr(), this->stream->GetURI().AsString().AsCharPtr());
//        return false;
//    }
//}
//
////------------------------------------------------------------------------------
///**
//*/
//bool
//BinaryModelReader::SetToFirstModelNode()
//{
//    n_assert(this->isOpen);
//    n_assert(this->isSetToModel);
//    n_assert(!this->isSetToModelNode);
//
//    FourCC tag(this->binaryReader->ReadUInt());
//    if (tag == FourCC('>NOD'))
//    {
//        // a valid model node block
//        this->isSetToModelNode = true;
//        this->ReadModelNodeData();
//        return true;
//    }
//    else if (tag == FourCC('<MDL'))
//    {
//        // end of model nodes reached
//        this->isSetToModelNode = false;
//        return false;
//    }
//    else
//    {
//        n_error("BinaryModelReader: unexpected tag '%s' in file '%s' (file corrupt).", 
//            tag.AsString().AsCharPtr(), this->stream->GetURI().AsString().AsCharPtr());
//        return false;
//    }
//}
//
////------------------------------------------------------------------------------
///**
//*/
//bool
//BinaryModelReader::SetToNextModelNode()
//{
//    n_assert(this->isOpen);
//    n_assert(this->isSetToModel);
//    n_assert(this->isSetToModelNode);
//
//    FourCC tag(this->binaryReader->ReadUInt());
//    if (tag == FourCC('>NOD'))
//    {
//        // a valid model node block
//        this->isSetToModelNode = true;
//        this->ReadModelNodeData();
//        return true;
//    }
//    else if (tag == FourCC('<MDL'))
//    {
//        // end of model nodes reached
//        this->isSetToModelNode = false;
//        return false;
//    }
//    else
//    {
//        n_error("BinaryModelReader: unexpected tag '%s' in file '%s' (file corrupt).", 
//            tag.AsString().AsCharPtr(), this->stream->GetURI().AsString().AsCharPtr());
//        return false;
//    }
//}
//
////------------------------------------------------------------------------------
///**
//*/
//void
//BinaryModelReader::ReadModelData()
//{
//    this->modelResId = this->binaryReader->ReadString();
//    this->ReadAttrs(this->modelAttrs);        
//}
//
////------------------------------------------------------------------------------
///**
//*/
//void
//BinaryModelReader::ReadModelNodeData()
//{
//    this->modelNodeName = this->binaryReader->ReadString();
//    FourCC classFourCC(this->binaryReader->ReadUInt());
//    if (Factory::Instance()->ClassExists(classFourCC))
//    {
//        this->modelNodeRtti = Factory::Instance()->GetClassRtti(classFourCC);
//    }
//    else
//    {
//        n_error("BinaryModelReader: invalid class fourcc '%s' in file '%s'!",
//            classFourCC.AsString().AsCharPtr(),
//            this->stream->GetURI().AsString().AsCharPtr());
//    }
//    this->modelNodeParentName = this->binaryReader->ReadString();
//    this->ReadAttrs(this->modelNodeAttrs);
//    FourCC endTag(this->binaryReader->ReadUInt());
//    if (endTag != FourCC('<NOD'))
//    {
//        n_error("BinaryModelReader: unexped node end tag '%s' in file '%s'!",
//            endTag.AsString().AsCharPtr(),
//            this->stream->GetURI().AsString().AsCharPtr());
//    }
//}
//
////------------------------------------------------------------------------------
///**
//*/
//void
//BinaryModelReader::ReadAttrs(AttributeContainer& attrs)
//{
//    n_assert(this->isSetToModel || this->isSetToModelNode);
//    attrs.Clear();
//    
//    // read attribute block start tag
//    FourCC startTag = this->binaryReader->ReadUInt();
//    if (startTag != FourCC('>ATT'))
//    {
//        n_error("BinaryModelReader: attribute start tag expected in file '%s' (got '%s')!",
//            this->stream->GetURI().AsString().AsCharPtr(),
//            startTag.AsString().AsCharPtr());
//    }
//
//    // read attributes
//    bool running = true;
//    while (running)
//    {
//        // read attribute tag
//        FourCC attrTag = this->binaryReader->ReadUInt();
//        if (attrTag == FourCC('<ATT'))
//        {
//            // end of attribute block reached
//            running = false;
//        }
//        else
//        {
//            // check if the fourcc is a valid attribute id
//            if (AttrId::IsValidFourCC(attrTag))
//            {
//                // read attribute
//                Attribute attr(attrTag);
//                switch (attr.GetValueType())
//                {
//                    case IntType:
//                        attr.SetInt(this->binaryReader->ReadInt());
//                        break;
//                    case FloatType:
//                        attr.SetFloat(this->binaryReader->ReadFloat());
//                        break;
//                    case BoolType:
//                        attr.SetBool(this->binaryReader->ReadBool());
//                        break;
//                    case Float4Type:
//                        attr.SetFloat4(this->binaryReader->ReadFloat4());
//                        break;
//                    case StringType:
//                        attr.SetString(this->binaryReader->ReadString());
//                        break;
//                    case Matrix44Type:
//                        attr.SetMatrix44(this->binaryReader->ReadMatrix44());
//                        break;
//                    case BlobType:
//                        attr.SetBlob(this->binaryReader->ReadBlob());
//                        break;
//                    case GuidType:
//                        attr.SetGuid(this->binaryReader->ReadGuid());
//                        break;
//                    default:
//                        n_error("BinaryModelWriter: invalid attribute value type in attribute '%s'!", attr.GetName().AsCharPtr());
//                        break;
//                }
//                if (!attrs.HasAttr(attr.GetAttrId()))
//                {
//                    attrs.SetAttr(attr);
//                }
//                else
//                {
//                    n_error("BinaryModelReader: duplicate attribute '%s' in file '%s'!",
//                        attr.GetName().AsCharPtr(), this->stream->GetURI().AsString().AsCharPtr());
//                }
//            }
//            else
//            {
//                n_error("BinaryModelReader: unexpected attribute fourcc '%s' in file '%s'\n(file corrupt or attribute types recently changed)!",
//                    attrTag.AsString().AsCharPtr(), this->stream->GetURI().AsString().AsCharPtr());
//            }
//        }
//    }
//}

} // namespace Models
