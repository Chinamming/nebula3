#ifndef UI_LABEL_H
#define UI_LABEL_H
//------------------------------------------------------------------------------
/**
    A label represents a simple backgroundplane that contains a graphic and
    can have a tooltip set.
    
    (C) 2005 Radon Labs GmbH
*/
#include "uifeature/elements/control.h"
#include "timing/time.h"

//------------------------------------------------------------------------------
namespace UI
{
class Label : public Element
{
	__DeclareClass(Label);
public:
    /// constructor
    Label();
    /// destructor
    virtual ~Label();
    /// called when gui hierarchy is created
    virtual void OnCreate();
    /// called on frame
    virtual void OnFrame();
    /// set a new texture resource on the label (all states)
    void SetTexture(const Util::String& texName);
    /// get the texture of the label
    const Util::String& GetTexture();
    /// set the color
	void SetColor(const Math::float4& c);   
    /// get the color
    const Math::float4& GetColor();
    /// set the emissive color
    void SetEmissiveColor(const Math::float4& e);
    /// get the emissive color
    const Math::float4& GetEmissiveColor();

protected:
    Math::float4 color;
    Math::float4 emissiveColor;
    Util::String textureName;    
};


//------------------------------------------------------------------------------
/**
    This gets the texture of the gfxNode (child textures are ignored)
*/
inline
const
Util::String&
Label::GetTexture()
{
    return this->textureName;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::float4&
Label::GetColor()
{
    return this->color;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::float4&
Label::GetEmissiveColor()
{
    return this->emissiveColor;
}
}; // namespace UI
//------------------------------------------------------------------------------
#endif