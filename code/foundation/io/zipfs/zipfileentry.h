#pragma once
#ifndef IO_ZIPFILEENTRY_H
#define IO_ZIPFILEENTRY_H
//------------------------------------------------------------------------------
/**
    @class IO::ZipFileEntry
  
    A file entry in a zip archive.
    
    (C) 2006 Radon Labs GmbH
*/    
#include "core/refcounted.h"
#include "io/stream.h"
#include "zlib/unzip.h"

//------------------------------------------------------------------------------
namespace IO
{
class ZipFileEntry : public Core::RefCounted
{
    __DeclareClass(ZipFileEntry);
public:
    /// constructor
    ZipFileEntry();
    /// destructor
    ~ZipFileEntry();
    /// get the uncompressed file size in bytes
    IO::Stream::Size GetFileSize();
    /// read the *entire* content into the provided memory buffer
    bool Read(void* buf, IO::Stream::Size numBytes, const Util::String& password="");

private:
    friend class ZipArchive;
    /// setup the file entry object
    void Setup(unzFile zipFileHandle);

    unzFile zipFileHandle;          // handle on zip file
    unz_file_pos filePosInfo;       // info about position in zip file
    uint uncompressedSize;          // uncompressed size of the file
};

} // namespace IO
//------------------------------------------------------------------------------
#endif

