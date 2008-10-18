#pragma once
#ifndef MODELS_XMLMODELREADER_H
#define MODELS_XMLMODELREADER_H
//------------------------------------------------------------------------------
/**
    @class Models::XmlModelReader
  
    Implements the ModelReader for the human-readable XML file format.
    
    (C) 2007 Radon Labs GmbH
*/    
#include "models/load/modelreader.h"
#include "io/xmlreader.h"

//------------------------------------------------------------------------------
namespace Models
{
class XmlModelReader : public ModelReader
{
    __DeclareClass(XmlModelReader);
public:
    /// constructor
    XmlModelReader();
    /// destructor
    virtual ~XmlModelReader();

    /// begin reading from the stream
    virtual bool Open();
    /// end reading from the stream
    virtual void Close();
    /// parse resource and build model hierarchy
    virtual bool FillModel();
/*
    /// set the read cursor to the first Model in the stream
    virtual bool SetToFirstModel();
    /// set the read cursor to the next Model in the stream
    virtual bool SetToNextModel();
    /// set the read cursor to the first ModelNode in the current Model
    virtual bool SetToFirstModelNode();
    /// set the read cursor to the next ModelNode in the current Model
    virtual bool SetToNextModelNode();

private:
    /// read Model data from xml stream
    void ReadModelData();
    /// read ModelNode data from xml stream
    void ReadModelNodeData();
    /// read attributes into provided attribute container
    void ReadAttrs(Attr::AttributeContainer& attrs); 
*/
    Ptr<IO::XmlReader> xmlReader;
    bool isSetToModel;
    bool isSetToModelNode;
};

} // namespace Models
//------------------------------------------------------------------------------
#endif
