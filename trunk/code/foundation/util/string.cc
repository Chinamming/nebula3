//------------------------------------------------------------------------------
//  string.cc
//  (C) 2006 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "util/string.h"

namespace Util
{
Memory::Heap* String::ObjectHeap = 0;

//------------------------------------------------------------------------------
/**
*/
void __cdecl
String::Format(const char* fmtString, ...)
{
    va_list argList;
    va_start(argList, fmtString);
    char buf[4096]; // an 4 kByte buffer
    #if __WIN32__
        // need to use non-CRT thread safe function under Win32
        StringCchVPrintf(buf, sizeof(buf), fmtString, argList);
    #elif __WII__
		vsnprintf(buf, sizeof(buf), fmtString, argList);
    #else
        _vsnprintf(buf, sizeof(buf), fmtString, argList);
    #endif
    *this = buf;
    va_end(argList);
}

//------------------------------------------------------------------------------
/**
*/
void __cdecl
String::FormatArgList(const char* fmtString, va_list argList)
{
    char buf[4096]; // an 4 kByte buffer
    #if __WIN32__
        // need to use non-CRT thread safe function under Win32
        StringCchVPrintf(buf, sizeof(buf), fmtString, argList);
    #elif __WII__
		vsnprintf(buf, sizeof(buf), fmtString, argList);
    #else
        _vsnprintf(buf, sizeof(buf), fmtString, argList);
    #endif
    *this = buf;
}

//------------------------------------------------------------------------------
/**
    Sets a new string content. This will handle all special cases and try
    to minimize heap allocations as much as possible.
*/
void
String::Set(const char* str, SizeT length)
{
    if (0 == str)
    {
        // a null string pointer is valid as a special case
        this->Delete();
        this->localBuffer[0] = 0;
    }
    else if ((0 == this->heapBuffer) && (length < LocalStringSize))
    {
        // the new contents fits into the local buffer,
        // however, if there is already an external buffer
        // allocated, we use the external buffer!
        this->Delete();
        Memory::Copy(str, this->localBuffer, length);
        this->localBuffer[length] = 0;
    }
    else if (length < this->heapBufferSize)
    {
        // the new contents fits into the existing buffer
        Memory::Copy(str, this->heapBuffer, length);
        this->heapBuffer[length] = 0;
        this->localBuffer[0] = 0;
    }
    else
    {
        // need to allocate bigger heap buffer
        this->Alloc(length + 1);
        Memory::Copy(str, this->heapBuffer, length);
        this->heapBuffer[length] = 0;
        this->localBuffer[0] = 0;
    }
    this->strLen = length;
}

//------------------------------------------------------------------------------
/**
*/
void
String::AppendRange(const char* append, SizeT length)
{
    n_assert(append);
    if (length > 0)
    {
        SizeT newLength = this->strLen + length;
        
        // if there is an external buffer allocated, we need to stay there
        // even if the resulting string is smaller then LOCALSTRINGSIZE,
        // a small string in an external buffer may be the result
        // of a call to Reserve()
        if ((0 == this->heapBuffer) && (newLength < LocalStringSize))
        {
            // the result fits into the local buffer
            Memory::Copy(append, this->localBuffer + this->strLen, length);
            this->localBuffer[newLength] = 0;
        }
        else if (newLength < this->heapBufferSize)
        {
            // the result fits into the existing buffer
            Memory::Copy(append, this->heapBuffer + this->strLen, length);
            this->heapBuffer[newLength] = 0;
        }
        else
        {
            // need to re-allocate
            this->Realloc(newLength + newLength / 2);
            Memory::Copy(append, this->heapBuffer + this->strLen, length);
            this->heapBuffer[newLength] = 0;
        }
        this->strLen = newLength;
    }
}

//------------------------------------------------------------------------------
/**
    Tokenize the string into a String array. 

    @param  whiteSpace      a string containing the whitespace characters
    @return                 a string array of tokens
*/
Array<String>
String::Tokenize(const String& whiteSpace) const
{
    Array<String> tokens;
    String str(*this);
    char* ptr = const_cast<char*>(str.AsCharPtr());
    const char* token;
    while (0 != (token = strtok(ptr, whiteSpace.AsCharPtr())))
    {
        tokens.Append(token);
        ptr = 0;
    }
    return tokens;
}

//------------------------------------------------------------------------------
/**
    Tokenize a string, but keeps the string within the fence-character
    intact. For instance for the sentence:

    He said: "I don't know."

    A Tokenize(" ", '"', tokens) would return:

    token 0:    He
    token 1:    said:
    token 2:    I don't know.
*/
Array<String>
String::Tokenize(const String& whiteSpace, char fence) const
{
    Array<String> tokens;
    String str(*this);
    char* ptr = const_cast<char*>(str.AsCharPtr());    
    char* end = ptr + strlen(ptr);
    while (ptr < end)
    {
        char* c;

        // skip white space
        while (*ptr && strchr(whiteSpace.AsCharPtr(), *ptr))
        {
            ptr++;
        }
        if (*ptr)
        {
            // check for fenced area
            if ((fence == *ptr) && (0 != (c = strchr(++ptr, fence))))
            {
                *c++ = 0;
                tokens.Append(ptr);
                ptr = c;
            }
            else if (0 != (c = strpbrk(ptr, whiteSpace.AsCharPtr())))
            {
                *c++ = 0;
                tokens.Append(ptr);
                ptr = c;
            }
            else
            {
                tokens.Append(ptr);
                break;
            }
        }
    }
    return tokens;
}

//------------------------------------------------------------------------------
/**
    Extract a substring range.
*/
String
String::ExtractRange(IndexT from, SizeT numChars) const
{
    n_assert(from <= this->strLen);
    n_assert((from + numChars) <= this->strLen);
    const char* str = this->AsCharPtr();
    String newString;
    newString.Set(&(str[from]), numChars);
    return newString;
}

//------------------------------------------------------------------------------
/**
    Extract a substring until the end of the original string.
*/
String
String::ExtractToEnd(IndexT fromIndex) const
{
    return this->ExtractRange(fromIndex, this->strLen - fromIndex);
}

//------------------------------------------------------------------------------
/**
    Terminates the string at the first occurance of one of the characters
    in charSet.
*/
void
String::Strip(const String& charSet)
{
    char* str = const_cast<char*>(this->AsCharPtr());
    char* ptr = strpbrk(str, charSet.AsCharPtr());
    if (ptr)
    {
        *ptr = 0;
    }
    this->strLen = (SizeT) strlen(this->AsCharPtr());
}

//------------------------------------------------------------------------------
/**
    Return the index of a substring, or InvalidIndex if not found.
*/
IndexT
String::FindStringIndex(const String& s, IndexT startIndex) const
{
    n_assert(startIndex < this->strLen);
    n_assert(s.IsValid());
    IndexT i;
    for (i = startIndex; i < this->strLen; i++)
    {
        if ((this->strLen - i) < s.strLen)
        {
            break;
        }

        if (strncmp(&(this->AsCharPtr()[i]), s.AsCharPtr(), s.strLen) == 0)
        {
            return i;
        }
    }
    return InvalidIndex;
}

//------------------------------------------------------------------------------
/**
    Return index of character in string, or InvalidIndex if not found.
*/
IndexT
String::FindCharIndex(char c, IndexT startIndex) const
{
    if (this->strLen > 0)
    {
        n_assert(startIndex < this->strLen);
        const char* ptr = strchr(this->AsCharPtr() + startIndex, c);
        if (ptr)
        {
            return IndexT(ptr - this->AsCharPtr());
        }
    }
    return InvalidIndex;
}

//------------------------------------------------------------------------------
/**
    Removes all characters in charSet from the left side of the string.
*/
void
String::TrimLeft(const String& charSet)
{
    n_assert(charSet.IsValid());
    if (this->IsValid())
    {
        SizeT charSetLen = charSet.strLen;
        IndexT thisIndex = 0;
        bool stopped = false;
        while (!stopped && (thisIndex < this->strLen))
        {
            IndexT charSetIndex;
            bool match = false;
            for (charSetIndex = 0; charSetIndex < charSetLen; charSetIndex++)
            {
                if ((*this)[thisIndex] == charSet[charSetIndex])
                {
                    // a match
                    match = true;
                    break;
                }
            }
            if (!match)
            {
                // stop if no match
                stopped = true;
            }
            else
            {
                // a match, advance to next character
                ++thisIndex;
            }
        }
        String trimmedString(&(this->AsCharPtr()[thisIndex]));
        *this = trimmedString;
    }
}

//------------------------------------------------------------------------------
/**
    Removes all characters in charSet from the right side of the string.
*/
void
String::TrimRight(const String& charSet)
{
    n_assert(charSet.IsValid());
    if (this->IsValid())
    {
        SizeT charSetLen = charSet.strLen;
        int thisIndex = this->strLen - 1;   // NOTE: may not be unsigned (thus not IndexT!)
        bool stopped = false;
        while (!stopped && (thisIndex >= 0))
        {
            IndexT charSetIndex;
            bool match = false;
            for (charSetIndex = 0; charSetIndex < charSetLen; charSetIndex++)
            {
                if ((*this)[thisIndex] == charSet[charSetIndex])
                {
                    // a match
                    match = true;
                    break;
                }
            }
            if (!match)
            {
                // stop if no match
                stopped = true;
            }
            else
            {
                // a match, advance to next character
                --thisIndex;
            }
        }
        String trimmedString;
        trimmedString.Set(this->AsCharPtr(), thisIndex + 1);
        *this = trimmedString;
    }
}

//------------------------------------------------------------------------------
/**
    Trim both sides of a string.
*/
void
String::Trim(const String& charSet)
{
    this->TrimLeft(charSet);
    this->TrimRight(charSet);
}

//------------------------------------------------------------------------------
/**
    Substitute every occurance of origStr with substStr.
*/
void
String::SubstituteString(const String& matchStr, const String& substStr)
{
    n_assert(matchStr.IsValid());

    const char* ptr = this->AsCharPtr();
    SizeT matchStrLen = matchStr.strLen;
    String dest;

    // walk original string for occurances of str
    const char* occur;
    while (0 != (occur = strstr(ptr, matchStr.AsCharPtr())))
    {
        // append string fragment until match
        dest.AppendRange(ptr, SizeT(occur - ptr));

        // append replacement string
        if (substStr.Length() > 0)
        {
            dest.Append(substStr);
        }

        // adjust source pointer
        ptr = occur + matchStrLen;
    }
    dest.Append(ptr);
    *this = dest;
}

//------------------------------------------------------------------------------
/**
    This converts an UTF-8 string to 8-bit-ANSI. Note that only characters
    in the range 0 .. 255 are converted, all other characters will be converted
    to a question mark.

    For conversion rules see http://www.cl.cam.ac.uk/~mgk25/unicode.html#utf-8
*/
void
String::UTF8toANSI()
{
    char* src = const_cast<char*>(this->AsCharPtr());
    char* dst = src;
    char c;
    while (0 != (c = *src++))
    {
        if (c >= 0x80)
        {
            if ((c & 0xE0) == 0xC0)
            {
                // a 2 byte sequence with 11 bits of information
                ushort wide = ((c & 0x1F) << 6) | (*src++ & 0x3F);
                if (wide > 0xff)
                {
                    c = '?';
                }
                else
                {
                    c = (char) wide;
                }
            }
            else if ((c & 0xF0) == 0xE0)
            {
                // a 3 byte sequence with 16 bits of information
                c = '?';
                src += 2;
            }
            else if ((c & 0xF8) == 0xF0)
            {
                // a 4 byte sequence with 21 bits of information
                c = '?';
                src += 3;
            }
            else if ((c & 0xFC) == 0xF8)
            {
                // a 5 byte sequence with 26 bits of information
                c = '?';
                src += 4;
            }
            else if ((c & 0xFE) == 0xFC)
            {
                // a 6 byte sequence with 31 bits of information
                c = '?';
                src += 5;
            }
        }
        *dst++ = c;
    }
    *dst = 0;
}

//------------------------------------------------------------------------------
/**
    Convert contained ANSI string to UTF-8 in place.
*/
void
String::ANSItoUTF8()
{
    n_assert(!this->IsEmpty());
    SizeT bufSize = this->strLen * 2 + 1;
    char* buffer = (char*) Memory::Alloc(Memory::ScratchHeap, bufSize);
    char* dstPtr = buffer;
    const char* srcPtr = this->AsCharPtr();
    unsigned char c;
    while (0 != (c = *srcPtr++))
    {
        // note: this only covers the 2 cases that the character
        // is between 0 and 127 and between 128 and 255
        if (c < 128)
        {
            *dstPtr++ = c;
        }
        else
        {
            *dstPtr++ = 192 + (c / 64);
            *dstPtr++ = 128 + (c % 64);
        }
    }
    *dstPtr = 0;
    this->SetCharPtr(buffer);
    Memory::Free(Memory::ScratchHeap, buffer);
}

//------------------------------------------------------------------------------
/**
    Return a String object containing the last directory of the path, i.e.
    a category.

    - 17-Feb-04     floh    fixed a bug when the path ended with a slash
*/
String
String::ExtractLastDirName() const
{
    String pathString(*this);
    char* lastSlash = pathString.GetLastSlash();
    
    // special case if path ends with a slash
    if (lastSlash)
    {
        if (0 == lastSlash[1])
        {
            *lastSlash = 0;
            lastSlash = pathString.GetLastSlash();
        }

        char* secLastSlash = 0;
        if (0 != lastSlash)
        {
            *lastSlash = 0; // cut filename
            secLastSlash = pathString.GetLastSlash();
            if (secLastSlash)
            {
                *secLastSlash = 0;
                return String(secLastSlash+1);
            }
        }
    }
    return "";
}

//------------------------------------------------------------------------------
/**
    Return a String object containing the part before the last
    directory separator.
    
    NOTE: I left my fix in that returns the last slash (or colon), this was 
    necessary to tell if a dirname is a normal directory or an assign.     

    - 17-Feb-04     floh    fixed a bug when the path ended with a slash
*/
String
String::ExtractDirName() const
{
    String pathString(*this);
    char* lastSlash = pathString.GetLastSlash();

    // special case if path ends with a slash
    if (lastSlash)
    {
        if (0 == lastSlash[1])
        {
            *lastSlash = 0;
            lastSlash = pathString.GetLastSlash();
        }
        if (lastSlash)
        {
            *++lastSlash = 0;
        }
    }
    pathString.strLen = (SizeT) strlen(pathString.AsCharPtr());
    return pathString;
}

//------------------------------------------------------------------------------
/**
    Pattern-matching, TCL-style.
*/
bool
String::MatchPattern(const String& string, const String& pattern)
{
    const char* str = string.AsCharPtr();
    const char* pat = pattern.AsCharPtr();

    char c2;
    bool always = true;
    while (always != false)
    {
        if (*pat == 0) 
        {
            if (*str == 0) return true;
            else           return false;
        }
        if ((*str == 0) && (*pat != '*')) return false;
        if (*pat=='*') 
        {
            pat++;
            if (*pat==0) return true;
            while (always)
            {
                if (String::MatchPattern(str, pat)) return true;
                if (*str==0) return false;
                str++;
            }
        }
        if (*pat=='?') goto match;
        if (*pat=='[') 
        {
            pat++;
            while (always) 
            {
                if ((*pat==']') || (*pat==0)) return false;
                if (*pat==*str) break;
                if (pat[1] == '-') 
                {
                    c2 = pat[2];
                    if (c2==0) return false;
                    if ((*pat<=*str) && (c2>=*str)) break;
                    if ((*pat>=*str) && (c2<=*str)) break;
                    pat+=2;
                }
                pat++;
            }
            while (*pat!=']') 
            {
                if (*pat==0) 
                {
                    pat--;
                    break;
                }
                pat++;
            }
            goto match;
        }
    
        if (*pat=='\\') 
        {
            pat++;
            if (*pat==0) return false;
        }
        if (*pat!=*str) return false;

match:
        pat++;
        str++;
    }
    // can't happen
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
String::ReplaceChars(const String& charSet, char replacement)
{
    n_assert(charSet.IsValid());
    char* ptr = const_cast<char*>(this->AsCharPtr());
    char c;
    while (0 != (c = *ptr))
    {
        if (strchr(charSet.AsCharPtr(), c))
        {
            *ptr = replacement;
        }
        ptr++;
    }
}

//------------------------------------------------------------------------------
/**
    Returns content as matrix44. Note: this method doesn't check whether the
    contents is actually a valid matrix44. Use the IsValidMatrix44() method
    for this!
*/
Math::matrix44
String::AsMatrix44() const
{
    Array<String> tokens = this->Tokenize(", \t");
    n_assert(tokens.Size() == 16);
    Math::matrix44 m(Math::float4(tokens[0].AsFloat(),  tokens[1].AsFloat(),  tokens[2].AsFloat(),  tokens[3].AsFloat()),
                     Math::float4(tokens[4].AsFloat(),  tokens[5].AsFloat(),  tokens[6].AsFloat(),  tokens[7].AsFloat()),
                     Math::float4(tokens[8].AsFloat(),  tokens[9].AsFloat(),  tokens[10].AsFloat(), tokens[11].AsFloat()),
                     Math::float4(tokens[12].AsFloat(), tokens[13].AsFloat(), tokens[14].AsFloat(), tokens[15].AsFloat()));
    return m;
}

//------------------------------------------------------------------------------
/**
*/
String
String::Concatenate(const Array<String>& strArray, const String& whiteSpace)
{
    String res;
    res.Reserve(256);
    IndexT i;
    SizeT num = strArray.Size();
    for (i = 0; i < num; i++)
    {
        res.Append(strArray[i]);
        if ((i + 1) < num)
        {
            res.Append(whiteSpace);
        }
    }
    return res;
}

} // namespace System
