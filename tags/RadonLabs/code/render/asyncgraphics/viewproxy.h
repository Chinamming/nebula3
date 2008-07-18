#pragma once
#ifndef ASYNCGRAPHICS_VIEWPROXY_H
#define ASYNCGRAPHOCS_VIEWPROXY_H
//------------------------------------------------------------------------------
/**
    @class AsyncGraphics::ViewProxy
    
    A client-side proxy of a Graphics::View in the AsyncGraphic subsystem.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "util/stringatom.h"
#include "resources/resourceid.h"
#include "asyncgraphics/handle.h"

//------------------------------------------------------------------------------
namespace AsyncGraphics
{
class ViewProxy : public Core::RefCounted
{
    DeclareClass(ViewProxy);
public:
    /// constructor
    ViewProxy();
    /// destructor
    virtual ~ViewProxy();

    /// return true if the ViewProxy is valid
    bool IsValid() const;    
    /// get name of view-proxy
    const Util::StringAtom& GetName() const;
    /// get the class-type of the server-side view object
    const Core::Rtti& GetViewClass() const;
    /// get the name of the stage this view is attached to
    const Util::StringAtom& GetStageName() const;
    /// get the name of the frame-shader this view will use for rendering
    const Resources::ResourceId& GetFrameShaderName() const;
    /// check whether this is the default view
    bool IsDefaultView() const;

private:
    friend class GraphicsServerProxy;

    /// set name of view-proxy
    void SetName(const Util::StringAtom& name);
    /// set the class-type of the server-side view object
    void SetViewClass(const Core::Rtti& rtti);
    /// set the name of the stage this view will be attached to (stage must already exist!)
    void SetStageName(const Util::StringAtom& stageName);
    /// set the name of the frame-shader this view will use for rendering
    void SetFrameShaderName(const Resources::ResourceId& frameShaderName);
    /// set to true if this is the default view
    void SetDefaultView(bool b);
    /// setup the view proxy (waits for completion)
    void Setup();
    /// discard the view proxy (waits for completion)
    void Discard();

    Util::StringAtom name;
    const Core::Rtti* viewClass;
    Util::StringAtom stageName;
    Resources::ResourceId frameShaderName;
    bool isDefaultView;
    AsyncGraphics::Handle viewHandle;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
ViewProxy::IsValid() const
{
    return (0 != this->viewHandle);
}

//------------------------------------------------------------------------------
/**
*/
inline void
ViewProxy::SetName(const Util::StringAtom& n)
{
    this->name = n;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
ViewProxy::GetName() const
{
    return this->name;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ViewProxy::SetViewClass(const Core::Rtti& rtti)
{
    this->viewClass = &rtti;
}

//------------------------------------------------------------------------------
/**
*/
inline const Core::Rtti& 
ViewProxy::GetViewClass() const
{
    n_assert(0 != this->viewClass);
    return *this->viewClass;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ViewProxy::SetStageName(const Util::StringAtom& n)
{
    this->stageName = n;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::StringAtom&
ViewProxy::GetStageName() const
{
    return this->stageName;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ViewProxy::SetFrameShaderName(const Resources::ResourceId& r)
{
    this->frameShaderName = r;
}

//------------------------------------------------------------------------------
/**
*/
inline const Resources::ResourceId&
ViewProxy::GetFrameShaderName() const
{
    return this->frameShaderName;
}

//------------------------------------------------------------------------------
/**
*/
inline void
ViewProxy::SetDefaultView(bool b)
{
    this->isDefaultView = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
ViewProxy::IsDefaultView() const
{
    return this->isDefaultView;
}

} // namespace AsyncGraphics
//------------------------------------------------------------------------------
#endif    