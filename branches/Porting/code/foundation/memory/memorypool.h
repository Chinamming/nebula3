#pragma once
#ifndef MEMORY_MEMORYPOOL_H
#define MEMORY_MEMORYPOOL_H
//------------------------------------------------------------------------------
/**
    @class Memory::MemoryPool
    
    A fast allocator for a fixed memory block size. Groups many memory
    blocks into a single allocation. The memory pool class is thread safe.

    (C) 2008 Radon Labs GmbH
*/
#include "memory/memory.h"
#include "util/array.h"
#include "util/string.h"
#include "threading/criticalsection.h"

//------------------------------------------------------------------------------
namespace Memory
{
class MemoryPoolPage;

class MemoryPool
{
public:
    /// default constructor
    MemoryPool();
    /// constructor
    MemoryPool(const char* name, SizeT blockSize, SizeT blocksPerPage);
    /// destructor
    ~MemoryPool();
    /// setup the memory pool (if default constructor used)
    void Setup(const char* name, SizeT blockSize, SizeT blocksPerPage);
    /// return true if memory pool is ready for use
    bool IsValid() const;
    /// allocate a block from the pool
    void* Alloc();
    /// free a block from the pool
    void Free(void* ptr);
    /// get the name of the memory pool
    const char* GetName() const;
    /// get the netto block size 
    SizeT GetBlockSize() const;
    /// get the number of blocks per page
    SizeT GetNumBlocksPerPage() const;
    /// get the current number of pages
    SizeT GetNumPages() const;
    /// verify all pages in the pool
    void Verify();

private:
    /// allocate a new page and append to both pages arrays
    void AllocPage();
    /// free a page
    void FreePage(MemoryPoolPage* page);

    Threading::CriticalSection critSect;
    const char* name;
    SizeT blockSize;
    SizeT blocksPerPage;
    Util::Array<MemoryPoolPage*> pages;
    Util::Array<MemoryPoolPage*> roomyPages;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
MemoryPool::IsValid() const
{
    return (this->blockSize > 0);
}

//------------------------------------------------------------------------------
/**
*/
inline const char*
MemoryPool::GetName() const
{
    return this->name;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
MemoryPool::GetBlockSize() const
{
    return this->blockSize;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
MemoryPool::GetNumBlocksPerPage() const
{
    return this->blocksPerPage;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
MemoryPool::GetNumPages() const
{
    return this->pages.Size();
}

} // namespace Memory
//------------------------------------------------------------------------------
#endif



