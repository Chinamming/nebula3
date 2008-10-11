//------------------------------------------------------------------------------
//  memorypooltest.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "memorypooltest.h"
#include "memory/memorypool.h"

namespace Test
{
__ImplementClass(Test::MemoryPoolTest, 'MEPT', Test::TestCase);

using namespace Memory;

//------------------------------------------------------------------------------
/**
*/
void
MemoryPoolTest::Run()
{
    const SizeT blockSize = 45;
    const SizeT numBlocks = 4;
    MemoryPool memPool("TestPool", blockSize, numBlocks);

    this->Verify(Util::String("TestPool") == memPool.GetName());
    this->Verify(memPool.GetBlockSize() == blockSize);
    this->Verify(memPool.GetNumBlocksPerPage() == numBlocks);
    this->Verify(memPool.GetNumPages() == 1);
    memPool.Verify();

    // do some allocations
    void* ptr[9] = { 0 };
    ptr[0] = memPool.Alloc();
    memPool.Verify();
    this->Verify(memPool.GetNumPages() == 1);
    ptr[1] = memPool.Alloc();
    memPool.Verify();
    ptr[2] = memPool.Alloc();
    memPool.Verify();
    ptr[3] = memPool.Alloc();
    memPool.Verify();
    this->Verify(memPool.GetNumPages() == 1);
    ptr[4] = memPool.Alloc();
    memPool.Verify();
    this->Verify(memPool.GetNumPages() == 2);
    ptr[5] = memPool.Alloc();
    memPool.Verify();
    ptr[6] = memPool.Alloc();
    memPool.Verify();
    ptr[7] = memPool.Alloc();
    memPool.Verify();
    this->Verify(memPool.GetNumPages() == 2);
    ptr[8] = memPool.Alloc();
    memPool.Verify();
    this->Verify(memPool.GetNumPages() == 3);

    // free in different order, check number of allocated pages
    memPool.Free(ptr[8]);
    memPool.Verify();
    this->Verify(memPool.GetNumPages() == 2);
    memPool.Free(ptr[1]);
    memPool.Verify();
    memPool.Free(ptr[3]);
    memPool.Verify();
    memPool.Free(ptr[2]);
    memPool.Verify();
    memPool.Free(ptr[0]);
    memPool.Verify();
    this->Verify(memPool.GetNumPages() == 1);
    memPool.Free(ptr[6]);
    memPool.Verify();
    this->Verify(memPool.GetNumPages() == 1);
    memPool.Free(ptr[4]);
    memPool.Verify();
    memPool.Free(ptr[5]);
    memPool.Verify();
    memPool.Free(ptr[7]);
    memPool.Verify();
    this->Verify(memPool.GetNumPages() == 1);

    // do some more allocations
    ptr[0] = memPool.Alloc();
    memPool.Verify();
    this->Verify(memPool.GetNumPages() == 1);
    ptr[1] = memPool.Alloc();
    memPool.Verify();
    ptr[2] = memPool.Alloc();
    memPool.Verify();
    ptr[3] = memPool.Alloc();
    memPool.Verify();
    this->Verify(memPool.GetNumPages() == 1);
    ptr[4] = memPool.Alloc();
    memPool.Verify();
    this->Verify(memPool.GetNumPages() == 2);
    ptr[5] = memPool.Alloc();
    memPool.Verify();
    ptr[6] = memPool.Alloc();
    memPool.Verify();
    ptr[7] = memPool.Alloc();
    memPool.Verify();
    this->Verify(memPool.GetNumPages() == 2);
    ptr[8] = memPool.Alloc();
    memPool.Verify();
    this->Verify(memPool.GetNumPages() == 3);

    // and free the shit again...
    IndexT i;
    for (i = 0; i < 9; i++)
    {
        memPool.Free(ptr[i]);
        memPool.Verify();
    }
    this->Verify(memPool.GetNumPages() == 1);
}

} // namespace Test
