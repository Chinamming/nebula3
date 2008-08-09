#pragma once
#ifndef N_NEBULA3_HOST_WINDOW_H
#define N_NEBULA3_HOST_WINDOW_H
//------------------------------------------------------------------------------
//  (c) 2008 Vadim Macagon
//  Contents are licensed under the terms of the Nebula License.
//------------------------------------------------------------------------------
#include "stdafx.h"

//------------------------------------------------------------------------------
/**
    @class CHostWindow
    @brief The main window of the ActiveX control.
    
    This is the parent window within which the Nebula3 window will be embedded.
*/
class CHostWindow : public CWindowImpl<CHostWindow>
{
public:
    CHostWindow();
    virtual ~CHostWindow();

    DECLARE_WND_CLASS(_T("Nebula3:ActiveXHostWindow"))

    BEGIN_MSG_MAP(CHostWindow)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_MOUSEACTIVATE, OnMouseActivate);
    END_MSG_MAP()

public:
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
    bool OpenNebula(int width, int height);
    bool IsNebulaOpen();

    bool isNebulaOpen;
    PROCESS_INFORMATION pi;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
CHostWindow::IsNebulaOpen()
{
    return this->isNebulaOpen;
}

#endif // N_NEBULA3_HOST_WINDOW_H
