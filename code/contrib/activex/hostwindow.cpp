//------------------------------------------------------------------------------
//  (c) 2008 Vadim Macagon
//  Contents are licensed under the terms of the Nebula License.
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "hostwindow.h"

// Nebula3's main window class
#define NEBULA3_WINDOW_CLASS "Nebula3::MainWindow"

//------------------------------------------------------------------------------
/**
*/
CHostWindow::CHostWindow() :
isNebulaOpen(false)
{
    ZeroMemory(&this->pi, sizeof(this->pi));
}

//------------------------------------------------------------------------------
/**
*/
CHostWindow::~CHostWindow()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Used as a parameter when calling GetModuleHandleEx().
*/
static void 
DummyProc()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool 
CHostWindow::OpenNebula(int width, int height)
{
    // get the full path to the plugin dll
    char modulePath[MAX_PATH] = { 0 };
    HMODULE hModule = 0;
    if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCSTR)DummyProc, &hModule))
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
              appName, (unsigned __int64)this->m_hWnd, x, y, width, height);

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
LRESULT 
CHostWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    this->SetWindowLong(GWL_STYLE, this->GetWindowLong(GWL_STYLE) | WS_CLIPCHILDREN);

    RECT rc;
    if (GetClientRect(&rc))
        if (rc.right && rc.bottom)
            if (!this->OpenNebula(rc.right - rc.left, rc.bottom - rc.top))
                return -1;
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
LRESULT 
CHostWindow::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if (this->isNebulaOpen)
    {
        HWND hWnd = FindWindowEx(m_hWnd, NULL, NEBULA3_WINDOW_CLASS, NULL);
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
    // let the default handler run
    bHandled = FALSE;
    return 1;
}

//------------------------------------------------------------------------------
/**
*/
LRESULT
CHostWindow::OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if (this->IsNebulaOpen())
    {
        HWND hNebulaWnd = FindWindowEx(m_hWnd, NULL, NEBULA3_WINDOW_CLASS, NULL);
        if (hNebulaWnd)
            ::SetFocus(hNebulaWnd);
    }
    return MA_ACTIVATE;
}
