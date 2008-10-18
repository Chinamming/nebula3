//------------------------------------------------------------------------------
//  projectinfo.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "projectinfo.h"
#include "io/ioserver.h"
#include "io/stream.h"
#include "io/xmlreader.h"
#include "system/win32/win32registry.h"

namespace ToolkitUtil
{
using namespace Util;
using namespace IO;
using namespace System;

//------------------------------------------------------------------------------
/**
*/
ProjectInfo::ProjectInfo() :
    isNebula2Project(false),
    defPlatform(Platform::InvalidPlatform),
    curPlatform(Platform::InvalidPlatform)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ProjectInfo::~ProjectInfo()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
ProjectInfo::HasAttr(const Util::String& attrName) const
{
    n_assert(attrName.IsValid());
    n_assert(Platform::InvalidPlatform != this->curPlatform);
    return this->platformAttrs[this->curPlatform].Contains(attrName);
}

//------------------------------------------------------------------------------
/**
*/
const Util::String&
ProjectInfo::GetAttr(const Util::String& attrName) const
{
    n_assert(attrName.IsValid());
    n_assert(Platform::InvalidPlatform != this->curPlatform);
    return this->platformAttrs[this->curPlatform][attrName];
}

//------------------------------------------------------------------------------
/**
*/
bool
ProjectInfo::IsValid() const
{
    return (this->platformAttrs.Size() > 0);
}

//------------------------------------------------------------------------------
/**
*/
ProjectInfo::Result
ProjectInfo::Setup()
{
    n_assert(!this->IsValid());

    // setup project and toolkit path assign
    String projPath = this->QueryProjectPathFromRegistry();
    if (projPath.IsEmpty())
    {
        return NoProjPathInRegistry;
    }
    String toolkitPath = this->QueryToolkitPathFromRegistry();
    n_assert(toolkitPath.IsValid());
    IoServer::Instance()->SetAssign(Assign("proj", projPath));
    IoServer::Instance()->SetAssign(Assign("toolkit", toolkitPath));

    // parse project info XML file
    Result res = ProjectInfo::Success;
    if (IoServer::Instance()->FileExists("proj:projectinfo.xml"))
    {
        res = this->ParseProjectInfoFile();
        this->isNebula2Project = false;
    }
    else
    {
        // no project info file, assume this is a Nebula2 project
        this->SetupNebula2DefaultAttrs();
        this->isNebula2Project = true;
    }
    return res;
}

//------------------------------------------------------------------------------
/**
    Parse the projectinfo.xml file which is expected in the project root 
    directory.
*/
ProjectInfo::Result
ProjectInfo::ParseProjectInfoFile()
{
    n_assert(!this->IsValid());

    Ptr<Stream> stream = IoServer::Instance()->CreateStream(URI("proj:projectinfo.xml"));
    Ptr<XmlReader> xmlReader = XmlReader::Create();
    xmlReader->SetStream(stream);
    if (xmlReader->Open())
    {
        // check if it's a valid project info file
        if (!xmlReader->HasNode("/Nebula3/Project"))
        {
            return ProjectFileContentInvalid;
        }
        xmlReader->SetToNode("/Nebula3/Project");
        n_assert(xmlReader->HasAttr("defaultPlatform"));
        this->defPlatform = Platform::FromString(xmlReader->GetString("defaultPlatform"));
        
        // for each platform...
        if (xmlReader->SetToFirstChild("Platform")) do
        {
            // setup a new set of platform attributes
            n_assert(xmlReader->HasAttr("name"));
            Platform::Code platform = Platform::FromString(xmlReader->GetString("name"));
            n_assert(!this->platformAttrs.Contains(platform));
            Dictionary<String,String> emptyDict;
            this->platformAttrs.Add(platform, emptyDict);

            // load attributes
            if (xmlReader->SetToFirstChild("Attr")) do
            {
                n_assert(xmlReader->HasAttr("name"));
                n_assert(xmlReader->HasAttr("value"));
                String attrName = xmlReader->GetString("name");
                String attrValue = xmlReader->GetString("value");
                n_assert(!this->platformAttrs[platform].Contains(attrName));
                this->platformAttrs[platform].Add(attrName, attrValue);
            }
            while (xmlReader->SetToNextChild("Attr"));
        }
        while (xmlReader->SetToNextChild("Platform"));
        return Success;
    }
    else
    {
        return CouldNotOpenProjectInfoFile;
    }
}

//------------------------------------------------------------------------------
/**
    Query the project path from the registry, the registry key is set
    by the "Nebula2 Toolkit For Maya". If no key is found, the method
    will return "home:".
*/
String
ProjectInfo::QueryProjectPathFromRegistry()
{
    String projDirectory;
    if (Win32Registry::Exists(Win32Registry::LocalMachine, "SOFTWARE\\RadonLabs\\Toolkit", "project"))
    {
        projDirectory = Win32Registry::Read(Win32Registry::LocalMachine, "SOFTWARE\\RadonLabs\\Toolkit", "project");
    }
    else
    {
        projDirectory = "home:";
    }
    return projDirectory;
}

//------------------------------------------------------------------------------
/**
    Query the toolkit path from the registry, the registry key is set
    by the "Nebula2 Toolkit For Maya". If no key is found, the method
    will return "home:".
*/
String
ProjectInfo::QueryToolkitPathFromRegistry()
{
    String toolkitDirectory;
    if (Win32Registry::Exists(Win32Registry::LocalMachine, "SOFTWARE\\RadonLabs\\Toolkit", "path"))
    {
        toolkitDirectory = Win32Registry::Read(Win32Registry::LocalMachine, "SOFTWARE\\RadonLabs\\Toolkit", "path");
    }
    else
    {
        toolkitDirectory = "home:";
    }
    return toolkitDirectory;
}

//------------------------------------------------------------------------------
/**
*/
String
ProjectInfo::GetErrorString(Result res) const
{
    switch (res)
    {
        case ProjectInfo::NoProjPathInRegistry:
            return String("ERROR: no Nebula toolkit entries found in registry!");
        case ProjectInfo::CouldNotOpenProjectInfoFile:
            return String("ERROR: could not open projectinfo.xml file in project directory!");
        case ProjectInfo::ProjectFileContentInvalid:
            return String("ERROR: content of projectinfo.xml is invalid!");
        default:
            return String("ERROR: unknown error from ProjectInfo object!\n");
    }
}

//------------------------------------------------------------------------------
/**
    This method is used to setup the object if no "proj:projectsettings.xml"
    setting is found. We assume that this is a Nebula2 project then and
    initialize the appropriate variables.
*/
void
ProjectInfo::SetupNebula2DefaultAttrs()
{
    n_assert(this->platformAttrs.IsEmpty());

    this->defPlatform = Platform::Win32;
    this->curPlatform = Platform::Win32;

    Dictionary<String,String> attrs;
    attrs.Add("DstDir", "proj:export");
    attrs.Add("SrcDir", "proj:work");
    attrs.Add("ShaderTool", "toolkit:bin/win32/fxc.exe");
    attrs.Add("ShaderToolParams", "");
    attrs.Add("ShaderSrcDir", "src:shaders/2.0");
    attrs.Add("ShaderDstDir", "dst:shaders/2.0");
    attrs.Add("FrameShaderSrcDir", "src:shaders");
    attrs.Add("FrameShaderDstDir", "dst:shaders");
    attrs.Add("TextureTool", "toolkit:bin/win32/nvdxt.exe");
    attrs.Add("TextureAttrTable", "src:textures/batchattributes.xml");
    attrs.Add("TextureSrcDir", "src:textures");
    attrs.Add("TextureDstDir", "dst:textures");
    this->platformAttrs.Add(Platform::Win32, attrs);
}

} // namespace ToolkitUtil