#pragma once
#ifndef UI_UIRENDERER_H
#define UI_UIRENDERER_H
//------------------------------------------------------------------------------
/**
    @class UI::UIRenderer
    
    Client-side proxy for rendering the user interface. Packs ui render requests
    into a message which is sent to the render thread once per frame.
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "math/rectangle.h"
#include "ui/uielement.h"

//------------------------------------------------------------------------------
namespace UI
{
class UIRenderer : public Core::RefCounted
{
    __DeclareClass(UIRenderer);
    __DeclareSingleton(UIRenderer);

public:
	/// constructor
    UIRenderer();
    /// destructor
    virtual ~UIRenderer();
    /// draw a simple element
	void DrawElement(const Math::rectangle<float>& rect, const Math::float4& color);

    /// call once per frame to send of accumulated draw commands
    void OnFrame();

private:
	Util::Array<Ptr<UIElement>> elements;
}; 

} // namespace Debug
//------------------------------------------------------------------------------
#endif

    
    