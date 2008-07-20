//------------------------------------------------------------------------------
//  posixfswrapper.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "io/posix/posixfswrapper.h"
#include "core/sysfunc.h"

#include <dirent.h>
#include <fnmatch.h>
#include <sys/param.h>
#include <sys/stat.h>

#ifdef __APPLE__
namespace CoreFoundation {
    #include <CoreFoundation/CoreFoundation.h>
}
#endif

namespace Posix
{

using namespace Util;
using namespace Core;
using namespace IO;

//------------------------------------------------------------------------------
/**
    Open a file using the Posix function CreateFile(). Returns a handle
    to the file which must be passed to the other PosixFSWrapper file methods.
    If opening the file fails, the function will return 0. The filename
    must be a native Posix path (no assigns, etc...).
*/
PosixFSWrapper::Handle
PosixFSWrapper::OpenFile(const String& path, Stream::AccessMode accessMode, Stream::AccessPattern accessPattern)
{
    const char * mode;
    switch (accessMode)
    {
        case Stream::ReadAccess:
            mode = "r";
            break;

        case Stream::WriteAccess:
            mode = "w";
            break;

        case Stream::ReadWriteAccess:
            mode = "w+";
            break;
        case Stream::AppendAccess:
            mode = "a";
            break;
    }
    // open/create the file
    return fopen(path.AsCharPtr(), mode);
}

//------------------------------------------------------------------------------
/**
    Closes a file opened by PosixFSWrapper::OpenFile().
*/
void
PosixFSWrapper::CloseFile(Handle handle)
{
    n_assert(0 != handle);
    fclose(handle);
}

//------------------------------------------------------------------------------
/**
    Write data to a file.
*/
void
PosixFSWrapper::Write(Handle handle, const void* buf, Stream::Size numBytes)
{
    n_assert(0 != handle);
    n_assert(buf != 0);
    n_assert(numBytes > 0);
    Stream::Size bytesWritten = fwrite(buf, 1, numBytes, handle);
    if (bytesWritten != numBytes)
    {
        n_error("PosixFSWrapper: WriteFile() failed with '%s'", Posix::SysFunc::GetLastError().AsCharPtr());
    }
}

//------------------------------------------------------------------------------
/**
    Read data from a file, returns number of bytes read.
*/
Stream::Size
PosixFSWrapper::Read(Handle handle, void* buf, Stream::Size numBytes)
{
    n_assert(0 != handle);
    n_assert(buf != 0);
    n_assert(numBytes > 0);
    Stream::Size bytesRead = fread(buf, 1, numBytes, handle);
    if (0 == bytesRead)
    {
        n_error("PosixFSWrapper: ReadFile() failed with '%s'", Posix::SysFunc::GetLastError().AsCharPtr());
    }
    return bytesRead;
}

//------------------------------------------------------------------------------
/**
    Seek in a file.
*/
void
PosixFSWrapper::Seek(Handle handle, Stream::Offset offset, Stream::SeekOrigin orig)
{
    n_assert(0 != handle);
    int moveMethod;
    switch (orig)
    {
        case Stream::Begin:
            moveMethod = SEEK_SET;
            break;
        case Stream::Current:
            moveMethod = SEEK_CUR;
            break;
        case Stream::End:
            moveMethod = SEEK_END;
            break;
        default:
            // can't happen
            moveMethod = SEEK_SET;
            break;
    }
    fseeko(handle, offset, moveMethod);
}

//------------------------------------------------------------------------------
/**
    Get current position in file.
*/
Stream::Position
PosixFSWrapper::Tell(Handle handle)
{
    n_assert(0 != handle);
    return ftello(handle);
}

//------------------------------------------------------------------------------
/**
    Flush unwritten data to file.
*/
void
PosixFSWrapper::Flush(Handle handle)
{
    n_assert(0 != handle);
    fflush(handle);
}

//------------------------------------------------------------------------------
/**
    Returns true if current position is at end of file.
*/
bool
PosixFSWrapper::Eof(Handle handle)
{
    n_assert(0 != handle);
    return 0 != feof(handle);
}

//------------------------------------------------------------------------------
/**
    Returns the size of a file in bytes.
*/
Stream::Size
PosixFSWrapper::GetFileSize(Handle handle)
{
    n_assert(0 != handle);
    struct stat s;
    fstat(fileno(handle), &s);
    return s.st_size;
}

//------------------------------------------------------------------------------
/**
    Set the read-only status of a file.
*/
void
PosixFSWrapper::SetReadOnly(const String& path, bool readOnly)
{
    n_assert(path.IsValid());
    struct stat s;
    int r = stat(path.AsCharPtr(), &s);
    if (0 != r)
    {
        return;
    }
    mode_t fileAttrs = s.st_mode;
    if (readOnly)
    {
        fileAttrs &= ~S_IWUSR;
    }
    else
    {
        fileAttrs |= S_IWUSR;
    }
    chmod(path.AsCharPtr(), fileAttrs);
}

//------------------------------------------------------------------------------
/**
    Get the read-only status of a file.
*/
bool
PosixFSWrapper::IsReadOnly(const String& path)
{
    n_assert(path.IsValid());
    struct stat s;
    int r = stat(path.AsCharPtr(), &s);
    if (0 == r)
    {
        if (s.st_mode & S_IWUSR)
        {
            return true;
        }
        else if ((s.st_gid == getegid()) && (s.st_mode & S_IWGRP))
        {
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
    Deletes a file. Returns true if the operation was successful. The delete
    will fail if the fail doesn't exist or the file is read-only.
*/
bool
PosixFSWrapper::DeleteFile(const String& path)
{
    n_assert(path.IsValid());
    return (0 != unlink(path.AsCharPtr()));
}

//------------------------------------------------------------------------------
/**
    Delete an empty directory. Returns true if the operation was successful.
*/
bool
PosixFSWrapper::DeleteDirectory(const String& path)
{
    n_assert(path.IsValid());
    return (0 != rmdir(path.AsCharPtr()));
}

//------------------------------------------------------------------------------
/**
    Return true if a file exists.
*/
bool
PosixFSWrapper::FileExists(const String& path)
{
    n_assert(path.IsValid());
    struct stat s;
    int r = stat(path.AsCharPtr(), &s);
    if ((0 == r) && (false == (S_ISDIR(s.st_mode))))
    {
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
    Return true if a directory exists.
*/
bool
PosixFSWrapper::DirectoryExists(const String& path)
{
    n_assert(path.IsValid());
    struct stat s;
    int r = stat(path.AsCharPtr(), &s);
    if ((0 == r) && (true == S_ISDIR(s.st_mode)))
    {
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
    Return the last write-access time to a file.
*/
FileTime
PosixFSWrapper::GetFileWriteTime(const String& path)
{
    n_assert(path.IsValid());
    FileTime fileTime;
    struct stat s;
    int r = stat(path.AsCharPtr(), &s);
    if (0 != r)
    {
        fileTime.time = s.st_mtimespec;
    }
    return fileTime;
}

//------------------------------------------------------------------------------
/**
    Creates a new directory.
*/
bool
PosixFSWrapper::CreateDirectory(const String& path)
{
    n_assert(path.IsValid());
    return (0 != chdir(path.AsCharPtr()));
}

//------------------------------------------------------------------------------
/**
    Lists all files in a directory, filtered by a pattern.
*/
Array<String>
PosixFSWrapper::ListFiles(const String& dirPath, const String& pattern)
{
    n_assert(dirPath.IsValid());
    n_assert(pattern.IsValid());
    
    String pathWithPattern = dirPath + "/" + pattern;
    Array<String> fileList;
    DIR * dir = opendir(dirPath.AsCharPtr());
    if (0 != dir)
    {
        struct dirent entry;
        struct dirent *result;
        int r;
        for (r = readdir_r(dir, &entry, &result); (result != NULL) && (r == 0); r = readdir_r(dir, &entry, &result))
        {
            if (0 != fnmatch(pattern.AsCharPtr(), entry.d_name, FNM_CASEFOLD))
            {
                continue;
            }
            String fullName = dirPath + "/" + entry.d_name;
            struct stat s;
            stat(fullName.AsCharPtr(), &s);
            if (false == S_ISDIR(s.st_mode))
            {
                continue;
            }
            fileList.Append(entry.d_name);
        }
    }
    return fileList;
}

//------------------------------------------------------------------------------
/**
    Lists all subdirectories in a directory, filtered by a pattern. This will
    not return the special directories ".." and ".".
*/
Array<String>
PosixFSWrapper::ListDirectories(const String& dirPath, const String& pattern)
{
    n_assert(dirPath.IsValid());
    n_assert(pattern.IsValid());
    
    String pathWithPattern = dirPath + "/" + pattern;
    Array<String> dirList;
    DIR * dir = opendir(dirPath.AsCharPtr());
    if (0 != dir)
    {
        struct dirent entry;
        struct dirent *result;
        int r;
        for (r = readdir_r(dir, &entry, &result); (result != NULL) && (r == 0); r = readdir_r(dir, &entry, &result))
        {
            if (0 != fnmatch(pattern.AsCharPtr(), entry.d_name, FNM_CASEFOLD))
            {
                continue;
            }
            String fullName = dirPath + "/" + entry.d_name;
            struct stat s;
            stat(fullName.AsCharPtr(), &s);
            if (true == S_ISDIR(s.st_mode))
            {
                continue;
            }
            dirList.Append(entry.d_name);
        }
    }
    return dirList;
}

//------------------------------------------------------------------------------
/**
    This method should return the path to the current user's home directory.
    This is the directory where application can write their data to. Under
    windows, this is the "My Files" directory.
*/
String
PosixFSWrapper::GetUserDirectory()
{
    String result = getenv("HOME");
    result.ConvertBackslashes();
    return String("file:///") + result;
}

//------------------------------------------------------------------------------
/**
    This method should return a directory for temporary files with
    read/write access for the current user.
*/
String
PosixFSWrapper::GetTempDirectory()
{
    return String("file:///tmp/");
}

//------------------------------------------------------------------------------
/**
    This method sould return the directory where the application executable
    is located.
*/
String
PosixFSWrapper::GetBinDirectory()
{
    char buf[MAXPATHLEN];
#ifdef __APPLE__
    CoreFoundation::CFBundleRef mainBundle = CoreFoundation::CFBundleGetMainBundle();
    CoreFoundation::CFURLRef bundleURL = CoreFoundation::CFBundleCopyBundleURL(mainBundle);
    CoreFoundation::CFURLGetFileSystemRepresentation(bundleURL, true, (CoreFoundation::UInt8 *)buf, MAXPATHLEN);
#endif /* __APPLE__ */
    
    String result = buf;
    result.ConvertBackslashes();
    result = result.ExtractDirName();
    result.TrimRight("/");
    return String("file:///") + result;
}

//------------------------------------------------------------------------------
/**
    This method should return the installation directory of the
    application. Under Nebula3, this is either the directory where the
    executable is located, or 2 levels above the executable (if it is in 
    home:bin/posix).
*/
String
PosixFSWrapper::GetHomeDirectory()
{
    char buf[MAXPATHLEN];
#if 0
    // XXX: Fix!
    DWORD res = GetModuleFileName(NULL, buf, sizeof(buf));
    n_assert(0 != res);
#endif

    String pathToExe(buf);
    pathToExe.ConvertBackslashes();

    // check if executable resides in a posix directory
    String pathToDir = pathToExe.ExtractLastDirName();
    if (n_stricmp(pathToDir.AsCharPtr(), "posix") == 0)
    {
        // normal home:bin/posix directory structure
        // strip bin/posix
        String homePath = pathToExe.ExtractDirName();
        homePath = homePath.ExtractDirName();
        homePath = homePath.ExtractDirName();
        homePath.TrimRight("/");
        return String("file:///") + homePath;
    }
    else
    {
        // not in normal home:bin/posix directory structure, 
        // use the exe's directory as home path
        String homePath = pathToExe.ExtractDirName();
        return String("file:///") + homePath;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
PosixFSWrapper::IsDeviceName(const Util::String& str)
{
    if (str.Length() == 1)
    {
        uchar c = str[0];
        if (((c >= 'A') && (c <= 'Z')) ||
            ((c >= 'a') && (c <= 'z')))
        {
            return true;
        }
    }
    return false;
}

} // namespace IO
