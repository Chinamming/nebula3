//------------------------------------------------------------------------------
//  binarymodelwriter.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/save/binarymodelwriter.h"

namespace Models
{
ImplementClass(Models::BinaryModelWriter, 'BNMW', Models::ModelWriter);

using namespace Util;
using namespace IO;
using namespace Attr;

//------------------------------------------------------------------------------
/**
*/
BinaryModelWriter::BinaryModelWriter() :
    isInBeginModel(false),
    isInBeginModelNode(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
BinaryModelWriter::~BinaryModelWriter()
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
BinaryModelWriter::GetFileExtension() const
{
    return "n3";
}

//------------------------------------------------------------------------------
/**
*/
bool
BinaryModelWriter::Open()
{
    n_assert(!this->isOpen);
    n_assert(this->stream.isvalid());
    n_assert(this->stream->CanWrite());

    this->isInBeginModel = false;
    this->isInBeginModelNode = false;

    // open a proxy BinaryWriter 
    this->binaryWriter = BinaryWriter::Create();
    this->binaryWriter->SetStream(this->stream);
    if (this->binaryWriter->Open())
    {
        // write root element
        this->binaryWriter->WriteUInt('>NB3');
        this->isOpen = true;
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
BinaryModelWriter::Close()
{
    n_assert(this->isOpen);
    n_assert(!this->isInBeginModel);
    n_assert(!this->isInBeginModelNode);
    this->binaryWriter->WriteUInt('<NB3');
    this->binaryWriter->Close();
    this->binaryWriter = 0;
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
bool
BinaryModelWriter::BeginModel(const Ptr<Model>& model)
{
    n_assert(this->isOpen);
    n_assert(!this->isInBeginModel);
    n_assert(!this->isInBeginModelNode);
    this->isInBeginModel = true;    
    this->binaryWriter->WriteUInt('>MDL');
    this->binaryWriter->WriteString(model->GetResourceId().Value());
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
BinaryModelWriter::EndModel()
{
    n_assert(this->isOpen);
    n_assert(this->isInBeginModel);
    n_assert(!this->isInBeginModelNode);
    this->binaryWriter->WriteUInt('<MDL');
    this->isInBeginModel = false;
}

//------------------------------------------------------------------------------
/**
*/
void
BinaryModelWriter::WriteModelAttrs(const AttributeContainer& attrs)
{
    n_assert(this->isOpen);
    n_assert(this->isInBeginModel);
    n_assert(!this->isInBeginModelNode);
    this->WriteAttrs(attrs);
}

//------------------------------------------------------------------------------
/**
*/
bool
BinaryModelWriter::BeginModelNode(const Ptr<ModelNode>& modelNode)
{
    n_assert(this->isOpen);
    n_assert(this->isInBeginModel);
    n_assert(!this->isInBeginModelNode);
    this->isInBeginModelNode = true;
    this->binaryWriter->WriteUInt('>NOD');
    this->binaryWriter->WriteString(modelNode->GetName().Value());
    this->binaryWriter->WriteUInt(modelNode->GetRtti()->GetFourCC().AsUInt());
    if (modelNode->HasParent())
    {
        this->binaryWriter->WriteString(modelNode->GetParent()->GetName().Value());
    }
    else
    {
        this->binaryWriter->WriteString("");
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
BinaryModelWriter::EndModelNode()
{
    n_assert(this->isOpen);
    n_assert(this->isInBeginModel);
    n_assert(this->isInBeginModelNode);
    this->binaryWriter->WriteUInt('<NOD');
    this->isInBeginModelNode = false;
}

//------------------------------------------------------------------------------
/**
*/
void
BinaryModelWriter::WriteModelNodeAttrs(const AttributeContainer& attrs)
{
    n_assert(this->isOpen);
    n_assert(this->isInBeginModel);
    n_assert(this->isInBeginModelNode);
    this->WriteAttrs(attrs);
}

//------------------------------------------------------------------------------
/**
    Internal helper method, write generic attribute block.
*/
void
BinaryModelWriter::WriteAttrs(const AttributeContainer& attrs)
{
    this->binaryWriter->WriteUInt('>ATT');
    IndexT i;
    const Util::Dictionary<AttrId, Attribute>& attributes = attrs.GetAttrs();
    for (i = 0; i < attributes.Size(); i++)
    {
        const Attribute& attr = attributes.ValueAtIndex(i);
        this->binaryWriter->WriteUInt(attr.GetFourCC().AsUInt());
        switch (attr.GetValueType())
        {
            case IntType:
                this->binaryWriter->WriteInt(attr.GetInt());
                break;
            case FloatType:
                this->binaryWriter->WriteFloat(attr.GetFloat());
                break;
            case BoolType:
                this->binaryWriter->WriteBool(attr.GetBool());
                break;
            case Float4Type:
                this->binaryWriter->WriteFloat4(attr.GetFloat4());
                break;
            case StringType:
                this->binaryWriter->WriteString(attr.GetString());
                break;
            case Matrix44Type:
                this->binaryWriter->WriteMatrix44(attr.GetMatrix44());
                break;
            case BlobType:
                this->binaryWriter->WriteBlob(attr.GetBlob());
                break;
            case GuidType:
                this->binaryWriter->WriteGuid(attr.GetGuid());
                break;
            default:
                n_error("BinaryModelWriter: invalid attribute value type in attribute '%s'!", attr.GetName().AsCharPtr());
                break;
        }
    }
    this->binaryWriter->WriteUInt('<ATT');
}

} // namespace Models