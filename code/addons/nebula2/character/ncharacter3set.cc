//------------------------------------------------------------------------------
//  ncharacter3set_main.cc
//  (C) 2005 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "ncharacter3set.h"
#include "coregraphics/texture.h"
#include "characterserver.h"
#include "models/nodes/characternode.h"
#include "models/modelnodeinstance.h"

namespace Nebula2
{
using namespace Math;
using namespace Util;

ImplementClass(nCharacter3Set, 'CHA3', Core::RefCounted);
//------------------------------------------------------------------------------
/**
*/
nCharacter3Set::nCharacter3Set() :
    isValid(false),
    isVariationDirty(false),
    areSkinsDirty(false),
    currentVariationIndex(InvalidIndex)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
nCharacter3Set::~nCharacter3Set()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
nCharacter3Set::Initialize(const Ptr<Models::CharacterNode>& charNode, const Ptr<nCharacter2>& nebCharacter)
{
    n_assert(charNode.isvalid());    
    n_assert(nebCharacter);
    
    this->character = nebCharacter;
    this->skins.Clear();
    this->variations.Clear();
    this->skinIndexMap.Clear();
    this->variationIndexMap.Clear();    
    this->currentVariationIndex = InvalidIndex;
    this->isValid = true;

    // setup skins
    SizeT numSkinInfos = charNode->GetNumSkins();
    IndexT skinIndex;
    for (skinIndex = 0; skinIndex < numSkinInfos; skinIndex++)
    {
        const Nebula2::SkinInfo& charSkinInfo = charNode->GetSkinInfoAt(skinIndex);
        SkinInfo skinInfo;
        skinInfo.name = charSkinInfo.name;
        skinInfo.visible = false;
        skinInfo.loaded = false;
        skinInfo.category = charSkinInfo.category;
        this->skins.Append(skinInfo);
        this->skinIndexMap.Add(skinInfo.name, skinIndex);
    }

    // setup variations
    SizeT numVars = charNode->GetNumVariations();
    IndexT varIndex;
    for (varIndex = 0; varIndex < numVars; varIndex++)
    {
        this->variations.Append(charNode->GetVariationNameAt(varIndex));
        this->variationIndexMap.Add(this->variations.Back(), varIndex);
    }
}

//------------------------------------------------------------------------------
/**
*/
bool 
nCharacter3Set::LoadSkinList()
{
    n_assert(this->IsValid());
    n_assert(this->skinListFileName.IsValid());
    
    this->ClearSkinVisibility();
    this->skinListIndices.Clear();

    Util::Array<Util::String> skinList;
    Util::String variation;
    if (CharacterServer::Instance()->LoadSkinListFromXml(this->skinListFileName, skinList, variation))
    {
        if (variation.IsValid())
        {
            this->SetCurrentVariation(variation);
        }
        IndexT i;
        for (i = 0; i < skinList.Size(); i++)
        {
            IndexT ii = this->skinIndexMap.FindIndex(skinList[i]);
            if (InvalidIndex != ii)
            {
                IndexT skinIndex = this->skinIndexMap.ValueAtIndex(ii);
                this->SetSkinVisibleAtIndex(skinIndex, true);
                this->skinListIndices.Append(skinIndex);
            }
        }
        this->areSkinsDirty = true;
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
    Clears the character3 set
*/
void
nCharacter3Set::Clear()
{
    this->character = 0;
    this->skins.Clear();
    this->variations.Clear();
    this->skinIndexMap.Clear();
    this->variationIndexMap.Clear();
    this->currentVariationIndex = -1;    
    IndexT catIndex;
    for (catIndex = 0; catIndex < this->skinTextures.Size(); catIndex++)
    {
        const Util::Array<SkinTextureInfo>& skinTextureInfos = this->skinTextures.ValueAtIndex(catIndex);
        IndexT texIndex;
        for (texIndex = 0; texIndex < skinTextureInfos.Size(); texIndex++)
        {
            skinTextureInfos[texIndex].texture = 0;
        }
    }
    this->skinTextures.Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
nCharacter3Set::AddSkinTexture(const Util::String& skinCategory, const CoreGraphics::ShaderVariable& shaderVar, CoreGraphics::Texture* tex)
{
    n_assert(skinCategory.IsValid());
    n_assert(0 != tex);
    if (!this->skinTextures.Contains(skinCategory))
    {
        Util::Array<SkinTextureInfo> skinTextureInfoArray;
        this->skinTextures.Add(skinCategory, skinTextureInfoArray);
    }
    SkinTextureInfo skinTextureInfo;
    skinTextureInfo.shaderVar = shaderVar;
    skinTextureInfo.texture = tex;
    this->skinTextures[skinCategory].Append(skinTextureInfo);
}

//------------------------------------------------------------------------------
/**
*/
const Util::Array<nCharacter3Set::SkinTextureInfo>*
nCharacter3Set::GetSkinTexturesBySkinCategory(const Util::String& skinCategory) const
{
    n_assert(skinCategory.IsValid());
    IndexT index = this->skinTextures.FindIndex(skinCategory);
    if (InvalidIndex != index)
    {
        return &(this->skinTextures.ValueAtIndex(index));
    }
    else
    {
        return 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
nCharacter3Set::SetSkinVisible(const Util::String& name, bool visible)
{
    IndexT ii = this->skinIndexMap.FindIndex(name);
    if (InvalidIndex == ii)
    {
        n_error("nCharacter3Set::SetSkinVisible(): skin '%s' not found!", name.AsCharPtr());
    }
    IndexT skinIndex = this->skinIndexMap.ValueAtIndex(ii);
    this->SetSkinVisibleAtIndex(skinIndex, visible);
}

//------------------------------------------------------------------------------
/**
*/
void
nCharacter3Set::SetSkinVisibleAtIndex(IndexT index, bool visible)
{
    // load skin resources - if necessary
    if (!this->skins[index].loaded && visible)
    {
        this->skins[index].loaded = true;
        //this->char3Node->LoadSkinResources(this->skins[index].name);
    }
    this->skins[index].visible = visible;
    this->areSkinsDirty = true;
}

//------------------------------------------------------------------------------
/**
*/
Util::Array<IndexT>
nCharacter3Set::GetVisibleSkinIndices() const
{
    Util::Array<IndexT> result;
    IndexT i;
    for (i = 0; i < this->skins.Size(); i++)
    {
        if (this->skins[i].visible)
        {
            result.Append(i);
        }
    }
    return result;
}

//------------------------------------------------------------------------------
/**
    Set the current variation by name. Use an empty string to clear the
    current variation.
*/
void
nCharacter3Set::SetCurrentVariation(const Util::String& name)
{
    this->currentVariationIndex = InvalidIndex;
    if (name.IsValid())
    {
        IndexT i = this->variationIndexMap.FindIndex(name);
        if (InvalidIndex != i)
        {
            this->currentVariationIndex = this->variationIndexMap.ValueAtIndex(i);
        }
        else
        {
            n_printf("*** RESOURCE ERROR: missing variation '%s' on character!\n",
                name.AsCharPtr());
        }
    }
    this->isVariationDirty = true;
}

//------------------------------------------------------------------------------
/**
    Returns the name of the current variation. If no variation is set, an
    empty string will be returned.
*/
const Util::String&
nCharacter3Set::GetCurrentVariation() const
{
    if (InvalidIndex != this->currentVariationIndex)
    {
        return this->variations[this->currentVariationIndex];
    }
    else
    {
        static const Util::String emptyString;   
        return emptyString;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
nCharacter3Set::CopyTo(nCharacter3Set* other)
{   
    other->isValid = this->isValid;                                            
    other->isVariationDirty = this->isVariationDirty;                          
    other->skins = this->skins;
    other->skinListIndices = this->skinListIndices;                            
    other->skinIndexMap = this->skinIndexMap;                                  
    other->skinTextures = this->skinTextures;  
    other->skinListFileName = this->skinListFileName;
    other->variations = this->variations;      
    other->variationIndexMap = this->variationIndexMap;                                                            
    other->currentVariationIndex = this->currentVariationIndex;
    other->character = this->character;
    other->areSkinsDirty = this->areSkinsDirty;
}

//------------------------------------------------------------------------------
/**
    Apply the currently selected variation to the character skeleton.
*/
void
nCharacter3Set::ApplyCurrentVariation()
{
    n_assert(this->IsVariationDirty());
        
    // apply the variation on the skeleton
    nCharSkeleton& skeleton = this->character->GetSkeleton();
    if (InvalidIndex != this->currentVariationIndex)
    {
        //const Util::Array<nCharJoint>& varJoints = this->char3Node->GetSkinAnimator()->GetVariationJointsAt(this->currentVariationIndex);
        /*n_assert(varJoints.Size() == skeleton.GetNumJoints());
        uint jointIndex;
        for (jointIndex = 0; jointIndex < varJoints.Size(); jointIndex++)
        {
            const vector& varScale = varJoints[jointIndex].GetScale();
            skeleton.GetJointAt(jointIndex).SetVariationScale(varScale);
        }*/
        skeleton.Evaluate();
    }
    else
    {
        // no variation applied, reset variation scale to one
        int jointIndex;
        static const vector noScale(1.0f, 1.0f, 1.0f);
        for (jointIndex = 0; jointIndex < skeleton.GetNumJoints(); jointIndex++)
        {
            skeleton.GetJointAt(jointIndex).SetVariationScale(noScale);
        }
        skeleton.Evaluate();
    }
    this->SetVariationDirty(false);
}

//------------------------------------------------------------------------------
/**    
*/
void
nCharacter3Set::ApplySkinVisibility(const Ptr<Models::ModelNodeInstance>& rootNode)
{
    if (this->areSkinsDirty)
    {
        n_assert(rootNode.isvalid());
        // go thru nodes and update visibility
        Array<Ptr<Models::ModelNodeInstance> > children = rootNode->GetChildren();
        IndexT index;
        for (index = 0; index < children.Size(); index++)
        {
            String name = children[index]->GetModelNode()->GetName().Value();
            name.Strip("|");
            if (this->HasSkin(name))
            {
                const SkinInfo& info = this->GetSkinInfoAtIndex(this->FindSkinIndexByName(name));
                children[index]->SetVisible(info.visible);
            }
        }

        this->areSkinsDirty = false;
    }
}

}; // namespace Nebula2