#pragma once
#ifndef UTIL_ARRAY_H
#define UTIL_ARRAY_H
//------------------------------------------------------------------------------
/**
    @class Util::Array

    Nebula3's dynamic array class. This class is also used by most other
    collection classes.

    The default constructor will not pre-allocate elements, so no space
    is wasted as long as no elements are added. As soon as the first element
    is added to the array, an initial buffer of 16 elements is created.
    Whenever the element buffer would overflow, a new buffer of twice
    the size of the previous buffer is created and the existing elements 
    are then copied over to the new buffer. The element buffer will
    never shrink, the only way to reclaim unused memory is to 
    copy the Array to a new Array object. This is usually not a problem
    since most arrays will oscillate around some specific size, so once
    the array has reached this specific size, no costly memory free or allocs 
    will be performed.

    It is possible to sort the array using the Sort() method, this uses
    std::sort (one of the very few exceptions where the STL is used in
    Nebula3).

    One should generally be careful with costly copy operators, the Array
    class (and the other container classes using Array) may do some heavy
    element shuffling in some situations (especially when sorting and erasing
    elements).

    (C) 2006 RadonLabs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Util
{
template<class TYPE> class Array
{
public:
    /// define iterator
    typedef TYPE* Iterator;

    /// constructor with default parameters
    Array();
    /// constuctor with initial size and grow size
    Array(SizeT initialCapacity, SizeT initialGrow);
    /// constructor with initial size, grow size and initial values
    Array(SizeT initialSize, SizeT initialGrow, const TYPE& initialValue);
    /// copy constructor
    Array(const Array<TYPE>& rhs);
    /// destructor
    ~Array();
    /// assignment operator
    void operator=(const Array<TYPE>& rhs);
    /// [] operator
    TYPE& operator[](IndexT index) const;
    /// equality operator
    bool operator==(const Array<TYPE>& rhs) const;
    /// inequality operator
    bool operator!=(const Array<TYPE>& rhs) const;

    /// append element to end of array
    void Append(const TYPE& elm);
    /// append the contents of an array to this array
    void AppendArray(const Array<TYPE>& rhs);
    /// reserve num elements at end of array
    void Reserve(SizeT num);
    /// get number of elements in array
    SizeT Size() const;
    /// get overall allocated size of array in number of elements
    SizeT Capacity() const;
    /// set element at index, grow array if necessary
    void Set(IndexT index, const TYPE& elm);
    /// return reference to nth element in array
    TYPE& At(IndexT index);
    /// return reference to first element
    TYPE& Front() const;
    /// return reference to last element
    TYPE& Back() const;
    /// return true if array empty
    bool IsEmpty() const;
    /// erase element at index
    void EraseIndex(IndexT index);
    /// erase element pointed to by iterator
    Iterator Erase(Iterator iter);
    /// insert element before element at index
    void Insert(IndexT index, const TYPE& elm);
    /// insert element into sorted array
    void InsertSorted(const TYPE& elm);
    /// clear array (calls destructors)
    void Clear();
    /// reset array (does NOT call destructors)
    void Reset();
    /// return iterator to beginning of array
    Iterator Begin() const;
    /// return iterator to end of array
    Iterator End() const;
    /// find identical element in array, return iterator
    Iterator Find(const TYPE& elm) const;
    /// find identical element in array, return index, InvalidIndex if not found
    IndexT FindIndex(const TYPE& elm) const;
    /// fill array range with element
    void Fill(IndexT first, SizeT num, const TYPE& elm);
    /// clear contents and preallocate with new attributes
    void Reallocate(SizeT capacity, SizeT grow);
    /// returns new array with elements which are not in rhs (slow!)
    Array<TYPE> Difference(const Array<TYPE>& rhs);
    /// sort the array
    void Sort();
    /// do a binary search, requires a sorted array
    IndexT BinarySearchIndex(const TYPE& elm) const;

private:
    /// check if index is in valid range, and grow array if necessary
    void CheckIndex(IndexT index);
    /// destroy an element (call destructor without freeing memory)
    void Destroy(TYPE* elm);
    /// copy content
    void Copy(const Array<TYPE>& src);
    /// delete content
    void Delete();
    /// grow array
    void Grow();
    /// grow array to target size
    void GrowTo(SizeT newCapacity);
    /// move elements, grows array if needed
    void Move(IndexT fromIndex, IndexT toIndex);

    SizeT grow;                 // grow by this number of elements if array exhausted
    SizeT capacity;             // number of elements allocated
    SizeT size;                 // number of elements in array
    TYPE* elements;             // pointer to element array
};

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
Array<TYPE>::Array() :
    grow(16),
    capacity(0),
    size(0),
    elements(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
Array<TYPE>::Array(SizeT _capacity, SizeT _grow) :
    grow(_grow),
    capacity(_capacity),
    size(0)
{
    if (0 == this->grow)
    {
        this->grow = 16;
    }
    if (this->capacity > 0)
    {
        this->elements = n_new_array(TYPE, this->capacity);
    }
    else
    {
        this->elements = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
Array<TYPE>::Array(SizeT initialSize, SizeT _grow, const TYPE& initialValue) :
    grow(_grow),
    capacity(initialSize),
    size(initialSize)
{
    if (0 == this->grow)
    {
        this->grow = 16;
    }
    if (initialSize > 0)
    {
        this->elements = n_new_array(TYPE, this->capacity);
        IndexT i;
        for (i = 0; i < initialSize; i++)
        {
            this->elements[i] = initialValue;
        }
    }
    else
    {
        this->elements = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
Array<TYPE>::Copy(const Array<TYPE>& src)
{
    n_assert(0 == this->elements);

    this->grow = src.grow;
    this->capacity = src.capacity;
    this->size = src.size;
    if (this->capacity > 0)
    {
        this->elements = n_new_array(TYPE, this->capacity);
        IndexT i;
        for (i = 0; i < this->size; i++)
        {
            this->elements[i] = src.elements[i];
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
Array<TYPE>::Delete()
{
    this->grow = 0;
    this->capacity = 0;
    this->size = 0;
    if (this->elements)
    {
        n_delete_array(this->elements);
        this->elements = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
Array<TYPE>::Destroy(TYPE* elm)
{
    elm->~TYPE();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
Array<TYPE>::Array(const Array<TYPE>& rhs) :
    grow(0),
    capacity(0),
    size(0),
    elements(0)
{
    this->Copy(rhs);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
Array<TYPE>::~Array()
{
    this->Delete();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
Array<TYPE>::Reallocate(SizeT _capacity, SizeT _grow)
{
    this->Delete();
    this->grow = _grow;
    this->capacity = _capacity;
    this->size = 0;
    if (this->capacity > 0)
    {
        this->elements = n_new_array(TYPE, this->capacity);
    }
    else
    {
        this->elements = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void 
Array<TYPE>::operator=(const Array<TYPE>& rhs)
{
    if (this != &rhs)
    {
        this->Delete();
        this->Copy(rhs);
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
Array<TYPE>::GrowTo(SizeT newCapacity)
{
    TYPE* newArray = n_new_array(TYPE, newCapacity);
    if (this->elements)
    {
        // copy over contents
        IndexT i;
        for (i = 0; i < this->size; i++)
        {
            newArray[i] = this->elements[i];
        }

        // discard old array and update contents
        n_delete_array(this->elements);
    }
    this->elements  = newArray;
    this->capacity = newCapacity;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
Array<TYPE>::Grow()
{
    n_assert(this->grow > 0);
    SizeT growToSize;
    if (0 == this->capacity)
    {
        growToSize = grow;
    }
    else
    {
        growToSize = 2 * this->capacity;
    }
    this->GrowTo(growToSize);
}

//------------------------------------------------------------------------------
/**
    30-Jan-03   floh    serious bugfixes!
	07-Dec-04	jo		bugfix: neededSize >= this->capacity => neededSize > capacity	
*/
template<class TYPE>
void
Array<TYPE>::Move(IndexT fromIndex, IndexT toIndex)
{
    n_assert(this->elements);
    n_assert(fromIndex < this->size);

    // nothing to move?
    if (fromIndex == toIndex)
    {
        return;
    }

    // compute number of elements to move
    SizeT num = this->size - fromIndex;

    // check if array needs to grow
    SizeT neededSize = toIndex + num;
    while (neededSize > this->capacity)
    {
        this->Grow();
    }

    if (fromIndex > toIndex)
    {
        // this is a backward move
        IndexT i;
        for (i = 0; i < num; i++)
        {
            this->elements[toIndex + i] = this->elements[fromIndex + i];
        }

        // destroy remaining elements
        for (i = (fromIndex + i) - 1; i < this->size; i++)
        {
            this->Destroy(&(this->elements[i]));
        }
    }
    else
    {
        // this is a forward move
        int i;  // NOTE: this must remain signed for the following loop to work!!!
        for (i = num - 1; i >= 0; --i)
        {
            this->elements[toIndex + i] = this->elements[fromIndex + i];
        }

        // destroy freed elements
        for (i = int(fromIndex); i < int(toIndex); i++)
        {
            this->Destroy(&(this->elements[i]));
        }
    }

    // adjust array size
    this->size = toIndex + num;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
Array<TYPE>::Append(const TYPE& elm)
{
    // grow allocated space if exhausted
    if (this->size == this->capacity)
    {
        this->Grow();
    }
    n_assert(this->elements);
    this->elements[this->size++] = elm;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
Array<TYPE>::AppendArray(const Array<TYPE>& rhs)
{
    IndexT i;
    SizeT num = rhs.Size();
    for (i = 0; i < num; i++)
    {
        this->Append(rhs[i]);
    }
}

//------------------------------------------------------------------------------
/**
    Make room for N new elements at the end of the array, and return a pointer 
    to the start of the reserved area. This can be (carefully!) used as a fast 
    shortcut to fill the array directly with data.
*/
template<class TYPE>
void
Array<TYPE>::Reserve(SizeT num)
{
    n_assert(num > 0);
    SizeT maxElement = this->size + num;
    while (maxElement >= this->capacity)
    {
        this->Grow();
    }
    n_assert(this->elements);
    this->size += num;
}

//------------------------------------------------------------------------------
/**
    This will check if the provided index is in the valid range. If it is
    not the array will be grown to that index.
*/
template<class TYPE>
void
Array<TYPE>::CheckIndex(IndexT index)
{
    if (index >= this->size)
    {
        // grow array if necessary
        if (index >= this->capacity)
        {
            n_assert(this->grow > 0);
            this->GrowTo(index + this->grow);
        }
        // update number of contained elements
        this->size = index + 1;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
Array<TYPE>::Set(IndexT index, const TYPE& elm)
{
    // make sure the array is big enough
    this->CheckIndex(index);
    this->elements[index] = elm;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
SizeT
Array<TYPE>::Size() const
{
    return this->size;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
SizeT
Array<TYPE>::Capacity() const
{
    return this->capacity;
}

//------------------------------------------------------------------------------
/**
    Access an element. This method may grow the array if the index is
    outside the array range.
*/
template<class TYPE>
TYPE&
Array<TYPE>::At(IndexT index)
{
    this->CheckIndex(index);
    return this->elements[index];
}

//------------------------------------------------------------------------------
/**
    Access an element. This method will NOT grow the array, and instead do
    a range check, which may throw an assertion.
*/
template<class TYPE>
TYPE&
Array<TYPE>::operator[](IndexT index) const
{
    n_assert(this->elements && (index < this->size));
    return this->elements[index];
}

//------------------------------------------------------------------------------
/**
    The equality operator returns true if all elements are identical. The
    TYPE class must support the equality operator.
*/
template<class TYPE>
bool
Array<TYPE>::operator==(const Array<TYPE>& rhs) const
{
    if (rhs.Size() == this->Size())
    {
        IndexT i;
        SizeT num = this->Size();
        for (i = 0; i < num; i++)
        {
            if (!(this->elements[i] == rhs.elements[i]))
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
    The inequality operator returns true if at least one element in the 
    array is different, or the array sizes are different.
*/
template<class TYPE>
bool
Array<TYPE>::operator!=(const Array<TYPE>& rhs) const
{
    return !(*this == rhs);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
TYPE&
Array<TYPE>::Front() const
{
    n_assert(this->elements && (this->size > 0));
    return this->elements[0];
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
TYPE&
Array<TYPE>::Back() const
{
    n_assert(this->elements && (this->size > 0));
    return this->elements[this->size - 1];
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
bool 
Array<TYPE>::IsEmpty() const
{
    return (this->size == 0);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
Array<TYPE>::EraseIndex(IndexT index)
{
    n_assert(this->elements && (index < this->size));
    if (index == (this->size - 1))
    {
        // special case: last element
        this->Destroy(&(this->elements[index]));
        this->size--;
    }
    else
    {
        this->Move(index + 1, index);
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
typename Array<TYPE>::Iterator
Array<TYPE>::Erase(typename Array<TYPE>::Iterator iter)
{
    n_assert(this->elements && (iter >= this->elements) && (iter < (this->elements + this->size)));
    this->EraseIndex(IndexT(iter - this->elements));
    return iter;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
Array<TYPE>::Insert(IndexT index, const TYPE& elm)
{
    n_assert(index <= this->size);
    if (index == this->size)
    {
        // special case: append element to back
        this->Append(elm);
    }
    else
    {
        this->Move(index, index + 1);
        this->elements[index] = elm;
    }
}

//------------------------------------------------------------------------------
/**
    The current implementation of this method does not shrink the 
    preallocated space. It simply sets the array size to 0.
*/
template<class TYPE>
void
Array<TYPE>::Clear()
{
    IndexT i;
    for (i = 0; i < this->size; i++)
    {
        this->Destroy(&(this->elements[i]));
    }
    this->size = 0;
}

//------------------------------------------------------------------------------
/**
    This is identical with Clear(), but does NOT call destructors (it just
    resets the size member. USE WITH CARE!
*/
template<class TYPE>
void
Array<TYPE>::Reset()
{
    this->size = 0;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
typename Array<TYPE>::Iterator
Array<TYPE>::Begin() const
{
    return this->elements;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
typename Array<TYPE>::Iterator
Array<TYPE>::End() const
{
    return this->elements + this->size;
}

//------------------------------------------------------------------------------
/**
    Find element in array, return iterator, or 0 if element not
    found.

    @param  elm     element to find
    @return         element iterator, or 0 if not found
*/
template<class TYPE>
typename Array<TYPE>::Iterator
Array<TYPE>::Find(const TYPE& elm) const
{
    IndexT index;
    for (index = 0; index < this->size; index++)
    {
        if (this->elements[index] == elm)
        {
            return &(this->elements[index]);
        }
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    Find element in array, return element index, or InvalidIndex if element not
    found.

    @param  elm     element to find
    @return         index to element, or InvalidIndex if not found
*/
template<class TYPE>
IndexT
Array<TYPE>::FindIndex(const TYPE& elm) const
{
    IndexT index;
    for (index = 0; index < this->size; index++)
    {
        if (this->elements[index] == elm)
        {
            return index;
        }
    }
    return InvalidIndex;
}

//------------------------------------------------------------------------------
/**
    Fills an array range with the given element value. Will grow the
    array if necessary

    @param  first   index of first element to start fill
    @param  num     num elements to fill
    @param  elm     fill value
*/
template<class TYPE>
void
Array<TYPE>::Fill(IndexT first, SizeT num, const TYPE& elm)
{
    if ((first + num) > this->size)
    {
        this->GrowTo(first + num);
    }
    IndexT i;
    for (i = first; i < (first + num); i++)
    {
        this->elements[i] = elm;
    }
}

//------------------------------------------------------------------------------
/**
    Returns a new array with all element which are in rhs, but not in this.
    Carefull, this method may be very slow with large arrays!

    @todo this method is broken, check test case to see why!
*/
template<class TYPE>
Array<TYPE>
Array<TYPE>::Difference(const Array<TYPE>& rhs)
{
    Array<TYPE> diff;
    IndexT i;
    SizeT num = rhs.Size();
    for (i = 0; i < num; i++)
    {
        if (0 == this->Find(rhs[i]))
        {
            diff.Append(rhs[i]);
        }
    }
    return diff;
}

//------------------------------------------------------------------------------
/**
    Sorts the array. This just calls the STL sort algorithm.
*/
template<class TYPE>
void
Array<TYPE>::Sort()
{
    std::sort(this->Begin(), this->End());
}

//------------------------------------------------------------------------------
/**
    Does a binary search on the array, returns the index of the identical
    element, or InvalidIndex if not found
*/
template<class TYPE>
IndexT
Array<TYPE>::BinarySearchIndex(const TYPE& elm) const
{
    SizeT num = this->Size();
    if (num > 0)
    {
        IndexT half;
        IndexT lo = 0;
	    IndexT hi = num - 1;
	    IndexT mid;
        while (lo <= hi) 
        {
            if (0 != (half = num/2)) 
            {
                mid = lo + ((num & 1) ? half : (half - 1));
                if (elm < this->elements[mid])
                {
                    hi = mid - 1;
                    num = num & 1 ? half : half - 1;
                } 
                else if (elm > this->elements[mid]) 
                {
                    lo = mid + 1;
                    num = half;
                } 
                else
                {
                    return mid;
                }
            } 
            else if (0 != num) 
            {
                if (elm != this->elements[lo])
                {
                    return InvalidIndex;
                }
                else      
                {
                    return lo;
                }
            } 
            else 
            {
                break;
            }
        }
    }
    return InvalidIndex;
}

//------------------------------------------------------------------------------
/**
    This inserts the element into a sorted array. In the current
    implementation this is a slow operation O(n). This should be
    optimized to O(log n).
*/
template<class TYPE>
void
Array<TYPE>::InsertSorted(const TYPE& elm)
{
    if (0 == this->Size())
    {
        // empty shortcut
        this->Append(elm);
    }
    else if (elm < this->Front())
    {
        // front shortcut
        this->Insert(0, elm);
    }
    else if (elm > this->Back())
    {
        // back shortcut
        this->Append(elm);
    }
    else
    {
        // default case
        IndexT i;
        for (i = 0; i < this->Size(); i++)
        {
            if (elm <= (*this)[i])
            {
                this->Insert(i, elm);
                break;
            }
        }
    }
}

} // namespace Core
//------------------------------------------------------------------------------
#endif
