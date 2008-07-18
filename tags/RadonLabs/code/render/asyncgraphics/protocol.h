#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "util/stringatom.h"
#include "core/rtti.h"
#include "coregraphics/displaymode.h"
#include "coregraphics/adapter.h"
#include "coregraphics/antialiasquality.h"
#include "coregraphics/pixelformat.h"
#include "coregraphics/adapterinfo.h"
#include "coregraphics/threadsafedisplayeventhandler.h"
#include "coregraphics/threadsaferendereventhandler.h"
#include "attr/attributecontainer.h"
#include "resources/resourceid.h"
#include "math/matrix44.h"
#include "math/float4.h"
#include "asyncgraphics/handle.h"

//------------------------------------------------------------------------------
namespace AsyncGraphics
{
//------------------------------------------------------------------------------
class SetupGraphics : public Messaging::Message
{
    DeclareClass(SetupGraphics);
    DeclareMsgId;
public:
    SetupGraphics() 
    { };
public:
    void SetAdapter(const CoreGraphics::Adapter::Code& val)
    {
        n_assert(!this->handled);
        this->adapter = val;
    };
    const CoreGraphics::Adapter::Code& GetAdapter() const
    {
        return this->adapter;
    };
private:
    CoreGraphics::Adapter::Code adapter;
public:
    void SetDisplayMode(const CoreGraphics::DisplayMode& val)
    {
        n_assert(!this->handled);
        this->displaymode = val;
    };
    const CoreGraphics::DisplayMode& GetDisplayMode() const
    {
        return this->displaymode;
    };
private:
    CoreGraphics::DisplayMode displaymode;
public:
    void SetAntiAliasQuality(const CoreGraphics::AntiAliasQuality::Code& val)
    {
        n_assert(!this->handled);
        this->antialiasquality = val;
    };
    const CoreGraphics::AntiAliasQuality::Code& GetAntiAliasQuality() const
    {
        return this->antialiasquality;
    };
private:
    CoreGraphics::AntiAliasQuality::Code antialiasquality;
public:
    void SetFullscreen(bool val)
    {
        n_assert(!this->handled);
        this->fullscreen = val;
    };
    bool GetFullscreen() const
    {
        return this->fullscreen;
    };
private:
    bool fullscreen;
public:
    void SetDisplayModeSwitchEnabled(bool val)
    {
        n_assert(!this->handled);
        this->displaymodeswitchenabled = val;
    };
    bool GetDisplayModeSwitchEnabled() const
    {
        return this->displaymodeswitchenabled;
    };
private:
    bool displaymodeswitchenabled;
public:
    void SetTripleBufferingEnabled(bool val)
    {
        n_assert(!this->handled);
        this->triplebufferingenabled = val;
    };
    bool GetTripleBufferingEnabled() const
    {
        return this->triplebufferingenabled;
    };
private:
    bool triplebufferingenabled;
public:
    void SetAlwaysOnTop(bool val)
    {
        n_assert(!this->handled);
        this->alwaysontop = val;
    };
    bool GetAlwaysOnTop() const
    {
        return this->alwaysontop;
    };
private:
    bool alwaysontop;
public:
    void SetVerticalSyncEnabled(bool val)
    {
        n_assert(!this->handled);
        this->verticalsyncenabled = val;
    };
    bool GetVerticalSyncEnabled() const
    {
        return this->verticalsyncenabled;
    };
private:
    bool verticalsyncenabled;
public:
    void SetIconName(const Util::String& val)
    {
        n_assert(!this->handled);
        this->iconname = val;
    };
    const Util::String& GetIconName() const
    {
        return this->iconname;
    };
private:
    Util::String iconname;
public:
    void SetSuccess(bool val)
    {
        n_assert(!this->handled);
        this->success = val;
    };
    bool GetSuccess() const
    {
        n_assert(this->handled);
        return this->success;
    };
private:
    bool success;
};
//------------------------------------------------------------------------------
class AdapterExists : public Messaging::Message
{
    DeclareClass(AdapterExists);
    DeclareMsgId;
public:
    AdapterExists() 
    { };
public:
    void SetAdapter(const CoreGraphics::Adapter::Code& val)
    {
        n_assert(!this->handled);
        this->adapter = val;
    };
    const CoreGraphics::Adapter::Code& GetAdapter() const
    {
        return this->adapter;
    };
private:
    CoreGraphics::Adapter::Code adapter;
public:
    void SetResult(bool val)
    {
        n_assert(!this->handled);
        this->result = val;
    };
    bool GetResult() const
    {
        n_assert(this->handled);
        return this->result;
    };
private:
    bool result;
};
//------------------------------------------------------------------------------
class GetAvailableDisplayModes : public Messaging::Message
{
    DeclareClass(GetAvailableDisplayModes);
    DeclareMsgId;
public:
    GetAvailableDisplayModes() 
    { };
public:
    void SetAdapter(const CoreGraphics::Adapter::Code& val)
    {
        n_assert(!this->handled);
        this->adapter = val;
    };
    const CoreGraphics::Adapter::Code& GetAdapter() const
    {
        return this->adapter;
    };
private:
    CoreGraphics::Adapter::Code adapter;
public:
    void SetPixelFormat(const CoreGraphics::PixelFormat::Code& val)
    {
        n_assert(!this->handled);
        this->pixelformat = val;
    };
    const CoreGraphics::PixelFormat::Code& GetPixelFormat() const
    {
        return this->pixelformat;
    };
private:
    CoreGraphics::PixelFormat::Code pixelformat;
public:
    void SetResult(const Util::Array<CoreGraphics::DisplayMode>& val)
    {
        n_assert(!this->handled);
        this->result = val;
    };
    const Util::Array<CoreGraphics::DisplayMode>& GetResult() const
    {
        n_assert(this->handled);
        return this->result;
    };
private:
    Util::Array<CoreGraphics::DisplayMode> result;
};
//------------------------------------------------------------------------------
class SupportsDisplayMode : public Messaging::Message
{
    DeclareClass(SupportsDisplayMode);
    DeclareMsgId;
public:
    SupportsDisplayMode() 
    { };
public:
    void SetAdapter(const CoreGraphics::Adapter::Code& val)
    {
        n_assert(!this->handled);
        this->adapter = val;
    };
    const CoreGraphics::Adapter::Code& GetAdapter() const
    {
        return this->adapter;
    };
private:
    CoreGraphics::Adapter::Code adapter;
public:
    void SetDisplayMode(const CoreGraphics::DisplayMode& val)
    {
        n_assert(!this->handled);
        this->displaymode = val;
    };
    const CoreGraphics::DisplayMode& GetDisplayMode() const
    {
        return this->displaymode;
    };
private:
    CoreGraphics::DisplayMode displaymode;
public:
    void SetResult(bool val)
    {
        n_assert(!this->handled);
        this->result = val;
    };
    bool GetResult() const
    {
        n_assert(this->handled);
        return this->result;
    };
private:
    bool result;
};
//------------------------------------------------------------------------------
class GetCurrentAdapterDisplayMode : public Messaging::Message
{
    DeclareClass(GetCurrentAdapterDisplayMode);
    DeclareMsgId;
public:
    GetCurrentAdapterDisplayMode() 
    { };
public:
    void SetAdapter(const CoreGraphics::Adapter::Code& val)
    {
        n_assert(!this->handled);
        this->adapter = val;
    };
    const CoreGraphics::Adapter::Code& GetAdapter() const
    {
        return this->adapter;
    };
private:
    CoreGraphics::Adapter::Code adapter;
public:
    void SetResult(const CoreGraphics::DisplayMode& val)
    {
        n_assert(!this->handled);
        this->result = val;
    };
    const CoreGraphics::DisplayMode& GetResult() const
    {
        n_assert(this->handled);
        return this->result;
    };
private:
    CoreGraphics::DisplayMode result;
};
//------------------------------------------------------------------------------
class GetAdapterInfo : public Messaging::Message
{
    DeclareClass(GetAdapterInfo);
    DeclareMsgId;
public:
    GetAdapterInfo() 
    { };
public:
    void SetAdapter(const CoreGraphics::Adapter::Code& val)
    {
        n_assert(!this->handled);
        this->adapter = val;
    };
    const CoreGraphics::Adapter::Code& GetAdapter() const
    {
        return this->adapter;
    };
private:
    CoreGraphics::Adapter::Code adapter;
public:
    void SetResult(const CoreGraphics::AdapterInfo& val)
    {
        n_assert(!this->handled);
        this->result = val;
    };
    const CoreGraphics::AdapterInfo& GetResult() const
    {
        n_assert(this->handled);
        return this->result;
    };
private:
    CoreGraphics::AdapterInfo result;
};
//------------------------------------------------------------------------------
class AttachDisplayEventHandler : public Messaging::Message
{
    DeclareClass(AttachDisplayEventHandler);
    DeclareMsgId;
public:
    AttachDisplayEventHandler() 
    { };
public:
    void SetHandler(const Ptr<CoreGraphics::ThreadSafeDisplayEventHandler>& val)
    {
        n_assert(!this->handled);
        this->handler = val;
    };
    const Ptr<CoreGraphics::ThreadSafeDisplayEventHandler>& GetHandler() const
    {
        return this->handler;
    };
private:
    Ptr<CoreGraphics::ThreadSafeDisplayEventHandler> handler;
};
//------------------------------------------------------------------------------
class RemoveDisplayEventHandler : public Messaging::Message
{
    DeclareClass(RemoveDisplayEventHandler);
    DeclareMsgId;
public:
    RemoveDisplayEventHandler() 
    { };
public:
    void SetHandler(const Ptr<CoreGraphics::ThreadSafeDisplayEventHandler>& val)
    {
        n_assert(!this->handled);
        this->handler = val;
    };
    const Ptr<CoreGraphics::ThreadSafeDisplayEventHandler>& GetHandler() const
    {
        return this->handler;
    };
private:
    Ptr<CoreGraphics::ThreadSafeDisplayEventHandler> handler;
};
//------------------------------------------------------------------------------
class AttachRenderEventHandler : public Messaging::Message
{
    DeclareClass(AttachRenderEventHandler);
    DeclareMsgId;
public:
    AttachRenderEventHandler() 
    { };
public:
    void SetHandler(const Ptr<CoreGraphics::ThreadSafeRenderEventHandler>& val)
    {
        n_assert(!this->handled);
        this->handler = val;
    };
    const Ptr<CoreGraphics::ThreadSafeRenderEventHandler>& GetHandler() const
    {
        return this->handler;
    };
private:
    Ptr<CoreGraphics::ThreadSafeRenderEventHandler> handler;
};
//------------------------------------------------------------------------------
class RemoveRenderEventHandler : public Messaging::Message
{
    DeclareClass(RemoveRenderEventHandler);
    DeclareMsgId;
public:
    RemoveRenderEventHandler() 
    { };
public:
    void SetHandler(const Ptr<CoreGraphics::ThreadSafeRenderEventHandler>& val)
    {
        n_assert(!this->handled);
        this->handler = val;
    };
    const Ptr<CoreGraphics::ThreadSafeRenderEventHandler>& GetHandler() const
    {
        return this->handler;
    };
private:
    Ptr<CoreGraphics::ThreadSafeRenderEventHandler> handler;
};
//------------------------------------------------------------------------------
class CreateGraphicsStage : public Messaging::Message
{
    DeclareClass(CreateGraphicsStage);
    DeclareMsgId;
public:
    CreateGraphicsStage() :
        stagebuilderclass(0)
    { };
public:
    void SetName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->name = val;
    };
    const Util::StringAtom& GetName() const
    {
        return this->name;
    };
private:
    Util::StringAtom name;
public:
    void SetStageBuilderClass(const Core::Rtti* val)
    {
        n_assert(!this->handled);
        this->stagebuilderclass = val;
    };
    const Core::Rtti* GetStageBuilderClass() const
    {
        return this->stagebuilderclass;
    };
private:
    const Core::Rtti* stagebuilderclass;
public:
    void SetStageBuilderAttrs(const Attr::AttributeContainer& val)
    {
        n_assert(!this->handled);
        this->stagebuilderattrs = val;
    };
    const Attr::AttributeContainer& GetStageBuilderAttrs() const
    {
        return this->stagebuilderattrs;
    };
private:
    Attr::AttributeContainer stagebuilderattrs;
public:
    void SetResult(const AsyncGraphics::Handle& val)
    {
        n_assert(!this->handled);
        this->result = val;
    };
    const AsyncGraphics::Handle& GetResult() const
    {
        n_assert(this->handled);
        return this->result;
    };
private:
    AsyncGraphics::Handle result;
};
//------------------------------------------------------------------------------
class DiscardGraphicsStage : public Messaging::Message
{
    DeclareClass(DiscardGraphicsStage);
    DeclareMsgId;
public:
    DiscardGraphicsStage() 
    { };
public:
    void SetStageHandle(const AsyncGraphics::Handle& val)
    {
        n_assert(!this->handled);
        this->stagehandle = val;
    };
    const AsyncGraphics::Handle& GetStageHandle() const
    {
        return this->stagehandle;
    };
private:
    AsyncGraphics::Handle stagehandle;
};
//------------------------------------------------------------------------------
class CreateGraphicsView : public Messaging::Message
{
    DeclareClass(CreateGraphicsView);
    DeclareMsgId;
public:
    CreateGraphicsView() :
        viewclass(0),
        defaultview(false)
    { };
public:
    void SetViewClass(const Core::Rtti* val)
    {
        n_assert(!this->handled);
        this->viewclass = val;
    };
    const Core::Rtti* GetViewClass() const
    {
        return this->viewclass;
    };
private:
    const Core::Rtti* viewclass;
public:
    void SetName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->name = val;
    };
    const Util::StringAtom& GetName() const
    {
        return this->name;
    };
private:
    Util::StringAtom name;
public:
    void SetStageName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->stagename = val;
    };
    const Util::StringAtom& GetStageName() const
    {
        return this->stagename;
    };
private:
    Util::StringAtom stagename;
public:
    void SetFrameShaderName(const Resources::ResourceId& val)
    {
        n_assert(!this->handled);
        this->frameshadername = val;
    };
    const Resources::ResourceId& GetFrameShaderName() const
    {
        return this->frameshadername;
    };
private:
    Resources::ResourceId frameshadername;
public:
    void SetDefaultView(bool val)
    {
        n_assert(!this->handled);
        this->defaultview = val;
    };
    bool GetDefaultView() const
    {
        return this->defaultview;
    };
private:
    bool defaultview;
public:
    void SetResult(const AsyncGraphics::Handle& val)
    {
        n_assert(!this->handled);
        this->result = val;
    };
    const AsyncGraphics::Handle& GetResult() const
    {
        n_assert(this->handled);
        return this->result;
    };
private:
    AsyncGraphics::Handle result;
};
//------------------------------------------------------------------------------
class DiscardGraphicsView : public Messaging::Message
{
    DeclareClass(DiscardGraphicsView);
    DeclareMsgId;
public:
    DiscardGraphicsView() 
    { };
public:
    void SetViewHandle(const AsyncGraphics::Handle& val)
    {
        n_assert(!this->handled);
        this->viewhandle = val;
    };
    const AsyncGraphics::Handle& GetViewHandle() const
    {
        return this->viewhandle;
    };
private:
    AsyncGraphics::Handle viewhandle;
};
//------------------------------------------------------------------------------
class CreateGraphicsEntity : public Messaging::Message
{
    DeclareClass(CreateGraphicsEntity);
    DeclareMsgId;
public:
    CreateGraphicsEntity() :
        visible(true)
    { };
public:
    void SetStageName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->stagename = val;
    };
    const Util::StringAtom& GetStageName() const
    {
        return this->stagename;
    };
private:
    Util::StringAtom stagename;
public:
    void SetTransform(const Math::matrix44& val)
    {
        n_assert(!this->handled);
        this->transform = val;
    };
    const Math::matrix44& GetTransform() const
    {
        return this->transform;
    };
private:
    Math::matrix44 transform;
public:
    void SetVisible(bool val)
    {
        n_assert(!this->handled);
        this->visible = val;
    };
    bool GetVisible() const
    {
        return this->visible;
    };
private:
    bool visible;
public:
    void SetEntityHandle(const AsyncGraphics::Handle& val)
    {
        n_assert(!this->handled);
        this->entityhandle = val;
    };
    const AsyncGraphics::Handle& GetEntityHandle() const
    {
        n_assert(this->handled);
        return this->entityhandle;
    };
private:
    AsyncGraphics::Handle entityhandle;
};
//------------------------------------------------------------------------------
class DiscardGraphicsEntity : public Messaging::Message
{
    DeclareClass(DiscardGraphicsEntity);
    DeclareMsgId;
public:
    DiscardGraphicsEntity() 
    { };
public:
    void SetEntityHandle(const AsyncGraphics::Handle& val)
    {
        n_assert(!this->handled);
        this->entityhandle = val;
    };
    const AsyncGraphics::Handle& GetEntityHandle() const
    {
        return this->entityhandle;
    };
private:
    AsyncGraphics::Handle entityhandle;
};
//------------------------------------------------------------------------------
class CreateModelEntity : public CreateGraphicsEntity
{
    DeclareClass(CreateModelEntity);
    DeclareMsgId;
public:
    CreateModelEntity() 
    { };
public:
    void SetResourceId(const Resources::ResourceId& val)
    {
        n_assert(!this->handled);
        this->resourceid = val;
    };
    const Resources::ResourceId& GetResourceId() const
    {
        return this->resourceid;
    };
private:
    Resources::ResourceId resourceid;
};
//------------------------------------------------------------------------------
class CreateCameraEntity : public CreateGraphicsEntity
{
    DeclareClass(CreateCameraEntity);
    DeclareMsgId;
public:
    CreateCameraEntity() :
        isperspective(true),
        perspfieldofview(0.0f),
        perspaspectratio(0.0f),
        orthowidth(0.0f),
        orthoheight(0.0f)
    { };
public:
    void SetViewName(const Util::StringAtom& val)
    {
        n_assert(!this->handled);
        this->viewname = val;
    };
    const Util::StringAtom& GetViewName() const
    {
        return this->viewname;
    };
private:
    Util::StringAtom viewname;
public:
    void SetIsPerspective(bool val)
    {
        n_assert(!this->handled);
        this->isperspective = val;
    };
    bool GetIsPerspective() const
    {
        return this->isperspective;
    };
private:
    bool isperspective;
public:
    void SetPerspFieldOfView(float val)
    {
        n_assert(!this->handled);
        this->perspfieldofview = val;
    };
    float GetPerspFieldOfView() const
    {
        return this->perspfieldofview;
    };
private:
    float perspfieldofview;
public:
    void SetPerspAspectRatio(float val)
    {
        n_assert(!this->handled);
        this->perspaspectratio = val;
    };
    float GetPerspAspectRatio() const
    {
        return this->perspaspectratio;
    };
private:
    float perspaspectratio;
public:
    void SetOrthoWidth(float val)
    {
        n_assert(!this->handled);
        this->orthowidth = val;
    };
    float GetOrthoWidth() const
    {
        return this->orthowidth;
    };
private:
    float orthowidth;
public:
    void SetOrthoHeight(float val)
    {
        n_assert(!this->handled);
        this->orthoheight = val;
    };
    float GetOrthoHeight() const
    {
        return this->orthoheight;
    };
private:
    float orthoheight;
public:
    void SetNearPlane(float val)
    {
        n_assert(!this->handled);
        this->nearplane = val;
    };
    float GetNearPlane() const
    {
        return this->nearplane;
    };
private:
    float nearplane;
public:
    void SetFarPlane(float val)
    {
        n_assert(!this->handled);
        this->farplane = val;
    };
    float GetFarPlane() const
    {
        return this->farplane;
    };
private:
    float farplane;
};
//------------------------------------------------------------------------------
class CreateGlobalLightEntity : public CreateGraphicsEntity
{
    DeclareClass(CreateGlobalLightEntity);
    DeclareMsgId;
public:
    CreateGlobalLightEntity() 
    { };
public:
    void SetLightColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->lightcolor = val;
    };
    const Math::float4& GetLightColor() const
    {
        return this->lightcolor;
    };
private:
    Math::float4 lightcolor;
public:
    void SetCastShadows(bool val)
    {
        n_assert(!this->handled);
        this->castshadows = val;
    };
    bool GetCastShadows() const
    {
        return this->castshadows;
    };
private:
    bool castshadows;
public:
    void SetBackLightColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->backlightcolor = val;
    };
    const Math::float4& GetBackLightColor() const
    {
        return this->backlightcolor;
    };
private:
    Math::float4 backlightcolor;
};
//------------------------------------------------------------------------------
class CreateSpotLightEntity : public CreateGraphicsEntity
{
    DeclareClass(CreateSpotLightEntity);
    DeclareMsgId;
public:
    CreateSpotLightEntity() 
    { };
public:
    void SetVisible(bool val)
    {
        n_assert(!this->handled);
        this->visible = val;
    };
    bool GetVisible() const
    {
        return this->visible;
    };
private:
    bool visible;
public:
    void SetLightColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->lightcolor = val;
    };
    const Math::float4& GetLightColor() const
    {
        return this->lightcolor;
    };
private:
    Math::float4 lightcolor;
public:
    void SetCastShadows(bool val)
    {
        n_assert(!this->handled);
        this->castshadows = val;
    };
    bool GetCastShadows() const
    {
        return this->castshadows;
    };
private:
    bool castshadows;
};
//------------------------------------------------------------------------------
class GraphicsEntityMsg : public Messaging::Message
{
    DeclareClass(GraphicsEntityMsg);
    DeclareMsgId;
public:
    GraphicsEntityMsg() 
    { };
public:
    void SetEntityHandle(const AsyncGraphics::Handle& val)
    {
        n_assert(!this->handled);
        this->entityhandle = val;
    };
    const AsyncGraphics::Handle& GetEntityHandle() const
    {
        return this->entityhandle;
    };
private:
    AsyncGraphics::Handle entityhandle;
};
//------------------------------------------------------------------------------
class UpdateTransform : public GraphicsEntityMsg
{
    DeclareClass(UpdateTransform);
    DeclareMsgId;
public:
    UpdateTransform() 
    { };
public:
    void SetTransform(const Math::matrix44& val)
    {
        n_assert(!this->handled);
        this->transform = val;
    };
    const Math::matrix44& GetTransform() const
    {
        return this->transform;
    };
private:
    Math::matrix44 transform;
};
//------------------------------------------------------------------------------
class SetVisibility : public GraphicsEntityMsg
{
    DeclareClass(SetVisibility);
    DeclareMsgId;
public:
    SetVisibility() 
    { };
public:
    void SetVisible(bool val)
    {
        n_assert(!this->handled);
        this->visible = val;
    };
    bool GetVisible() const
    {
        return this->visible;
    };
private:
    bool visible;
};
} // namespace AsyncGraphics
//------------------------------------------------------------------------------
