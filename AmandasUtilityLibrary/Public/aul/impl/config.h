#pragma once

//////////////////////////////////////////////////////////////////////////
// General error and exception handling
//////////////////////////////////////////////////////////////////////////

// Definitions:

// Settings:
#define AUL_DEBUG_SAFETY_CHECKS 1

//////////////////////////////////////////////////////////////////////////
// Debug safety overrides
//////////////////////////////////////////////////////////////////////////

// Definitions:

// Settings:
#define AUL_DEBUG_OVERRIDE_CONNECTION 0

//////////////////////////////////////////////////////////////////////////
// Math settings
//////////////////////////////////////////////////////////////////////////

// Definitions:
#define AUL_LEFT_HANDED 0
#define AUL_RIGHT_HANDED 1
#define AUL_Y_UP 1 // Y-up, X-right, Z-forward
#define AUL_Z_UP 2 // Left-Handed: Z-up, Y-right, X-forward | Right-Handed: Z-up, X-right, Y-forward
#define AUL_VECTOR_RGBA 0
#define AUL_VECTOR_ARGB 1


// Settings:
#define AUL_USE_COORDINATE_HANDEDNESS AUL_LEFT_HANDED
#define AUL_USE_MATRIX_MULTIPLICATION_ORDER AUL_LEFT_HANDED
#define AUL_USE_UP_VECTOR AUL_Y_UP
#define AUL_USE_MATRIX_MULTI_LINE_STRING_REPRESENTATION 1
#define AUL_USE_VECTOR_COLOR_ALIAS AUL_VECTOR_RGBA

//////////////////////////////////////////////////////////////////////////
// SIMD support
//////////////////////////////////////////////////////////////////////////

// Definitions:

// Settings:
#define AUL_USE_SSE 1
#define AUL_USE_SSE2 2
#define AUL_USE_SSE3 3
#define AUL_USE_SSSE3 4
#define AUL_USE_SSE4 5
#define AUL_USE_AVX 6
#define AUL_USE_AVX2 7
#define AUL_USE_AVX_512 8