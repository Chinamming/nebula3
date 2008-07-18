#ifndef MSG_INPUTFOCUS_H
#define MSG_INPUTFOCUS_H
//------------------------------------------------------------------------------
/**
    @class GraphicsFeature::InputFocus

    Commands an entity to turn into the specified direction defined
    by a 3d vector. The direction vector can be absolute or camera relative.
    
    (C) 2007 Radon Labs GmbH
*/
#include "messaging/message.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class InputFocus : public Messaging::Message
{
	DeclareClass(InputFocus);
    DeclareMsgId;

public:
    /// constructor
    InputFocus();
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
InputFocus::SetObtainFocus(bool b)
{
    this->obtainFocus = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
InputFocus::GetObtainFocus() const
{
    return this->obtainFocus;
}

}; // namespace GraphicsFeature
//------------------------------------------------------------------------------
#endif

