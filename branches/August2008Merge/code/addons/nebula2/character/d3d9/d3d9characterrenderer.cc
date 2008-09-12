//------------------------------------------------------------------------------
//  D3D9CharacterRenderer.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "nebula2/character/d3d9/d3d9characterrenderer.h"
#include "models/nodes/skinshapenode.h"
#include "coregraphics/transformdevice.h"
#include "coregraphics/renderdevice.h"
#include "coregraphics/shaderserver.h"

namespace Direct3D9
{
ImplementClass(Direct3D9::D3D9CharacterRenderer, 'WCHR', Base::CharacterRendererBase);

using namespace Math;
using namespace Models;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
D3D9CharacterRenderer::D3D9CharacterRenderer()
{
}

//------------------------------------------------------------------------------
/**
*/
D3D9CharacterRenderer::~D3D9CharacterRenderer()
{
}
    
//------------------------------------------------------------------------------
/**
*/
void 
D3D9CharacterRenderer::RenderSkinning(const Util::FixedArray<Char::CharFragment>& fragmentArray)
{    
    IndexT fragIndex;
    for (fragIndex = 0; fragIndex < fragmentArray.Size(); fragIndex++)
    {
        Char::CharFragment& fragment = fragmentArray[fragIndex];
        this->RenderFragment(fragment.GetMeshGroupIndex(), fragment.GetJointPalette());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9CharacterRenderer::RenderFragment(int primGroupIndex, Char::CharJointPalette& jointPalette)
{
    // @todo: split into platform dependend classes with different palettesizes and number of matrixarrays
    const int maxJointPaletteSize = 72;
    static matrix44 jointArray[maxJointPaletteSize];
   
    // extract the current joint palette from the skeleton in the
    // right format for the skinning shader
    int paletteSize = jointPalette.GetNumJoints();
    n_assert(paletteSize <= maxJointPaletteSize);

    n_assert(this->skinningCharacter.isvalid());
    Char::CharSkeleton& skeleton = this->skinningCharacter->GetSkeleton();
    
    int paletteIndex;
    for (paletteIndex = 0; paletteIndex < paletteSize; paletteIndex++)
    {
        const Char::CharJoint& joint = skeleton.GetJointAt(jointPalette.GetJointIndexAt(paletteIndex));
        
        const matrix44& skinnedMatrix = joint.GetSkinMatrix44();            
        jointArray[paletteIndex] = skinnedMatrix;
    }
    
    // transfer the joint palette to the current shader            
    n_assert(this->paletteShdVar.isvalid());
    this->paletteShdVar->SetMatrixArray(jointArray, paletteSize);    

    // commit shader variable changes
    ShaderServer* shdServer = CoreGraphics::ShaderServer::Instance();
    shdServer->GetActiveShaderInstance()->Commit();

    // set current vertex and index range and draw mesh
    RenderDevice* renderDevice = RenderDevice::Instance();            
    renderDevice->SetPrimitiveGroup(this->skinningMesh->GetPrimitiveGroupAtIndex(primGroupIndex));
    renderDevice->Draw();
}

} // namespace Direct3D9
