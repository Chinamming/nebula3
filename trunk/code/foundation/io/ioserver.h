#pragma once
#ifndef IO_IOSERVER_H
#define IO_IOSERVER_H
//------------------------------------------------------------------------------
/**
    @class IO::IoServer
  
    The central server object of the IO subsystem offers the following
    services:

    * associate stream classes with URI schemes
    * create the right stream object for a given URI
    * transparant ZIP archive support
    * path assign management
    * global filesystem manipulation and query methods
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "util/array.h"
#include "util/string.h"
#include "util/hashtable.h"
#include "io/filetime.h"
#include "io/assign.h"

//------------------------------------------------------------------------------
namespace IO
{

#ifndef __NOZIP__
class ZipFileSystem;
#endif

class Stream;
class URI;

class IoServer : public Core::RefCounted
{
    __DeclareClass(IoServer);
    __DeclareSingleton(IoServer);
public:
    /// constructor
    IoServer();
    /// destructor
    virtual ~IoServer();

    /// register standard URI schemes
    void RegisterStandardUriSchemes();
    /// associate an uri scheme with a stream class
    void RegisterUriScheme(const Util::String& uriScheme, const Core::Rtti& classRtti);
    /// unregister an uri scheme
    void UnregisterUriScheme(const Util::String& uriScheme);
    /// return true if an uri scheme has been registered
    bool IsUriSchemeRegistered(const Util::String& uriScheme) const;
    /// get the registered stream class for an uri scheme
    const Core::Rtti& GetStreamClassByUriScheme(const Util::String& uriScheme) const;
    /// get an array of all registered schemes
    Util::Array<Util::String> GetAllRegisteredUriSchemes() const;
    /// create a stream object for the given uri
    Ptr<Stream> CreateStream(const URI& uri) const;

    #ifndef __NOZIP__
    /// mount a zip file archive
    bool MountZipArchive(const URI& uri);
    /// unmount a zip file archive
    void UnmountZipArchive(const URI& uri);
    /// return true if a zip archive is mounted
    bool IsZipArchiveMounted(const URI& uri) const;
    #endif
    /// enable/disable transparent zip filesystem layering (default is yes)
    void SetZipFileSystemEnabled(bool b);
    /// return true if transparent zip filesystem is enabled
    bool IsZipFileSystemEnabled() const;
    /// mount standard zip archives (home:export.zip and home:export_$(platform).zip)
    void MountStandardZipArchives();
    /// unmount standard zip archives
    void UnmountStandardZipArchives();

    /// setup standard assigns
    void SetupStandardAssigns();
    /// define a directory assign
    void SetAssign(const Assign& assign);
    /// return true if an assign exists
    bool HasAssign(const Util::String& assignName) const;
    /// get an assign
    const Util::String& GetAssign(const Util::String& assignName) const;
    /// clear an assign
    void ClearAssign(const Util::String& assignName);
    /// return an array of all currently defined assigns
    Util::Array<Assign> GetAllAssigns() const;
    /// resolve any assigns in an URI
    URI ResolveAssigns(const URI& uri) const;
    /// resolve any assigns in a string (must have URI form)
    Util::String ResolveAssignsInString(const Util::String& uriString) const;

    /// create all missing directories in the path
    bool CreateDirectory(const URI& uri) const;
    /// delete an empty directory
    bool DeleteDirectory(const URI& path) const;
    /// return true if directory exists
    bool DirectoryExists(const URI& path) const;

    /// copy a file
    bool CopyFile(const URI& from, const URI& to) const;
    /// delete a file
    bool DeleteFile(const URI& path) const;
    /// return true if file exists
    bool FileExists(const URI& path) const;
    /// set the readonly status of a file
    void SetReadOnly(const URI& path, bool b) const;
    /// return read only status of a file
    bool IsReadOnly(const URI& path) const;
    /// get the CRC checksum of a file
    unsigned int ComputeFileCrc(const URI& path) const;
    /// return the last write-time of a file
    FileTime GetFileWriteTime(const URI& path) const;

    /// list all files matching a pattern in a directory
    Util::Array<Util::String> ListFiles(const URI& dir, const Util::String& pattern) const;
    /// list all subdirectories matching a pattern in a directory
    Util::Array<Util::String> ListDirectories(const URI& dir, const Util::String& pattern) const;

private:
    bool zipFileSystemEnabled;
    
    #ifndef __NOZIP__
    Ptr<ZipFileSystem> zipFileSystem;
    #endif
    
    Util::HashTable<Util::String,Util::String> assignTable;
    Util::Dictionary<Util::String, const Core::Rtti*> streamClassRegistry;
};

//------------------------------------------------------------------------------
/**
*/
inline void
IoServer::SetZipFileSystemEnabled(bool b)
{
    this->zipFileSystemEnabled = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool
IoServer::IsZipFileSystemEnabled() const
{
    return this->zipFileSystemEnabled;
}

//------------------------------------------------------------------------------
/**
*/
inline Util::Array<Util::String> 
IoServer::GetAllRegisteredUriSchemes() const
{
    return this->streamClassRegistry.KeysAsArray();
}

} // namespace IO
//------------------------------------------------------------------------------
#endif
