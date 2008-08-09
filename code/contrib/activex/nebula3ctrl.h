#pragma once
#ifndef N_NEBULA3_ACTIVEX_CTRL_H
#define N_NEBULA3_ACTIVEX_CTRL_H
//------------------------------------------------------------------------------
//  (c) 2008 Vadim Macagon
//  Contents are licensed under the terms of the Nebula License.
//------------------------------------------------------------------------------
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "neb3ac.h"
#include "hostwindow.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// CNebula3Ctrl
class ATL_NO_VTABLE CNebula3Ctrl :
    public CComObjectRootEx<CComSingleThreadModel>,
    public IDispatchImpl<INebula3Ctrl, &IID_INebula3Ctrl, &LIBID_neb3acLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
    public IPersistStreamInitImpl<CNebula3Ctrl>,
    public IOleControlImpl<CNebula3Ctrl>,
    public IOleObjectImpl<CNebula3Ctrl>,
    public IOleInPlaceActiveObjectImpl<CNebula3Ctrl>,
    public IViewObjectExImpl<CNebula3Ctrl>,
    public IOleInPlaceObjectWindowlessImpl<CNebula3Ctrl>,
    public ISupportErrorInfo,
    public IQuickActivateImpl<CNebula3Ctrl>,
    public IProvideClassInfo2Impl<&CLSID_Nebula3Ctrl, NULL, &LIBID_neb3acLib>,
#ifdef _WIN32_WCE // IObjectSafety is required on Windows CE for the control to be loaded correctly
    public IObjectSafetyImpl<CNebula3Ctrl, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
#endif
    public CComCoClass<CNebula3Ctrl, &CLSID_Nebula3Ctrl>,
    public CComControl<CNebula3Ctrl, CHostWindow>
{
public:
    CNebula3Ctrl()
    {
        m_bWindowOnly = TRUE;
    }

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE |
    OLEMISC_CANTLINKINSIDE |
    OLEMISC_INSIDEOUT |
    OLEMISC_ACTIVATEWHENVISIBLE |
    OLEMISC_SETCLIENTSITEFIRST
)

DECLARE_REGISTRY_RESOURCEID(IDR_NEBULA3CTRL)

DECLARE_NOT_AGGREGATABLE(CNebula3Ctrl)

BEGIN_COM_MAP(CNebula3Ctrl)
    COM_INTERFACE_ENTRY(INebula3Ctrl)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IViewObjectEx)
    COM_INTERFACE_ENTRY(IViewObject2)
    COM_INTERFACE_ENTRY(IViewObject)
    COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
    COM_INTERFACE_ENTRY(IOleInPlaceObject)
    COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
    COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
    COM_INTERFACE_ENTRY(IOleControl)
    COM_INTERFACE_ENTRY(IOleObject)
    COM_INTERFACE_ENTRY(IPersistStreamInit)
    COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
    COM_INTERFACE_ENTRY(IQuickActivate)
    COM_INTERFACE_ENTRY(IProvideClassInfo)
    COM_INTERFACE_ENTRY(IProvideClassInfo2)
#ifdef _WIN32_WCE // IObjectSafety is required on Windows CE for the control to be loaded correctly
    COM_INTERFACE_ENTRY_IID(IID_IObjectSafety, IObjectSafety)
#endif
END_COM_MAP()

BEGIN_PROP_MAP(CNebula3Ctrl)
    PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
    PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
    // Example entries
    // PROP_ENTRY("Property Description", dispid, clsid)
    // PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_MSG_MAP(CHostWindow)
    CHAIN_MSG_MAP(CHostWindow)
    DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// ISupportsErrorInfo
    STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
    {
        static const IID* arr[] =
        {
            &IID_INebula3Ctrl,
        };

        for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
        {
            if (InlineIsEqualGUID(*arr[i], riid))
                return S_OK;
        }
        return S_FALSE;
    }

// IViewObjectEx
    DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// INebula3Ctrl
public:
    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct()
    {
        return S_OK;
    }

    void FinalRelease()
    {
    }
};

OBJECT_ENTRY_AUTO(__uuidof(Nebula3Ctrl), CNebula3Ctrl)

#endif // N_NEBULA3_ACTIVEX_CTRL_H
