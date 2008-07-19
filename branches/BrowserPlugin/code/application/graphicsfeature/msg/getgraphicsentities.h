#ifndef MSG_GETGRAPHICSENTITIES_H
#define MSG_GETGRAPHICSENTITIES_H
//------------------------------------------------------------------------------
/**
    @class GraphicsFeature::GetGraphicsEntities

    Returns the graphics entities used by a game entity for rendering.

    (C) 2007 Radon Labs GmbH
*/
#include "messaging/message.h"
#include "graphics/modelentity.h"

//------------------------------------------------------------------------------
namespace GraphicsFeature
{
class GetGraphicsEntities : public Messaging::Message
{
    DeclareClass(GetGraphicsEntities);
    DeclareMsgId;
public:
    /// set graphics entities array
    void SetEntities(const Util::Array<Ptr<Graphics::ModelEntity> >& e);
    /// set a single graphics entity
    void SetEntity(const Ptr<Graphics::ModelEntity>& e);
    /// get graphics entities array
    const Util::Array<Ptr<Graphics::ModelEntity> >& GetEntities() const;

private:
    Util::Array<Ptr<Graphics::ModelEntity> > entities;
};

//------------------------------------------------------------------------------
/**
*/
inline void
GetGraphicsEntities::SetEntities(const Util::Array<Ptr<Graphics::ModelEntity> >& e)
{
    this->entities = e;
}

//------------------------------------------------------------------------------
/**
*/
inline void
GetGraphicsEntities::SetEntity(const Ptr<Graphics::ModelEntity>& e)
{
    n_assert(0 != e);
    this->entities.Clear();
    this->entities.Append(e);
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<Graphics::ModelEntity> >&
GetGraphicsEntities::GetEntities() const
{
    return this->entities;
}

} // namespace GraphicsFeature
//------------------------------------------------------------------------------
#endif

