#pragma once
#ifndef MODELS_BINARYMODELWRITER_H
#define MODELS_BINARYMODELWRITER_H
//------------------------------------------------------------------------------
/**
    @class Models::BinaryModelWriter
  
    Implements the ModelWriter for the binary file format.
    
    (C) 2007 Radon Labs GmbH
*/    
#include "models/save/modelwriter.h"
#include "io/binarywriter.h"

//------------------------------------------------------------------------------
namespace Models
{
class BinaryModelWriter : public ModelWriter
{
    __DeclareClass(BinaryModelWriter);
public:
    /// constructor
    BinaryModelWriter();
    /// destructor
    virtual ~BinaryModelWriter();
    
    /// get the file extension used by the writer
    virtual Util::String GetFileExtension() const;

    /// begin reading from the stream
    virtual bool Open();
    /// end reading from the stream
    virtual void Close();

    /// begin writing a new Model
    virtual bool BeginModel(const Ptr<Model>& model);
    /// write model attributes
    virtual void WriteModelAttrs(const Attr::AttributeContainer& attrs);
    /// end writing current Model
    virtual void EndModel();

    /// begin writing a new ModelNode
    virtual bool BeginModelNode(const Ptr<ModelNode>& modelNode);
    /// write mode node attributes
    virtual void WriteModelNodeAttrs(const Attr::AttributeContainer& attrs);
    /// end writing current ModelNode
    virtual void EndModelNode();

private:
    /// generic helper method to write attribute block
    void WriteAttrs(const Attr::AttributeContainer& attrs);

    Ptr<IO::BinaryWriter> binaryWriter;
    bool isInBeginModel;
    bool isInBeginModelNode;
};

} // namespace Models
//------------------------------------------------------------------------------
#endif
