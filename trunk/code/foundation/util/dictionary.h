#pragma once
#ifndef UTIL_DICTIONARY_H
#define UTIL_DICTIONARY_H
//------------------------------------------------------------------------------
/**
    @class Util::Dictionary
  
    A collection of key/value pairs with quick value retrieval
    by key at roughly O(log n). 

    Internally the dictionary is implemented as a sorted array.

    On insertion performance:
    Key/value pairs can be added at any time with the Add() methods.
    Internally, lazy evaluation is used to keep the array sorted 
    only when it needs to be sorted. Thus adding many keys
    is very fast, but the first search may be slow because that's
    when the sorting happens. 

    (C) 2006 Radon Labs GmbH
*/    
#include "util/array.h"
#include "util/keyvaluepair.h"

//------------------------------------------------------------------------------
namespace Util
{
template<class KEYTYPE, class VALUETYPE> class Dictionary
{
public:
    /// default constructor
    Dictionary();
    /// copy constructor
    Dictionary(const Dictionary<KEYTYPE, VALUETYPE>& rhs);
    /// assignment operator
    void operator=(const Dictionary<KEYTYPE, VALUETYPE>& rhs);
    /// read/write [] operator
    VALUETYPE& operator[](const KEYTYPE& key);
    /// read-only [] operator
    const VALUETYPE& operator[](const KEYTYPE& key) const;
    /// return number of key/value pairs in the dictionary
    SizeT Size() const;
    /// clear the dictionary
    void Clear();
    /// return true if empty
    bool IsEmpty() const;
    /// add a key/value pair
    void Add(const KeyValuePair<KEYTYPE, VALUETYPE>& kvp);
    /// add a key and associated value
    void Add(const KEYTYPE& key, const VALUETYPE& value);
    /// erase a key and its associated value
    void Erase(const KEYTYPE& key);
    /// erase a key at index
    void EraseAtIndex(IndexT index);
    /// find index of key/value pair (InvalidIndex if doesn't exist)
    IndexT FindIndex(const KEYTYPE& key) const;
    /// return true if key exists in the array
    bool Contains(const KEYTYPE& key) const;
    /// get a key at given index
    const KEYTYPE& KeyAtIndex(IndexT index) const;
    /// access to value at given index
    VALUETYPE& ValueAtIndex(IndexT index);
    /// get a value at given index
    const VALUETYPE& ValueAtIndex(IndexT index) const;
    /// get key/value pair at index
    KeyValuePair<KEYTYPE, VALUETYPE>& KeyValuePairAtIndex(IndexT index) const;
    /// get all keys as array (slow)
    Array<KEYTYPE> KeysAsArray() const;
    /// get all values as array (slow)
    Array<VALUETYPE> ValuesAsArray() const;

protected:
    /// make sure the key value pair array is sorted
    void SortIfDirty() const;

    Array<KeyValuePair<KEYTYPE, VALUETYPE> > keyValuePairs;
    bool dirty;
};

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
Dictionary<KEYTYPE, VALUETYPE>::Dictionary() :
    dirty(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
Dictionary<KEYTYPE, VALUETYPE>::Dictionary(const Dictionary<KEYTYPE, VALUETYPE>& rhs) :
    keyValuePairs(rhs.keyValuePairs),
    dirty(rhs.dirty)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> void
Dictionary<KEYTYPE, VALUETYPE>::operator=(const Dictionary<KEYTYPE, VALUETYPE>& rhs)
{
    this->keyValuePairs = rhs.keyValuePairs;
    this->dirty = rhs.dirty;
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> void
Dictionary<KEYTYPE, VALUETYPE>::Clear()
{
    this->keyValuePairs.Clear();
    this->dirty = false;
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> SizeT
Dictionary<KEYTYPE, VALUETYPE>::Size() const
{
    return this->keyValuePairs.Size();
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> bool
Dictionary<KEYTYPE, VALUETYPE>::IsEmpty() const
{
    return (0 == this->keyValuePairs.Size());
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> void
Dictionary<KEYTYPE, VALUETYPE>::SortIfDirty() const
{
    if (this->dirty)
    {
        // cast this to no-const
        Dictionary<KEYTYPE, VALUETYPE>* noConstThis = const_cast<Dictionary<KEYTYPE, VALUETYPE>*>(this);
        noConstThis->keyValuePairs.Sort();
        noConstThis->dirty = false;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> void
Dictionary<KEYTYPE, VALUETYPE>::Add(const KeyValuePair<KEYTYPE, VALUETYPE>& kvp)
{
    this->keyValuePairs.Append(kvp);
    this->dirty = true;
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> void
Dictionary<KEYTYPE, VALUETYPE>::Add(const KEYTYPE& key, const VALUETYPE& value)
{
    KeyValuePair<KEYTYPE, VALUETYPE> kvp(key, value);
    this->keyValuePairs.Append(kvp);
    this->dirty = true;
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> void
Dictionary<KEYTYPE, VALUETYPE>::Erase(const KEYTYPE& key)
{
    this->SortIfDirty();
    IndexT eraseIndex = this->keyValuePairs.BinarySearchIndex(key);
    n_assert(InvalidIndex != eraseIndex);
    this->keyValuePairs.EraseIndex(eraseIndex);
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> void
Dictionary<KEYTYPE, VALUETYPE>::EraseAtIndex(IndexT index)
{
    this->SortIfDirty();
    this->keyValuePairs.EraseIndex(index);
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> IndexT
Dictionary<KEYTYPE, VALUETYPE>::FindIndex(const KEYTYPE& key) const
{
    this->SortIfDirty();
    return this->keyValuePairs.BinarySearchIndex(key);
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> bool
Dictionary<KEYTYPE, VALUETYPE>::Contains(const KEYTYPE& key) const
{
    this->SortIfDirty();
    return (InvalidIndex != this->keyValuePairs.BinarySearchIndex(key));
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> const KEYTYPE&
Dictionary<KEYTYPE, VALUETYPE>::KeyAtIndex(IndexT index) const
{
    this->SortIfDirty();
    return this->keyValuePairs[index].Key();
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> VALUETYPE&
Dictionary<KEYTYPE, VALUETYPE>::ValueAtIndex(IndexT index)
{
    this->SortIfDirty();
    return this->keyValuePairs[index].Value();
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> const VALUETYPE&
Dictionary<KEYTYPE, VALUETYPE>::ValueAtIndex(IndexT index) const
{
    this->SortIfDirty();
    return this->keyValuePairs[index].Value();
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> KeyValuePair<KEYTYPE, VALUETYPE>&
Dictionary<KEYTYPE, VALUETYPE>::KeyValuePairAtIndex(IndexT index) const
{
    this->SortIfDirty();
    return this->keyValuePairs[index];
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> VALUETYPE&
Dictionary<KEYTYPE, VALUETYPE>::operator[](const KEYTYPE& key)
{
    int keyValuePairIndex = this->FindIndex(key);
    n_assert(InvalidIndex != keyValuePairIndex);
    return this->keyValuePairs[keyValuePairIndex].Value();
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> const VALUETYPE&
Dictionary<KEYTYPE, VALUETYPE>::operator[](const KEYTYPE& key) const
{
    int keyValuePairIndex = this->FindIndex(key);
    n_assert(InvalidIndex != keyValuePairIndex);
    return this->keyValuePairs[keyValuePairIndex].Value();
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> Array<VALUETYPE>
Dictionary<KEYTYPE, VALUETYPE>::ValuesAsArray() const
{
    this->SortIfDirty();
    Array<VALUETYPE> result(this->Size(),this->Size());
    IndexT i;
    for (i = 0; i < this->keyValuePairs.Size(); i++)
    {
        result.Append(this->keyValuePairs[i].Value());
    }
    return result;
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> Array<KEYTYPE>
Dictionary<KEYTYPE, VALUETYPE>::KeysAsArray() const
{
    this->SortIfDirty();
    Array<KEYTYPE> result(this->Size(),this->Size());
    IndexT i;
    for (i = 0; i < this->keyValuePairs.Size(); i++)
    {
        result.Append(this->keyValuePairs[i].Key());
    }
    return result;
}

} // namespace Util
//------------------------------------------------------------------------------
#endif