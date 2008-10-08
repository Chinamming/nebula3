//------------------------------------------------------------------------------
//  io/ioserver.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "io/ioserver.h"
#include "core/ptr.h"
#include "io/fswrapper.h"
#include "util/stack.h"
#include "util/crc.h"
#include "system/systeminfo.h"
#include "io/filestream.h"
#include "io/assign.h"

#ifndef __NOZIP__
#include "io/zipfs/ziparchive.h"
#include "io/zipfs/zipfilesystem.h"
#include "io/zipfilestream.h"
#endif

namespace IO
{
__ImplementClass(IO::IoServer, 'IOSV', Core::RefCounted);
__ImplementSingleton(IO::IoServer);

using namespace Core;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
IoServer::IoServer() :
    zipFileSystemEnabled(true)
{
    __ConstructSingleton;

    // initialize standard assigns
    String homeLocation = Internal::FSWrapper::GetHomeDirectory();
    if (homeLocation.IsValid())
    {
        this->SetAssign(Assign("home", homeLocation));
    }
    String binLocation = Internal::FSWrapper::GetBinDirectory();
    if (binLocation.IsValid())
    {
        this->SetAssign(Assign("bin", binLocation));
    }
    String tempLocation = Internal::FSWrapper::GetTempDirectory();
    if (tempLocation.IsValid())
    {
        this->SetAssign(Assign("temp", tempLocation));
    }
    String userLocation = Internal::FSWrapper::GetUserDirectory();
    if (userLocation.IsValid())
    {
        this->SetAssign(Assign("user", userLocation));
    }

	#ifndef __NOZIP__
    // setup a zip filesystem singleton
    this->zipFileSystem = ZipFileSystem::Create();
    this->RegisterUriScheme("zip", ZipFileStream::RTTI);
    #endif
}

//------------------------------------------------------------------------------
/**
*/
IoServer::~IoServer()
{
	#ifndef __NOZIP__
    this->zipFileSystem = 0;
    #endif
    
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
    Registers the standard URI schemes.
*/
void
IoServer::RegisterStandardUriSchemes()
{
    #if __WII__        
        this->RegisterUriScheme("file", IO::WiiDvdStream::RTTI);
        this->RegisterUriScheme("dvd", IO::WiiDvdStream::RTTI);
    #else        
        this->RegisterUriScheme("file", FileStream::RTTI);
    #endif
}

//------------------------------------------------------------------------------
/**
    Associates an URI scheme with a stream class. If the same URI
    scheme is already registered, the old association will be overwritten.
*/
void
IoServer::RegisterUriScheme(const String& uriScheme, const Core::Rtti& classRtti)
{
    n_assert(classRtti.IsDerivedFrom(Stream::RTTI));
    n_assert(uriScheme.IsValid());
    if (this->streamClassRegistry.Contains(uriScheme))
    {
        this->UnregisterUriScheme(uriScheme);
    }
    this->streamClassRegistry.Add(uriScheme, &classRtti);
}

//------------------------------------------------------------------------------
/**
*/
void
IoServer::UnregisterUriScheme(const String& uriScheme)
{
    n_assert(uriScheme.IsValid());
    n_assert(this->streamClassRegistry.Contains(uriScheme));
    this->streamClassRegistry.Erase(uriScheme);
}

//------------------------------------------------------------------------------
/**
*/
bool
IoServer::IsUriSchemeRegistered(const String& uriScheme) const
{
    n_assert(uriScheme.IsValid());
    return this->streamClassRegistry.Contains(uriScheme);
}

//------------------------------------------------------------------------------
/**
*/
const Core::Rtti&
IoServer::GetStreamClassByUriScheme(const String& uriScheme) const
{
    n_assert(uriScheme.IsValid());
    return *(this->streamClassRegistry[uriScheme]);
}

//------------------------------------------------------------------------------
/**
*/
Ptr<Stream>
IoServer::CreateStream(const URI& uri) const
{
    n_assert(!uri.IsEmpty());
    n_assert(this->IsUriSchemeRegistered(uri.Scheme()));

	#ifndef __NOZIP__
    // check if the URI points into a mounted ZIP archive
    if (this->IsZipFileSystemEnabled())
    {
        URI newUri = ZipFileSystem::Instance()->ConvertFileToZipURIIfExists(uri);
        Ptr<Stream> stream = (Stream*) this->streamClassRegistry[newUri.Scheme()]->Create();
        stream->SetURI(newUri);
        return stream;
    }
    else
    #endif
    {
        Ptr<Stream> stream = (Stream*) this->streamClassRegistry[uri.Scheme()]->Create();
        stream->SetURI(uri);
        return stream;
    }
}

//------------------------------------------------------------------------------
/**
*/
#ifndef __NOZIP__
bool
IoServer::MountZipArchive(const URI& uri)
{
    Ptr<ZipArchive> zipArchive = this->zipFileSystem->Mount(uri);
    return zipArchive.isvalid();
}
#endif

//------------------------------------------------------------------------------
/**
*/
#ifndef __NOZIP__
void
IoServer::UnmountZipArchive(const URI& uri)
{
    this->zipFileSystem->Unmount(uri);
}
#endif

//------------------------------------------------------------------------------
/**
*/
#ifndef __NOZIP__
bool
IoServer::IsZipArchiveMounted(const URI& uri) const
{
    return this->zipFileSystem->IsMounted(uri);
}
#endif

//------------------------------------------------------------------------------
/**
*/
void
IoServer::MountStandardZipArchives()
{
    #ifndef __NOZIP__
    // first load the platform specific archives, since platform specific
    // files override any generic files
    System::SystemInfo systemInfo;  
    String platformString = System::SystemInfo::PlatformAsString(systemInfo.GetPlatform());
    String platformArchivePath;
    platformArchivePath.Format("home:export_%s.zip", platformString.AsCharPtr());
    if (this->FileExists(platformArchivePath))
    {
        this->MountZipArchive(platformArchivePath);
    }

    // platform agnostic data zip archive
    if (this->FileExists("home:export.zip"))
    {
        this->MountZipArchive("home:export.zip");
    }
    #endif
}

//------------------------------------------------------------------------------
/**
*/
void
IoServer::UnmountStandardZipArchives()
{
    #ifndef __NOZIP__
    System::SystemInfo systemInfo;  
    String platformString = System::SystemInfo::PlatformAsString(systemInfo.GetPlatform());
    String platformArchivePath;
    platformArchivePath.Format("home:export_%s.zip", platformString.AsCharPtr());
    if (this->IsZipArchiveMounted(platformArchivePath))
    {
        this->UnmountZipArchive(platformArchivePath);
    }

    // platform agnostic data zip archive
    if (this->IsZipArchiveMounted("home:export.zip"))
    {
        this->UnmountZipArchive("home:export.zip");
    }
    #endif
}

//------------------------------------------------------------------------------
/**
*/
void
IoServer::SetupStandardAssigns()
{
    this->SetAssign(Assign("shd", "home:export/shaders"));
    this->SetAssign(Assign("frame", "home:export/frame"));
    this->SetAssign(Assign("msh", "home:export/meshes"));
    this->SetAssign(Assign("tex", "home:export/textures"));
    this->SetAssign(Assign("ani", "home:export/anims"));
    this->SetAssign(Assign("mdl", "home:export/gfxlib"));
    this->SetAssign(Assign("audio", "home:export/audio"));
    this->SetAssign(Assign("data", "home:export/data"));        
    this->SetAssign(Assign("export", "home:export"));

    // platform specific assigns
    #if __WIN32__
        this->SetAssign(Assign("stream", "home:export_win32/audio"));
    #elif __XBOX360__
        this->SetAssign(Assign("stream", "home:export_xbox360/audio"));
    #endif

    // Nebul2 backward compat assigns:
    this->SetAssign(Assign("meshes", "home:export/meshes"));
    this->SetAssign(Assign("textures", "home:export/textures"));
    this->SetAssign(Assign("anims", "home:export/anims"));
}

//------------------------------------------------------------------------------
/**
*/
void
IoServer::SetAssign(const Assign& assign)
{
    if (this->HasAssign(assign.GetName()))
    {
        this->ClearAssign(assign.GetName());
    }
    this->assignTable.Add(assign);
}

//------------------------------------------------------------------------------
/**
*/
bool
IoServer::HasAssign(const String& assignName) const
{
    n_assert(assignName.IsValid());
    return this->assignTable.Contains(assignName);
}

//------------------------------------------------------------------------------
/**
*/
const String&
IoServer::GetAssign(const String& assignName) const
{
    n_assert(assignName.IsValid());
    n_assert(this->HasAssign(assignName));
    return this->assignTable[assignName];
}

//------------------------------------------------------------------------------
/**
*/
void
IoServer::ClearAssign(const String& assignName)
{
    n_assert(assignName.IsValid());
    n_assert(this->HasAssign(assignName));
    this->assignTable.Erase(assignName);
}

//------------------------------------------------------------------------------
/**
*/
Array<Assign>
IoServer::GetAllAssigns() const
{
    Array<KeyValuePair<String,String> > content = this->assignTable.Content();
    Array<Assign> assigns;
    IndexT i;
    for (i = 0; i < content.Size(); i++)
    {
        assigns.Append(Assign(content[i].Key(), content[i].Value()));
    }
    return assigns;
}

//------------------------------------------------------------------------------
/**
    Resolves all assigns from an URI returning an URI. It is allowed to
    "stack" assigns, which means, defining an assign as pointing to
    another assign.
*/
URI
IoServer::ResolveAssigns(const URI& uri) const
{
    return URI(this->ResolveAssignsInString(uri.AsString()));
}

//------------------------------------------------------------------------------
/**
    Resolves all assigns from a URI. It is allowed to
    "stack" assigns, which means, defining an assign as pointing to
    another assign.
*/
String
IoServer::ResolveAssignsInString(const String& uriString) const
{
    String result = uriString;

    // check for assigns
    int colonIndex;
    while ((colonIndex = result.FindCharIndex(':', 0)) > 0)
    {
        // special case: ignore one-caracter "assigns" because they are 
        // really DOS drive letters
        if (colonIndex > 1)
        {
            String assignString = result.ExtractRange(0, colonIndex);
            
            // ignore unknown assigns, because these may be part of an URI
            if (this->HasAssign(assignString))
            {
                String postAssignString = result.ExtractRange(colonIndex + 1, result.Length() - (colonIndex + 1));
                String replace = this->GetAssign(assignString);
                if (!replace.IsEmpty())
                {
                    replace.Append("/");
                    replace.Append(postAssignString);
                }
                result = replace;
            }
            else break;
        }
        else break;
    }
    result.ConvertBackslashes();
    result.TrimRight("/");
    return result;
}

//------------------------------------------------------------------------------
/**
    This method creates all missing directories in a path.
*/
bool
IoServer::CreateDirectory(const URI& uri) const
{
    n_assert(uri.Scheme() == "file");
    String path = uri.LocalPath();

    // build stack of non-existing dir components
    Stack<String> pathStack;
    while ((!path.IsEmpty()) && (!this->DirectoryExists(path)))
    {
        pathStack.Push(path);
        String nextPath = path.ExtractDirName();
        nextPath.TrimRight("/\\:");
        if ((nextPath == path) || Internal::FSWrapper::IsDeviceName(nextPath))
        {
            break;
        }
        path = nextPath;
    }

    // error in original path?
    if (path.IsEmpty())
    {
        return false;
    }

    // create missing directory components
    while (!pathStack.IsEmpty())
    {
        String curPath = pathStack.Pop();
        if (!Internal::FSWrapper::CreateDirectory(curPath))
        {
            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------
/**
    This copies a file to another file.
*/
bool
IoServer::CopyFile(const URI& fromUri, const URI& toUri) const
{
    // transparent zip archive support...
    URI srcUri = fromUri;
    
    #ifndef __NOZIP__
    if (this->IsZipFileSystemEnabled())
    {
        srcUri = ZipFileSystem::Instance()->ConvertFileToZipURIIfExists(fromUri);
    }
    #endif

    // if target file exists, remove read-only flag
    if (this->FileExists(toUri))
    {
        this->SetReadOnly(toUri, false);
    }

    // create file streams
    Ptr<Stream> srcStream = this->CreateStream(srcUri);
    srcStream->SetAccessMode(Stream::ReadAccess);
    if (srcStream->Open())
    {
        Ptr<Stream> toStream = this->CreateStream(toUri);
        toStream->SetAccessMode(Stream::WriteAccess);
        if (toStream->Open())
        {
            // allocate a buffer, and copy contents in a loop
            const int bufSize = (1<<16);
            void* buffer = Memory::Alloc(Memory::ScratchHeap, bufSize);
            while (!srcStream->Eof())
            {
                Stream::Size bytesRead = srcStream->Read(buffer, bufSize);
                toStream->Write(buffer, bytesRead);
            }
            Memory::Free(Memory::ScratchHeap, buffer);
            toStream->Close();
        }
        else
        {
            n_error("IO::IoServer::CopyFile(): could not open target file '%s'", toUri.AsString().AsCharPtr());
            return false;
        }
        srcStream->Close();
    }
    else
    {
        n_error("IO::IoServer::CopyFile(): could not open source file '%s'", fromUri.AsString().AsCharPtr());
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
/**
    This method computes the CRC checksum for a file.
*/
unsigned int
IoServer::ComputeFileCrc(const URI& uri) const
{
    unsigned int result = 0;

    // create a stream
    Ptr<Stream> stream = this->CreateStream(uri);
    stream->SetAccessMode(Stream::ReadAccess);
    if (stream->Open())
    {
        Crc crc;
        crc.Begin();
        const int bufSize = (1<<16);
        unsigned char* buffer = (unsigned char*) Memory::Alloc(Memory::ScratchHeap, bufSize);
        while (!stream->Eof())
        {
            Stream::Size bytesRead = stream->Read(buffer, bufSize);
            crc.Compute(buffer, bytesRead);
        }
        Memory::Free(Memory::ScratchHeap, buffer);
        crc.End();
        result = crc.GetResult();
        stream->Close();
    }
    else
    {
        n_error("IO::IoServer::ComputeFileCrc(): could not open file '%s'!", uri.AsString().AsCharPtr());
    }
    return result;
}

//------------------------------------------------------------------------------
/**
*/
bool
IoServer::DeleteFile(const URI& uri) const
{
    n_assert(uri.Scheme() == "file");
    const String path = uri.LocalPath();
    n_assert(path.IsValid());
    return Internal::FSWrapper::DeleteFile(path);
}

//------------------------------------------------------------------------------
/**
*/
bool
IoServer::DeleteDirectory(const URI& uri) const
{
    n_assert(uri.Scheme() == "file");
    const String path = uri.LocalPath();
    n_assert(path.IsValid());
    return Internal::FSWrapper::DeleteDirectory(path);
}

//------------------------------------------------------------------------------
/**
*/
bool
IoServer::FileExists(const URI& uri) const
{
	#ifndef __NOZIP__
    // transparent zip archive support
    if (this->IsZipFileSystemEnabled())
    {
        URI zipUri = ZipFileSystem::Instance()->ConvertFileToZipURIIfExists(uri);
        if (zipUri.Scheme() == "zip")
        {
            return true;
        }
    }
    #endif

    // doesn't exist as zip, check conventional filesystem
    const String path = uri.LocalPath();
    n_assert(path.IsValid());
    return Internal::FSWrapper::FileExists(path);
}

//------------------------------------------------------------------------------
/**
*/
bool
IoServer::DirectoryExists(const URI& uri) const
{
	#ifndef __NOZIP__
    // transparent zip archive support
    if (this->IsZipFileSystemEnabled())
    {
        URI zipUri = ZipFileSystem::Instance()->ConvertDirToZipURIIfExists(uri);
        if (zipUri.Scheme() == "zip")
        {
            return true;
        }
    }
    #endif

    // doesn't exist as zip, check conventional filesystem
    const String path = uri.LocalPath();
    n_assert(path.IsValid());
    return Internal::FSWrapper::DirectoryExists(path);
}

//------------------------------------------------------------------------------
/**
*/
void
IoServer::SetReadOnly(const URI& uri, bool readOnly) const
{
    n_assert(uri.Scheme() == "file");
    const String path = uri.LocalPath();
    n_assert(path.IsValid());
    Internal::FSWrapper::SetReadOnly(path, readOnly);
}

//------------------------------------------------------------------------------
/**
*/
bool
IoServer::IsReadOnly(const URI& uri) const
{
    n_assert(uri.Scheme() == "file");

	#ifndef __NOZIP__
    // transparent zip file system support
    if (this->IsZipFileSystemEnabled())
    {
        URI zipUri = ZipFileSystem::Instance()->ConvertFileToZipURIIfExists(uri);
        if (zipUri.Scheme() == "zip")
        {
            // file exists in zip archive, zip archive are
            // generally read only
            return true;
        }
    }
    #endif

    // file not in zip archive, handle conventionally
    const String path = uri.LocalPath();
    n_assert(path.IsValid());
    return Internal::FSWrapper::IsReadOnly(path);
}

//------------------------------------------------------------------------------
/**
*/
FileTime
IoServer::GetFileWriteTime(const URI& uri) const
{
    n_assert(uri.Scheme() == "file");
    const String path = uri.LocalPath();
    n_assert(path.IsValid());
    return Internal::FSWrapper::GetFileWriteTime(path);
}

//------------------------------------------------------------------------------
/**
*/
Array<String>
IoServer::ListFiles(const URI& uri, const String& pattern) const
{
    n_assert(pattern.IsValid());

	#ifndef __NOZIP__
    // transparent zip file system support
    if (this->IsZipFileSystemEnabled())
    {
        const Ptr<ZipArchive>& zipArchive = ZipFileSystem::Instance()->FindZipArchiveWithDir(uri);
        if (zipArchive.isvalid())
        {
            String pathInZipArchive = zipArchive->ConvertToPathInZipArchive(uri.LocalPath());
            return zipArchive->ListFiles(pathInZipArchive, pattern);
        }
    }
    #endif

    // fallthrough: not contained in zip archive, handle conventionally
    return Internal::FSWrapper::ListFiles(uri.LocalPath(), pattern);
}

//------------------------------------------------------------------------------
/**
*/
Array<String>
IoServer::ListDirectories(const URI& uri, const String& pattern) const
{
    n_assert(pattern.IsValid());

	#ifndef __NOZIP__
    // transparent zip file system support
    if (this->IsZipFileSystemEnabled())
    {
        const Ptr<ZipArchive>& zipArchive = ZipFileSystem::Instance()->FindZipArchiveWithDir(uri);
        if (zipArchive.isvalid())
        {
            String pathInZipArchive = zipArchive->ConvertToPathInZipArchive(uri.LocalPath());
            return zipArchive->ListDirectories(pathInZipArchive, pattern);
        }
    }
    #endif
    
    // fallthrough: not contained in zip archive, handle conventionally
    return Internal::FSWrapper::ListDirectories(uri.LocalPath(), pattern);
}

} // namespace IO
