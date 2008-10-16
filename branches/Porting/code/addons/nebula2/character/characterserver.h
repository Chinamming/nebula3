#ifndef N_CHARACTERSERVER_H
#define N_CHARACTERSERVER_H
//------------------------------------------------------------------------------
/**
    @class CharacterServer
    @ingroup Character

    @brief Holds all the data necessary to animate an character in one place.

    (C) 2003 RadonLabs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"

namespace Nebula2
{
class CharacterServer : public Core::RefCounted
{
    __DeclareClass(CharacterServer);
    __DeclareSingleton(CharacterServer);
public:
    /// constructor
    CharacterServer();
    /// destructor
    virtual ~CharacterServer();

    /// read skinlist from a XML file
    bool LoadSkinListFromXml(const Util::String& fileName, Util::Array<Util::String>& outSkinList, Util::String& outVariation);    

private:
      
};
}; // namespace Nebula2
//------------------------------------------------------------------------------
#endif
