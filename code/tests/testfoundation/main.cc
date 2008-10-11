//------------------------------------------------------------------------------
//  testfoundation/main.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "foundation.h"
#include "core/coreserver.h"
#include "core/sysfunc.h"
#include "testbase/testrunner.h"
#include "stringtest.h"
#include "arraytest.h"
#include "stacktest.h"
#include "listtest.h"
#include "dictionarytest.h"
#include "fixedarraytest.h"
#include "fixedtabletest.h"
#include "hashtabletest.h"
#include "queuetest.h"
#include "memorystreamtest.h"
#include "guidtest.h"
#include "fileservertest.h"
#include "uritest.h"
#include "textreaderwritertest.h"
#include "messagereaderwritertest.h"
#include "xmlreaderwritertest.h"
#include "binaryreaderwritertest.h"
#include "uritest.h"
#include "mediatypetest.h"
#include "varianttest.h"
#include "iointerfacetest.h"
#include "attributetabletest.h"
#include "cmdlineargstest.h"
#include "streamservertest.h"
#include "luaservertest.h"
#include "zipfstest.h"
#include "attributecontainertest.h"
#include "proxytest.h"
#include "atomtest.h"
#include "float4test.h"
#include "matrix44test.h"
#include "threadtest.h"
#include "memorypooltest.h"
#include "runlengthcodectest.h"
#include "ringbuffertest.h"
#include "excelxmlreadertest.h"

using namespace Core;
using namespace Test;

void
__cdecl main()
{
    // create Nebula3 runtime
    Ptr<CoreServer> coreServer = CoreServer::Create();
    coreServer->SetAppName("Nebula3 Foundation Tests");
    coreServer->Open();

    n_printf("NEBULA3 FOUNDATION TESTS\n");
    n_printf("========================\n");

    // setup and run test runner
    Ptr<TestRunner> testRunner = TestRunner::Create();
    testRunner->AttachTestCase(ExcelXmlReaderTest::Create());
    testRunner->AttachTestCase(RingBufferTest::Create());
    testRunner->AttachTestCase(RunLengthCodecTest::Create());
    testRunner->AttachTestCase(MemoryPoolTest::Create());
    testRunner->AttachTestCase(Matrix44Test::Create());
    testRunner->AttachTestCase(Float4Test::Create());
    testRunner->AttachTestCase(AtomTest::Create());
    testRunner->AttachTestCase(ProxyTest::Create());
    testRunner->AttachTestCase(AttributeContainerTest::Create());
    testRunner->AttachTestCase(ZipFSTest::Create());
    testRunner->AttachTestCase(LuaServerTest::Create());
    testRunner->AttachTestCase(StreamServerTest::Create());
    testRunner->AttachTestCase(CmdLineArgsTest::Create());
    testRunner->AttachTestCase(AttributeTableTest::Create());
    testRunner->AttachTestCase(MediaTypeTest::Create());
    testRunner->AttachTestCase(URITest::Create());
    testRunner->AttachTestCase(StringTest::Create());   
    testRunner->AttachTestCase(ArrayTest::Create());
    testRunner->AttachTestCase(StackTest::Create());
    testRunner->AttachTestCase(ListTest::Create());
    testRunner->AttachTestCase(DictionaryTest::Create());
    testRunner->AttachTestCase(FixedArrayTest::Create());
    testRunner->AttachTestCase(FixedTableTest::Create());
    testRunner->AttachTestCase(HashTableTest::Create());
    testRunner->AttachTestCase(QueueTest::Create());
    testRunner->AttachTestCase(MemoryStreamTest::Create());
    testRunner->AttachTestCase(GuidTest::Create());
    testRunner->AttachTestCase(FileServerTest::Create());
    testRunner->AttachTestCase(TextReaderWriterTest::Create());
    testRunner->AttachTestCase(MessageReaderWriterTest::Create());
    testRunner->AttachTestCase(XmlReaderWriterTest::Create());
    testRunner->AttachTestCase(BinaryReaderWriterTest::Create());
    testRunner->AttachTestCase(VariantTest::Create());
    testRunner->AttachTestCase(IOInterfaceTest::Create());
    testRunner->AttachTestCase(ThreadTest::Create());
    testRunner->Run(); 

    coreServer->Close();
    testRunner = 0;
    coreServer = 0;
    
    Core::SysFunc::Exit(0);
}
