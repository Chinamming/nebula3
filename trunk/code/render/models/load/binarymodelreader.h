#pragma once
#ifndef MODELS_BINARYMODELREADER_H
#define MODELS_BINARYMODELREADER_H
//------------------------------------------------------------------------------
/** 
    @class Models::BinaryModelReader
  
    Implements the ModelReader for the binary file format.  
    
    (C) 2007 Radon Labs GmbH
*/
#include "models/load/modelreader.h"
#include "io/binaryreader.h"

//------------------------------------------------------------------------------
namespace Models
{
class BinaryModelReader : public ModelReader
{
    __DeclareClass(BinaryModelReader);
public:
    /// constructor
    BinaryModelReader();
    /// destructor
    virtual ~BinaryModelReader();

    /// begin reading from the stream
    virtual bool Open();
    /// end reading from the stream
    virtual void Close();
    /// parse resource and build model hierarchy
    virtual bool FillModel();

    ///// set the read cursor to the first Model in the stream
    //virtual bool SetToFirstModel();
    ///// set the read cursor to the next Model in the stream
    //virtual bool SetToNextModel();
    ///// set the read cursor to the first ModelNode in the current Model
    //virtual bool SetToFirstModelNode();
    ///// set the read cursor to the next ModelNode in the current Model
    //virtual bool SetToNextModelNode();

private:
    ///// read Model data from stream
    //void ReadModelData();
    ///// read ModelNode data from stream
    //void ReadModelNodeData();
    ///// read attributes into attribute container
    //void ReadAttrs(Attr::AttributeContainer& attrs); 

    Ptr<IO::BinaryReader> binaryReader;
    bool isSetToModel;
    bool isSetToModelNode;
};

} // namespace Models
//------------------------------------------------------------------------------
#endif
