//------------------------------------------------------------------------------
//  win32inetconnection.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "http/win32/win32inetconnection.h"

namespace Win32
{
ImplementClass(Win32::Win32InetConnection, 'incn', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
Win32InetConnection::Win32InetConnection() :
    agent("Nebula3 Application"),
    hInternet(0),
    isConnected(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Win32InetConnection::~Win32InetConnection()
{
    n_assert(!this->IsConnected());
}

//------------------------------------------------------------------------------
/**
    This "opens the internet", generally speaking. The subclasses call
    this method first to make sure everything is fine for connecting
    to a service.
*/
bool
Win32InetConnection::Connect()
{
    n_assert(!this->IsConnected());
    n_assert(0 == this->hInternet);

    // attempt to connect to the Internet, this should bring up
    // a dialup dialog if no connection can be made...
    bool lineOpen = this->DoDialupStuff();
    if (!lineOpen)
    {
        return false;
    }

    // get a handle to the internet
    this->hInternet = InternetOpen(this->agent.AsCharPtr(),         // lpszAgent
                                   INTERNET_OPEN_TYPE_PRECONFIG,    // dwAccessType
                                   NULL,                            // lpszProxyName
                                   NULL,                            // lpszProxyBypass
                                   0);                              // dwFlags
    if (0 == this->hInternet)
    {
        // error
        this->SetError("InternetOpen() failed!");
        return false;
    }
    this->isConnected = true;
    return true;
}

//------------------------------------------------------------------------------
/**
    This shuts down the connection established with Connect().
*/
void
Win32InetConnection::Disconnect()
{
    n_assert(this->IsConnected());
    if (0 != this->hInternet)
    {
        InternetCloseHandle(this->hInternet);
        this->hInternet = 0;
    }
    this->isConnected = false;
}

//------------------------------------------------------------------------------
/**
    This brings up the dialup dialog to make a connection to the
    internet. @todo is this still required nowadays???
*/
bool
Win32InetConnection::DoDialupStuff()
{
    // check if we are on a modem
    DWORD connectedState;
    BOOL res = InternetGetConnectedState(&connectedState, 0);
    if (res)
    {
        // all ok, we are connected...
        return true;
    }
    else
    {
        // not connected, bring up the dialup dialog
        DWORD connection;
        DWORD dialRes = InternetDial(NULL, NULL, INTERNET_AUTODIAL_FORCE_ONLINE, &connection, 0);
        if (dialRes == ERROR_SUCCESS)
        {
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
    Sets the current error to InternetGetLastResponseInfo().
*/
void
Win32InetConnection::SetErrorToInternetLastResponse()
{
    char buf[1024];
    DWORD bufSize = sizeof(buf);
    DWORD errorCode;
    InternetGetLastResponseInfo(&errorCode, buf, &bufSize);
    this->SetError(buf);
}

} // namespace Win32