//------------------------------------------------------------------------------
//  CharacterServer.cc
//  (C) 2003 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "characterserver.h"
#include "io/ioserver.h"
#include "io/xmlreader.h"

namespace Nebula2
{
ImplementClass(CharacterServer, 'CHSR', Core::RefCounted);
ImplementSingleton(CharacterServer);
//------------------------------------------------------------------------------
/**
*/
CharacterServer::CharacterServer()
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
CharacterServer::~CharacterServer()
{
    DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
bool 
CharacterServer::LoadSkinListFromXml(const Util::String& fileName, Util::Array<Util::String>& outSkinList, Util::String& outVariation)
{
    outVariation.Clear();
    IO::IoServer* ioServer = IO::IoServer::Instance();

    if (!ioServer->FileExists(fileName))
    {
        n_printf("*** RESOURCE ERROR: failed to load skin list '%s'!\n", fileName.AsCharPtr());
        return false;
    }
    Util::String skins;
    
    Ptr<IO::XmlReader> xmlReader = IO::XmlReader::Create();
    xmlReader->SetStream(ioServer->CreateStream(fileName));
    if (!xmlReader->Open()|| !xmlReader->HasNode("/skins"))
    {
        // something went wrong, return
        return false;
    }

    xmlReader->SetToNode("/skins");
    if (xmlReader->SetToFirstChild())
    {
        do
        {
            if (!xmlReader->SetToFirstChild())
            {
                // something went wrong, return
                return false;
            }
            if (!xmlReader->SetToFirstChild())
            {
                // something went wrong, return
                return false;
            }    
            outSkinList.Append(xmlReader->GetCurrentNodeName());
            xmlReader->SetToParent();
            xmlReader->SetToParent();
        }
        while (xmlReader->SetToNextChild());
    }

    if (xmlReader->HasNode("/variation"))
    {
        xmlReader->SetToNode("/variation");
        if (xmlReader->SetToFirstChild())
        {
            outVariation = xmlReader->GetCurrentNodeName();
        }
    }
    xmlReader->Close();
    return true;
}

}; // namespace Nebula2