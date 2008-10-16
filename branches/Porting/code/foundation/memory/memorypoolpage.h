#pragma once
#ifndef MEMORY_MEMORYPOOLPAGE_H
#define MEMORY_MEMORYPOOLPAGE_H
//------------------------------------------------------------------------------
/**
    @class Memory::MemoryPoolPage
    
    A page of blocks in a memory pool. Memory pages are allocated by a 
    MemoryPool if all other pages are already full. Note that
    a MemoryPoolPage is NOT thread safe.

    Returned pointers will generally be 16-byte-aligned!
    
    (C) 2008 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Memory
{
class MemoryPoolPage
{
public:
    /// constructor
    MemoryPoolPage(SizeT blockSize, SizeT numBlocks);
    /// destructor
    ~MemoryPoolPage();
    /// allocate a block of memory
    void* Alloc();
    /// free a block of memory
    void Free(void* ptr);
    /// return true if pointer is owned by this page
    bool IsOwned(void* ptr) const;
    /// get overall number of blocks in the pool
    SizeT GetNumBlocks() const;    
    /// get number of allocated blocks
    SizeT GetNumAllocatedBlocks() const;
    /// get number of free blocks
    SizeT GetNumFreeBlocks() const;
    /// get block size
    SizeT GetBlockSize() const;
    /// get block size including overhead (management and alignment)
    SizeT GetBlockStride() const;
    /// get the overall page size (numBlocks * blockStride)
    SizeT GetPageSize() const;
    /// verify data integrity of a single memory block
    void VerifyBlock(void* ptr) const;
    /// verify data integrity of all blocks in the page
    void VerifyAllBlocks() const;
    /// get the MemoryPoolPage object which owns a memory chunk
    static MemoryPoolPage* GetMemoryPoolPageFromDataPointer(void* ptr);

private:

    static const uint HeaderStartCanary = 0xDEADBEEF;
    static const uint DataStartCanary = 0xDEADF00D;
    #if NEBULA3_DEBUG
        static const uchar NewBlockPattern = 0xCC; 
        static const uchar FreeBlockPattern = 0xDD;
    #endif
    typedef ushort BlockIndex;
    static const BlockIndex ListHeadIndex = 0xFFFF;
    static const BlockIndex ListTailIndex = 0xFFFE;
    static const BlockIndex InvalidBlockIndex = 0xFFFD;

    struct BlockHeader
    {
        // keep size of this structure at 16 bytes!
        uint headerStartCanary;     // used for detecting overflow errors
        BlockIndex prevBlockIndex;  // index of prev block (InvalidBlockIndex if first block)
        BlockIndex nextBlockIndex;  // index of next block (InvalidBlockIndex if last block)
        MemoryPoolPage* pageObject; // pointer to memory pool page which owns this block
        uint dataStartCanary;       // used for detecting overflow errors
    };

    /// compute a block pointer from block index
    BlockHeader* BlockIndexToPointer(BlockIndex i) const;
    /// convert a block pointer to a block index
    BlockIndex BlockPointerToIndex(BlockHeader* blockHead) const;
    /// link a block into free or alloc list
    void InsertBlockIntoList(BlockIndex& listHead, BlockIndex blockIndex);
    /// remove a block from free or alloc list
    void RemoveBlockFromList(BlockIndex& listHead, BlockIndex blockIndex);
    /// verify the canary values of a memory block
    void VerifyBlockHeader(BlockHeader* block) const;
    /// verify all blocks in the free or alloc list
    void VerifyBlockList(const BlockIndex& listHead) const;

    SizeT blockSize;
    SizeT blockStride;          // number of bytes to next block (includes header and padding)
    SizeT numBlocks;
    SizeT numAllocBlocks;
    SizeT pageSize;
    ubyte* pageStart;
    ubyte* pageEnd;

    BlockIndex firstFreeBlockIndex;      // index of first free block
    BlockIndex firstAllocBlockIndex;     // index of first allocated block
};

//------------------------------------------------------------------------------
/**
*/
inline SizeT
MemoryPoolPage::GetNumBlocks() const
{
    return this->numBlocks;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
MemoryPoolPage::GetNumAllocatedBlocks() const
{
    return this->numAllocBlocks;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
MemoryPoolPage::GetNumFreeBlocks() const
{
    return (this->numBlocks - this->numAllocBlocks);
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
MemoryPoolPage::GetBlockSize() const
{
    return this->blockSize;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
MemoryPoolPage::GetBlockStride() const
{
    return this->blockStride;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
MemoryPoolPage::GetPageSize() const
{
    return this->pageSize;
}

} // namespace Memory
//------------------------------------------------------------------------------
#endif


    