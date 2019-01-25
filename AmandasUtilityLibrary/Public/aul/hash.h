/** Hash.h
*
* Implementation of hashing functions.
*
* FNV-1a hash constants from http://www.isthe.com/chongo/tech/comp/fnv/
* FNV-1a implementation based on http://create.stephan-brumme.com/fnv-hash/
*/
#pragma once

#include <stdint.h>
#include "aul/types.h"

namespace aul::hash
{
    const uint32 FNV_PRIME_32 = 0x1000193;
    const uint32 FNV_SEED_32 = 0x811C9DC5;
    const uint64 FNV_PRIME_64 = 0x100000001B3;
    const uint64 FNV_SEED_64 = 0x14650FB0739D0383;

    inline uint32 fnv32(byte data, uint32 hash = FNV_SEED_32)
    {
        return (data ^ hash) * FNV_PRIME_32;
    }

    inline uint32 fnv32(const byte* data, size_t dataSize, uint32 hash = FNV_SEED_32)
    {
        while (dataSize--)
            hash = fnv32(*data++, hash);
        return hash;
    }

    inline uint32 fnv32(const char* data, size_t dataSize, uint32 hash = FNV_SEED_32)
    {
        return fnv32((byte*)data, dataSize, hash);
    }

    inline uint32 fnv32(const wchar_t* data, size_t dataSize)
    {
        return fnv32((byte*)data, dataSize);
    }

    inline uint32 fnv32(const char16_t* data, size_t dataSize)
    {
        return fnv32((byte*)data, dataSize);
    }

    inline uint32 fnv32(const wide_string& str)
    {
        return fnv32(str.c_str(), str.length() * sizeof(wchar_t));
    }

    inline uint32 fnv32(const mb_string& str)
    {
        return fnv32(str.c_str(), str.length() * sizeof(char));
    }

    inline uint32 fnv32(const wide_string16& str)
    {
        return fnv32(str.c_str(), str.length() * sizeof(char16_t));
    }

    inline uint64 fnv64(byte data, uint64 hash = FNV_SEED_64)
    {
        return (data ^ hash) * FNV_PRIME_64;
    }

    inline uint64 fnv64(const byte* data, size_t dataSize, uint64 hash = FNV_SEED_64)
    {
        while (dataSize--)
            hash = fnv64(*data++, hash);
        return hash;
    }

    inline uint64 fnv64(const char* data, size_t dataSize, uint64 hash = FNV_SEED_64)
    {
        return fnv64((byte*)data, dataSize, hash);
    }

    inline uint64 fnv64(const wchar_t* data, size_t dataSize)
    {
        return fnv64((byte*)data, dataSize);
    }

    inline uint64 fnv64(const char16_t* data, size_t dataSize)
    {
        return fnv64((byte*)data, dataSize);
    }

    inline uint64 fnv64(const wide_string& str)
    {
        return fnv64(str.c_str(), str.length() * sizeof(wchar_t));
    }

    inline uint64 fnv64(const mb_string& str)
    {
        return fnv64(str.c_str(), str.length() * sizeof(char));
    }

    inline uint64 fnv64(const wide_string16& str)
    {
        return fnv64(str.c_str(), str.length() * sizeof(char16_t));
    }
}