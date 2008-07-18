//------------------------------------------------------------------------------
//  n2modelreader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/load/n2modelreader.h"
#include "math/point.h"
#include "math/vector.h"
#include "models/attributes.h"
#include "models/nodes/transformnode.h"
#include "models/nodes/shapenode.h"
#include "models/nodes/skinshapenode.h"
#include "models/nodes/characternode.h"

#include "models/nodes/particlesystemnode.h"

#include "system/byteorder.h"

namespace Models
{
ImplementClass(Models::N2ModelReader, 'N2MR', Models::ModelReader);

using namespace IO;
using namespace Util;
using namespace Math;
using namespace Attr;
using namespace System;

//------------------------------------------------------------------------------
/**
*/
N2ModelReader::N2ModelReader() :
    isSetToModel(false),
    isSetToModelNode(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
N2ModelReader::~N2ModelReader()
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
N2ModelReader::Open()
{
    n_assert(!this->IsOpen());
    n_assert(this->stream.isvalid());
    n_assert(this->stream->CanRead());
    n_assert(this->stream->CanSeek());
    n_assert(!this->stream->IsMapped());
    n_assert(this->modelResId.Value().IsValid());
    n_assert(this->model.isvalid());

    this->isSetToModel = false;
    this->isSetToModelNode = false;

    // open a proxy binary reader and position on root node
    this->binaryReader = BinaryReader::Create();
    this->binaryReader->SetStream(this->stream);
    this->stream->SetAccessPattern(Stream::Random);
    this->binaryReader->SetStreamByteOrder(ByteOrder::LittleEndian);
    if (this->binaryReader->Open())
    {        
        // make sure it's a binary N2 file
        FourCC magic(this->binaryReader->ReadUInt());
        if (magic == FourCC('NOB0'))
        {
            // skip the header data
            String headerData = this->binaryReader->ReadString();
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
N2ModelReader::Close()
{
    n_assert(this->IsOpen());
    this->binaryReader->Close();
    this->binaryReader = 0;
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
bool
N2ModelReader::FillModel()
{
    n_assert(this->model.isvalid());

    // check if special case: if its character file without character3node
    bool success = true;
    ModelType type = GatherModelType();
    switch (type)
    {
        case Character:
            {
                this->SetToFirstModel();
                // jump to skinanimator and fill characternode with animators attributes
                Ptr<CharacterNode> characterNode = this->CreateCharacterNode();

                // attach to our model
                this->model->AttachNode(characterNode.upcast<ModelNode>());

                // go thru rest of hierarchy and build skinshapenodes
                FourCC fourCC = this->binaryReader->ReadUInt();
                String selPath = this->binaryReader->ReadString();   
                this->isSetToModelNode = true;
                while(this->SetToNextModelNode()){}
            }
            break;        
        case Static:
            {
                this->SetToFirstModel();
                // go thru hierarchy and build modelnodes
                if (this->SetToFirstModelNode()) do
                {
                }
                while(this->SetToNextModelNode());
            }
            break;
    }

    return success;
}

//------------------------------------------------------------------------------
/**
*/
N2ModelReader::ModelType
N2ModelReader::GatherModelType()
{    
    // is object skinned character or only static 
    ModelType type = Static;
    Array<String> characterTypes;
    characterTypes.Append("ncharacter3node");
    characterTypes.Append("nskinanimator");
    
	if (SeekToNodeOfType(characterTypes))
    {
        // character 
        type = Character;
    }
	// back to begining of stream
    this->stream->Seek(0, Stream::Begin);
    this->modelNodeStack.Clear();

    return type;
}

//------------------------------------------------------------------------------
/**
*/
bool 
N2ModelReader::SeekToNodeOfType(const Util::Array<Util::String>& nodeNames)
{
    // search for node with one name of nodeNames
    const FourCC newFourCC('_new');
    const FourCC selFourCC('_sel');
    FourCC curFourCC;
    while (!this->Eof())
    {
        curFourCC = this->binaryReader->ReadUInt();   
        if (curFourCC == FourCC('_new'))
        {
            String objClass = this->binaryReader->ReadString();
            String name = this->binaryReader->ReadString();
            if (nodeNames.FindIndex(objClass) != -1)
            {
                return true;
            }
            this->modelNodeStack.Push(name);
        }
        else if (curFourCC == selFourCC)
        {
            // skip relative path
            String selPath = this->binaryReader->ReadString();     
            this->modelNodeStack.Pop();

            // need to skip any additional sels
            while (!this->Eof() && ((curFourCC = this->binaryReader->ReadUInt()) == selFourCC))
            {                
                selPath = this->binaryReader->ReadString();              
                this->modelNodeStack.Pop();
            }
            
            if (!this->Eof())
            {                
                // go back to last FourCC
                this->stream->Seek(-4, Stream::Current);
            }
        }
        else
        {
            // skip data
            ushort length = this->binaryReader->ReadUShort();
            this->stream->Seek(length, Stream::Current);
        }
    };
    return false;
}

//------------------------------------------------------------------------------
/**
    Seeks to nskinanimator and sets all characternode attributes
*/
Ptr<CharacterNode> 
N2ModelReader::CreateCharacterNode()
{
    // search for ncharacter3node or nskinanimator
    Array<String> characterTypes;
    characterTypes.Append("ncharacter3skinanimator");
    characterTypes.Append("nskinanimator");
    Ptr<CharacterNode> characterNode;
    if (SeekToNodeOfType(characterTypes))
    {        
        // create characternode and attach to model
        characterNode = CharacterNode::Create();
        this->modelNodeStack.Push(Util::StringAtom("character"));
        characterNode->SetName(GetStackPath());
        // read attributes    
        this->ReadModelNodeAttributes(characterNode.upcast<ModelNode>());
    }   
    else
    {
        n_error("No Character3Node or skinanimator found!");
    }

    return characterNode;
}

//------------------------------------------------------------------------------
/**
*/
bool
N2ModelReader::SetToFirstModel()
{
    n_assert(this->isOpen);
    n_assert(!this->isSetToModel);
    n_assert(!this->isSetToModelNode);

    // note: there is only one Model per n2 file, so this method is kind of simple
    this->isSetToModel = true;
    this->isSetToModelNode = false;
    this->ReadModelData();
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
N2ModelReader::SetToFirstModelNode()
{
    n_assert(this->isOpen);
    n_assert(this->isSetToModel);
    n_assert(!this->isSetToModelNode);

    // the fourcc tag we read SHOULD be a _new, which marks
    // the beginning of a new object
    FourCC fourCC = this->binaryReader->ReadUInt();
    if (fourCC == FourCC('_new'))
    {
        this->isSetToModelNode = true;
        this->ReadModelNodeData();
        return true;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
N2ModelReader::SetToNextModelNode()
{
    n_assert(this->isOpen);
    n_assert(this->isSetToModel);
    n_assert(this->isSetToModelNode);

    // read the next FourCC tag
    const FourCC newFourCC('_new');
    const FourCC selFourCC('_sel');
    FourCC curFourCC = this->binaryReader->ReadUInt();
	Util::String strFourCC = curFourCC.AsString();
    if (curFourCC == newFourCC)
    {
        // a new model node
        this->isSetToModelNode = true;
        this->ReadModelNodeData();
    }
    else if (curFourCC == selFourCC)
    {
        // the end of a model node
        this->modelNodeStack.Pop();

        // skip relative path (we assume it's "..")
        String selPath = this->binaryReader->ReadString();

        // need to skip any additional sels
        while (!this->Eof() && ((curFourCC = this->binaryReader->ReadUInt()) == selFourCC))
        {
            if (this->modelNodeStack.Size() > 0)
            {
                this->modelNodeStack.Pop();
            }
            selPath = this->binaryReader->ReadString();
        }
        if (this->Eof())
        {
            return false;
        }
        // go back to last FourCC
        this->stream->Seek(-4, Stream::Current);
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
N2ModelReader::ReadModelData()
{
    // just skip all data tags until we find a _new statement
    const FourCC newFourCC('_new');
    const FourCC selFourCC('_sel');
    FourCC curFourCC;
    bool stop = false; 
    do
    {
        // read next FourCC and check for special case
        curFourCC = this->binaryReader->ReadUInt();
        if ((curFourCC != newFourCC) && (curFourCC != selFourCC))
        {
            // skip data block
            ushort length = this->binaryReader->ReadUShort();
            this->stream->Seek(length, Stream::Current);
        }
        else
        {
            stop = true;
        }
    }
    while (!stop && !this->Eof());
    n_assert(curFourCC == newFourCC);

    // need to seek back to the beginning of the _new statement
    this->stream->Seek(-4, Stream::Current);
}

//------------------------------------------------------------------------------
/**
*/
void
N2ModelReader::ReadModelNodeData()
{
    // read the stuff following a '_new' tag
    String objClass = this->binaryReader->ReadString();
    String objName  = this->binaryReader->ReadString();

    Ptr<ModelNode> newModelNode;
    if (objClass == "ntransformnode")
    {
        newModelNode = TransformNode::Create();
    }
    else if (objClass == "nshapenode")
    {
        newModelNode = ShapeNode::Create();
    }        
    else if (objClass == "nskinshapenode" || objClass == "ncharacter3skinshapenode")
    {
        newModelNode = SkinShapeNode::Create();          
    }
	else if(objClass == "nparticleshapenode2")
	{
		newModelNode = ParticleSystemNode::Create();
	}
    else
    {
        // unknown class, just simulate this with a transform node
        newModelNode = TransformNode::Create();
    }
    
    if (this->modelNodeStack.IsEmpty())
    {
        this->modelNodeParentName.Clear();
    }
    else
    {
        // sort into node hierachy
        this->modelNodeParentName = GetStackPath();
		//debug
		Util::String test = this->modelNodeParentName.Value();
		//debug
        n_assert(this->model->HasNode(modelNodeParentName));
        const Ptr<ModelNode>& parentNode = this->model->LookupNode(modelNodeParentName);
        newModelNode->SetParent(parentNode);
        parentNode->AddChild(newModelNode);
    }
    this->modelNodeStack.Push(objName);
	Util::String test = GetStackPath();
    newModelNode->SetName(GetStackPath());
    
    // read known attributes, skip unknown attributes
    // until a _new oder _sel tag occurs
    this->ReadModelNodeAttributes(newModelNode);

    // attach to our model
    this->model->AttachNode(newModelNode);
}

//------------------------------------------------------------------------------
/**
*/
void
N2ModelReader::ReadModelNodeAttributes(const Ptr<ModelNode>& modelNode)
{
    // read known attributes, skip unknown attributes
    // until a _new oder _sel tag occurs
    const FourCC newFourCC('_new');
    const FourCC selFourCC('_sel');
    FourCC curFourCC;
    bool stop = false; 
    do
    {
        // read next FourCC and check for special case
        curFourCC = this->binaryReader->ReadUInt();
        if ((curFourCC != newFourCC) && (curFourCC != selFourCC))
        {
            ushort length = this->binaryReader->ReadUShort();
            if (FourCC('SLCB') == curFourCC)
            {
                // .setlocalbox
                float x, y, z;
                x = this->binaryReader->ReadFloat();
                y = this->binaryReader->ReadFloat();
                z = this->binaryReader->ReadFloat();
                modelNode->SetFloat4(Attr::BoxCenter, point(x, y, z));
                x = this->binaryReader->ReadFloat();
                y = this->binaryReader->ReadFloat();
                z = this->binaryReader->ReadFloat();
                modelNode->SetFloat4(Attr::BoxExtents, vector(x, y, z));
            }
            else if (FourCC('SPOS') == curFourCC)
            {
                // .setposition
                float x = this->binaryReader->ReadFloat();
                float y = this->binaryReader->ReadFloat();
                float z = this->binaryReader->ReadFloat();
                modelNode->SetFloat4(Attr::Position, point(x, y, z));
            }
            else if (FourCC('SQUT') == curFourCC)
            {
                // .setquat
                float4 q = this->binaryReader->ReadFloat4();
                modelNode->SetFloat4(Attr::Rotate, q);
            }
            else if (FourCC('SSCL') == curFourCC)
            {
                // .setscale
                float x = this->binaryReader->ReadFloat();
                float y = this->binaryReader->ReadFloat();
                float z = this->binaryReader->ReadFloat();
                modelNode->SetFloat4(Attr::Scale, vector(x, y, z));
            }
            else if (FourCC('STXT') == curFourCC)
            {
                // .settexture
                String shaderParam = this->binaryReader->ReadString();
                String texResId = this->binaryReader->ReadString();
            #if __WII__
                // substitute .dds extension with tpl
                texResId.SubstituteString(".dds", ".tpl");   
            #endif
                
                if (AttrId::IsValidName(shaderParam))
                {
                    modelNode->SetString(shaderParam, texResId);
                }
            }
            else if (FourCC('SINT') == curFourCC)
            {
                // .setint
                String shaderParam = this->binaryReader->ReadString();
                int val = this->binaryReader->ReadInt();
                if (AttrId::IsValidName(shaderParam))
                {
                    modelNode->SetInt(shaderParam, val);
                }
            }
            else if (FourCC('SFLT') == curFourCC)
            {
                // .setfloat
                String shaderParam = this->binaryReader->ReadString();
                float val = this->binaryReader->ReadFloat();
                if (AttrId::IsValidName(shaderParam))
                {
                    modelNode->SetFloat(shaderParam, val);
                }
            }
            else if (FourCC('SSHD') == curFourCC)
            {
                // .setshader
                String shaderResId("shd:");
                shaderResId.Append(this->binaryReader->ReadString());
                // FIXME: n2 -> n3 shader mapping
                if (shaderResId == "shd:hair" 
                    || shaderResId == "shd:environment_skinned")
                {
                    if (shaderResId == "shd:hair")
                    {
                        modelNode->SetType(Models::ModelNodeType::Alpha);
                    }
                    shaderResId = "shd:skinned";
                }
				else if(shaderResId == "shd:particle2_additive"
				        || shaderResId == "shd:alpha" )
				{
				    if (shaderResId == "shd:alpha")
                    {
                        modelNode->SetType(Models::ModelNodeType::Alpha);
                    }
					shaderResId = "shd:static";
				}

                modelNode->SetString(Attr::Shader, shaderResId);                          
            }
            else if (FourCC('SMSH') == curFourCC)
            {
                // .setmesh
                String meshName = this->binaryReader->ReadString();
                modelNode->SetString(Attr::MeshResourceId, meshName);
            }
            else if (FourCC('SGRI') == curFourCC)
            {
                // .setgroupindex
                int groupIndex = this->binaryReader->ReadInt();
                modelNode->SetInt(Attr::MeshGroupIndex, groupIndex);
            }
            else if (FourCC('SANM') == curFourCC)
            {
                //.setanim
                n_assert(modelNode->IsA(CharacterNode::RTTI));
                CharacterNode* characterNode = modelNode.downcast<CharacterNode>().get();
                characterNode->SetAnim(this->binaryReader->ReadString());
            }
            else if (FourCC('BJNT') == curFourCC)
            {
                //.beginjoints
                n_assert(modelNode->IsA(CharacterNode::RTTI));
                CharacterNode* characterNode = modelNode.downcast<CharacterNode>().get();
                characterNode->BeginJoints(this->binaryReader->ReadInt());
            }
            else if (FourCC('SJNT') == curFourCC)
            {
                //.setjoints
                n_assert(modelNode->IsA(CharacterNode::RTTI));
                CharacterNode* characterNode = modelNode.downcast<CharacterNode>().get();

                int jointIndex = this->binaryReader->ReadInt();
                int parentJointIndex = this->binaryReader->ReadInt(); 
                vector translation;
                translation.x() = this->binaryReader->ReadFloat();
                translation.y() = this->binaryReader->ReadFloat();
                translation.z() = this->binaryReader->ReadFloat();
                quaternion rotation;
                rotation.x() = this->binaryReader->ReadFloat();
                rotation.y() = this->binaryReader->ReadFloat();
                rotation.z() = this->binaryReader->ReadFloat();
                rotation.w() = this->binaryReader->ReadFloat();
                vector scale;
                scale.x() = this->binaryReader->ReadFloat();
                scale.y() = this->binaryReader->ReadFloat();
                scale.z() = this->binaryReader->ReadFloat();
                String name = this->binaryReader->ReadString();

                characterNode->SetJoint(jointIndex, parentJointIndex, translation, rotation, scale, name);
            }
            else if (FourCC('EJNT') == curFourCC)
            {
                //.endjoints
                n_assert(modelNode->IsA(CharacterNode::RTTI));
                CharacterNode* characterNode = modelNode.downcast<CharacterNode>().get();
                characterNode->EndJoints();
            }
            else if (FourCC('BGCL') == curFourCC)
            {
                //.beginclips
                n_assert(modelNode->IsA(CharacterNode::RTTI));
                CharacterNode* characterNode = modelNode.downcast<CharacterNode>().get();
                characterNode->BeginClips(this->binaryReader->ReadInt());
            }
            else if (FourCC('STCL') == curFourCC)
            {
                //.setclips
                n_assert(modelNode->IsA(CharacterNode::RTTI));
                CharacterNode* characterNode = modelNode.downcast<CharacterNode>().get();
                int clipIndex = this->binaryReader->ReadInt();
                int animGroupIndex = this->binaryReader->ReadInt();
                Util::String clipName = this->binaryReader->ReadString();
                characterNode->SetClip(clipIndex, animGroupIndex, clipName);
            }
            else if (FourCC('EDCL') == curFourCC)
            {
                //.endclips
                n_assert(modelNode->IsA(CharacterNode::RTTI));                
                CharacterNode* characterNode = modelNode.downcast<CharacterNode>().get();
                characterNode->EndClips();
            }
            else if (FourCC('SVRT') == curFourCC)
            {
                //.setvariations
                n_assert(modelNode->IsA(CharacterNode::RTTI));                
                CharacterNode* characterNode = modelNode.downcast<CharacterNode>().get();
                characterNode->SetVariationsUri(this->binaryReader->ReadString());
            }
            else if (FourCC('BGFR') == curFourCC)
            {
                //.beginfragments
                n_assert(modelNode->IsA(SkinShapeNode::RTTI));                
                SkinShapeNode* skinShapeNode = modelNode.downcast<SkinShapeNode>().get();
                skinShapeNode->BeginFragments(this->binaryReader->ReadInt());
            }
            else if (FourCC('SFGI') == curFourCC)
            {
                //.setfraggroupindex
                n_assert(modelNode->IsA(SkinShapeNode::RTTI));                
                SkinShapeNode* skinShapeNode = modelNode.downcast<SkinShapeNode>().get();
                int fragIndex = this->binaryReader->ReadInt();
                int primGrpIndex = this->binaryReader->ReadInt();
                skinShapeNode->SetFragGroupIndex(fragIndex, primGrpIndex);
            }
            else if (FourCC('EDFR') == curFourCC)
            {
                //.endfragments
                n_assert(modelNode->IsA(SkinShapeNode::RTTI));                
                SkinShapeNode* skinShapeNode = modelNode.downcast<SkinShapeNode>().get();
                skinShapeNode->EndFragments();
            }
            else if (FourCC('BGJP') == curFourCC)
            {
                //.beginjointpalette
                n_assert(modelNode->IsA(SkinShapeNode::RTTI));                
                SkinShapeNode* skinShapeNode = modelNode.downcast<SkinShapeNode>().get();
                int fragIndex = this->binaryReader->ReadInt();
                int numJoints = this->binaryReader->ReadInt();
                skinShapeNode->BeginJointPalette(fragIndex, numJoints);
            }
            else if (FourCC('SJID') == curFourCC)
            {
                //.setjointindices
                n_assert(modelNode->IsA(SkinShapeNode::RTTI));                
                SkinShapeNode* skinShapeNode = modelNode.downcast<SkinShapeNode>().get();
                int fragIndex         = this->binaryReader->ReadInt();
                int paletteStartIndex = this->binaryReader->ReadInt();
                int j0                = this->binaryReader->ReadInt();
                int j1                = this->binaryReader->ReadInt();
                int j2                = this->binaryReader->ReadInt();
                int j3                = this->binaryReader->ReadInt();
                int j4                = this->binaryReader->ReadInt();
                int j5                = this->binaryReader->ReadInt();
                int j6                = this->binaryReader->ReadInt();
                int j7                = this->binaryReader->ReadInt();

                skinShapeNode->SetJointIndices(fragIndex, paletteStartIndex, j0, j1, j2, j3, j4, j5, j6, j7);
            }
            else if (FourCC('EDJP') == curFourCC)
            {
                //.endjointpalette
                n_assert(modelNode->IsA(SkinShapeNode::RTTI));                
                SkinShapeNode* skinShapeNode = modelNode.downcast<SkinShapeNode>().get();
                skinShapeNode->EndJointPalette(this->binaryReader->ReadInt());
            }
			//Particle System cases
			else if(FourCC('SINV') == curFourCC)//cl->AddCmd("v_setinvisible_b", 'SINV', n_p2setinvisible);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particlesystemnode = modelNode.downcast<ParticleSystemNode>().get();
				particlesystemnode->SetInvisible(this->binaryReader->ReadBool());
			}
			else if(FourCC('SEMD') == curFourCC)//cl->AddCmd("v_setemissionduration_f", 'SEMD', n_p2setemissionduration);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				particleSystemNode->SetEmissionDuration(this->binaryReader->ReadFloat());
			}
			else if(FourCC('SLOP') == curFourCC)//cl->AddCmd("v_setloop_b", 'SLOP', n_p2setloop);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				particleSystemNode->SetLoop(this->binaryReader->ReadBool());
			}
			else if(FourCC('SACD') == curFourCC)//cl->AddCmd("v_setactivitydistance_f", 'SACD', n_p2setactivitydistance);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				particleSystemNode->SetActivityDistance(this->binaryReader->ReadFloat());
			}
			else if(FourCC('SROF') == curFourCC)//cl->AddCmd("v_setrenderoldestfirst_b", 'SROF', n_p2setrenderoldestfirst);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				particleSystemNode->SetRenderOldestFirst(this->binaryReader->ReadBool());
			}
			else if(FourCC('SBBO') == curFourCC)//cl->AddCmd("v_setbillboardorientation_b", 'SBBO', n_p2setbillboardorientation);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				particleSystemNode->SetBillboardOrientation(this->binaryReader->ReadBool());
			}
			else if(FourCC('SRMN') == curFourCC)//cl->AddCmd("v_setstartrotationmin_f", 'SRMN', n_p2setstartrotationmin);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				particleSystemNode->SetStartRotationMin(this->binaryReader->ReadFloat());

			}
			else if(FourCC('SRMX') == curFourCC)//cl->AddCmd("v_setstartrotationmax_f", 'SRMX', n_p2setstartrotationmax);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				particleSystemNode->SetStartRotationMax(this->binaryReader->ReadFloat());
			}
			else if(FourCC('SGRV') == curFourCC)//cl->AddCmd("v_setgravity_f", 'SGRV', n_p2setgravity);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				particleSystemNode->SetGravity(this->binaryReader->ReadFloat());
			}
			else if(FourCC('SPST') == curFourCC)//cl->AddCmd("v_setparticlestretch_f", 'SPST', n_p2setparticlestretch);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				particleSystemNode->SetParticleStretch(this->binaryReader->ReadFloat());
			}
			else if(FourCC('STTX') == curFourCC)//cl->AddCmd("v_settiletexture_i", 'STTX', n_p2settiletexture);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				particleSystemNode->SetTileTexture(this->binaryReader->ReadInt());

			}
			else if(FourCC('SSTS') == curFourCC)//cl->AddCmd("v_setstretchtostart_b", 'SSTS', n_p2setstretchtostart);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				particleSystemNode->SetStretchToStart(this->binaryReader->ReadBool());
			}
			else if(FourCC('SPCT') == curFourCC)//cl->AddCmd("v_setprecalctime_f", 'SPCT', n_p2setprecalctime);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				particleSystemNode->SetPrecalcTime(this->binaryReader->ReadFloat());
			}
			else if(FourCC('SSDT') == curFourCC)//cl->AddCmd("v_setstretchdetail_i", 'SSDT', n_p2setstretchdetail);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				particleSystemNode->SetStretchDetail(this->binaryReader->ReadInt());
			}
			else if(FourCC('SVAF') == curFourCC)//cl->AddCmd("v_setviewanglefade_b", 'SVAF', n_p2setviewanglefade);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				particleSystemNode->SetViewAngleFade(this->binaryReader->ReadBool());
			}
			else if(FourCC('STDL') == curFourCC)//cl->AddCmd("v_setstartdelay_f", 'STDL', n_p2setstartdelay);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				particleSystemNode->SetStartDelay(this->binaryReader->ReadFloat());
			}
			else if(FourCC('SCVA') == curFourCC)//cl->AddCmd("v_setemissionfrequency_ffffffffi", 'SCVA', n_p2setemissionfrequency);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				float values[8];
				for(int i = 0; i < 8; i++) values[i] = binaryReader->ReadFloat();
				particleSystemNode->SetCurve(ParticleSystem::ParticleEmitter::EmissionFrequency, ParticleSystem::EnvelopeCurve(
					values[0], values[1], values[2], values[3],
					values[4], values[5], values[6], values[7],
					this->binaryReader->ReadInt()));
			}
			else if(FourCC('SCVB') == curFourCC)//cl->AddCmd("v_setparticlelifetime_ffffffffi", 'SCVB', n_p2setparticlelifetime);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				float values[8];
				for(int i = 0; i < 8; i++) values[i] = this->binaryReader->ReadFloat();
				particleSystemNode->SetCurve(ParticleSystem::ParticleEmitter::ParticleLifeTime, ParticleSystem::EnvelopeCurve(
					values[0], values[1], values[2], values[3],
					values[4], values[5], values[6], values[7],
					this->binaryReader->ReadInt()));
			}
			else if(FourCC('SCVC') == curFourCC)//cl->AddCmd("v_setparticlergb_ffffffffffffff", 'SCVC', n_p2setparticlergb);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				float values[14];
				for(int i = 0; i < 14; i++) values[i] = this->binaryReader->ReadFloat();   
				particleSystemNode->SetRGBCurve(ParticleSystem::Vector3EnvelopeCurve(
					float4(values[0], values[1], values[2],1),
					float4(values[3], values[4], values[5],1),
					float4(values[6], values[7], values[8],1),
					float4(values[9], values[10], values[11],1),
					values[12], values[13]));
			}
			else if(FourCC('SCVD') == curFourCC)//cl->AddCmd("v_setparticlespreadmin_ffffffffi", 'SCVD', n_p2setparticlespreadmin);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				float values[8];
				for(int i = 0; i < 8; i++) values[i] = this->binaryReader->ReadFloat();
				particleSystemNode->SetCurve(ParticleSystem::ParticleEmitter::ParticleSpreadMin, ParticleSystem::EnvelopeCurve(
					values[0], values[1], values[2], values[3],
					values[4], values[5], values[6], values[7],
					this->binaryReader->ReadInt()));
				
			}
			else if(FourCC('SCVE') == curFourCC)//cl->AddCmd("v_setparticlespreadmax_ffffffffi", 'SCVE', n_p2setparticlespreadmax);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				float values[8];
				for(int i = 0; i < 8; i++) values[i] = this->binaryReader->ReadFloat();
				particleSystemNode->SetCurve(ParticleSystem::ParticleEmitter::ParticleSpreadMax, ParticleSystem::EnvelopeCurve(
					values[0], values[1], values[2], values[3],
					values[4], values[5], values[6], values[7],
					this->binaryReader->ReadInt()));
			}
			else if(FourCC('SCVF') == curFourCC)//cl->AddCmd("v_setparticlestartvelocity_ffffffffi", 'SCVF', n_p2setparticlestartvelocity);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				float values[8];
				for(int i = 0; i < 8; i++) values[i] = this->binaryReader->ReadFloat();
				particleSystemNode->SetCurve(ParticleSystem::ParticleEmitter::ParticleStartVelocity, ParticleSystem::EnvelopeCurve(
					values[0], values[1], values[2], values[3],
					values[4], values[5], values[6], values[7],
					this->binaryReader->ReadInt()));
			}
			else if(FourCC('SCVH') == curFourCC)//cl->AddCmd("v_setparticlerotationvelocity_ffffffffi", 'SCVH', n_p2setparticlerotationvelocity);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				float values[8];
				for(int i = 0; i < 8; i++) values[i] = this->binaryReader->ReadFloat();
				particleSystemNode->SetCurve(ParticleSystem::ParticleEmitter::ParticleRotationVelocity, ParticleSystem::EnvelopeCurve(
					values[0], values[1], values[2], values[3],
					values[4], values[5], values[6], values[7],
					this->binaryReader->ReadInt()));
			}
			else if(FourCC('SCVJ') == curFourCC)//cl->AddCmd("v_setparticlesize_ffffffffi", 'SCVJ', n_p2setparticlesize);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				float values[8];
				for(int i = 0; i < 8; i++) values[i] = this->binaryReader->ReadFloat();
				particleSystemNode->SetCurve(ParticleSystem::ParticleEmitter::ParticleScale, ParticleSystem::EnvelopeCurve(
					values[0], values[1], values[2], values[3],
					values[4], values[5], values[6], values[7],
					this->binaryReader->ReadInt()));
			}
			else if(FourCC('SCVL') == curFourCC)//cl->AddCmd("v_setparticlemass_ffffffffi", 'SCVL', n_p2setparticlemass);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				float values[8];
				for(int i = 0; i < 8; i++) values[i] = this->binaryReader->ReadFloat();
				particleSystemNode->SetCurve(ParticleSystem::ParticleEmitter::ParticleMass, ParticleSystem::EnvelopeCurve(
					values[0], values[1], values[2], values[3],
					values[4], values[5], values[6], values[7],
					this->binaryReader->ReadInt()));
			}
			else if(FourCC('STMM') == curFourCC)//cl->AddCmd("v_settimemanipulator_ffffffffi", 'STMM', n_p2settimemanipulator);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				float values[8];
				for(int i = 0; i < 8; i++) values[i] = this->binaryReader->ReadFloat();
				particleSystemNode->SetCurve(ParticleSystem::ParticleEmitter::TimeManipulator, ParticleSystem::EnvelopeCurve(
					values[0], values[1], values[2], values[3],
					values[4], values[5], values[6], values[7],
					this->binaryReader->ReadInt()));
			}
			else if(FourCC('SCVM') == curFourCC)//cl->AddCmd("v_setparticlealpha_ffffffffi", 'SCVM', n_p2setparticlealpha);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				float values[8];
				for(int i = 0; i < 8; i++) values[i] = this->binaryReader->ReadFloat();
				particleSystemNode->SetCurve(ParticleSystem::ParticleEmitter::ParticleAlpha, ParticleSystem::EnvelopeCurve(
					values[0], values[1], values[2], values[3],
					values[4], values[5], values[6], values[7],
					this->binaryReader->ReadInt()));
			}
			else if(FourCC('SCVN') == curFourCC)//cl->AddCmd("v_setparticlevelocityfactor_ffffffffi", 'SCVN', n_p2setparticlevelocityfactor);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				float values[8];
				for(int i = 0; i < 8; i++) values[i] = this->binaryReader->ReadFloat();
				particleSystemNode->SetCurve(ParticleSystem::ParticleEmitter::ParticleVelocityFactor, ParticleSystem::EnvelopeCurve(
					values[0], values[1], values[2], values[3],
					values[4], values[5], values[6], values[7],
					this->binaryReader->ReadInt()));
			}
			else if(FourCC('SCVQ') == curFourCC)//cl->AddCmd("v_setparticleairresistance_ffffffffi", 'SCVQ', n_p2setparticleairresistance);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				float values[8];
				for(int i = 0; i < 8; i++) values[i] = this->binaryReader->ReadFloat();
				particleSystemNode->SetCurve(ParticleSystem::ParticleEmitter::ParticleAirResistance, ParticleSystem::EnvelopeCurve(
					values[0], values[1], values[2], values[3],
					values[4], values[5], values[6], values[7],
					this->binaryReader->ReadInt()));
			}

			else if(FourCC('SCVR') == curFourCC)//cl->AddCmd("v_setparticlevelocityrandomize_f", 'SCVR', n_p2setparticlevelocityrandomize);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				particleSystemNode->SetParticleVelocityRandomize(this->binaryReader->ReadFloat());
			}
			else if(FourCC('SCVS') == curFourCC)//cl->AddCmd("v_setparticlerotationrandomize_f", 'SCVS', n_p2setparticlerotationrandomize);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				particleSystemNode->SetParticleRotationRandomize(this->binaryReader->ReadFloat());
				
			}
			else if(FourCC('SCVT') == curFourCC)//cl->AddCmd("v_setparticlesizerandomize_f", 'SCVT', n_p2setparticlesizerandomize);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				particleSystemNode->SetParticleSizeRandomize(this->binaryReader->ReadFloat());
			}
			else if(FourCC('SCVU') == curFourCC)//cl->AddCmd("v_setrandomrotdir_b", 'SCVU', n_p2setrandomrotdir);
			{
				n_assert(modelNode->IsA(ParticleSystemNode::RTTI));                
                ParticleSystemNode* particleSystemNode = modelNode.downcast<ParticleSystemNode>().get();
				particleSystemNode->SetRandomRotDir(this->binaryReader->ReadBool());
			}
			else
            {
                // skip data block if unknown attribute
                this->stream->Seek(length, Stream::Current);
            }
        }
        else
        {
            stop = true;
        }
    }
    while (!stop && !this->Eof());
    
    // need to seek back to the beginning of the _new or _sel statement
    this->stream->Seek(-4, Stream::Current);

    Attr::AttributeContainer attr = modelNode->GetAttrs();
    modelNode->LoadFromAttrs(attr);
}

//------------------------------------------------------------------------------
/**
*/
Util::String
N2ModelReader::GetStackPath() const
{
    String path;
    IndexT i;
    for (i = 0; i < this->modelNodeStack.Size(); i++)
    {
        path.Append(this->modelNodeStack[i].Value());
        path.Append("|");
    }
    return path;
}
} // namespace Models
