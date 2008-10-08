#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "graphics/modelentity.h"
#include "util/variant.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
//------------------------------------------------------------------------------
class CameraDistance : public Messaging::Message
{
    __DeclareClass(CameraDistance);
    DeclareMsgId;
public:
    CameraDistance() :
        relativedistancechange(0.0f)
    { };
public:
    void SetRelativeDistanceChange(float val)
    {
        n_assert(!this->handled);
        this->relativedistancechange = val;
    };
    float GetRelativeDistanceChange() const
    {
        return this->relativedistancechange;
    };
private:
    float relativedistancechange;
};
//------------------------------------------------------------------------------
class CameraFocus : public Messaging::Message
{
    __DeclareClass(CameraFocus);
    DeclareMsgId;
public:
    CameraFocus() 
    { };
public:
    void SetObtainFocus(bool val)
    {
        n_assert(!this->handled);
        this->obtainfocus = val;
    };
    bool GetObtainFocus() const
    {
        return this->obtainfocus;
    };
private:
    bool obtainfocus;
};
//------------------------------------------------------------------------------
class CameraOrbit : public Messaging::Message
{
    __DeclareClass(CameraOrbit);
    DeclareMsgId;
public:
    CameraOrbit() :
        horizontalrotation(0.0f),
        verticalrotation(0.0f)
    { };
public:
    void SetHorizontalRotation(float val)
    {
        n_assert(!this->handled);
        this->horizontalrotation = val;
    };
    float GetHorizontalRotation() const
    {
        return this->horizontalrotation;
    };
private:
    float horizontalrotation;
public:
    void SetVerticalRotation(float val)
    {
        n_assert(!this->handled);
        this->verticalrotation = val;
    };
    float GetVerticalRotation() const
    {
        return this->verticalrotation;
    };
private:
    float verticalrotation;
};
//------------------------------------------------------------------------------
class CameraReset : public Messaging::Message
{
    __DeclareClass(CameraReset);
    DeclareMsgId;
public:
    CameraReset() 
    { };
};
//------------------------------------------------------------------------------
class GetGraphicsEntities : public Messaging::Message
{
    __DeclareClass(GetGraphicsEntities);
    DeclareMsgId;
public:
    GetGraphicsEntities() 
    { };
public:
    void SetEntities(const Util::Array<Ptr<Graphics::ModelEntity> >& val)
    {
        n_assert(!this->handled);
        this->entities = val;
    };
    const Util::Array<Ptr<Graphics::ModelEntity> >& GetEntities() const
    {
        return this->entities;
    };
private:
    Util::Array<Ptr<Graphics::ModelEntity> > entities;
public:
    void SetEntity(const Ptr<Graphics::ModelEntity>& val)
    {
        n_assert(!this->handled);
        this->entity = val;
    };
    const Ptr<Graphics::ModelEntity>& GetEntity() const
    {
        return this->entity;
    };
private:
    Ptr<Graphics::ModelEntity> entity;
};
//------------------------------------------------------------------------------
class InputFocus : public Messaging::Message
{
    __DeclareClass(InputFocus);
    DeclareMsgId;
public:
    InputFocus() :
        obtainfocus(true)
    { };
public:
    void SetObtainFocus(bool val)
    {
        n_assert(!this->handled);
        this->obtainfocus = val;
    };
    bool GetObtainFocus() const
    {
        return this->obtainfocus;
    };
private:
    bool obtainfocus;
};
//------------------------------------------------------------------------------
class SetVisibleMsg : public Messaging::Message
{
    __DeclareClass(SetVisibleMsg);
    DeclareMsgId;
public:
    SetVisibleMsg() 
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
//------------------------------------------------------------------------------
class SetOverwriteColor : public Messaging::Message
{
    __DeclareClass(SetOverwriteColor);
    DeclareMsgId;
public:
    SetOverwriteColor() 
    { };
public:
    void SetColor(const Math::float4& val)
    {
        n_assert(!this->handled);
        this->color = val;
    };
    const Math::float4& GetColor() const
    {
        return this->color;
    };
private:
    Math::float4 color;
public:
    void SetNodeName(const Util::String& val)
    {
        n_assert(!this->handled);
        this->nodename = val;
    };
    const Util::String& GetNodeName() const
    {
        return this->nodename;
    };
private:
    Util::String nodename;
};
//------------------------------------------------------------------------------
class SetShaderVariable : public Messaging::Message
{
    __DeclareClass(SetShaderVariable);
    DeclareMsgId;
public:
    SetShaderVariable() 
    { };
public:
    void SetShaderVarName(const Util::String& val)
    {
        n_assert(!this->handled);
        this->shadervarname = val;
    };
    const Util::String& GetShaderVarName() const
    {
        return this->shadervarname;
    };
private:
    Util::String shadervarname;
public:
    void SetValue(const Util::Variant& val)
    {
        n_assert(!this->handled);
        this->value = val;
    };
    const Util::Variant& GetValue() const
    {
        return this->value;
    };
private:
    Util::Variant value;
public:
    void SetNodeName(const Util::String& val)
    {
        n_assert(!this->handled);
        this->nodename = val;
    };
    const Util::String& GetNodeName() const
    {
        return this->nodename;
    };
private:
    Util::String nodename;
};
} // namespace GraphicsFeature
//------------------------------------------------------------------------------
