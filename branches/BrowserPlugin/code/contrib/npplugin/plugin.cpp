//------------------------------------------------------------------------------
//  (c) 2008 Vadim Macagon
//  Contents are licensed under the terms of the Nebula License.
//------------------------------------------------------------------------------
#include "plugin.h"
#include <string.h>

//------------------------------------------------------------------------------
/**
    Called by browser prior to creating the first plugin instance,
    one off initialization of any data that should be shared between
    all plugin instances should happen here.
*/
NPError NS_PluginInitialize()
{
    return NPERR_NO_ERROR;
}

//------------------------------------------------------------------------------
/**
    Called by browser after the last plugin instance is destroyed,
    cleanup any data setup in NS_PluginInitialize() here.
*/
void NS_PluginShutdown()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Called by browser to create a new plugin instance.
*/
NPPluginBase* NS_NewPluginInstance(NPPluginCreateData* aCreateDataStruct)
{
    if (!aCreateDataStruct)
        return NULL;

    NPPlugin* plugin = new NPPlugin(aCreateDataStruct->instance);
    return plugin;
}

//------------------------------------------------------------------------------
/**
    Called by browser to destroy the given plugin instance
*/
void NS_DestroyPluginInstance(NPPluginBase * aPlugin)
{
    if (aPlugin)
        delete (NPPlugin *)aPlugin;
}

//------------------------------------------------------------------------------
/**
*/
NPPlugin::NPPlugin(NPP aInstance) : NPPluginBase(),
instance(aInstance),
isOpen(false),
isNebulaOpen(false),
hWnd(0),
lpOldProc(0)
{
    ZeroMemory(&this->pi, sizeof(this->pi));
}

//------------------------------------------------------------------------------
/**
*/
NPPlugin::~NPPlugin()
{
    // empty
}

static LRESULT CALLBACK PluginWinProc(HWND, UINT, WPARAM, LPARAM);

//------------------------------------------------------------------------------
/**
*/
bool NPPlugin::OpenNebula(int width, int height)
{
    // get the full path to the plugin dll
    char modulePath[MAX_PATH] = { 0 };
    HMODULE hModule = 0;
    if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCSTR)PluginWinProc, &hModule))
    {
        GetModuleFileName(hModule, modulePath, MAX_PATH);
        FreeLibrary(hModule);
    }

    if (!modulePath[0])
        return FALSE;

    // strip the filename off the end
    char* lastSlash = strrchr(modulePath, '\\');
    if (lastSlash)
        *lastSlash = 0;

    // construct full path to Nebula3 app that the plugin should execute
    const char* appName = "testviewer.exe";
    char appPath[MAX_PATH] = { 0 };
    strcpy_s(appPath, sizeof(appPath), modulePath);
    strcat_s(appPath, sizeof(appPath), "\\");
    strcat_s(appPath, sizeof(appPath), appName);

    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    int x = 0;
    int y = 0;
    char args[256];
    sprintf_s(args, sizeof(args), "\"%s\" -parentwnd=%I64u -x=%d -y=%d -w=%d -h=%d", 
              appName, (unsigned __int64)this->hWnd, x, y, width, height);

    this->isNebulaOpen = CreateProcess(
        appPath, // application to launch
        args, // command line
        NULL, // process attributes
        NULL, // thread attributes
        FALSE, // don't inherit handles from parent
        NORMAL_PRIORITY_CLASS, // priority & other creation flags
        NULL, // use parent's environment block
        modulePath, // set the current directory for the new process to the plugin directory
        &si,
        &this->pi);
    return this->isNebulaOpen;
}

//------------------------------------------------------------------------------
/**
*/
NPBool NPPlugin::Open(NPWindow* aWindow)
{
    if (!aWindow)
        return FALSE;

    this->hWnd = (HWND)aWindow->window;
    if (!this->hWnd)
        return FALSE;

    // if window size is unknown wait until we find out what it is before
    // creating the Nebula window
    if (aWindow->width && aWindow->height)
        this->isOpen = this->OpenNebula(aWindow->width, aWindow->height);
    else
        this->isOpen = true;

    // subclass window so we can do evil things
    this->lpOldProc = SetWindowLongPtr(this->hWnd, GWLP_WNDPROC, (LONG_PTR)PluginWinProc);

    // associate window with our plugin instance so we can access 
    // it in the winproc
    SetWindowLongPtr(this->hWnd, GWL_USERDATA, (LONG_PTR)this);

    // make sure the window doesn't attempt to draw in the area occupied by the 
    // child Nebula3 window, otherwise we'll get flickering when scrolling
    LONG lStyle = GetWindowLong(this->hWnd, GWL_STYLE);
    SetWindowLong(this->hWnd, GWL_STYLE, lStyle | WS_CLIPCHILDREN);

    return this->isOpen;
}

// Nebula3's main window class
#define NEBULA3_WINDOW_CLASS "Nebula3::MainWindow"

//------------------------------------------------------------------------------
/**
*/
void NPPlugin::Close()
{
    if (this->isNebulaOpen)
    {
        HWND hWnd = FindWindowEx(this->hWnd, NULL, NEBULA3_WINDOW_CLASS, NULL);
        if (hWnd)
        {
            // ask nebula politely to shutdown
            SendMessage(hWnd, WM_CLOSE, 0, 0);
        }
        else
        {
            // can't find the nebula window handle so forcefully terminate 
            // the child process
            TerminateProcess(this->pi.hProcess, -1);
        }

        // wait until child process terminates
        WaitForSingleObject(this->pi.hProcess, INFINITE);

        if (this->pi.hProcess)
            CloseHandle(this->pi.hProcess);
        if (this->pi.hThread)
            CloseHandle(this->pi.hThread);

        this->isNebulaOpen = false;
    }
    // subclass it back
    SetWindowLongPtr(this->hWnd, GWLP_WNDPROC, this->lpOldProc);
    this->hWnd = NULL;
    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
*/
NPBool NPPlugin::IsOpen()
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/**
*/
bool NPPlugin::IsNebulaOpen()
{
    return this->isNebulaOpen;  
}

//------------------------------------------------------------------------------
/**
*/
const char* NPPlugin::GetVersion()
{
    return NPN_UserAgent(this->instance);
}

//------------------------------------------------------------------------------
/**
*/
NPError NPPlugin::SetWindow(NPWindow* aWindow)
{
    // Create the Nebula window as soon as we find out what the size of the
    // parent window is!
    if (!this->isNebulaOpen)
        if (aWindow->width && aWindow->height)
            this->OpenNebula(aWindow->width, aWindow->height);
    return NPERR_NO_ERROR;
}

//------------------------------------------------------------------------------
/**
*/
static LRESULT CALLBACK PluginWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    NPPlugin* plugin = (NPPlugin*)GetWindowLongPtr(hWnd, GWL_USERDATA);
    if (plugin)
    {
        switch (msg)
        {   
            case WM_MOUSEACTIVATE:
            {
                // This is here to work around an issue where minimizing the browser 
                // window, restoring it, and then clicking on the Nebula window doesn't
                // set the input focus back to the Nebula window.
                //
                // Not sure if this should be handled here or in
                // Win32DisplayDevice::WinProc, but I'll leave it here for now to
                // minimize changes to core.
                if (plugin->IsNebulaOpen())
                {
                    HWND hNebulaWnd = FindWindowEx(hWnd, NULL, NEBULA3_WINDOW_CLASS, NULL);
                    if (hNebulaWnd)
                        SetFocus(hNebulaWnd);
                }
                break;
            }
        }
        return CallWindowProc((WNDPROC)plugin->GetOldWndProc(), hWnd, msg, wParam, lParam);
    }
    else
    {
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}
