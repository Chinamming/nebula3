//------------------------------------------------------------------------------
//  (c) 2008 Vadim Macagon
//  Contents are licensed under the terms of the Nebula License.
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "resource.h"
#include "neb3ac.h"

class Cneb3acModule : public CAtlDllModuleT< Cneb3acModule >
{
public :
    DECLARE_LIBID(LIBID_neb3acLib)
    DECLARE_REGISTRY_APPID_RESOURCEID(IDR_NEB3AC, "{D1617309-AAD1-49B6-81FE-E67347F0BCC9}")
};

Cneb3acModule _AtlModule;

#ifdef _MANAGED
#pragma managed(push, off)
#endif

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    // Because Internet Explorer makes extensive use of multi-threading, 
    // frequent DLL_THREAD_ATTACH and DLL_THREAD_DETACH notifications to 
    // DllMain can slow the overall performance of the extension and the 
    // browser process. Since this BHO does not require thread-level tracking, 
    // we can call DisableThreadLibraryCalls during the DLL_PROCESS_ATTACH 
    // notification to avoid the overhead of new thread notifications.
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hInstance);
    }
    return _AtlModule.DllMain(dwReason, lpReserved); 
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

// Used to determine whether the DLL can be unloaded by OLE
STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}

// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}

// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    HRESULT hr = _AtlModule.DllRegisterServer();
    return hr;
}

// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
    HRESULT hr = _AtlModule.DllUnregisterServer();
    return hr;
}
