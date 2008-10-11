#pragma once
#ifndef MODELS_MODELREADER_H
#define MODELS_MODELREADER_H
//------------------------------------------------------------------------------
/** 
    @class Models::ModelReader
  
    Implements a stream reader for Model persistency. This is just a 
    base class for more specialized stream readers which implement
    different file formats (binary, XML, etc...).
    
    (C) 2007 Radon Labs GmbH
*/    
#include "io/streamreader.h"
#include "attr/attributecontainer.h"
#include "util/stringatom.h"
#include "resources/resourceid.h"
#include "models/model.h"

//------------------------------------------------------------------------------
namespace Models
{
class ModelReader : public IO::StreamReader
{
    __DeclareAbstractClass(ModelReader);
public:
    /// destructor
    virtual ~ModelReader();

    /// begin reading from the stream
    virtual bool Open() = 0;
    /// end reading from the stream
    virtual void Close() = 0;

    /// set the read cursor to the first Model in the stream
    //virtual bool SetToFirstModel() = 0;
    /// set the read cursor to the next Model in the stream
    //virtual bool SetToNextModel() = 0;
    /// set model resource id (some file format's don't provide their own)
    void SetModelResId(const Resources::ResourceId& resId);
    /// get the model resource id
    const Resources::ResourceId& GetModelResId() const;
    /// get attributes of current model
    //const Attr::AttributeContainer& GetModelAttrs() const;
        
    /// set the read cursor to the first ModelNode in the current Model
    //virtual bool SetToFirstModelNode() = 0;
    /// set the read cursor to the next ModelNode in the current Model
    //virtual bool SetToNextModelNode() = 0;
    /// get the model node name
    //const Util::StringAtom& GetModelNodeName() const;
    /// get model node class
    //const Core::Rtti& GetModelNodeClassRtti() const;
    /// get the name of the parent node
    //const Util::StringAtom& GetModelNodeParentName() const;
    /// get attributes of current model node
    //const Attr::AttributeContainer& GetModelNodeAttrs() const;

    /// set model for filling up with modelnodes and attributes
    void SetModel(const Ptr<Model>& model); 
    /// parse resource and build model hierarchy
    virtual bool FillModel() = 0;

protected:
    Resources::ResourceId modelResId;
    Ptr<Model> model;
    Attr::AttributeContainer modelAttrs;
    Util::StringAtom modelNodeName;
    //const Core::Rtti* modelNodeRtti;
    Util::StringAtom modelNodeParentName;
    Attr::AttributeContainer modelNodeAttrs;
};
//------------------------------------------------------------------------------
/**
*/
inline void 
ModelReader::SetModel(const Ptr<Model>& model)
{
    this->model = model;
}

} // namespace Models

//------------------------------------------------------------------------------
#endif
