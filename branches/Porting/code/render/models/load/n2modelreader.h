#pragma once
#ifndef MODELS_N2MODELREADER_H
#define MODELS_N2MODELREADER_H
//------------------------------------------------------------------------------
/**
    @class Models::N2ModelReader
    
    Legacy N2 binary reader, reads a subset of .n2 files into a Model.
    NOTE: This class is very inefficient and shouldn't be used
    for production code.
    
    (C) 2007 Radon Labs GmbH
*/
#include "models/load/modelreader.h"
#include "io/binaryreader.h"
#include "util/stack.h"

//------------------------------------------------------------------------------
namespace Models
{
class CharacterNode;
class ParticleSystemNode;

class N2ModelReader : public ModelReader
{
    __DeclareClass(N2ModelReader);
public:
    /// constructor
    N2ModelReader();
    /// destructor
    virtual ~N2ModelReader();

    /// begin reading from the stream
    virtual bool Open();
    /// end reading from the stream
    virtual void Close();
    /// parse resource and build model hierarchy
    virtual bool FillModel();

private:
    enum ModelType
    {
        Static = 0,
        Character,
		ParticleSystem,
        InvalidType
    };

    /// set the read cursor to the first Model in the stream
    bool SetToFirstModel();
    /// set the read cursor to the first ModelNode in the current Model
    bool SetToFirstModelNode();
    /// set the read cursor to the next ModelNode in the current Model
    bool SetToNextModelNode();
    /// read Model data from stream
    void ReadModelData();
    /// read ModelNode data from stream
    void ReadModelNodeData();
    /// read attributes and set in modelnode
    void ReadModelNodeAttributes(const Ptr<ModelNode>& modelNode);
    /// check type of stream
    ModelType GatherModelType();
    /// fill characternode from skinanimator
    Ptr<CharacterNode> CreateCharacterNode();
	/// fill particle system node from nParticle2shapnode
	//Ptr<ParticleSystemNode> CreateParticleSystemNode();
    /// seek to a node with one of the given types, returns false if node is not found
    bool SeekToNodeOfType(const Util::Array<Util::String>& nodeNames);
    /// get current stack path as string
    Util::String GetStackPath() const;

    Ptr<IO::BinaryReader> binaryReader;
    bool isSetToModel;
    bool isSetToModelNode;
    Util::Stack<Util::StringAtom> modelNodeStack;

};

} // namespace Models
//------------------------------------------------------------------------------
#endif
    