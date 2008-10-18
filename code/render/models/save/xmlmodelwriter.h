#pragma once
#ifndef MODELS_XMLMODELWRITER_H
#define MODELS_XMLMODELWRITER_H
//------------------------------------------------------------------------------
/**
    @class Models::XmlModelWriter
  
    Implements the ModelWriter class for the human-readable XML
    file format.
    
    (C) 2007 Radon Labs GmbH
*/    
#include "models/save/modelwriter.h"
#include "io/xmlwriter.h"

//------------------------------------------------------------------------------
namespace Models
{
class XmlModelWriter : public ModelWriter
{
    __DeclareClass(XmlModelWriter);
public:
    /// constructor
    XmlModelWriter();
    /// destructor
    virtual ~XmlModelWriter();

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
    Ptr<IO::XmlWriter> xmlWriter;
    bool isInBeginModel;
    bool isInBeginModelNode;
};

} // namespace Models
//------------------------------------------------------------------------------
#endif
