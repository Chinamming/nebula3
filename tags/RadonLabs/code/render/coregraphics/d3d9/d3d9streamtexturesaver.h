#pragma once
#ifndef COREGRAPHICS_D3D9STREAMTEXTURESAVER_H
#define COREGRAPHICS_D3D9STREAMTEXTURESAVER_H
//------------------------------------------------------------------------------
/**
    @class Direct3D9::D3D9StreamTextureSaver
    
    D3D9 implementation of StreamTextureSaver.
    
    (C) 2007 Radon Labs GmbH
*/
#include "coregraphics/base/streamtexturesaverbase.h"

//------------------------------------------------------------------------------
namespace Direct3D9
{
class D3D9StreamTextureSaver : public Base::StreamTextureSaverBase
{
    DeclareClass(D3D9StreamTextureSaver);
public:
    /// called by resource when a save is requested
    virtual bool OnSave();
};

} // Direct3D9
//------------------------------------------------------------------------------
#endif
    
    