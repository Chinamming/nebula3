#ifndef N_CHARACTER3SET_H
#define N_CHARACTER3SET_H
//------------------------------------------------------------------------------
/**
    @class nCharacter3Set
    @ingroup Character

    A character set is used to query and manipulate the data associated with
    a character3.

    (C) 2005 RadonLabs GmbH
*/
#include "util/dictionary.h"
#include "ncharacter2.h"
#include "core/ptr.h"
#include "coregraphics/shadervariable.h"

namespace Models
{
    class CharacterNode;
    class ModelNodeInstance;
}

namespace Nebula2
{
//------------------------------------------------------------------------------
class nCharacter3Set : public Core::RefCounted
{
__DeclareClass(nCharacter3Set);

public:
    /// skin texture info (returned by GetSkinTextures)
    struct SkinTextureInfo
    {
        CoreGraphics::ShaderVariable shaderVar;
        Ptr<CoreGraphics::Texture> texture;
    };

    /// skin info structure
    struct SkinInfo
    {
        Util::String name;
        Util::String category;
        bool visible;
        bool loaded;
    };

    /// constructor
    nCharacter3Set();
    /// destructor
    virtual ~nCharacter3Set();
    
    /// init characterset
    void Initialize(const Ptr<Models::CharacterNode>& charNode, const Ptr<nCharacter2>& nebCharacter);
    /// is the character3set correctly initialized ?
    bool IsValid() const;
    /// clear character3 set
    void Clear();
    /// configure other character set from self
    void CopyTo(nCharacter3Set* other);

    /// set skin list filename
    void SetSkinListFileName(const Util::String& fileName);
    /// get skin list filename
    const Util::String& GetSkinListFileName() const;
    /// load skin list (makes skins in skin list visible)
    bool LoadSkinList();
    /// get skin list skins
    const Util::Array<IndexT>& GetSkinListSkinIndices() const;

    /// return number of skins
    SizeT GetNumSkins() const;
    /// get info about all character skins
    const Util::Array<SkinInfo>& GetAllSkinInfos() const;
    /// get skin info at index
    const SkinInfo& GetSkinInfoAtIndex(IndexT i) const;
    /// gets all visible skins (slow)
    Util::Array<IndexT> GetVisibleSkinIndices() const;
    /// find skin index by name
    IndexT FindSkinIndexByName(const Util::String& name) const;
    /// activate/deactivate skin by name, load required resources - if necessary
    void SetSkinVisible(const Util::String& name, bool value);
    /// return if skin is visible by name
    bool IsSkinVisible(const Util::String& name) const;
    /// activate/deactivate skin by index, load required resources - if necessary
    void SetSkinVisibleAtIndex(IndexT index, bool value);
    /// return if skin is visible at index
    bool IsSkinVisibleAtIndex(int index) const;
    /// find out if skin exist
    bool HasSkin(const Util::String& name);
    /// make all skins invisible
    void ClearSkinVisibility();

    /// return number of variations
    SizeT GetNumVariations() const;
    /// get variation at index
    const Util::String& GetVariationAtIndex(int index) const;
    /// get names of all variations
    const Util::Array<Util::String>& GetVariationNames() const;
    /// set current variation by name
    void SetCurrentVariation(const Util::String& name);
    /// get current variation name
    const Util::String& GetCurrentVariation() const;
    /// set current variation by index
    void SetCurrentVariationByIndex(int index);
    /// get current variation index
    IndexT GetCurrentVariationIndex() const;
    /// map variation name to index
    IndexT FindVariationIndex(const Util::String& name) const;
    /// set variation dirty flag
    void SetVariationDirty(bool b);
    /// return true if the variation is dirty
    bool IsVariationDirty() const;
    /// apply the currently selected variation (call if variation is dirty)
    void ApplyCurrentVariation();
    /// add a replacement skin texture
    void AddSkinTexture(const Util::String& skinCategory, const CoreGraphics::ShaderVariable& shaderVar, CoreGraphics::Texture* tex);
    /// get skin textures for given skin category (returns NULL ptr if not exists)
    const Util::Array<SkinTextureInfo>* GetSkinTexturesBySkinCategory(const Util::String& skinCategory) const;
    /// return true if skin textures have been added
    bool HasSkinTextures() const;

    /// apply skin visibility to our node instances
    void ApplySkinVisibility(const Ptr<Models::ModelNodeInstance>& rootNode);

protected:
    bool isValid;
    bool isVariationDirty;
    bool areSkinsDirty;
    Ptr<nCharacter2> character;
    Util::Array<SkinInfo> skins;
    Util::Array<Util::String> variations;
    Util::Array<IndexT> skinListIndices;
    Util::Dictionary<Util::String,IndexT> skinIndexMap;
    Util::Dictionary<Util::String,IndexT> variationIndexMap;
    Util::Dictionary<Util::String,Util::Array<SkinTextureInfo> > skinTextures;
    IndexT currentVariationIndex;
    Util::String skinListFileName;
};

//------------------------------------------------------------------------------
/**
*/
inline void
nCharacter3Set::SetSkinListFileName(const Util::String& fileName)
{
    this->skinListFileName = fileName;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String&
nCharacter3Set::GetSkinListFileName() const
{
    return this->skinListFileName;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
nCharacter3Set::IsValid() const
{
    return this->isValid;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
nCharacter3Set::GetNumSkins() const
{
    return this->skins.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline const nCharacter3Set::SkinInfo&
nCharacter3Set::GetSkinInfoAtIndex(IndexT index) const
{
    return this->skins[index];
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<nCharacter3Set::SkinInfo>&
nCharacter3Set::GetAllSkinInfos() const
{
    return this->skins;
}

//------------------------------------------------------------------------------
/** 
    Check if a skin exist, in skinIndexMap
*/
inline bool
nCharacter3Set::HasSkin(const Util::String& name)
{
    return this->skinIndexMap.Contains(name);
}

//------------------------------------------------------------------------------
/**
*/
inline bool
nCharacter3Set::IsSkinVisible(const Util::String& name) const
{
    IndexT skinIndex = this->skinIndexMap[name];
    n_assert(InvalidIndex != skinIndex);
    return this->skins[skinIndex].visible;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
nCharacter3Set::IsSkinVisibleAtIndex(int index) const
{
    return this->skins[index].visible;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
nCharacter3Set::FindSkinIndexByName(const Util::String& name) const
{
    IndexT skinIndexMapIndex = this->skinIndexMap.FindIndex(name);
    if (InvalidIndex == skinIndexMapIndex)
    {
        return InvalidIndex;
    }
    else
    {
        return this->skinIndexMap.ValueAtIndex(skinIndexMapIndex);
    }
}

//------------------------------------------------------------------------------
/**
*/
inline void
nCharacter3Set::ClearSkinVisibility()
{
    IndexT i;
    for (i = 0; i < this->skins.Size(); i++)
    {
        this->skins[i].visible = false;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
nCharacter3Set::GetNumVariations() const
{
    return this->variations.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::String&
nCharacter3Set::GetVariationAtIndex(int index) const
{
    return this->variations[index];
}

//------------------------------------------------------------------------------
/**
    Set current variation by index, set -1 to clear any variation.
*/
inline void
nCharacter3Set::SetCurrentVariationByIndex(int index)
{
    this->currentVariationIndex = index;
    this->isVariationDirty = true;
}

//------------------------------------------------------------------------------
/**
    Returns the current variation index. Returns InvalidIndex if no variation is set.
*/
inline IndexT
nCharacter3Set::GetCurrentVariationIndex() const
{
    return this->currentVariationIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
nCharacter3Set::FindVariationIndex(const Util::String& name) const
{
    IndexT varIndexMapIndex = this->variationIndexMap.FindIndex(name);
    if (InvalidIndex == varIndexMapIndex)
    {
        return InvalidIndex;
    }
    else
    {
        return this->variationIndexMap.ValueAtIndex(varIndexMapIndex);
    }
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Util::String>&
nCharacter3Set::GetVariationNames() const
{
    return this->variations;
}

//------------------------------------------------------------------------------
/**
*/
inline void
nCharacter3Set::SetVariationDirty(bool b)
{
    this->isVariationDirty = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
nCharacter3Set::IsVariationDirty() const
{
    return this->isVariationDirty;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
nCharacter3Set::HasSkinTextures() const
{
    return this->skinTextures.Size() > 0;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<IndexT>&
nCharacter3Set::GetSkinListSkinIndices() const
{
    return this->skinListIndices;
}
}; // namespace Nebula2
//------------------------------------------------------------------------------
#endif
