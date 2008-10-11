//------------------------------------------------------------------------------
//  toolkitapp.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "toolkitutil/toolkitapp.h"

namespace ToolkitUtil
{
using namespace IO;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
ToolkitApp::ToolkitApp() :
    waitForKey(false),
    platform(Platform::Win32)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
ToolkitApp::ParseCmdLineArgs()
{
    bool help = this->args.GetBool("-help");
    if (help)
    {
        this->ShowHelp();
        return false;
    }
    this->platform = Platform::FromString(this->args.GetString("-platform", "win32"));;
    this->waitForKey = this->args.GetBool("-waitforkey");
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
ToolkitApp::SetupProjectInfo()
{
    // read the projectinfo.xml file
    ProjectInfo::Result res = this->projectInfo.Setup();
    if (res != ProjectInfo::Success)
    {
        n_printf("%s\n", this->projectInfo.GetErrorString(res).AsCharPtr());
        this->SetReturnCode(10);
        return false;
    }

    // gather the relevant paths from the project info object
    if (!this->projectInfo.HasPlatform(this->platform))
    {
        n_printf("ERROR: platform '%s' not defined in projectinfo.xml file!\n",
            Platform::ToString(this->platform).AsCharPtr());
        this->SetReturnCode(10);
        return false;
    }

    // prepare generic info about project
    this->projectInfo.SetCurrentPlatform(this->platform);
    IoServer::Instance()->SetAssign(Assign("src", this->projectInfo.GetAttr("SrcDir")));
    IoServer::Instance()->SetAssign(Assign("dst", this->projectInfo.GetAttr("DstDir")));

    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
ToolkitApp::ShowHelp()
{
    n_printf("Generic ToolkitApp help text. FIXME!!!\n");
}

} // namespace ToolkitUtil
