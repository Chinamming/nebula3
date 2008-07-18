//------------------------------------------------------------------------------
//  corepagehandler.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "core/debug/corepagehandler.h"
#include "http/htmlpagewriter.h"

namespace Debug
{
ImplementClass(Debug::CorePageHandler, 'CPGH', Http::HttpRequestHandler);

using namespace IO;
using namespace Http;
using namespace Util;
using namespace Core;

//------------------------------------------------------------------------------
/**
*/
CorePageHandler::CorePageHandler()
{
    this->SetName("Core");
    this->SetDesc("show debug information about Core subsystem");
    this->SetRootLocation("/core");
}

//------------------------------------------------------------------------------
/**
*/
bool
CorePageHandler::AcceptsRequest(const Ptr<HttpRequest>& request)
{
    return (HttpMethod::Get == request->GetMethod()) && String::MatchPattern(request->GetURI().LocalPath(), "core");
}

//------------------------------------------------------------------------------
/**
*/
void
CorePageHandler::HandleRequest(const Ptr<HttpRequest>& request) 
{
    n_assert(HttpMethod::Get == request->GetMethod());

    // configure a HTML page writer
    Ptr<HtmlPageWriter> htmlWriter = HtmlPageWriter::Create();
    htmlWriter->SetStream(request->GetResponseContentStream());
    htmlWriter->SetTitle("Nebula3 Core Info");
    if (htmlWriter->Open())
    {
        htmlWriter->Element(HtmlElement::Heading1, "Core Subsystem");
        htmlWriter->AddAttr("href", "/index.html");
        htmlWriter->Element(HtmlElement::Anchor, "Home");

        // if not compiled with NEBULA_DEBUG, display a message
        #if (NEBULA3_MEMORY_STATS == 0)
        htmlWriter->LineBreak();
        htmlWriter->LineBreak();
        htmlWriter->Text("Core stats not available because application was not compiled with debug information!");
        #else
        htmlWriter->Element(HtmlElement::Heading3, "RefCounted Instances");
        Dictionary<String, RefCounted::Stats> refCountedStats = RefCounted::GetOverallStats();
        /// count overall number of instances
        IndexT i;
        SizeT overallNumInstances = 0;
        for (i = 0; i < refCountedStats.Size(); i++)
        {
            overallNumInstances += refCountedStats.ValueAtIndex(i).numObjects;
        }
        htmlWriter->Text("Overall Number Of Instances: " + String::FromInt(overallNumInstances));
        htmlWriter->LineBreak();
        htmlWriter->LineBreak();

        htmlWriter->AddAttr("border", "1");
        htmlWriter->AddAttr("rules", "cols");
        htmlWriter->Begin(HtmlElement::Table);
            htmlWriter->AddAttr("bgcolor", "lightsteelblue");
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableHeader, "Class Name");
                htmlWriter->Element(HtmlElement::TableHeader, "Class FourCC");
                htmlWriter->Element(HtmlElement::TableHeader, "NumInstances");
                htmlWriter->Element(HtmlElement::TableHeader, "References");
            htmlWriter->End(HtmlElement::TableRow);

            for (i = 0; i < refCountedStats.Size(); i++)
            {
                const RefCounted::Stats& curStats = refCountedStats.ValueAtIndex(i);
                htmlWriter->Begin(HtmlElement::TableRow);
                    htmlWriter->Element(HtmlElement::TableData, curStats.className);
                    htmlWriter->Element(HtmlElement::TableData, curStats.classFourCC.AsString());
                    htmlWriter->Element(HtmlElement::TableData, String::FromInt(curStats.numObjects));
                    htmlWriter->Element(HtmlElement::TableData, String::FromInt(curStats.overallRefCount));
                htmlWriter->End(HtmlElement::TableRow);
            }
        htmlWriter->End(HtmlElement::Table);
        #endif
        htmlWriter->Close();
        request->SetStatus(HttpStatus::OK);
    }
    else
    {
        request->SetStatus(HttpStatus::InternalServerError);
    }
}

} // namespace Debug