#ifndef MSG_REMSKIN_H
#define MSG_REMSKIN_H
//------------------------------------------------------------------------------
/**
    @class Messaging::RemSkin
    
    Makes the given skin invisibe on a Character3.

    (C) 2006 Radon Labs GmbH
*/
#include "messaging/message.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class RemSkin : public Messaging::Message
{
    DeclareClass(RemSkin);
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
RemSkin::SetSkinName(const Util::String& s)
{
    this->skinName = s;
}

//------------------------------------------------------------------------------
/**
*/
inline
const Util::String&
RemSkin::GetSkinName() const
{
    return this->skinName;
}

}; // namespace GraphicsFeature
//------------------------------------------------------------------------------
#endif