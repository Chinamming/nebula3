#ifndef N_NP_PLUGIN_H
#define N_NP_PLUGIN_H
//------------------------------------------------------------------------------
//  (c) 2008 Vadim Macagon
//  Contents are licensed under the terms of the Nebula License.
//------------------------------------------------------------------------------
#include "pluginbase.h"

class NPPlugin : public NPPluginBase
{
public:
    NPPlugin(NPP aInstance);
    ~NPPlugin();

    NPBool Open(NPWindow* aWindow);
    void Close();
    NPBool IsOpen();

    LONG_PTR GetOldWndProc();
    const char* GetVersion();

private:
    NPP instance;
    bool isOpen;
    HWND hWnd;
    PROCESS_INFORMATION pi;
    LONG_PTR lpOldProc;
};

//------------------------------------------------------------------------------
/**
    Get the original WndProc of the sub-classed window.
*/
inline
LONG_PTR NPPlugin::GetOldWndProc()
{
    return this->lpOldProc;
}

#endif // N_NP_PLUGIN_H
