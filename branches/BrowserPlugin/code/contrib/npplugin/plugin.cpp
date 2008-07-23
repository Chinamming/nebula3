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
hWnd(0)
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
static WNDPROC lpOldProc = NULL;

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
    int w = 0;
    int h = 0;
    RECT rc;
    if (GetClientRect(this->hWnd, &rc))
    {
        x = rc.left;
        y = rc.top;
        w = rc.right - rc.left;
        h = rc.bottom - rc.top;
    }
    // keep getting a (0,0,0,0) rc for some reason
    if (0 == w)
        w = 760;
    if (0 == h)
        h = 510;
    char args[256];
    sprintf_s(args, sizeof(args), "\"%s\" -parentwnd=%I64u -x=%d -y=%d -w=%d -h=%d", 
              appName, (unsigned __int64)this->hWnd, x, y, w, h);

    OutputDebugString("Launching Process...\n");
    OutputDebugString(args);
    OutputDebugString("\n");

    this->isOpen = CreateProcess(
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

    // subclass window so we can forward them to the nebula window
    lpOldProc = SubclassWindow(this->hWnd, (WNDPROC)PluginWinProc);

    // associate window with our plugin instance so we can access 
    // it in the winproc
    SetWindowLong(this->hWnd, GWL_USERDATA, (LONG)this);

    return this->isOpen;
}

// Nebula3's main window class
#define NEBULA3_WINDOW_CLASS "Nebula3::MainWindow"

//------------------------------------------------------------------------------
/**
*/
void NPPlugin::Close()
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
    // subclass it back
    SubclassWindow(this->hWnd, lpOldProc);
    this->hWnd = NULL;
    this->isOpen = FALSE;
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
const char* NPPlugin::GetVersion()
{
    return NPN_UserAgent(this->instance);
}

//------------------------------------------------------------------------------
/**
*/
static LRESULT CALLBACK PluginWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    /*
    switch (msg)
    {
        case WM_PAINT:
        {
            // draw a frame and display the version string
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            RECT rc;
            GetClientRect(hWnd, &rc);
            FrameRect(hdc, &rc, GetStockBrush(BLACK_BRUSH));

            // get our plugin instance object and ask it for the version string
            NPPlugin *plugin = (NPPlugin*)GetWindowLong(hWnd, GWL_USERDATA);
            if (plugin) 
            {
                const char * string = plugin->GetVersion();
                DrawText(hdc, string, strlen(string), &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            }
            else 
            {
                char string[] = "Error occured";
                DrawText(hdc, string, strlen(string), &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            }

            EndPaint(hWnd, &ps);
        }
        break;
        
        default:
        break;
    }
    */

    HWND hNebulaWnd = FindWindowEx(hWnd, NULL, NEBULA3_WINDOW_CLASS, NULL);
    if (hNebulaWnd)
    {
        SendMessage(hNebulaWnd, msg, wParam, lParam);
    }
    
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
