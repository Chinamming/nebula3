#pragma once
//------------------------------------------------------------------------------
/**
    @class Core::CoreServer
    
    The central core server object initializes a minimal Nebula3 runtime
    environment necessary to boot up the rest of Nebula3. It should be the
    first object a Nebula3 application creates, and the last to destroy
    before shutdown.
    
    (C) 2006 Radon Labs GmbH
*/    
#include "core/ptr.h"
#include "core/refcounted.h"
#include "core/singleton.h"
#include "io/console.h"

//------------------------------------------------------------------------------
namespace Core
{
class CoreServer : public RefCounted
{
    __DeclareClass(CoreServer);
    __DeclareSingleton(CoreServer);
public:
    /// constructor
    CoreServer();
    /// destructor
    virtual ~CoreServer();
    /// set the company name
    void SetCompanyName(const Util::String& s);
    /// get the company name
    const Util::String& GetCompanyName() const;
    /// set the application name
    void SetAppName(const Util::String& s);
    /// get the application name
    const Util::String& GetAppName() const;
    /// open the core server
    void Open();
    /// close the core server
    void Close();
    /// return true if currently open
    bool IsOpen() const;
    /// trigger core server, updates console
    void Trigger();

private:
    Ptr<IO::Console> con;
    Util::String companyName;
    Util::String appName;
    bool isOpen;
};

//------------------------------------------------------------------------------
/*
*/
inline bool
CoreServer::IsOpen() const
{
    return this->isOpen;
}

//------------------------------------------------------------------------------
/*
*/
inline void
CoreServer::SetCompanyName(const Util::String& s)
{
    this->companyName = s;
}

//------------------------------------------------------------------------------
/*
*/
inline const Util::String&
CoreServer::GetCompanyName() const
{
    return this->companyName;
}

//------------------------------------------------------------------------------
/*
*/
inline void
CoreServer::SetAppName(const Util::String& s)
{
    this->appName = s;
}

//------------------------------------------------------------------------------
/*
*/
inline const Util::String&
CoreServer::GetAppName() const
{
    return this->appName;
}

} // namespace Core
//------------------------------------------------------------------------------
