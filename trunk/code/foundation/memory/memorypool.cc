//------------------------------------------------------------------------------
//  memorypool.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "memory/memorypool.h"
#include "memory/memorypoolpage.h"

namespace Memory
{
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
MemoryPool::MemoryPool() :
    name(0),
    blockSize(0),
    blocksPerPage(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
MemoryPool::MemoryPool(const char* _name, SizeT _blockSize, SizeT _blocksPerPage) :
    name(_name),
    blockSize(_blockSize),
    blocksPerPage(_blocksPerPage)
{   
    n_assert(_name != 0);
    n_assert(_blockSize > 0);
    n_assert(_blocksPerPage > 0);
    this->AllocPage();
}

//------------------------------------------------------------------------------
/**
*/
MemoryPool::~MemoryPool()
{
    this->critSect.Enter();    
    if (this->IsValid())
    {
        // make sure that all blocks in all remaining pages are freed
        SizeT numAllocatedBlocks = 0;
        IndexT i;
        for (i = 0; i < this->pages.Size(); i++)
        {
            numAllocatedBlocks += this->pages[i]->GetNumAllocatedBlocks();
        }
        if (0 != numAllocatedBlocks)
        {
            String error;
            error.Format("MemoryPool: %d block still allocated in pool '%s'!\n", 
                numAllocatedBlocks, this->name);
        }

        // free remaining blocks
        for (i = 0; i < this->pages.Size(); i++)
        {
            delete this->pages[i];
            this->pages[i] = 0;
        }
        this->pages.Clear();
        this->roomyPages.Clear();
    }
    this->critSect.Leave();
}

//------------------------------------------------------------------------------
/**
*/
void
MemoryPool::Setup(const char* _name, SizeT _blockSize, SizeT _blocksPerPage)
{
    n_assert(_name != 0);
    n_assert(_blockSize > 0);
    n_assert(_blocksPerPage > 0);
    this->blockSize = _blockSize;
    this->blocksPerPage = _blocksPerPage;
    this->name = _name;
    this->AllocPage();
}

//------------------------------------------------------------------------------
/**
*/
void*
MemoryPool::Alloc()
{
    n_assert(this->IsValid());
    this->critSect.Enter();

    // choose the page from which to allocate
    if (this->roomyPages.IsEmpty())
    {
        // all pages are full, or no pages exist yet, need to allocate new page
        this->AllocPage();
    }
    MemoryPoolPage* page = this->roomyPages.Back();
    n_assert(page->GetNumFreeBlocks() > 0);

    // note: the page pointer is guaranteed to be the one
    // from the end of the roomyPages array!
    void* ptr = page->Alloc();

    // if the page is now full, remove it from the empty array
    if (0 == page->GetNumFreeBlocks())
    {
        this->roomyPages.EraseIndex(this->roomyPages.Size() - 1);
    }
    this->critSect.Leave();
    return ptr;
}

//------------------------------------------------------------------------------
/**
*/
void
MemoryPool::Free(void* ptr)
{
    n_assert(this->IsValid());
    n_assert(0 != ptr);
    this->critSect.Enter();

    // obtain the memory page from the pointer
    MemoryPoolPage* page = MemoryPoolPage::GetMemoryPoolPageFromDataPointer(ptr);

    #if NEBULA3_DEBUG
        // in debug mode, make sure that this is actually one of our own pages!
        n_assert(InvalidIndex != this->pages.FindIndex(page));
        IndexT roomyIndex = this->roomyPages.FindIndex(page);
        if (page->GetNumFreeBlocks() == 0)
        {
            n_assert(InvalidIndex == roomyIndex);
        }
        else
        {
            n_assert(InvalidIndex != roomyIndex);
        }
    #endif

    // release the memory block from the page, and check if the page
    // should be moved to the roomy pages array or deleted entirely
    page->Free(ptr);
    if (page->GetNumFreeBlocks() == 1)
    {
        // we've been full previously, but are roomy now
        this->roomyPages.Append(page);
    }
    else if (page->GetNumAllocatedBlocks() == 0)
    {
        // the page no longer contains any allocated blocks, free 
        // the entire page, unless this is the last page to
        // prevent expensive allocation/deallocations if only
        // one block is allocated/freed in this memory pool
        if (this->pages.Size() > 1)
        {
            this->FreePage(page);
        }
    }
    this->critSect.Leave();
}

//------------------------------------------------------------------------------
/**
*/
void
MemoryPool::AllocPage()
{
    MemoryPoolPage* newPage = new MemoryPoolPage(this->blockSize, this->blocksPerPage);
    this->pages.Append(newPage);
    this->roomyPages.Append(newPage);
}

//------------------------------------------------------------------------------
/**
*/
void
MemoryPool::FreePage(MemoryPoolPage* page)
{
    n_assert(page->GetNumAllocatedBlocks() == 0);
    IndexT pagesIndex = this->pages.FindIndex(page);
    n_assert(pagesIndex != InvalidIndex);
    IndexT roomyPagesIndex = this->roomyPages.FindIndex(page);
    n_assert(roomyPagesIndex != InvalidIndex);

    // note: we don't use EraseIndexSwap to keep the page order
    // in the order of allocations
    this->pages.EraseIndex(pagesIndex);
    this->roomyPages.EraseIndex(roomyPagesIndex);
    delete page;
}

//------------------------------------------------------------------------------
/**
*/
void
MemoryPool::Verify()
{
    n_assert(this->IsValid());
    IndexT i;
    for (i = 0; i < this->pages.Size(); i++)
    {
        this->pages[i]->VerifyAllBlocks();
    }
}

} // namespace Memory