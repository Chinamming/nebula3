//------------------------------------------------------------------------------
//  win32fswrapper.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "io/win32/win32fswrapper.h"
#include "core/sysfunc.h"

namespace Win32
{

using namespace Util;
using namespace Core;
using namespace IO;

//------------------------------------------------------------------------------
/**
    Open a file using the Win32 function CreateFile(). Returns a handle
    to the file which must be passed to the other Win32FSWrapper file methods.
    If opening the file fails, the function will return 0. The filename
    must be a native Win32 path (no assigns, etc...).
*/
Win32FSWrapper::Handle
Win32FSWrapper::OpenFile(const String& path, Stream::AccessMode accessMode, Stream::AccessPattern accessPattern)
{
    DWORD access = 0;
    DWORD disposition = 0;
    DWORD shareMode = 0;
    DWORD flagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
    switch (accessMode)
    {
        case Stream::ReadAccess:
            access = GENERIC_READ;            
            disposition = OPEN_EXISTING;
            shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
            break;

        case Stream::WriteAccess:
            access = GENERIC_WRITE;
            disposition = CREATE_ALWAYS;
            shareMode = FILE_SHARE_READ;
            break;

        case Stream::ReadWriteAccess:
        case Stream::AppendAccess:
            access = GENERIC_READ | GENERIC_WRITE;
            disposition = OPEN_ALWAYS;
            shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
            break;
    }
    switch (accessPattern)
    {
        case Stream::Random:
            flagsAndAttributes |= FILE_FLAG_RANDOM_ACCESS;
            break;

        case Stream::Sequential:
            flagsAndAttributes |= FILE_FLAG_SEQUENTIAL_SCAN;
            break;
    }

    // open/create the file
    Handle handle = CreateFile(path.AsCharPtr(),        // lpFileName
                               access,                  // dwDesiredAccess
                               shareMode,               // dwShareMode
                               0,                       // lpSecurityAttributes
                               disposition,             // dwCreationDisposition,
                               flagsAndAttributes,      // dwFlagsAndAttributes
                               NULL);                   // hTemplateFile
    if (handle != INVALID_HANDLE_VALUE)
    {
        // in append mode, we need to seek to the end of the file
        if (Stream::AppendAccess == accessMode)
        {
            SetFilePointer(handle, 0, NULL, FILE_END);
        }
        return handle;
    }
    else
    {
        return 0;
    }
}

//------------------------------------------------------------------------------
/**
    Closes a file opened by Win32FSWrapper::OpenFile().
*/
void
Win32FSWrapper::CloseFile(Handle handle)
{
    n_assert(0 != handle);
    CloseHandle(handle);
}

//------------------------------------------------------------------------------
/**
    Write data to a file.
*/
void
Win32FSWrapper::Write(Handle handle, const void* buf, Stream::Size numBytes)
{
    n_assert(0 != handle);
    n_assert(buf != 0);
    n_assert(numBytes > 0);
    DWORD bytesWritten;
    BOOL result = WriteFile(handle, buf, numBytes, &bytesWritten, NULL);
    if ((0 == result) || ((DWORD)numBytes != bytesWritten))
    {
        n_error("Win32FSWrapper: WriteFile() failed with '%s'", Win32::SysFunc::GetLastError().AsCharPtr());
    }
}

//------------------------------------------------------------------------------
/**
    Read data from a file, returns number of bytes read.
*/
Stream::Size
Win32FSWrapper::Read(Handle handle, void* buf, Stream::Size numBytes)
{
    n_assert(0 != handle);
    n_assert(buf != 0);
    n_assert(numBytes > 0);
    DWORD bytesRead;
    BOOL result = ReadFile(handle, buf, numBytes, &bytesRead, NULL);
    if (0 == result)
    {
        n_error("Win32FSWrapper: ReadFile() failed with '%s'", Win32::SysFunc::GetLastError().AsCharPtr());
    }
    return bytesRead;
}

//------------------------------------------------------------------------------
/**
    Seek in a file.
*/
void
Win32FSWrapper::Seek(Handle handle, Stream::Offset offset, Stream::SeekOrigin orig)
{
    n_assert(0 != handle);
    DWORD moveMethod;
    switch (orig)
    {
        case Stream::Begin:
            moveMethod = FILE_BEGIN;
            break;
        case Stream::Current:
            moveMethod = FILE_CURRENT;
            break;
        case Stream::End:
            moveMethod = FILE_END;
            break;
        default:
            // can't happen
            moveMethod = FILE_BEGIN;
            break;
    }
    SetFilePointer(handle, offset, NULL, moveMethod);
}

//------------------------------------------------------------------------------
/**
    Get current position in file.
*/
Stream::Position
Win32FSWrapper::Tell(Handle handle)
{
    n_assert(0 != handle);
    return SetFilePointer(handle, 0, NULL, FILE_CURRENT);
}

//------------------------------------------------------------------------------
/**
    Flush unwritten data to file.
*/
void
Win32FSWrapper::Flush(Handle handle)
{
    n_assert(0 != handle);
    FlushFileBuffers(handle);
}

//------------------------------------------------------------------------------
/**
    Returns true if current position is at end of file.
*/
bool
Win32FSWrapper::Eof(Handle handle)
{
    n_assert(0 != handle);
    DWORD fpos = SetFilePointer(handle, 0, NULL, FILE_CURRENT);
    DWORD size = ::GetFileSize(handle, NULL);

    // NOTE: THE '>=' IS NOT A BUG!!!
    return fpos >= size;
}

//------------------------------------------------------------------------------
/**
    Returns the size of a file in bytes.
*/
Stream::Size
Win32FSWrapper::GetFileSize(Handle handle)
{
    n_assert(0 != handle);
    return ::GetFileSize(handle, NULL);
}

//------------------------------------------------------------------------------
/**
    Set the read-only status of a file.
*/
void
Win32FSWrapper::SetReadOnly(const String& path, bool readOnly)
{
    n_assert(path.IsValid());
    DWORD fileAttrs = GetFileAttributes(path.AsCharPtr());
    if (readOnly)
    {
        fileAttrs |= FILE_ATTRIBUTE_READONLY;
    }
    else
    {
        fileAttrs &= ~FILE_ATTRIBUTE_READONLY;
    }
    SetFileAttributes(path.AsCharPtr(), fileAttrs);
}

//------------------------------------------------------------------------------
/**
    Get the read-only status of a file.
*/
bool
Win32FSWrapper::IsReadOnly(const String& path)
{
    n_assert(path.IsValid());
    DWORD fileAttrs = GetFileAttributes(path.AsCharPtr());
    return (fileAttrs & FILE_ATTRIBUTE_READONLY);
}

//------------------------------------------------------------------------------
/**
    Deletes a file. Returns true if the operation was successful. The delete
    will fail if the fail doesn't exist or the file is read-only.
*/
bool
Win32FSWrapper::DeleteFile(const String& path)
{
    n_assert(path.IsValid());
    return (0 != ::DeleteFileA(path.AsCharPtr()));
}

//------------------------------------------------------------------------------
/**
    Delete an empty directory. Returns true if the operation was successful.
*/
bool
Win32FSWrapper::DeleteDirectory(const String& path)
{
    n_assert(path.IsValid());
    return (0 != ::RemoveDirectory(path.AsCharPtr()));
}

//------------------------------------------------------------------------------
/**
    Return true if a file exists.
*/
bool
Win32FSWrapper::FileExists(const String& path)
{
    n_assert(path.IsValid());
    DWORD fileAttrs = GetFileAttributes(path.AsCharPtr());
    if ((INVALID_FILE_ATTRIBUTES != fileAttrs) && (0 == (FILE_ATTRIBUTE_DIRECTORY & fileAttrs)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
    Return true if a directory exists.
*/
bool
Win32FSWrapper::DirectoryExists(const String& path)
{
    n_assert(path.IsValid());
    DWORD fileAttrs = GetFileAttributes(path.AsCharPtr());
    if ((INVALID_FILE_ATTRIBUTES != fileAttrs) && (0 != (FILE_ATTRIBUTE_DIRECTORY & fileAttrs)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
    Return the last write-access time to a file.
*/
FileTime
Win32FSWrapper::GetFileWriteTime(const String& path)
{
    n_assert(path.IsValid());
    FileTime fileTime;
    Handle h = Win32FSWrapper::OpenFile(path, Stream::ReadAccess, Stream::Sequential);
    if (0 != h)
    {
        GetFileTime(h, NULL, NULL, &fileTime.time);
        Win32FSWrapper::CloseFile(h);
    }
    else
    {
        n_error("Win32FSWrapper::GetFileWriteTime(): failed to open file '%s'!", path.AsCharPtr());
    }
    return fileTime;
}

//------------------------------------------------------------------------------
/**
    Creates a new directory.
*/
bool
Win32FSWrapper::CreateDirectory(const String& path)
{
    n_assert(path.IsValid());
    return (0 != ::CreateDirectory(path.AsCharPtr(), NULL));
}

//------------------------------------------------------------------------------
/**
    Lists all files in a directory, filtered by a pattern.
*/
Array<String>
Win32FSWrapper::ListFiles(const String& dirPath, const String& pattern)
{
    n_assert(dirPath.IsValid());
    n_assert(pattern.IsValid());
    
    String pathWithPattern = dirPath + "/" + pattern;
    Array<String> result;
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;
    hFind = FindFirstFile(pathWithPattern.AsCharPtr(), &findFileData);
    if (INVALID_HANDLE_VALUE != hFind) 
    {
        do
        {
            if (0 == (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                result.Append(findFileData.cFileName);
            }
        }
        while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
    return result;
}

//------------------------------------------------------------------------------
/**
    Lists all subdirectories in a directory, filtered by a pattern. This will
    not return the special directories ".." and ".".
*/
Array<String>
Win32FSWrapper::ListDirectories(const String& dirPath, const String& pattern)
{
    n_assert(dirPath.IsValid());
    n_assert(pattern.IsValid());
    
    String pathWithPattern = dirPath + "/" + pattern;
    Array<String> result;
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;
    hFind = FindFirstFile(pathWithPattern.AsCharPtr(), &findFileData);
    if (INVALID_HANDLE_VALUE != hFind) 
    {
        do
        {
            if ((0 != (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) &&
                (0 != strcmp(findFileData.cFileName, "..")) &&
                (0 != strcmp(findFileData.cFileName, ".")))
            {
                result.Append(findFileData.cFileName);
            }
        }
        while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
    return result;
}

//------------------------------------------------------------------------------
/**
    This method should return the path to the current user's home directory.
    This is the directory where application can write their data to. Under
    windows, this is the "My Files" directory.
*/
String
Win32FSWrapper::GetUserDirectory()
{
    char buf[MAX_PATH] = { 0 };
    HRESULT hr = SHGetFolderPath(NULL, 
                                 CSIDL_PERSONAL | CSIDL_FLAG_CREATE, 
                                 NULL, 
                                 0,
                                 buf);
    n_assert(SUCCEEDED(hr));
    String result = buf;
    result.ConvertBackslashes();
    return String("file:///") + result;
}

//------------------------------------------------------------------------------
/**
    This method should return a directory for temporary files with
    read/write access for the current user.
*/
String
Win32FSWrapper::GetTempDirectory()
{
    char buf[MAX_PATH] = { 0 };
    GetTempPath(sizeof(buf), buf);
    String result = buf;
    result.ConvertBackslashes();
    result.TrimRight("/");
    return String("file:///") + result;
}

//------------------------------------------------------------------------------
/**
    This method sould return the directory where the application executable
    is located.
*/
String
Win32FSWrapper::GetBinDirectory()
{
    char buf[MAX_PATH];
    DWORD res = GetModuleFileName(NULL, buf, sizeof(buf));
    n_assert(0 != res);
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
    home:bin/win32).
*/
String
Win32FSWrapper::GetHomeDirectory()
{
    char buf[MAX_PATH];
    DWORD res = GetModuleFileName(NULL, buf, sizeof(buf));
    n_assert(0 != res);

    String pathToExe(buf);
    pathToExe.ConvertBackslashes();

    // check if executable resides in a win32 directory
    String pathToDir = pathToExe.ExtractLastDirName();
    if (n_stricmp(pathToDir.AsCharPtr(), "win32") == 0)
    {
        // normal home:bin/win32 directory structure
        // strip bin/win32
        String homePath = pathToExe.ExtractDirName();
        homePath = homePath.ExtractDirName();
        homePath = homePath.ExtractDirName();
        homePath.TrimRight("/");
        return String("file:///") + homePath;
    }
    else
    {
        // not in normal home:bin/win32 directory structure, 
        // use the exe's directory as home path
        String homePath = pathToExe.ExtractDirName();
        return String("file:///") + homePath;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
Win32FSWrapper::IsDeviceName(const Util::String& str)
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
