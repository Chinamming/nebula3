#ifndef GRAPHICSFEATURE_MAYACAMERAPROPERTY_H
#define GRAPHICSFEATURE_MAYACAMERAPROPERTY_H
//------------------------------------------------------------------------------
/**
	@class GraphicsFeature::MayaCameraProperty

    A manually controlled camera property which implements different
    control models.
    
    (C) 2008 Radon Labs GmbH
*/
#include "properties/cameraproperty.h"
#include "renderutil/mayacamerautil.h"
#include "graphicsattr/graphicsattributes.h"

//------------------------------------------------------------------------------
namespace Attr
{
    // maya camera specific attributes
    DeclareFloat4(MayaCameraCenterOfInterest, 'MCCI', ReadWrite);
    DeclareFloat4(MayaCameraDefaultUpVec, 'MCDU', ReadWrite);
};

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class MayaCameraProperty : public CameraProperty
{
	__DeclareClass(MayaCameraProperty);
public:
    /// constructor
    MayaCameraProperty();
    /// destructor
    virtual ~MayaCameraProperty();
    /// called from Entity::ActivateProperties()
    virtual void OnActivate();
	/// setup callbacks for this property
    virtual void SetupCallbacks();
    /// called when camera focus is obtained
    virtual void OnObtainFocus();
    /// called before rendering happens
    virtual void OnRender();
    /// setup default entity attributes
    virtual void SetupDefaultAttributes();
    /// override to register accepted messages
    virtual void SetupAcceptedMessages();
    /// handle a single message
    virtual void HandleMessage(const Ptr<Messaging::Message>& msg);

private:
	RenderUtil::MayaCameraUtil mayaCameraUtil;

};
__RegisterClass(MayaCameraProperty);
}; // namespace GraphicsFeature
//------------------------------------------------------------------------------
#endif    