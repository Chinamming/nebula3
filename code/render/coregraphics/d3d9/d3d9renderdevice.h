#pragma once
#ifndef DIRECT3D9_D3D9RENDERDEVICE_H
#define DIRECT3D9_D3D9RENDERDEVICE_H
//------------------------------------------------------------------------------
/**
    @class Direct3D9::D3D9RenderDevice
  
    Implements a RenderDevice on top of Direct3D9.
    
    (C) 2007 Radon Labs GmbH
*/    
#include "coregraphics/base/renderdevicebase.h"
#include "coregraphics/pixelformat.h"
#include "coregraphics/imagefileformat.h"

//------------------------------------------------------------------------------
namespace Direct3D9
{
class D3D9RenderDevice : public Base::RenderDeviceBase
{
    __DeclareClass(D3D9RenderDevice);
    __DeclareSingleton(D3D9RenderDevice);
public:
    /// constructor
    D3D9RenderDevice();
    /// destructor
    virtual ~D3D9RenderDevice();

    /// test if a compatible render device can be created on this machine
    static bool CanCreate();
    /// get pointer to Direct3D interface, opens Direct3D if not happened yet
    static IDirect3D9* GetDirect3D();
    /// get pointer to the d3d device
    IDirect3DDevice9* GetDirect3DDevice() const;

    /// open the device
    bool Open();
    /// close the device
    void Close();

    /// begin complete frame
    bool BeginFrame();
    /// set current vertex buffer
    void SetVertexBuffer(const Ptr<CoreGraphics::VertexBuffer>& vb);
    /// set current index buffer
    void SetIndexBuffer(const Ptr<CoreGraphics::IndexBuffer>& ib);
    /// draw current primitives
    void Draw();
    /// end current pass
    void EndPass();
    /// end complete frame
    void EndFrame();
    /// present the rendered scene
    void Present();
    /// save a screenshot to the provided stream
    void SaveScreenshot(CoreGraphics::ImageFileFormat::Code fmt, const Ptr<IO::Stream>& outStream);

private:
    /// open Direct3D
    static void OpenDirect3D();
    /// close Direct3D
    void CloseDirect3D();
    /// open the Direct3D device
    bool OpenDirect3DDevice();
    /// close the Direct3D device
    void CloseDirect3DDevice();
    /// setup the requested adapter for the Direct3D device
    void SetupAdapter();
    /// select the requested buffer formats for the Direct3D device
    void SetupBufferFormats();
    /// setup the device behaviour flags
    void SetupDeviceBehaviour();
    /// setup the remaining presentation parameters
    void SetupPresentParams();
    /// set the initial Direct3D device state
    void SetInitialDeviceState();
    /// test for and handle lost device 
    bool TestResetDevice();
    /// unbind D3D resources in the device
    void UnbindD3D9Resources();

    static IDirect3D9* d3d9;
    IDirect3DDevice9* d3d9Device;
    D3DCAPS9 d3d9DeviceCaps;
    D3DPRESENT_PARAMETERS presentParams;
    UINT adapter;
    D3DFORMAT displayFormat;
    DWORD deviceBehaviourFlags;
};

} // namespace Direct3D9
//------------------------------------------------------------------------------
#endif

