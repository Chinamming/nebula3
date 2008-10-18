#ifndef UI_BUTTON_H
#define UI_BUTTON_H
//------------------------------------------------------------------------------
/**
    @class UI::Button

    A simple graphical button UI element.

    (C) 2005 Radon Labs GmbH
*/
#include "uifeature/elements/label.h"
#include "util/fixedarray.h"

//------------------------------------------------------------------------------
namespace UI
{
class Button : public Control
{
	__DeclareClass(Button);
public:

    /// control states
    enum VisualState
    {
        Normal,
        Pressed,
        MouseOver,
        Disabled,

        NumVisualStates,
        InvalidVisualState,
    };

    /// constructor
    Button();
    /// destructor
    virtual ~Button();

    /// return true if mouse currently over button
    bool IsMouseOver() const;

    /// called when gui hierarchy is created
    virtual void OnCreate();
    /// called on frame
    virtual void OnFrame();
    /// called when gui hierarchy is destroyed
    virtual void OnDestroy();
    /// called when mouse is moved
	virtual bool OnMouseMove(const Math::float2& mousePos);
    /// called when left mouse button is pressed over element
    virtual bool OnLeftButtonDown(const Math::float2& mousePos);
    /// called when left mouse button is held pressed
    virtual bool OnLeftButtonPressed(const Math::float2& mousePos);
    /// called when left mouse button is released over element
    virtual bool OnLeftButtonUp(const Math::float2& mousePos);    
    
	/// change texture of a visual state
    void SetStateTexture(VisualState visualState, const Util::String& texName);    
    /// get the texture aof a visual state
    Util::String GetStateTexture(VisualState visualState);

    /// convert visualstate to a string
    static Util::String VisualStateToString(VisualState visualState);

    /// return time since button is pressed
    Timing::Time GetPressedTime() const;
    /// return true if currently pressed
    bool IsPressed() const;
    /// return true if button is pressed since given time
	bool IsPressedSince(Timing::Time since = 0.0) const;
    /// set pressed state
    void SetPressed(bool b);

protected:
    /// set the active visual state
    virtual void SetActiveVisualState(VisualState state);

	Timing::Time pressedTime;
    bool isMouseOver;
    bool isPressed;
    VisualState activeState;
};

//------------------------------------------------------------------------------
/**
*/
inline
void
Button::SetPressed(bool b)
{
    this->isPressed = b;

    if (!b)
    {
        this->pressedTime = 0.0;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
Button::IsPressed() const
{
    return this->isPressed;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
Button::IsPressedSince(Timing::Time since) const
{
    if (this->isPressed)
    {
        if (since <= this->pressedTime + N_TINY)
        {
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
inline
Timing::Time
Button::GetPressedTime() const
{
    return this->pressedTime;
}


//------------------------------------------------------------------------------
/**
*/
inline
bool
Button::IsMouseOver() const
{
    return this->isMouseOver;
}

//------------------------------------------------------------------------------
/**
*/
//inline
//nTransformNode*
//Button::GetGfxNodeOfVisualState(VisualState visualState) const
//{
//    return this->visuals[visualState];
//}

}; // namespace UI
//------------------------------------------------------------------------------
#endif