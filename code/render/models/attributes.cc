//------------------------------------------------------------------------------
//  models/attributes.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/attributes.h"

namespace Attr
{
    DefineFloat4(BoxCenter, 'BOXC', ReadWrite);
    DefineFloat4(BoxExtents, 'BOXE', ReadWrite);

    // Models::TransformNode
    DefineFloat4(Position, 'POSN', ReadWrite);
    DefineFloat4(Rotate, 'ROTA', ReadWrite);
    DefineFloat4(Scale, 'SCAL', ReadWrite);
    DefineFloat4(RotatePivot, 'RPVT', ReadWrite);
    DefineFloat4(ScalePivot, 'SPVT', ReadWrite);

    // Models::StateNode
    DefineString(Shader, 'SHDR', ReadWrite);

    // Models::ShapeNode
    DefineString(MeshResourceId, 'MSRI', ReadWrite);
    DefineInt(MeshGroupIndex, 'MSGI', ReadWrite);

    // legacy Nebula2 shader params
    DefineString(DiffMap0, 'DIM0', ReadWrite);
    DefineString(EmsvMap0, 'EVM0', ReadWrite);
    DefineString(SpecMap0, 'SCM0', ReadWrite);
    DefineString(BumpMap0, 'BMM0', ReadWrite);
    DefineInt(CullMode, 'CULM', ReadWrite);
    DefineFloat(MatEmissiveIntensity, 'MESI', ReadWrite);
    DefineFloat(MatSpecularIntensity, 'MSCI', ReadWrite);
    DefineFloat(MatSpecularPower, 'MSCP', ReadWrite);
    DefineFloat(BumpScale, 'BMSC', ReadWrite);

    // Models::CharacterNode
    DefineString(AnimFilename, 'ANFN', ReadWrite);
    DefineBlob(JointIndices, 'JIND', ReadWrite);
    DefineBlob(JointFragments, 'JFRA', ReadWrite);
    DefineBlob(Clips, 'CLPS', ReadWrite);

	//Models::ParticleSystem

}
