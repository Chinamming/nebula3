#pragma once
#ifndef IO_ZIPDIRENTRY_H
#define IO_ZIPDIRENTRY_H
//------------------------------------------------------------------------------
/**
    @class IO::ZipDirEntry

    A directory entry in a zip arcive.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "io/zipfs/zipfileentry.h"

//------------------------------------------------------------------------------
namespace IO    
{
class ZipDirEntry : public Core::RefCounted
{
    __DeclareClass(ZipDirEntry);
public:
    /// constructor
    ZipDirEntry();
    /// add a file child entry
    void AddFileEntry(const Util::String& name, const Ptr<ZipFileEntry>& fileEntry);
    /// add a directory child entry
    void AddDirEntry(const Util::String& name, const Ptr<ZipDirEntry>& dirEntry);
    /// find a direct child file entry, return 0 if not exists
    const Ptr<ZipFileEntry>& FindFileEntry(const Util::String& name);
    /// find a direct child directory entry, return 0 if not exists
    const Ptr<ZipDirEntry>& FindDirEntry(const Util::String& name);
    /// access to dir entries
    const Util::Dictionary<Util::String,Ptr<ZipDirEntry> >& GetDirEntries() const;
    /// access to file entries
    const Util::Dictionary<Util::String,Ptr<ZipFileEntry> >& GetFileEntries() const;
    
private:
    Util::Dictionary<Util::String, Ptr<ZipFileEntry> > fileEntries;
    Util::Dictionary<Util::String, Ptr<ZipDirEntry> > dirEntries;
};

} // namespace IO
//------------------------------------------------------------------------------
#endif

    