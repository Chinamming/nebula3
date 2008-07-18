#ifndef PROPERTIES_POINTNCLICKINPUTPROPERTY_H
#define PROPERTIES_POINTNCLICKINPUTPROPERTY_H
//------------------------------------------------------------------------------
/**
    An basic point'n'click third person input property which creates Goto 
    messages when clicking into the environment.

    (C) 2005 Radon Labs GmbH
*/

#include "properties/inputproperty.h"
#include "timing/timer.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class PointNClickInputProperty : public InputProperty
{
	DeclareClass(PointNClickInputProperty);
public:
    /// constructor
    PointNClickInputProperty();
    /// destructor
    virtual ~PointNClickInputProperty();
    /// setup callbacks for this property
    virtual void SetupCallbacks();
    /// called on begin of frame
    virtual void OnBeginFrame();

protected:
    /// handle the default action on left mouse click
    virtual void OnLmbDown();
    /// handle continuous left mouse press
    virtual void OnLmbPressed();
    /// handle a right mouse button click
    virtual void OnRmbDown();
    /// handle camera orbit movement
    virtual void OnMmbPressed();
    /// handle camera zoom in
    virtual void OnCameraZoomIn();
    /// handle camera zoom out
    virtual void OnCameraZoomOut();

private:
    /// send a MoveGoto message to the world intersection point
    void SendMoveGoto();

    Timing::Time moveGotoTime;
};
RegisterClass(PointNClickInputProperty);
};

//------------------------------------------------------------------------------
#endif
