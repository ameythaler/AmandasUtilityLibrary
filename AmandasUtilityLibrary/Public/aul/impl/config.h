#pragma once

//////////////////////////////////////////////////////////////////////////
// General error and exception handling
//////////////////////////////////////////////////////////////////////////
#define AUL_DEBUG_SAFETY_CHECKS 1

//////////////////////////////////////////////////////////////////////////
// Debug safety overrides
//////////////////////////////////////////////////////////////////////////
#define AUL_DEBUG_OVERRIDE_CONNECTION 0

//////////////////////////////////////////////////////////////////////////
// SIMD Support
//////////////////////////////////////////////////////////////////////////
#define AUL_USE_SSE 1
#define AUL_USE_SSE2 2
#define AUL_USE_SSE3 3
#define AUL_USE_SSSE3 4
#define AUL_USE_SSE4 5
#define AUL_USE_AVX 6
#define AUL_USE_AVX2 7
#define AUL_USE_AVX_512 8