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
__ImplementClass(Test::ZipFSTest, 'ZPTS', Test::TestCase);

using namespace IO;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
void
ZipFSTest::Run()
{
    Ptr<IoServer> ioServer = IoServer::Create();
    ioServer->RegisterStandardUriSchemes();

    // mount a zip file
    Ptr<ZipArchive> zipArchive = ZipFileSystem::Instance()->Mount("home:export.zip");
    this->Verify(zipArchive.isvalid());
    if (zipArchive.isvalid())
    {
        this->Verify(zipArchive->IsOpen());

        ZipArchive* archive = ZipFileSystem::Instance()->FindZipArchive("home:export.zip");
        this->Verify(archive == zipArchive);
        this->Verify(ZipFileSystem::Instance()->IsMounted("home:export.zip"));

        // verify that some files exist
        this->Verify(archive->FindDirEntry("export") != 0);
        this->Verify(archive->FindFileEntry("export/data/tables/blueprints.xml") != 0);
        this->Verify(archive->FindFileEntry("export/data/tables/materials.xml") != 0);

        // open a file, read its data and write it into a temp file
        ZipFileEntry* fileEntry = archive->FindFileEntry("export/data/tables/blueprints.xml");
        Stream::Size size = fileEntry->GetFileSize();
        this->Verify(size > 0);
        void* buf = Memory::Alloc(Memory::ScratchHeap, size);
        this->Verify(fileEntry->Read(buf, size));

        Ptr<Stream> outFile = ioServer->CreateStream("temp:blueprints.xml");
        outFile->SetAccessMode(Stream::WriteAccess);
        if (outFile->Open())
        {
            outFile->Write(buf, size);
            outFile->Close();
        }
        Memory::Free(Memory::ScratchHeap, buf);

        Array<String> files = archive->ListFiles("export/data/tables", "*");
        this->Verify(files.Size() == 4);
        this->Verify(0 != files.Find("anims.xml"));
        this->Verify(0 != files.Find("blueprints.xml"));
        this->Verify(0 != files.Find("globals.xml"));
        this->Verify(0 != files.Find("materials.xml"));
        
        // unmount the zip archive
        ZipFileSystem::Instance()->Unmount(zipArchive);
        this->Verify(!zipArchive->IsOpen());
    }
}

}