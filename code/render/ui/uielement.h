#pragma once
#ifndef UI_UIELEMENT_H
#define UI_UIELEMENT_H
//------------------------------------------------------------------------------
/**
    @class UI::UIElement
    
    This is the container for the message wich is sent from the client side
	UI::UIRenderer to the CoreUI::UIGraphicsHandler
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "math/rectangle.h"
#include "util/string.h"

//------------------------------------------------------------------------------
namespace UI
{
class UIElement : public Core::RefCounted
{
    __DeclareClass(UIElement);

public:
	/// constructor
	UIElement();
	/// deconstructor
	virtual ~UIElement();

	/// set the color
	void SetColor(const Math::float4& c);
	/// get the color
	const Math::float4& GetColor() const;
	
	/// set the dimension
	void SetDimension(const Math::rectangle<float>& r);
	/// get the dimension
	const Math::rectangle<float>& GetDimension() const;
	
	/// set the texture
	void SetTexture(const Util::String& t);
	/// get the texture
	const Util::String& GetTexture() const;

private:
	Math::float4 color;
	Util::String texture;
	Math::rectangle<float> dimension;
};

//------------------------------------------------------------------------------
/**
*/
inline
void
UIElement::SetColor(const Math::float4& c)
{
	this->color = c;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::float4&
UIElement::GetColor() const
{
	return this->color;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
UIElement::SetDimension(const Math::rectangle<float>& r)
{
	this->dimension = r;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Math::rectangle<float>&
UIElement::GetDimension() const
{
	return this->dimension;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
UIElement::SetTexture(const Util::String& t)
{
	this->texture = t;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String&
UIElement::GetTexture() const
{
	return this->texture;
}

} // namespace Debug
//------------------------------------------------------------------------------
#endif

    
    