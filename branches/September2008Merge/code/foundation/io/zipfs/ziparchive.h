#pragma once
#ifndef IO_ZIPARCHIVE_H
#define IO_ZIPARCHIVE_H
//------------------------------------------------------------------------------
/**
    @class IO::ZipArchive
    
    Private helper class for ZipFileSystem to hold per-Zip-archive data.
    Uses the zlib and the minizip lib for zip file access.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "zlib/unzip.h"
#include "io/stream.h"
#include "io/zipfs/zipfileentry.h"
#include "io/zipfs/zipdirentry.h"

//------------------------------------------------------------------------------
namespace IO
{
class ZipArchive : public Core::RefCounted
{
    __DeclareClass(ZipArchive);
public:
    /// constructor
    ZipArchive();
    /// destructor
    virtual ~ZipArchive();
    /// set the uri of the zip archive
    void SetURI(const URI& n);
    /// get the uri of the zip archive
    const URI& GetURI() const;
    /// set optional password
    void SetPassword(const Util::String& pw);
    /// get optional password
    const Util::String& GetPassword() const;
    /// open the zip archive
    bool Open();
    /// close the zip archive
    void Close();
    /// return true if the zip archive is open
    bool IsOpen() const;
    /// find a file entry in the zip archive, return 0 if not exists
    Ptr<ZipFileEntry> FindFileEntry(const Util::String& pathInZipArchive);
    /// find a directory entry in the zip archive, return 0 if not exists
    Ptr<ZipDirEntry> FindDirEntry(const Util::String& pathInZipArchive);
    /// list all files in a directory in the archive
    Util::Array<Util::String> ListFiles(const Util::String& dirPathInZipArchive, const Util::String& pattern);
    /// list all subdirectories in a directory in the archive
    Util::Array<Util::String> ListDirectories(const Util::String& dirPathInZipArchive, const Util::String& pattern);
    /// convert a "file:" URI into a "zip:" URI pointing into this archive
    URI ConvertToZipURI(const URI& fileURI) const;
    /// convert an absolute path to local path inside zip archive, returns empty string if absPath doesn't point into this archive
    Util::String ConvertToPathInZipArchive(const Util::String& absPath) const;

private:
    /// parse the table of contents into memory
    void ParseTableOfContents();
    /// add a new file entry, create missing dir entries on the way
    void AddEntry(const Util::String& path);

    URI uri;
    Util::String rootPath;          // location of the zip archive file
    Util::String password;
    unzFile zipFileHandle;          // the zip file handle
    Ptr<ZipDirEntry> rootEntry;     // the root entry of the zip archive
};

//------------------------------------------------------------------------------
/**
*/
inline bool
ZipArchive::IsOpen() const
{
    return (0 != this->zipFileHandle);
}

//------------------------------------------------------------------------------
/**
*/
inline void
ZipArchive::SetURI(const URI& u)
{
    this->uri = u;
}

//------------------------------------------------------------------------------
/**
*/
inline const URI&
ZipArchive::GetURI() const
{
    return this->uri;
}

} // namespace IO
//------------------------------------------------------------------------------
#endif
    