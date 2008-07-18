//------------------------------------------------------------------------------
//  zipfilesystem.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "io/zipfs/zipfilesystem.h"
#include "io/ioserver.h"
#include "io/zipfs/ziparchive.h"

namespace IO
{
ImplementClass(IO::ZipFileSystem, 'ZPFS', Core::RefCounted);
ImplementSingleton(IO::ZipFileSystem);

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
ZipFileSystem::ZipFileSystem()
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
ZipFileSystem::~ZipFileSystem()
{
    // unmount all mounted filesystems
    while (!this->archives.IsEmpty())
    {
        this->Unmount(this->archives.ValueAtIndex(0));
    }
    DestructSingleton;
}

//------------------------------------------------------------------------------
/**
    Resolve a zip archive path into a ZipArchive pointer. Returns 0
    if no archive with that name exists. The filename will be resolved into
    an absolute path internally before the lookup happens.
*/
const Ptr<ZipArchive>&
ZipFileSystem::FindZipArchive(const URI& uri)
{
    String path = IoServer::Instance()->ResolveAssigns(uri).LocalPath();
    IndexT index = this->archives.FindIndex(path);
    if (InvalidIndex != index)
    {
        return this->archives.ValueAtIndex(index);
    }
    else
    {
        static Ptr<ZipArchive> invalidPtr;
        return invalidPtr;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
ZipFileSystem::IsMounted(const URI& uri) const
{
    String path = IoServer::Instance()->ResolveAssigns(uri).LocalPath();
    return this->archives.Contains(path);
}

//------------------------------------------------------------------------------
/**
    This "mounts" a zip file of the given filename by creating a ZipArchive
    object and reading the zip file structure. If opening the zip
    archive fails, 0 will be returned.
*/
Ptr<ZipArchive>
ZipFileSystem::Mount(const URI& uri)
{
    n_assert(!this->IsMounted(uri));
    String path = IoServer::Instance()->ResolveAssigns(uri).LocalPath();
    Ptr<ZipArchive> newZipArchive = ZipArchive::Create();
    newZipArchive->SetURI(uri);
    if (newZipArchive->Open())
    {
        this->archives.Add(path, newZipArchive);
    }
    else
    {
        newZipArchive = 0;
    }
    return newZipArchive;
}

//------------------------------------------------------------------------------
/**
    Unmount a zip archive, this will close the ZipArchive object and
    remove it from the internal registry.
*/
void
ZipFileSystem::Unmount(const Ptr<ZipArchive>& zipArchive)
{
    n_assert(this->IsMounted(zipArchive->GetURI()));
    zipArchive->Close();
    String path = IoServer::Instance()->ResolveAssigns(zipArchive->GetURI()).LocalPath();
    this->archives.Erase(path);
}

//------------------------------------------------------------------------------
/**
    Unmount a zip archive by zip filename. 
*/
void
ZipFileSystem::Unmount(const URI& uri)
{
    n_assert(this->IsMounted(uri));
    String path = IoServer::Instance()->ResolveAssigns(uri).LocalPath();
    Ptr<ZipArchive> zipArchive = this->archives[path];
    zipArchive->Close();
    this->archives.Erase(path);
}

//------------------------------------------------------------------------------
/**
    This method takes a normal file URI and checks if the local path
    of the URI is contained as file entry in any mounted zip archive. If yes
    the name of the zip archive is returned, otherwise a 0 pointer. NOTE: if the 
    same path resides in several zip archives, it is currently not defined
    which one will be returned (the current implementation returns the
    first zip archive in alphabetical order which contains the file).
*/
const Ptr<ZipArchive>&
ZipFileSystem::FindZipArchiveWithFile(const URI& uri)
{
    // get the local path from the URI
    String localPath = IoServer::Instance()->ResolveAssigns(uri).LocalPath();
    n_assert(localPath.IsValid());

    // check each mounted archive
    IndexT i;
    for (i = 0; i < this->archives.Size(); i++)
    {
        const Ptr<ZipArchive>& arch = this->archives.ValueAtIndex(i);
        String pathInZipArchive = arch->ConvertToPathInZipArchive(localPath);
        if (pathInZipArchive.IsValid())
        {
            if (arch->FindFileEntry(pathInZipArchive).isvalid())
            {
                return arch;
            }
        }
    }
    // fallthrough: not found
    static const Ptr<ZipArchive> invalidPtr;
    return invalidPtr;
}

//------------------------------------------------------------------------------
/**
    Same as FindZipArchiveWithFile(), but checks for a directory entry 
    in a zip file.
*/
const Ptr<ZipArchive>&
ZipFileSystem::FindZipArchiveWithDir(const URI& uri)
{
    // get the local path from the URI
    String localPath = IoServer::Instance()->ResolveAssigns(uri).LocalPath();
    n_assert(localPath.IsValid());

    // check each mounted archive
    IndexT i;
    for (i = 0; i < this->archives.Size(); i++)
    {
        const Ptr<ZipArchive>& arch = this->archives.ValueAtIndex(i);
        String pathInZipArchive = arch->ConvertToPathInZipArchive(localPath);
        if (pathInZipArchive.IsValid())
        {
            if (arch->FindDirEntry(pathInZipArchive).isvalid())
            {
                return arch;
            }
        }
    }
    static Ptr<ZipArchive> invalidPtr;
    return invalidPtr;
}

//------------------------------------------------------------------------------
/**
    This checks if the provided file URI points into one of the
    mounted zip archives, and if yes returns a matching zip URI
    into the archive. If no match exists, the original URI will be
    returned.
*/
URI
ZipFileSystem::ConvertFileToZipURIIfExists(const URI& uri)
{
    if (uri.Scheme() == "file")
    {
        const Ptr<ZipArchive>& zipArchive = this->FindZipArchiveWithFile(uri);
        if (zipArchive.isvalid())
        {
            return zipArchive->ConvertToZipURI(uri);
        }
    }
    // fallthrough: no match, return original uri
    return uri;
}

//------------------------------------------------------------------------------
/**
    This checks if the provided directory URI points into one of the
    mounted zip archives, and if yes returns a matching zip URI
    into the archive. If no match exists, the original URI will be
    returned.
*/
URI
ZipFileSystem::ConvertDirToZipURIIfExists(const URI& uri)
{
    if (uri.Scheme() == "file")
    {
        const Ptr<ZipArchive>& zipArchive = this->FindZipArchiveWithDir(uri);
        if (zipArchive.isvalid())
        {
            return zipArchive->ConvertToZipURI(uri);
        }
    }
    // fallthrough: no match, return original uri
    return uri;
}

} // namespace IO