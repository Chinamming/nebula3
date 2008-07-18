#ifndef MESSAGING_CAMERADISTANCE_H
#define MESSAGING_CAMERADISTANCE_H
//------------------------------------------------------------------------------
/**
    @class GraphicsFeature::CameraDistance
    
    Change the distance of a 3rd camera to its lookat point.
    
    (C) 2007 Radon Labs GmbH
*/
#include "messaging/message.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class CameraDistance : public Messaging::Message
{
    DeclareClass(CameraDistance);
    DeclareMsgId;

public:
    /// constructor
    CameraDistance();
    /// set relative camera distance change (-1.0f..+1.0f)
    void SetRelativeDistanceChange(float d);
    /// get relative camera distance change
    float GetRelativeDistanceChange() const;

private:
    float relDistChange;
};

//------------------------------------------------------------------------------
/**
*/
inline CameraDistance::CameraDistance() :
    relDistChange(0.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline void
CameraDistance::SetRelativeDistanceChange(float d)
{
    this->relDistChange = d;
}

//------------------------------------------------------------------------------
/**
*/
inline float
CameraDistance::GetRelativeDistanceChange() const
{
    return this->relDistChange;
}

};
//------------------------------------------------------------------------------
#endif

