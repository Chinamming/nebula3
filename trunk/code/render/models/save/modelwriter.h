#pragma once
#ifndef MODELS_MODELWRITER_H
#define MODELS_MODELWRITER_H
//------------------------------------------------------------------------------
/**
    @class Models::ModelWriter
  
    Implements a stream writer class for Model persistency. This
    is just a base class for more specialized stream writers which
    implement different file formats.
    
    (C) 2007 Radon Labs GmbH
*/
#include "io/streamwriter.h"
#include "models/model.h"
#include "models/modelnode.h"

//------------------------------------------------------------------------------
namespace Models
{
class ModelWriter : public IO::StreamWriter
{
    __DeclareAbstractClass(ModelWriter);
public:
    /// destructor
    virtual ~ModelWriter(); 

    /// get the file extension used by the writer
    virtual Util::String GetFileExtension() const = 0;

    /// begin reading from the stream
    virtual bool Open() = 0;
    /// end reading from the stream
    virtual void Close() = 0;

    /// begin writing a new Model
    virtual bool BeginModel(const Ptr<Model>& model) = 0;
    /// write model attributes
    virtual void WriteModelAttrs(const Attr::AttributeContainer& attrs) = 0;
    /// end writing current Model
    virtual void EndModel() = 0;

    /// begin writing a new ModelNode
    virtual bool BeginModelNode(const Ptr<ModelNode>& modelNode) = 0;
    /// write mode node attributes
    virtual void WriteModelNodeAttrs(const Attr::AttributeContainer& attrs) = 0;
    /// end writing current ModelNode
    virtual void EndModelNode() = 0;
};

} // namespace Models
//------------------------------------------------------------------------------
#endif
