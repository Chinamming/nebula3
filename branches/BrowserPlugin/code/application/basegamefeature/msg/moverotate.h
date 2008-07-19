#ifndef MESSAGIN_MOVEROTATE_H
#define MESSAGIN_MOVEROTATE_H
//------------------------------------------------------------------------------
/**
    @class Messaging::MoveRotate

    Commands an entity rotate around the y-axis for a new heading.
    
    (C) 2006 Radon Labs GmbH
*/
#include "messaging/message.h"

//------------------------------------------------------------------------------
namespace BaseGameFeature
{
class MoveRotate : public Messaging::Message
{
    DeclareClass(MoveRotate);
    DeclareMsgId;

public:
    /// constructor
    MoveRotate();
    /// set the rotation angle (in rad)
    void SetAngle(float angle);
    /// get the rotation angle (in rad)
    float GetAngle() const;

private:
    float angle;
};

//------------------------------------------------------------------------------
/**
*/
inline
void
MoveRotate::SetAngle(float a)
{
    this->angle = a;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
MoveRotate::GetAngle() const
{
    return this->angle;
}

}; // namespace Msg
//------------------------------------------------------------------------------
#endif

