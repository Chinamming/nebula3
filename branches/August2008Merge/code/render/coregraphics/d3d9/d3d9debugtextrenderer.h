#pragma once
#ifndef DIRECT3D9_D3D9DEBUGTEXTRENDERER_H
#define DIRECT3D9_D3D9DEBUGTEXTRENDERER_H
//------------------------------------------------------------------------------
/**
    @class Direct3D9::D3D9DebugTextRenderer
  
    Implements a debugtextrenderer for Direct3D9.
    
    (C) 2007 Radon Labs GmbH
*/    
#include "core/refcounted.h"
#include "core/singleton.h"
#include "util/string.h"
#include "math/float4.h"
#include "math/float2.h"
#include <d3dx9.h>

namespace Direct3D9
{
    class D3D9RenderDeviceBase;
}

//------------------------------------------------------------------------------
namespace Direct3D9
{
class D3D9DebugTextRenderer : public Core::RefCounted
{
    DeclareClass(D3D9DebugTextRenderer);
    DeclareSingleton(D3D9DebugTextRenderer);
public:
    /// constructor
    D3D9DebugTextRenderer();
    /// destructor
    virtual ~D3D9DebugTextRenderer();

    /// open the device
    bool Open();
    /// close the device
    void Close();
    /// check if open
    bool IsOpen() const;

    /// draw text with color at 2d position
    void DrawText(const Util::String& str, const Math::float4& color, const Math::float2& pos);
    /// draw text with color at 2d position immediately
    void DrawTextImmediate(const Util::String& str, const Math::float4& color, const Math::float2& pos);
    /// draw text with color at 3d position
    void DrawTextIn3D(const Util::String& str, const Math::float4& color, const Math::float4& pos);    

private:
    friend class D3D9RenderDevice;
    /// load font
    bool LoadFont();
    /// unload font
    void UnloadFont();
    /// render text buffer
    void RenderTextBuffer();
    /// is text buffer empty
    bool IsTextBufferEmpty() const;

    struct TextEntry
    {
        Util::String txt;
        Math::float4 color;
        Math::float2 pos;
    };
    Util::Array<TextEntry> textBuffer;
    bool isOpen;
    ID3DXFont* d3dFont;
    ID3DXSprite* d3dSprite;
};

//------------------------------------------------------------------------------
inline bool
D3D9DebugTextRenderer::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
inline bool
D3D9DebugTextRenderer::IsTextBufferEmpty() const
{
    return this->textBuffer.Size() == 0;
}

} // namespace Direct3D9
//------------------------------------------------------------------------------
#endif

