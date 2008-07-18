#pragma once
#ifndef COREGRAPHICS_VERTEXLAYOUTSERVER_H
#define COREGRAPHICS_VERTEXLAYOUTSERVER_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::VertexLayoutServer
    
    The VertexLayoutServer creates VertexLayout objects shared by their
    vertex component signature. On some platforms it is more efficient
    to share VertexLayout objects across meshes with identical
    vertex structure. Note that there is no way to manually discard
    vertex components. Vertex components stay alive for the life time
    of the application until the Close() method of the VertexLayoutServer
    is called.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "coregraphics/vertexcomponent.h"
#include "util/stringatom.h"

//------------------------------------------------------------------------------
namespace CoreGraphics
{
class VertexLayout;

class VertexLayoutServer : public Core::RefCounted
{
    DeclareClass(VertexLayoutServer);
    DeclareSingleton(VertexLayoutServer);
public:
    /// constructor
    VertexLayoutServer();
    /// destructor
    virtual ~VertexLayoutServer();
    
    /// open the server
    void Open();
    /// close the server
    void Close();
    /// return true if open
    bool IsOpen() const;
    
    /// create shared vertex layout object
    Ptr<VertexLayout> CreateSharedVertexLayout(const Util::Array<VertexComponent>& vertexComponents);

private:
    bool isOpen;
    Util::Dictionary<Util::StringAtom, Ptr<VertexLayout> > vertexLayouts;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
VertexLayoutServer::IsOpen() const
{
    return this->isOpen;
}

} // namespace VertexLayoutServer
//------------------------------------------------------------------------------
#endif


    