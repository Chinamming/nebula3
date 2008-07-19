#ifndef MSG_CAMERAFOCUS_H
#define MSG_CAMERAFOCUS_H
//------------------------------------------------------------------------------
/**
    @class GraphicsFeature::CameraFocus

    
    
    (C) 2007 Radon Labs GmbH
*/
#include "messaging/message.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class CameraFocus : public Messaging::Message
{
	DeclareClass(CameraFocus);
    DeclareMsgId;

public:
    /// constructor
    CameraFocus();
    /// set the direction vector
    void SetObtainFocus(bool b);
    /// get the direction vector
    bool GetObtainFocus() const;

private:
    bool obtainFocus;
};

//------------------------------------------------------------------------------
/**
*/
inline void
CameraFocus::SetObtainFocus(bool b)
{
    this->obtainFocus = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
CameraFocus::GetObtainFocus() const
{
    return this->obtainFocus;
}

}; // namespace GraphicsFeature
//------------------------------------------------------------------------------
#endif

