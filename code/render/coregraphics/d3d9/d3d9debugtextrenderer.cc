//------------------------------------------------------------------------------
//  d3d9debugtextrenderer.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/d3d9/d3d9debugtextrenderer.h"
#include "coregraphics/d3d9/d3d9renderdevice.h"
#include "coregraphics/displaydevice.h"

namespace Direct3D9
{
ImplementClass(Direct3D9::D3D9DebugTextRenderer, 'D9TR', Core::RefCounted);
ImplementSingleton(Direct3D9::D3D9DebugTextRenderer);

//------------------------------------------------------------------------------
/**
*/
D3D9DebugTextRenderer::D3D9DebugTextRenderer():
    isOpen(false),
    d3dFont(0),
    d3dSprite(0)
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
D3D9DebugTextRenderer::~D3D9DebugTextRenderer()
{
    if (this->IsOpen())
    {
        this->Close();
    }
    DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
bool
D3D9DebugTextRenderer::Open()
{
    n_assert(!this->IsOpen());
    bool success = false;
    if (this->LoadFont())
    {
        this->isOpen = true;
             
        // create sprite object for batched rendering
        HRESULT hr = D3DXCreateSprite(D3D9RenderDevice::Instance()->GetDirect3DDevice(), &this->d3dSprite);
        n_dxtrace(hr, "D3DCreateSprite() failed!");

        success = true;
    }

    return success;
}

//------------------------------------------------------------------------------
/**
    Close the render device. The RenderEvent::DeviceClose will be sent to
    all registered event handlers.
*/
void
D3D9DebugTextRenderer::Close()
{
    n_assert(this->IsOpen());

    this->UnloadFont();

    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
bool
D3D9DebugTextRenderer::LoadFont()
{    
    n_assert(0 == this->d3dFont);

    int fontFlags = 0;
    fontFlags |= FW_NORMAL;
    const int fontHeight = 20;
    
    HRESULT hr = D3DXCreateFont(D3D9RenderDevice::Instance()->GetDirect3DDevice(),
        fontHeight, 0,
        fontFlags, 0,
        false,              // not italic
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        NONANTIALIASED_QUALITY,
        DEFAULT_PITCH|FF_DONTCARE,
        "Arial",            // type face
        &this->d3dFont);

    n_dxtrace(hr, "D3DXCreateFont() failed!");
    n_assert(this->d3dFont);
    
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void 
D3D9DebugTextRenderer::UnloadFont()
{
    // delete d3d font object
    this->d3dFont->Release();
    this->d3dFont = 0;
}

//------------------------------------------------------------------------------
/**
    Draw text immediatly.
*/
void 
D3D9DebugTextRenderer::DrawTextImmediate(const Util::String& str, const Math::float4& color, const Math::float2& pos)
{
    n_assert(this->IsOpen());
    
    DWORD d3dFlags = 0;
    d3dFlags |= DT_TOP;
    d3dFlags |= DT_LEFT;
    d3dFlags |= DT_NOCLIP;
    d3dFlags |= DT_EXPANDTABS;      

    DWORD d3dColor = D3DCOLOR_COLORVALUE(color.x(), color.y(), color.z(), color.w());    

    CoreGraphics::DisplayDevice* displayDevice = CoreGraphics::DisplayDevice::Instance();
    uint dispWidth = displayDevice->GetDisplayMode().GetWidth();
    uint dispHeight = displayDevice->GetDisplayMode().GetHeight();
    // build rectangle
    RECT r;
    r.left   = (LONG) (pos.x() * dispWidth);
    r.top    = (LONG) (pos.y() * dispHeight);
    r.right  = r.left;
    r.bottom = r.top;    
    this->d3dFont->DrawText(this->d3dSprite, str.AsCharPtr(), -1, &r, d3dFlags | DT_CALCRECT, d3dColor);    

    this->d3dSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);      
    this->d3dFont->DrawText(this->d3dSprite, str.AsCharPtr(), -1, &r, d3dFlags, d3dColor);    
    this->d3dSprite->End();
}

//------------------------------------------------------------------------------
/**
    Append new debug text to get rendered on screen at the end of the frame.
*/
void 
D3D9DebugTextRenderer::DrawText(const Util::String& str, const Math::float4& color, const Math::float2& pos)
{
    TextEntry newEntry;
    newEntry.txt = str;
    newEntry.color = color;
    newEntry.pos = pos;
    this->textBuffer.Append(newEntry);
}

//------------------------------------------------------------------------------
/**
    Render all text entries inside our text buffer.
    Is called from the RenderDevice at EndFrame.
*/
void 
D3D9DebugTextRenderer::RenderTextBuffer()
{
    n_assert(this->IsOpen());
    
    DWORD d3dFlags = 0;
    d3dFlags |= DT_TOP;
    d3dFlags |= DT_LEFT;
    d3dFlags |= DT_NOCLIP;
    d3dFlags |= DT_EXPANDTABS; 

    CoreGraphics::DisplayDevice* displayDevice = CoreGraphics::DisplayDevice::Instance();
    uint dispWidth = displayDevice->GetDisplayMode().GetWidth();
    uint dispHeight = displayDevice->GetDisplayMode().GetHeight();

    this->d3dSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);    
    IndexT i;
    for (i = 0; i < this->textBuffer.Size(); i++)
    {        
        const Math::float4& color = this->textBuffer[i].color;
        const Math::float2& pos = this->textBuffer[i].pos;
        const Util::String& str = this->textBuffer[i].txt;

        DWORD d3dColor = D3DCOLOR_COLORVALUE(color.x(), color.y(), color.z(), color.w());   
        // build rectangle
        RECT r;
        r.left   = (LONG) (pos.x() * dispWidth);
        r.top    = (LONG) (pos.y() * dispHeight);
        r.right  = r.left;
        r.bottom = r.top;    
        // resize rect to fit text
        this->d3dFont->DrawText(this->d3dSprite, str.AsCharPtr(), -1, &r, d3dFlags | DT_CALCRECT, d3dColor);    
        // draw
        this->d3dFont->DrawText(this->d3dSprite, str.AsCharPtr(), -1, &r, d3dFlags, d3dColor);            
    }
    this->d3dSprite->End();

    this->textBuffer.Clear();
}
} // namespace CoreGraphics
