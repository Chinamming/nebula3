#pragma once
//------------------------------------------------------------------------------
/**
    This file was generated with Nebula3's idlc compiler tool.
    DO NOT EDIT
*/
#include "messaging/message.h"
#include "graphics/modelentity.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
//------------------------------------------------------------------------------
class CameraDistance : public Messaging::Message
{
    DeclareClass(CameraDistance);
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
    DeclareClass(CameraFocus);
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
    DeclareClass(CameraOrbit);
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
    DeclareClass(CameraReset);
    DeclareMsgId;
public:
    CameraReset() 
    { };
};
//------------------------------------------------------------------------------
class GetGraphicsEntities : public Messaging::Message
{
    DeclareClass(GetGraphicsEntities);
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
    DeclareClass(InputFocus);
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
    DeclareClass(SetVisibleMsg);
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
} // namespace GraphicsFeature
//------------------------------------------------------------------------------
