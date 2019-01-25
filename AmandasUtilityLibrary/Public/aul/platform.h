#pragma once

#include "aul/impl/config.h"

#if _MSC_VER
#pragma warning(disable: 4005)
#endif

// Compiler and Architecture
#define AUL_LLVM 0
#define AUL_MSVC 0
#define AUL_GCC 0

#define AUL_32BIT 0
#define AUL_64BIT 0
#define AUL_X86 0
#define AUL_X64 0

#if defined(__llvm__)

#define AUL_LLVM 1
#if defined(__i386__)
#define AUL_32BIT 1
#define AUL_X86 1
#elif defined(__x86_64__)
#define AUL_64BIT 1
#define AUL_X64 1
#endif

#elif defined(_MSC_VER)

#define AUL_MSVC 1
#if defined(_M_IX86)
#define AUL_32BIT 1
#define AUL_X86 1
#elif defined(_M_X64)
#define AUL_64BIT 1
#define AUL_X64 1
#endif

#elif defined(__GNUC__)

#define AUL_GCC 1
#if defined(__i386__)
#define AUL_32BIT 1
#define AUL_X86 1
#elif defined(__x86_64__)
#define AUL_64BIT 1
#define AUL_X64 1
#endif

#endif

#define AUL_STD AUL_LLVM | AUL_GCC

// Operating System and Debug
#define AUL_WIN 0
#define AUL_LINUX 0
#define AUL_MAC 0

#define AUL_DEBUG 0

#if defined(_WIN32) || defined(WIN32)

#define AUL_WIN 1
#if AUL_MSVC
#define _CRT_SECURE_NO_WARNINGS
#endif
#if defined(_DEBUG) || (DEBUG)
#define AUL_DEBUG 1
#endif

#elif defined(__linux)

#define AUL_LINUX 1
#ifdef DEBUG
#define AUL_DEBUG 1
#endif

#elif defined(__APPLE__)

#define AUL_MAC 1
#if DEBUG
#define AUL_DEBUG 1
#endif

#endif

#define AUL_POSIX AUL_MAC | AUL_LINUX

// Compatibility
#define AUL_NOEXCEPT noexcept

#if AUL_MSVC && _MSC_VER < 1910
#define AUL_NOEXCEPT throw()
#endif

// Unicode
#define AUL_UNICODE 0

#if defined(_UNICODE) || defined(UNICODE)
#define AUL_UNICODE 1
#endif

#define AUL_MATRIX_LAYOUT_GL 0
#define AUL_MATRIX_LAYOUT_COLUMN_MAJOR 0
#define AUL_MATRIX_LAYOUT_D3D 1
#define AUL_MATRIX_LAYOUT_ROW_MAJOR 1

#if _MSC_VER
#pragma warning(default: 4005)
#endif

#if AUL_MSVC

#if defined(_CPPUNWIND) || defined(__cpp_exceptions) || defined(__EXCEPTIONS)
#define AUL_THROW_EXCEPTIONS 1
#else // defined(_CPPUNWIND) || defined(__cpp_exceptions) || defined(__EXCEPTIONS)
#define AUL_THROW_EXCEPTIONS 0
#endif // defined(_CPPUNWIND) || defined(__cpp_exceptions) || defined(__EXCEPTIONS)

#elif AUL_STD

#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
#define AUL_THROW_EXCEPTIONS 1
#else // defined(__cpp_exceptions) || defined(__EXCEPTIONS)
#define AUL_THROW_EXCEPTIONS 0
#endif // defined(__cpp_exceptions) || defined(__EXCEPTIONS)

#endif