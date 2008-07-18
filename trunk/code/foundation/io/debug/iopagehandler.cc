//------------------------------------------------------------------------------
//  iopagehandler.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "io/debug/iopagehandler.h"
#include "http/htmlpagewriter.h"
#include "io/ioserver.h"
#include "io/zipfs/zipfilesystem.h"
#include "io/zipfs/ziparchive.h"
#include "io/console.h"

namespace Debug
{
ImplementClass(Debug::IoPageHandler, 'DIOP', Http::HttpRequestHandler);

using namespace Http;
using namespace IO;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
IoPageHandler::IoPageHandler()
{
    this->SetName("IO");
    this->SetDesc("show debug information about IO subsystem");
    this->SetRootLocation("/inout");
}

//------------------------------------------------------------------------------
/**
*/
bool
IoPageHandler::AcceptsRequest(const Ptr<HttpRequest>& request)
{
    return (HttpMethod::Get == request->GetMethod()) && String::MatchPattern(request->GetURI().LocalPath(), "inout*");
}

//------------------------------------------------------------------------------
/**
*/
void
IoPageHandler::HandleRequest(const Ptr<HttpRequest>& request) 
{
    n_assert(HttpMethod::Get == request->GetMethod());

    // first check if a command has been defined in the URI
    Dictionary<String,String> query = request->GetURI().ParseQuery();
    if (query.Contains("ls"))
    {
        this->ListDirectory(query["ls"], request);
        return;
    }

    // no command, display root page
    Ptr<HtmlPageWriter> htmlWriter = HtmlPageWriter::Create();
    htmlWriter->SetStream(request->GetResponseContentStream());
    htmlWriter->SetTitle("Nebula3 IO Subsystem Info");
    if (htmlWriter->Open())
    {
        IoServer* ioServer = IoServer::Instance();        
        htmlWriter->Element(HtmlElement::Heading1, "IO Subsystem");
        htmlWriter->AddAttr("href", "/index.html");
        htmlWriter->Element(HtmlElement::Anchor, "Home");

        // display registered URI schemes
        htmlWriter->Element(HtmlElement::Heading3, "Registered Schemes");
        htmlWriter->Begin(HtmlElement::Table);
        Array<String> uriSchemes = ioServer->GetAllRegisteredUriSchemes();
        IndexT schemeIndex;
        for (schemeIndex = 0; schemeIndex < uriSchemes.Size(); schemeIndex++)
        {
            htmlWriter->Begin(HtmlElement::TableRow);
            htmlWriter->Element(HtmlElement::TableData, uriSchemes[schemeIndex]);
            htmlWriter->Element(HtmlElement::TableData, ioServer->GetStreamClassByUriScheme(uriSchemes[schemeIndex]).GetName());
            htmlWriter->End(HtmlElement::TableRow);
        }
        htmlWriter->End(HtmlElement::Table);
        
        // display registered assigns
        htmlWriter->Element(HtmlElement::Heading3, "Registered Assigns");
        htmlWriter->Begin(HtmlElement::Table);
        Array<Assign> assigns = ioServer->GetAllAssigns();
        IndexT assignIndex;
        for (assignIndex = 0; assignIndex < assigns.Size(); assignIndex++)
        {
            htmlWriter->Begin(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableData);
                htmlWriter->AddAttr("href", "/inout?ls=" + assigns[assignIndex].GetName() + ":");
                htmlWriter->Element(HtmlElement::Anchor, assigns[assignIndex].GetName());
            htmlWriter->End(HtmlElement::TableData);
            htmlWriter->Element(HtmlElement::TableData, assigns[assignIndex].GetPath());
            htmlWriter->Element(HtmlElement::TableData, ioServer->ResolveAssigns(assigns[assignIndex].GetPath()).AsString());
            htmlWriter->End(HtmlElement::TableRow);
        }
        htmlWriter->End(HtmlElement::Table);

        // display mounted zip archives
        htmlWriter->Element(HtmlElement::Heading3, "Mounted Zip Archives");
        Array<Ptr<ZipArchive>> zipArchives = ZipFileSystem::Instance()->GetMountedZipArchives();
        if (zipArchives.Size() > 0)
        {
            htmlWriter->Begin(HtmlElement::UnorderedList);
            IndexT zipIndex;
            for (zipIndex = 0; zipIndex < zipArchives.Size(); zipIndex++)
            {
                htmlWriter->Element(HtmlElement::ListItem, zipArchives[zipIndex]->GetURI().AsString());
            }
            htmlWriter->End(HtmlElement::UnorderedList);
        }
        else
        {
            htmlWriter->Text("[NONE]");
        }

        // display attached console handlers
        htmlWriter->Element(HtmlElement::Heading3, "Attached Console Handlers");
        htmlWriter->Begin(HtmlElement::UnorderedList);
        SizeT numConHandlers = Console::Instance()->GetNumHandlers();
        IndexT conHandlerIndex;
        for (conHandlerIndex = 0; conHandlerIndex < numConHandlers; conHandlerIndex++)
        {
            htmlWriter->Element(HtmlElement::ListItem, Console::Instance()->GetHandlerByIndex(conHandlerIndex)->GetClassName());
        }
        htmlWriter->End(HtmlElement::UnorderedList);
        htmlWriter->Close();
        request->SetStatus(HttpStatus::OK);
    }
    else
    {
        request->SetStatus(HttpStatus::InternalServerError);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
IoPageHandler::ListDirectory(const Util::String& path, const Ptr<HttpRequest>& request)
{
    Ptr<HtmlPageWriter> htmlWriter = HtmlPageWriter::Create();
    htmlWriter->SetStream(request->GetResponseContentStream());
    htmlWriter->SetTitle("Nebula3 Directory Listing");
    if (htmlWriter->Open())
    {
        IoServer* ioServer = IoServer::Instance();        
        htmlWriter->Element(HtmlElement::Heading1, "Directory Listing");
        htmlWriter->AddAttr("href", "/index.html");
        htmlWriter->Element(HtmlElement::Anchor, "Home");
        htmlWriter->LineBreak();
        htmlWriter->AddAttr("href", "/inout");
        htmlWriter->Element(HtmlElement::Anchor, "IO Subsystem Home");
        htmlWriter->LineBreak();
        if (path[path.Length() - 1] != ':')
        {
            // add parent directory navigation entry (if not top-level)
            String str = "/inout?ls=" + path.ExtractDirName();
            str.TrimRight("/");
            htmlWriter->AddAttr("href", str);
            htmlWriter->Element(HtmlElement::Anchor, "Parent Directory");
        }
        htmlWriter->LineBreak();
        
        htmlWriter->Element(HtmlElement::Heading3, path + "    (" + ioServer->ResolveAssigns(path).AsString() + ")");
        Array<String> dirs = ioServer->ListDirectories(path, "*");
        Array<String> files = ioServer->ListFiles(path, "*");

        htmlWriter->Begin(HtmlElement::UnorderedList);
        IndexT i;
        for (i = 0; i < dirs.Size(); i++)
        {
            htmlWriter->Begin(HtmlElement::ListItem);
            htmlWriter->AddAttr("href", "/inout?ls=" + path + "/" + dirs[i]);
            htmlWriter->Element(HtmlElement::Anchor, dirs[i]);
            htmlWriter->End(HtmlElement::ListItem);
        }
        for (i = 0; i < files.Size(); i++)
        {
            htmlWriter->Element(HtmlElement::ListItem, files[i]);
        }
        htmlWriter->End(HtmlElement::UnorderedList);

        htmlWriter->Close();
        request->SetStatus(HttpStatus::OK);
    }
    else
    {
        request->SetStatus(HttpStatus::InternalServerError);
    }
}

} // namespace Debug
