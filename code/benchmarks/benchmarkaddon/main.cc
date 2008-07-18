//------------------------------------------------------------------------------
//  runbenchmarks.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "core/coreserver.h"
#include "core/sysfunc.h"
#include "benchmarkbase/benchmarkrunner.h"
#include "io/ioserver.h"
#include "io/filestream.h"

#include "databaseinsert.h"
#include "databasequery.h"

using namespace Core;
using namespace Benchmarking;

void __cdecl
main()
{
    // create Nebula3 runtime
    Ptr<CoreServer> coreServer = CoreServer::Create();
    coreServer->SetAppName("Nebula3 Benchmark Runner");
    coreServer->Open();

    Ptr<IO::IoServer> ioServer = IO::IoServer::Create();    
    ioServer->RegisterUriScheme("file", IO::FileStream::RTTI);

    // setup and run benchmarks
    Ptr<BenchmarkRunner> runner = BenchmarkRunner::Create();    
    runner->AttachBenchmark(DatabaseInsert::Create());
    runner->AttachBenchmark(DatabaseQuery::Create());
    runner->Run();
    
    // shutdown Nebula3 runtime
    coreServer->Close();
    SysFunc::Exit(0);
}
