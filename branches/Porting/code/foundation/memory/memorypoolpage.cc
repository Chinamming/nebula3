//------------------------------------------------------------------------------
//  memorypoolpage.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "memory/memorypoolpage.h"

namespace Memory
{

//------------------------------------------------------------------------------
/**
*/
MemoryPoolPage::MemoryPoolPage(SizeT _blockSize, SizeT _numBlocks)
{
    n_assert(_numBlocks < ((1<<16)-4));
    this->blockSize = _blockSize;
    this->numBlocks = _numBlocks;
    this->numAllocBlocks = 0;
    
    // compute block stride with block header and 16-byte alignment padding
    SizeT blockSizeWithHeader = this->blockSize + sizeof(BlockHeader);
    const int align = 16;
    SizeT padding = (align - (blockSizeWithHeader % align)) % align;
    this->blockStride = blockSizeWithHeader + padding;
    n_assert(0 == (this->blockStride & (align - 1)));

    // allocate the actual memory
    this->pageSize = this->numBlocks * this->blockStride;
    this->pageStart = (ubyte*) Memory::Alloc(Memory::PoolHeap, this->pageSize);
    this->pageEnd = this->pageStart + this->pageSize;

    // setup memory block headers
    BlockIndex blockIndex;
    for (blockIndex = 0; blockIndex < this->numBlocks; blockIndex++)
    {
        BlockHeader* curBlockHeader = this->BlockIndexToPointer(blockIndex);
        curBlockHeader->headerStartCanary = HeaderStartCanary;
        curBlockHeader->dataStartCanary = DataStartCanary;
        curBlockHeader->pageObject = 0;
        if (0 == blockIndex)
        {
            curBlockHeader->prevBlockIndex = ListHeadIndex;
        }
        else
        {
            curBlockHeader->prevBlockIndex = blockIndex - 1;
        }
        if (blockIndex == (this->numBlocks - 1))
        {
            curBlockHeader->nextBlockIndex = ListTailIndex;
        }
        else
        {
            curBlockHeader->nextBlockIndex = blockIndex + 1;
        }
    }

    // setup alloc and free list
    this->firstFreeBlockIndex = 0;
    this->firstAllocBlockIndex = ListTailIndex;
}

//------------------------------------------------------------------------------
/**
*/
MemoryPoolPage::~MemoryPoolPage()
{
    n_assert(ListTailIndex == this->firstAllocBlockIndex);
    n_assert(0 == this->numAllocBlocks);
    Memory::Free(Memory::PoolHeap, this->pageStart);
    this->pageStart = 0;
    this->pageEnd = 0;
    this->firstFreeBlockIndex = InvalidBlockIndex;
    this->firstAllocBlockIndex = InvalidBlockIndex;
}

//------------------------------------------------------------------------------
/**
*/
void*
MemoryPoolPage::Alloc()
{
    n_assert((ListTailIndex != this->firstFreeBlockIndex) && (this->numAllocBlocks < this->numBlocks));
    BlockIndex newBlockIndex = this->firstFreeBlockIndex;
    BlockHeader* newBlock  = this->BlockIndexToPointer(newBlockIndex);
    n_assert(0 == newBlock->pageObject);
    newBlock->pageObject = this;

    // unlink the new block from the free list and link into alloc list
    this->RemoveBlockFromList(this->firstFreeBlockIndex, newBlockIndex);
    this->InsertBlockIntoList(this->firstAllocBlockIndex, newBlockIndex);

    // make sure the canaries are intact
    this->VerifyBlockHeader(newBlock);

    // in debug mode, overwrite the data with a pattern
    void* dataPtr = (void*) (newBlock + 1);
    #if NEBULA3_DEBUG
    Memory::Fill(dataPtr, this->blockSize, NewBlockPattern);
    #endif

    this->numAllocBlocks++;
    return dataPtr;
}

//------------------------------------------------------------------------------
/**
*/
void
MemoryPoolPage::Free(void* ptr)
{
    n_assert(0 != ptr);
    n_assert(this->numAllocBlocks > 0);

    // get pointer to block header
    BlockHeader* block = ((BlockHeader*)ptr) - 1;
    this->VerifyBlockHeader(block);
    BlockIndex blockIndex = this->BlockPointerToIndex(block);

    // tets if the block isn't free'd twice
    n_assert(this == block->pageObject);
    block->pageObject = 0;

    // in debug mode, overwrite the data with a pattern,
    #if NEBULA3_DEBUG
    Memory::Fill(ptr, this->blockSize, FreeBlockPattern);
    #endif
    
    // unlink from alloc list, link into free list
    this->RemoveBlockFromList(this->firstAllocBlockIndex, blockIndex);
    this->InsertBlockIntoList(this->firstFreeBlockIndex, blockIndex);
    
    this->numAllocBlocks--;
}

//------------------------------------------------------------------------------
/**
*/
bool
MemoryPoolPage::IsOwned(void* ptr) const
{
    return ((ptr >= this->pageStart) && (ptr < this->pageEnd));
}

//------------------------------------------------------------------------------
/**
*/
void
MemoryPoolPage::VerifyBlockHeader(BlockHeader* block) const
{
    n_assert(HeaderStartCanary == block->headerStartCanary);    
    n_assert(DataStartCanary == block->dataStartCanary);

    // if this is a free block, pageObject is 0, otherwise
    // it must point to this MemoryPoolPage
    n_assert((0 == block->pageObject) || (this == block->pageObject));

    // if this is not the last block in the page, check the
    // header start canary of the next block
    BlockHeader* nextBlock = (BlockHeader*) (((ubyte*)block) + this->blockStride);
    if (((ubyte*)nextBlock) != this->pageEnd)
    {
        n_assert(HeaderStartCanary == nextBlock->headerStartCanary);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
MemoryPoolPage::VerifyBlock(void* ptr) const
{
    n_assert(0 != ptr);
    n_assert(this->IsOwned(ptr));
    BlockHeader* block = ((BlockHeader*)ptr) - 1;
    this->VerifyBlockHeader(block);
}

//------------------------------------------------------------------------------
/**
*/
void
MemoryPoolPage::VerifyAllBlocks() const
{
    this->VerifyBlockList(this->firstFreeBlockIndex);
    this->VerifyBlockList(this->firstAllocBlockIndex);
}

//------------------------------------------------------------------------------
/**
*/
void
MemoryPoolPage::VerifyBlockList(const BlockIndex& listHead) const
{
    // walk the free list
    BlockIndex blockIndex;
    for (blockIndex = listHead; blockIndex != ListTailIndex;)
    {
        BlockHeader* block = this->BlockIndexToPointer(blockIndex);
        this->VerifyBlockHeader(block);
        if (blockIndex == listHead)
        {
            n_assert(block->prevBlockIndex == ListHeadIndex);
        }
        if (block->prevBlockIndex != ListHeadIndex)
        {
            BlockHeader* prevBlock = this->BlockIndexToPointer(block->prevBlockIndex);
            n_assert(prevBlock->nextBlockIndex == blockIndex);
        }
        if (block->nextBlockIndex != ListTailIndex)
        {
            BlockHeader* nextBlock = this->BlockIndexToPointer(block->nextBlockIndex);
            n_assert(nextBlock->prevBlockIndex == blockIndex);
        }
        blockIndex = block->nextBlockIndex;
    }
}

//------------------------------------------------------------------------------
/**
*/
MemoryPoolPage*
MemoryPoolPage::GetMemoryPoolPageFromDataPointer(void* ptr)
{
    BlockHeader* block = ((BlockHeader*)ptr) - 1;
    n_assert(HeaderStartCanary == block->headerStartCanary);
    n_assert(DataStartCanary == block->dataStartCanary);
    MemoryPoolPage* pageObj = block->pageObject;
    n_assert(0 != pageObj);
    return pageObj;
}

//------------------------------------------------------------------------------
/**
*/
MemoryPoolPage::BlockHeader*
MemoryPoolPage::BlockIndexToPointer(BlockIndex i) const
{
    n_assert(i < InvalidBlockIndex);
    return (BlockHeader*) (this->pageStart + i * this->blockStride);
}

//------------------------------------------------------------------------------
/**
*/
MemoryPoolPage::BlockIndex
MemoryPoolPage::BlockPointerToIndex(BlockHeader* blockHead) const
{
    BlockIndex blockIndex = (BlockIndex) ((((ubyte*)blockHead) - this->pageStart) / this->blockStride);
    return blockIndex;
}

//------------------------------------------------------------------------------
/**
*/
void
MemoryPoolPage::InsertBlockIntoList(BlockIndex& listHead, BlockIndex blockIndex)
{
    BlockHeader* block = this->BlockIndexToPointer(blockIndex);
    
    // make sure the block is currently not in a list
    n_assert((block->prevBlockIndex == InvalidBlockIndex) && (block->nextBlockIndex == InvalidBlockIndex));

    // always add the block at the start of the list
    if (listHead != ListTailIndex)
    {
        BlockHeader* nextBlock = this->BlockIndexToPointer(listHead);
        n_assert(ListHeadIndex == nextBlock->prevBlockIndex);
        nextBlock->prevBlockIndex = blockIndex;
    }
    block->nextBlockIndex = listHead;
    block->prevBlockIndex = ListHeadIndex;
    listHead = blockIndex;
}

//------------------------------------------------------------------------------
/**
*/
void
MemoryPoolPage::RemoveBlockFromList(BlockIndex& listHead, BlockIndex blockIndex)
{
    BlockHeader* block = this->BlockIndexToPointer(blockIndex);
    
    // make sure the block is currently linked into a list
    n_assert((block->prevBlockIndex != InvalidBlockIndex) && (block->nextBlockIndex != InvalidBlockIndex));

    // unlink from predecessor
    if (ListHeadIndex != block->prevBlockIndex)
    {
        BlockHeader* prevBlock = this->BlockIndexToPointer(block->prevBlockIndex);
        prevBlock->nextBlockIndex = block->nextBlockIndex;
    }
    else
    {
        listHead = block->nextBlockIndex;
    }

    // unlink from succeedor
    if (ListTailIndex != block->nextBlockIndex)
    {
        BlockHeader* nextBlock = this->BlockIndexToPointer(block->nextBlockIndex);
        nextBlock->prevBlockIndex = block->prevBlockIndex;
    }

    // invalidate our own neighbour indices (just as a safety measure)
    block->nextBlockIndex = InvalidBlockIndex;
    block->prevBlockIndex = InvalidBlockIndex;
}

} // namespace Memory
