#pragma once
#ifndef IO_ZIPFILESYSTEM_H
#define IO_ZIPFILESYSTEM_H
//------------------------------------------------------------------------------
/**
    @class IO::ZipFileSystem
    
    A filesystem wrapper for ZIP files. It is
    recommended to use the ZipStreamClass to access Zip files, and only
    fall back to the ZipFileSystem class if advanced functionality is 
    needed which is not provided by the ZipStreamClass.

    Uses the zlib and the minizip package under the hood.

    Limitations:
    * No write access.
    * No seek on compressed data, the ZipFileSystem will generally decompress 
      an entire file into memory at once, so that the ZipStreamClass can
      provide random access on the decompressed data. Thus the typical 
      "audio streaming scenario" is not possible from zip files (that's
      what XACT's sound banks is there for anyway ;)

    How to fix the no-seek problem:
    * zlib processes datas in chunks, and cannot seek randomly within 
      a chunk, and the chunk size is dependent on the compress application
      being used to create the zip file(?), if those internals are known,
      it would be possible to write a chunked filesystem which keeps
      buffered chunks around for each client, probably not worth the effort.
    * Another appoach would be to split stream-files into "chunk-files"
      before compressing, and to read the next complete chunk files
      when new data is needed. This approach doesn't require changes to 
      the strip filesystem.
      
    (C) 2006 Radon Labs GmbH
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "util/string.h"
#include "util/dictionary.h"
#include "io/uri.h"

//------------------------------------------------------------------------------
namespace IO
{
class ZipArchive;

class ZipFileSystem : public Core::RefCounted
{
    __DeclareClass(ZipFileSystem);
    __DeclareSingleton(ZipFileSystem);
public:
    /// constructor
    ZipFileSystem();
    /// destructor
    virtual ~ZipFileSystem();
    /// "mount" a zip archive
    Ptr<ZipArchive> Mount(const URI& uri);
    /// "unmount" a zip archive by its file name
    void Unmount(const URI& uri);
    /// "unmount a zip archive by pointer
    void Unmount(const Ptr<ZipArchive>& zipArchive);
    /// return true if a zip archive is already mounted
    bool IsMounted(const URI& uri) const;
    /// get all mounted zip archives
    Util::Array<Ptr<ZipArchive> > GetMountedZipArchives() const;
    /// find a zip archive by name, returns 0 if not exists
    const Ptr<ZipArchive>& FindZipArchive(const URI& zipUri);
    /// find first zip archive which contains the file path
    const Ptr<ZipArchive>& FindZipArchiveWithFile(const URI& fileUri);
    /// find first zip archive which contains the directory path
    const Ptr<ZipArchive>& FindZipArchiveWithDir(const URI& dirUri);
    /// transparently convert a URI pointing to a file into a matching zip URI
    URI ConvertFileToZipURIIfExists(const URI& uri);
    /// transparently convert a URI pointing to a directory into a matching zip URI    
    URI ConvertDirToZipURIIfExists(const URI& uri);
private:
    Util::Dictionary<Util::String,Ptr<ZipArchive> > archives;    // note: use absolute path for name!
};

//------------------------------------------------------------------------------
/**
*/
inline Util::Array<Ptr<ZipArchive> >
ZipFileSystem::GetMountedZipArchives() const
{
    return this->archives.ValuesAsArray();
}

} // namespace IO
//------------------------------------------------------------------------------
#endif
    
    
    