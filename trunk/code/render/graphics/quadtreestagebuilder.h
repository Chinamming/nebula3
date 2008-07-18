#pragma once
#ifndef GRAPHICS_QUATREESTAGEBUILDER_H
#define GRAPHICS_QUATREESTAGEBUILDER_H
//------------------------------------------------------------------------------
/**
    @class Graphics::QuadtreeStageBuilder
    
    A quadtree stage builder, which sort cells
    into a quadtree for culling.
    
    (C) 2007 Radon Labs GmbH
*/
#include "graphics/stagebuilder.h"
#include "util/quadtree.h"
#include "attr/attribute.h"

//------------------------------------------------------------------------------
namespace Attr
{
    DeclareInt(QuadTreeStageBuilderDepth, 'QTBD', Attr::ReadOnly);
    DeclareFloat4(QuadTreeStageBuilderCenter, 'QTBC', Attr::ReadOnly);
    DeclareFloat4(QuadTreeStageBuilderExtents, 'QTBE', Attr::ReadOnly);
};
//------------------------------------------------------------------------------
namespace Graphics
{
class Cell;

class QuadtreeStageBuilder : public StageBuilder
{
    DeclareClass(QuadtreeStageBuilder);
public:
    /// constructor
    QuadtreeStageBuilder();
    /// destructor
    virtual ~QuadtreeStageBuilder();
    /// setup the stage
    virtual void BuildStage(const Ptr<Stage>& stage);
    /// set quad tree depth and boudning box
    void SetQuadTreeSettings(uchar depth, const Math::bbox& worldBBox);

private:
    /// create a quad tree and its children, recursively
    Graphics::Cell* CreateQuadTreeCell(Graphics::Cell* parentCell, uchar curLevel, ushort curCol, ushort curRow);

    int numCellsBuilt;
    Util::QuadTree<Cell> quadTree;
};

} // namespace Graphics
//------------------------------------------------------------------------------
#endif
    