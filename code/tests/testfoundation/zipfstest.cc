//------------------------------------------------------------------------------
//  zipfstest.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "zipfstest.h"
#include "io/ioserver.h"
#include "io/filestream.h"
#include "io/zipfs/zipfilesystem.h"
#include "io/zipfs/ziparchive.h"
#include "io/zipfs/zipfileentry.h"

namespace Test
{
ImplementClass(Test::ZipFSTest, 'ZPTS', Test::TestCase);

using namespace IO;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
void
ZipFSTest::Run()
{
    Ptr<IoServer> ioServer = IoServer::Create();
    ioServer->RegisterUriScheme("file", FileStream::RTTI);

    // mount a zip file
    Ptr<ZipArchive> zipArchive = ZipFileSystem::Instance()->Mount("home:work/testdata/testarchive.zip");
    this->Verify(zipArchive.isvalid());
    if (zipArchive.isvalid())
    {
        this->Verify(zipArchive->IsOpen());

        ZipArchive* archive = ZipFileSystem::Instance()->FindZipArchive("home:work/testdata/testarchive.zip");
        this->Verify(archive == zipArchive);
        this->Verify(ZipFileSystem::Instance()->IsMounted("home:work/testdata/testarchive.zip"));

        // verify that some files exist
        this->Verify(archive->FindDirEntry("testarchive") != 0);
        this->Verify(archive->FindFileEntry("testarchive/bla.txt") != 0);
        this->Verify(archive->FindFileEntry("testarchive/bla.txt") != 0);

        // open a file, read its data and write it into a temp file
        ZipFileEntry* fileEntry = archive->FindFileEntry("testarchive/bla.txt");
        Stream::Size size = fileEntry->GetFileSize();
        this->Verify(size > 0);
        void* buf = Memory::Alloc(size);
        this->Verify(fileEntry->Read(buf, size));

        Ptr<Stream> outFile = ioServer->CreateStream("temp:bla.txt");
        outFile->SetAccessMode(Stream::WriteAccess);
        if (outFile->Open())
        {
            outFile->Write(buf, size);
            outFile->Close();
        }
        Memory::Free(buf);

        Array<String> files = archive->ListFiles("testarchive", "*");
        this->Verify(files.Size() == 2);
        this->Verify(0 != files.Find("bla.txt"));
        this->Verify(0 != files.Find("blub.txt"));
        
        // unmount the zip archive
        ZipFileSystem::Instance()->Unmount(zipArchive);
        this->Verify(!zipArchive->IsOpen());
    }
}

}