//------------------------------------------------------------------------------
//  meshpagehandler.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/debug/meshpagehandler.h"
#include "coregraphics/mesh.h"
#include "http/htmlpagewriter.h"
#include "resources/sharedresourceserver.h"
#include "io/ioserver.h"

namespace Debug
{
ImplementClass(Debug::MeshPageHandler, 'DMSH', Http::HttpRequestHandler);

using namespace IO;
using namespace CoreGraphics;
using namespace Util;
using namespace Http;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
MeshPageHandler::MeshPageHandler()
{
    this->SetName("Meshes");
    this->SetDesc("show debug information about shared mesh resources");
    this->SetRootLocation("/mesh");
}

//------------------------------------------------------------------------------
/**
*/
bool
MeshPageHandler::AcceptsRequest(const Ptr<HttpRequest>& request)
{
    return (HttpMethod::Get == request->GetMethod()) && (String::MatchPattern(request->GetURI().LocalPath(), "mesh*"));
}

//------------------------------------------------------------------------------
/**
*/
void
MeshPageHandler::HandleRequest(const Ptr<HttpRequest>& request)
{
    n_assert(HttpMethod::Get == request->GetMethod());

    // first check if a command has been defined in the URI
    Dictionary<String,String> query = request->GetURI().ParseQuery();
    if (query.Contains("meshinfo"))
    {
        request->SetStatus(this->HandleMeshInfoRequest(ResourceId(query["meshinfo"]), request->GetResponseContentStream()));
        return;
    }

    // no command, send the home page
    Ptr<HtmlPageWriter> htmlWriter = HtmlPageWriter::Create();
    htmlWriter->SetStream(request->GetResponseContentStream());
    htmlWriter->SetTitle("Nebula3 Meshes");
    if (htmlWriter->Open())
    {
        htmlWriter->Element(HtmlElement::Heading1, "Shared Mesh Resources");
        htmlWriter->AddAttr("href", "/index.html");
        htmlWriter->Element(HtmlElement::Anchor, "Home");
        htmlWriter->LineBreak();
        htmlWriter->LineBreak();

        // get all mesh resources
        Array<Ptr<Resource>> meshResources = SharedResourceServer::Instance()->GetSharedResourcesByType(Mesh::RTTI);
    
        // create a table of all existing meshes
        htmlWriter->AddAttr("border", "1");
        htmlWriter->AddAttr("rules", "cols");
        htmlWriter->Begin(HtmlElement::Table);
            htmlWriter->AddAttr("bgcolor", "lightsteelblue");
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableHeader, "ResId");
                htmlWriter->Element(HtmlElement::TableHeader, "UseCount");
            htmlWriter->End(HtmlElement::TableRow);
            
            // iterate over shared resources
            IndexT i;
            for (i = 0; i < meshResources.Size(); i++)
            {
                const Ptr<Mesh>& mesh = meshResources[i].downcast<Mesh>();
                htmlWriter->Begin(HtmlElement::TableRow);
                    htmlWriter->Begin(HtmlElement::TableData);
                        htmlWriter->AddAttr("href", "/mesh?meshinfo=" + mesh->GetResourceId().Value());
                        htmlWriter->Element(HtmlElement::Anchor, mesh->GetResourceId().Value());
                    htmlWriter->End(HtmlElement::TableData);
                    htmlWriter->Element(HtmlElement::TableData, String::FromInt(mesh->GetUseCount()));
                htmlWriter->End(HtmlElement::TableRow);
            }
        htmlWriter->End(HtmlElement::Table);
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
HttpStatus::Code
MeshPageHandler::HandleMeshInfoRequest(const ResourceId& resId, const Ptr<Stream>& responseContentStream)
{
    // lookup the mesh in the SharedResourceServer
    SharedResourceServer* resServer = SharedResourceServer::Instance();
    if (!resServer->HasSharedResource(resId))
    {
        return HttpStatus::NotFound;
    }
    const Ptr<Resource>& res = resServer->LookupSharedResource(resId);
    if (!res->IsA(Mesh::RTTI))
    {
        // resource exists but is not a mesh
        return HttpStatus::NotFound;
    }
    const Ptr<Mesh>& mesh = res.downcast<Mesh>();

    Ptr<HtmlPageWriter> htmlWriter = HtmlPageWriter::Create();
    htmlWriter->SetStream(responseContentStream);
    htmlWriter->SetTitle("Nebula3 Mesh Info");
    if (htmlWriter->Open())
    {
        htmlWriter->Element(HtmlElement::Heading1, resId.Value());
        htmlWriter->AddAttr("href", "/index.html");
        htmlWriter->Element(HtmlElement::Anchor, "Home");
        htmlWriter->LineBreak();
        htmlWriter->AddAttr("href", "/mesh");
        htmlWriter->Element(HtmlElement::Anchor, "Meshes Home");
        htmlWriter->LineBreak();
        htmlWriter->LineBreak();

        // display some info about the mesh
        htmlWriter->Element(HtmlElement::Heading3, "Resource Info");
        htmlWriter->Begin(HtmlElement::Table);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Resource Id: ");
                htmlWriter->Element(HtmlElement::TableData, resId.Value());
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Resolved Path: ");
                htmlWriter->Element(HtmlElement::TableData, IoServer::Instance()->ResolveAssigns(resId.Value()).AsString());
            htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableData, "Use Count: ");
                htmlWriter->Element(HtmlElement::TableData, String::FromInt(mesh->GetUseCount()));
            htmlWriter->End(HtmlElement::TableRow);
        htmlWriter->End(HtmlElement::Table);

        // write vertex buffer info
        htmlWriter->Element(HtmlElement::Heading3, "Vertices");
        if (mesh->HasVertexBuffer())
        {
            const Ptr<VertexBuffer>& vb = mesh->GetVertexBuffer();
            const Ptr<VertexLayout>& vertexLayout = vb->GetVertexLayout();
            htmlWriter->Begin(HtmlElement::Table);
                htmlWriter->Begin(HtmlElement::TableRow);
                    htmlWriter->Element(HtmlElement::TableData, "Num Vertices: ");
                    htmlWriter->Element(HtmlElement::TableData, String::FromInt(vb->GetNumVertices()));
                htmlWriter->End(HtmlElement::TableRow);
                htmlWriter->Begin(HtmlElement::TableRow);
                    htmlWriter->Element(HtmlElement::TableData, "Vertex Stride: ");
                    htmlWriter->Element(HtmlElement::TableData, String::FromInt(vertexLayout->GetVertexByteSize()) + " bytes");                
                htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->End(HtmlElement::Table);
            htmlWriter->Element(HtmlElement::Heading3, "Vertex Components");
            htmlWriter->AddAttr("border", "1");
            htmlWriter->AddAttr("rules", "cols");
            htmlWriter->Begin(HtmlElement::Table);
                htmlWriter->AddAttr("bgcolor", "lightsteelblue");
                htmlWriter->Begin(HtmlElement::TableRow);
                    htmlWriter->Element(HtmlElement::TableHeader, "Semantic");
                    htmlWriter->Element(HtmlElement::TableHeader, "SemIdx");
                    htmlWriter->Element(HtmlElement::TableHeader, "Format");
                    htmlWriter->Element(HtmlElement::TableHeader, "Offset");
                    htmlWriter->Element(HtmlElement::TableHeader, "Size");
                htmlWriter->End(HtmlElement::TableRow);
                IndexT i;
                for (i = 0; i < vertexLayout->GetNumComponents(); i++)
                {
                    htmlWriter->Begin(HtmlElement::TableRow);
                        const VertexComponent& comp = vertexLayout->GetComponentAt(i);
                        htmlWriter->Element(HtmlElement::TableData, VertexComponent::SemanticNameToString(comp.GetSemanticName()));
                        htmlWriter->Element(HtmlElement::TableData, String::FromInt(comp.GetSemanticIndex()));
                        htmlWriter->Element(HtmlElement::TableData, VertexComponent::FormatToString(comp.GetFormat()));
                        htmlWriter->Element(HtmlElement::TableData, String::FromInt(vertexLayout->GetComponentByteOffset(comp.GetSemanticName(), comp.GetSemanticIndex())));
                        htmlWriter->Element(HtmlElement::TableData, String::FromInt(comp.GetByteSize()));
                    htmlWriter->End(HtmlElement::TableRow);
                }
            htmlWriter->End(HtmlElement::Table);
        }
        else
        {
            htmlWriter->Text("NO VERTICES");
            htmlWriter->LineBreak();
        }

        // write index buffer info
        htmlWriter->Element(HtmlElement::Heading3, "Indices");
        if (mesh->HasIndexBuffer())
        {
            const Ptr<IndexBuffer>& ib = mesh->GetIndexBuffer();
            htmlWriter->Begin(HtmlElement::Table);
                htmlWriter->Begin(HtmlElement::TableRow);
                    htmlWriter->Element(HtmlElement::TableData, "Num Indices: ");
                    htmlWriter->Element(HtmlElement::TableData, String::FromInt(ib->GetNumIndices()));
                htmlWriter->End(HtmlElement::TableRow);
                htmlWriter->Begin(HtmlElement::TableRow);
                    htmlWriter->Element(HtmlElement::TableData, "Index Type: ");
                    htmlWriter->Element(HtmlElement::TableData, IndexType::ToString(ib->GetIndexType()));
                htmlWriter->End(HtmlElement::TableRow);
            htmlWriter->End(HtmlElement::Table);
        }
        else
        {
            htmlWriter->Text("NO INDICES");
            htmlWriter->LineBreak();
        }

        // write primitive group info
        htmlWriter->Element(HtmlElement::Heading3, "Primitive Groups");
        htmlWriter->AddAttr("border", "1");
        htmlWriter->AddAttr("rules", "cols");
        htmlWriter->Begin(HtmlElement::Table);
            htmlWriter->AddAttr("bgcolor", "lightsteelblue");
            htmlWriter->Begin(HtmlElement::TableRow);
                htmlWriter->Element(HtmlElement::TableHeader, "Num Primitives");
                htmlWriter->Element(HtmlElement::TableHeader, "Base Vertex");
                htmlWriter->Element(HtmlElement::TableHeader, "Num Vertices");
                htmlWriter->Element(HtmlElement::TableHeader, "Base Index");
                htmlWriter->Element(HtmlElement::TableHeader, "Num Indices");
                htmlWriter->Element(HtmlElement::TableHeader, "Topology");
            htmlWriter->End(HtmlElement::TableRow);
            IndexT i;
            for (i = 0; i < mesh->GetNumPrimitiveGroups(); i++)
            {
                const PrimitiveGroup& primGroup = mesh->GetPrimitiveGroupAtIndex(i);
                htmlWriter->Begin(HtmlElement::TableRow);
                    htmlWriter->Element(HtmlElement::TableData, String::FromInt(primGroup.GetNumPrimitives()));
                    htmlWriter->Element(HtmlElement::TableData, String::FromInt(primGroup.GetBaseVertex()));
                    htmlWriter->Element(HtmlElement::TableData, String::FromInt(primGroup.GetNumVertices()));
                    htmlWriter->Element(HtmlElement::TableData, String::FromInt(primGroup.GetBaseIndex()));
                    htmlWriter->Element(HtmlElement::TableData, String::FromInt(primGroup.GetNumIndices()));
                    htmlWriter->Element(HtmlElement::TableData, PrimitiveTopology::ToString(primGroup.GetPrimitiveTopology()));
                htmlWriter->End(HtmlElement::TableRow);
            }
        htmlWriter->End(HtmlElement::Table);
        htmlWriter->Close();
        return HttpStatus::OK;
    }
    return HttpStatus::InternalServerError;
}

} // namespace Debug
