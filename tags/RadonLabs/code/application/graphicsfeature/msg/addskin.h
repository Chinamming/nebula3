#ifndef MSG_ADDSKIN_H
#define MSG_ADDSKIN_H
//------------------------------------------------------------------------------
/**
    @class Messaging::AddSkin
    
    Makes the given skin visible on a Character3.
    
    (C) 2006 Radon Labs GmbH
*/
#include "messaging/message.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class AddSkin : public Messaging::Message
{
    DeclareClass(AddSkin);
    DeclareMsgId;
public:
    /// set skin name
    void SetSkinName(const Util::String& s);
    /// get skin name
    const Util::String& GetSkinName() const;

private:
    Util::String skinName;
};

//------------------------------------------------------------------------------
/**
*/
inline
void
AddSkin::SetSkinName(const Util::String& s)
{
    this->skinName = s;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String&
AddSkin::GetSkinName() const
{
    return this->skinName;
}

}; // namespace GraphicsFeature
//------------------------------------------------------------------------------
#endif