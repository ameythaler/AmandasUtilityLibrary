#include "stdafx.h"
#include <aul/xform.h>
#include <aul/matrix3x3.h>
#include <aul/matrix4x4.h>

namespace aul
{
#define AUL_INTERNAL_ZERO_NON_DIAGONAL() m01 = m02 = m03 = m10 = m12 = m13 = m20 = m21 = m23 = m30 = m31 = m32 = scalar<T>::ZERO

    template<typename T>
    const xform<T> xform<T>::ZERO = xform<T>(vector3<T>::ZERO, vector3<T>::ZERO, vector3<T>::ZERO, vector3<T>::ZERO);

    template<typename T>
    const xform<T> xform<T>::IDENTITY = xform<T>(vector3<T>::ONE);

    template<typename T>
    const xform<T> xform<T>::NEGATIVE_IDENTITY = xform<T>(-vector3<T>::ONE);

#define AUL_INTERNAL_EXPLICIT_INST_DEF(T) template struct xform<T>

    AUL_INTERNAL_EXPLICIT_INST_DEF(float);
    AUL_INTERNAL_EXPLICIT_INST_DEF(double);

#undef AUL_INTERNAL_EXPLICIT_INST_DEF

    template<typename T>
    xform<T>::xform(const T* arr_data)
    {
        memcpy(data, arr_data, sizeof(T) * 16);
        x.w = y.w = z.w = scalar<T>::ZERO;
        w.w = scalar<T>::ONE;
    }

    template<typename T>
    xform<T>::xform(const matrix4x4<T>& rhs)
#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        : m00(rhs.m00), m01(rhs.m01), m02(rhs.m02), m03(scalar<T>::ZERO)
        , m10(rhs.m10), m11(rhs.m11), m12(rhs.m12), m13(scalar<T>::ZERO)
        , m20(rhs.m20), m21(rhs.m21), m22(rhs.m22), m23(scalar<T>::ZERO)
        , m30(rhs.m30), m31(rhs.m31), m32(rhs.m32), m33(scalar<T>::ONE)
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        : m00(rhs.m00), m01(rhs.m01), m02(rhs.m02), m03(rhs.m03)
        , m10(rhs.m10), m11(rhs.m11), m12(rhs.m12), m13(rhs.m13)
        , m20(rhs.m20), m21(rhs.m21), m22(rhs.m22), m23(rhs.m23)
        , m30(scalar<T>::ZERO), m31(scalar<T>::ZERO), m32(scalar<T>::ZERO), m33(scalar<T>::ONE)
#endif // AUL_USE_COORDINATE_HANDEDNESS
    { }

    template<typename T>
    xform<T>::xform(const matrix3x3<T>& rhs)
        : m00(rhs.m00), m01(rhs.m01), m02(rhs.m02), m03(scalar<T>::ZERO)
        , m10(rhs.m10), m11(rhs.m11), m12(rhs.m12), m13(scalar<T>::ZERO)
        , m20(rhs.m20), m21(rhs.m21), m22(rhs.m22), m23(scalar<T>::ZERO)
        , m30(scalar<T>::ZERO), m31(scalar<T>::ZERO), m32(scalar<T>::ZERO), m33(scalar<T>::ONE)
    { }

    template<typename T>
    xform<T>& xform<T>::operator=(const T* arr_data)
    {
        memcpy(data, arr_data, sizeof(T) * 16);
        x.w = y.w = z.w = scalar<T>::ZERO;
        w.w = scalar<T>::ONE;
        return *this;
    }

    template<typename T>
    xform<T>& xform<T>::operator=(const vector3<T>& diagonal)
    {
        m00 = diagonal.x;
        m11 = diagonal.y;
        m22 = diagonal.z;
        m33 = scalar<T>::ONE;
        AUL_INTERNAL_ZERO_NON_DIAGONAL();
        return *this;
    }

    template<typename T>
    xform<T>& xform<T>::operator=(const matrix3x3<T>& rhs)
    {
        x.xyz = rhs.x;
        y.xyz = rhs.y;
        z.xyz = rhs.z;
        w = vector4<T>::W_AXIS;
        x.w = y.w = z.w = scalar<T>::ZERO;
        return *this;
    }

    template<typename T>
    xform<T>& xform<T>::operator=(const matrix4x4<T>& rhs)
    {
        x.xyz = rhs.x.xyz;
        y.xyz = rhs.y.xyz;
        z.xyz = rhs.z.xyz;
        w.xyz = rhs.w.xyz;
        x.w = y.w = z.w = scalar<T>::ZERO;
        w.w = scalar<T>::ONE;
        return *this;
    }

    template<typename T>
    xform<T> xform<T>::operator*(const xform<T>& rhs) const
    {
#if AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_LEFT_HANDED

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return xform<T>(
            // row 0
            m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20,
            m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21,
            m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22,
            // row 1
            m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20,
            m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21,
            m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22,
            // row 2
            m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20,
            m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21,
            m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22,
            // row 3
            m30 * rhs.m00 + m31 * rhs.m10 + m32 * rhs.m20 + rhs.m30,
            m30 * rhs.m01 + m31 * rhs.m11 + m32 * rhs.m21 + rhs.m31,
            m30 * rhs.m02 + m31 * rhs.m12 + m32 * rhs.m22 + rhs.m32
            );
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return xform<T>(
            // row 0
            rhs.m00 * m00 + rhs.m01 * m10 + rhs.m02 * m20,
            rhs.m00 * m01 + rhs.m01 * m11 + rhs.m02 * m21,
            rhs.m00 * m02 + rhs.m01 * m12 + rhs.m02 * m22,
            rhs.m00 * m03 + rhs.m01 * m13 + rhs.m02 * m23 + rhs.m03,
            // row 1
            rhs.m10 * m00 + rhs.m11 * m10 + rhs.m12 * m20,
            rhs.m10 * m01 + rhs.m11 * m11 + rhs.m12 * m21,
            rhs.m10 * m02 + rhs.m11 * m12 + rhs.m12 * m22,
            rhs.m10 * m03 + rhs.m11 * m13 + rhs.m12 * m23 + rhs.m13,
            // row 2
            rhs.m20 * m00 + rhs.m21 * m10 + rhs.m22 * m20,
            rhs.m20 * m01 + rhs.m21 * m11 + rhs.m22 * m21,
            rhs.m20 * m02 + rhs.m21 * m12 + rhs.m22 * m22,
            rhs.m20 * m03 + rhs.m21 * m13 + rhs.m22 * m23 + rhs.m23
            );
#endif // AUL_USE_COORDINATE_HANDEDNESS

#elif AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_RIGHT_HANDED

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return xform<T>(
            // row 0
            rhs.m00 * m00 + rhs.m01 * m10 + rhs.m02 * m20,
            rhs.m00 * m01 + rhs.m01 * m11 + rhs.m02 * m21,
            rhs.m00 * m02 + rhs.m01 * m12 + rhs.m02 * m22,
            // row 1
            rhs.m10 * m00 + rhs.m11 * m10 + rhs.m12 * m20,
            rhs.m10 * m01 + rhs.m11 * m11 + rhs.m12 * m21,
            rhs.m10 * m02 + rhs.m11 * m12 + rhs.m12 * m22,
            // row 2
            rhs.m20 * m00 + rhs.m21 * m10 + rhs.m22 * m20,
            rhs.m20 * m01 + rhs.m21 * m11 + rhs.m22 * m21,
            rhs.m20 * m02 + rhs.m21 * m12 + rhs.m22 * m22,
            // row 3
            rhs.m30 * m00 + rhs.m31 * m10 + rhs.m32 * m20 + m30,
            rhs.m30 * m01 + rhs.m31 * m11 + rhs.m32 * m21 + m31,
            rhs.m30 * m02 + rhs.m31 * m12 + rhs.m32 * m22 + m32
            );
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return xform<T>(
            // row 0
            m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20,
            m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21,
            m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22,
            m00 * rhs.m03 + m01 * rhs.m13 + m02 * rhs.m23 + m03,
            // row 1
            m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20,
            m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21,
            m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22,
            m10 * rhs.m03 + m11 * rhs.m13 + m12 * rhs.m23 + m13,
            // row 2
            m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20,
            m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21,
            m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22,
            m20 * rhs.m03 + m21 * rhs.m13 + m22 * rhs.m23 + m23
            );
#endif // AUL_USE_COORDINATE_HANDEDNESS

#endif // AUL_USE_MATRIX_MULTIPLICATION_ORDER
    }

    template<typename T>
    vector4<T> xform<T>::operator*(const vector4<T>& rhs) const
    {
#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return vector4<T>(m00 * rhs.x + m10 * rhs.y + m20 * rhs.z + m30 * rhs.w,
            m01 * rhs.x + m11 * rhs.y + m21 * rhs.z + m31 * rhs.w,
            m02 * rhs.x + m12 * rhs.y + m22 * rhs.z + m32 * rhs.w,
            rhs.w);
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return vector4<T>(m00 * rhs.x + m01 * rhs.y + m02 * rhs.z + m03 * rhs.w,
            m10 * rhs.x + m11 * rhs.y + m12 * rhs.z + m13 * rhs.w,
            m20 * rhs.x + m21 * rhs.y + m22 * rhs.z + m23 * rhs.w,
            rhs.w);
#endif // AUL_USE_COORDINATE_HANDEDNESS
    }

    template<typename T>
    xform<T>& xform<T>::operator*=(const xform<T>& rhs)
    {
#if AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_LEFT_HANDED

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        // row 0
        T t00 = m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20;
        T t01 = m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21;
        T t02 = m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22;
        // row 1
        T t10 = m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20;
        T t11 = m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21;
        T t12 = m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22;
        // row 2
        T t20 = m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20;
        T t21 = m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21;
        T t22 = m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22;
        // row 3
        T t30 = m30 * rhs.m00 + m31 * rhs.m10 + m32 * rhs.m20 + rhs.m30;
        T t31 = m30 * rhs.m01 + m31 * rhs.m11 + m32 * rhs.m21 + rhs.m31;
        T t32 = m30 * rhs.m02 + m31 * rhs.m12 + m32 * rhs.m22 + rhs.m32;

        m00 = t00; m01 = t01; m02 = t02;
        m10 = t10; m11 = t11; m12 = t12;
        m20 = t20; m21 = t21; m22 = t22;
        m30 = t30; m31 = t31; m32 = t32;
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        // row 0
        T t00 = rhs.m00 * m00 + rhs.m01 * m10 + rhs.m02 * m20;
        T t01 = rhs.m00 * m01 + rhs.m01 * m11 + rhs.m02 * m21;
        T t02 = rhs.m00 * m02 + rhs.m01 * m12 + rhs.m02 * m22;
        T t03 = rhs.m00 * m03 + rhs.m01 * m13 + rhs.m02 * m23 + rhs.m03;
        // row 1
        T t10 = rhs.m10 * m00 + rhs.m11 * m10 + rhs.m12 * m20;
        T t11 = rhs.m10 * m01 + rhs.m11 * m11 + rhs.m12 * m21;
        T t12 = rhs.m10 * m02 + rhs.m11 * m12 + rhs.m12 * m22;
        T t13 = rhs.m10 * m03 + rhs.m11 * m13 + rhs.m12 * m23 + rhs.m13;
        // row 2
        T t20 = rhs.m20 * m00 + rhs.m21 * m10 + rhs.m22 * m20;
        T t21 = rhs.m20 * m01 + rhs.m21 * m11 + rhs.m22 * m21;
        T t22 = rhs.m20 * m02 + rhs.m21 * m12 + rhs.m22 * m22;
        T t23 = rhs.m20 * m03 + rhs.m21 * m13 + rhs.m22 * m23 + rhs.m23;

        m00 = t00; m01 = t01; m02 = t02; m03 = t03;
        m10 = t10; m11 = t11; m12 = t12; m13 = t13;
        m20 = t20; m21 = t21; m22 = t22; m23 = t23;
#endif // AUL_USE_COORDINATE_HANDEDNESS

#elif AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_RIGHT_HANDED

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        // row 0
        T t00 = rhs.m00 * m00 + rhs.m01 * m10 + rhs.m02 * m20;
        T t01 = rhs.m00 * m01 + rhs.m01 * m11 + rhs.m02 * m21;
        T t02 = rhs.m00 * m02 + rhs.m01 * m12 + rhs.m02 * m22;
        // row 1
        T t10 = rhs.m10 * m00 + rhs.m11 * m10 + rhs.m12 * m20;
        T t11 = rhs.m10 * m01 + rhs.m11 * m11 + rhs.m12 * m21;
        T t12 = rhs.m10 * m02 + rhs.m11 * m12 + rhs.m12 * m22;
        // row 2
        T t20 = rhs.m20 * m00 + rhs.m21 * m10 + rhs.m22 * m20;
        T t21 = rhs.m20 * m01 + rhs.m21 * m11 + rhs.m22 * m21;
        T t22 = rhs.m20 * m02 + rhs.m21 * m12 + rhs.m22 * m22;
        // row 3
        T t30 = rhs.m30 * m00 + rhs.m31 * m10 + rhs.m32 * m20 + m30;
        T t31 = rhs.m30 * m01 + rhs.m31 * m11 + rhs.m32 * m21 + m31;
        T t32 = rhs.m30 * m02 + rhs.m31 * m12 + rhs.m32 * m22 + m32;

        m00 = t00; m01 = t01; m02 = t02;
        m10 = t10; m11 = t11; m12 = t12;
        m20 = t20; m21 = t21; m22 = t22;
        m30 = t30; m31 = t31; m32 = t32;
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        // row 0
        T t00 = m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20;
        T t01 = m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21;
        T t02 = m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22;
        T t03 = m00 * rhs.m03 + m01 * rhs.m13 + m02 * rhs.m23 + m03;
        // row 1
        T t10 = m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20;
        T t11 = m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21;
        T t12 = m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22;
        T t13 = m10 * rhs.m03 + m11 * rhs.m13 + m12 * rhs.m23 + m13;
        // row 2
        T t20 = m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20;
        T t21 = m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21;
        T t22 = m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22;
        T t23 = m20 * rhs.m03 + m21 * rhs.m13 + m22 * rhs.m23 + m23;

        m00 = t00; m01 = t01; m02 = t02; m03 = t03;
        m10 = t10; m11 = t11; m12 = t12; m13 = t13;
        m20 = t20; m21 = t21; m22 = t22; m23 = t23;
#endif // AUL_USE_COORDINATE_HANDEDNESS

#endif // AUL_USE_MATRIX_MULTIPLICATION_ORDER

        return *this;
    }

    template<typename T>
    matrix4x4<T> xform<T>::operator*(const matrix4x4<T>& rhs) const
    {
#if AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_LEFT_HANDED

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return matrix4x4<T>(
            // row 0
            m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20,
            m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21,
            m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22,
            m00 * rhs.m03 + m01 * rhs.m13 + m02 * rhs.m23,
            // row 1
            m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20,
            m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21,
            m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22,
            m10 * rhs.m03 + m11 * rhs.m13 + m12 * rhs.m23,
            // row 2
            m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20,
            m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21,
            m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22,
            m20 * rhs.m03 + m21 * rhs.m13 + m22 * rhs.m23,
            // row 3
            m30 * rhs.m00 + m31 * rhs.m10 + m32 * rhs.m20 + rhs.m30,
            m30 * rhs.m01 + m31 * rhs.m11 + m32 * rhs.m21 + rhs.m31,
            m30 * rhs.m02 + m31 * rhs.m12 + m32 * rhs.m22 + rhs.m32,
            m30 * rhs.m03 + m31 * rhs.m13 + m32 * rhs.m23 + rhs.m33
            );
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return matrix4x4<T>(
            // row 0
            rhs.m00 * m00 + rhs.m01 * m10 + rhs.m02 * m20,
            rhs.m00 * m01 + rhs.m01 * m11 + rhs.m02 * m21,
            rhs.m00 * m02 + rhs.m01 * m12 + rhs.m02 * m22,
            rhs.m00 * m03 + rhs.m01 * m13 + rhs.m02 * m23 + rhs.m03,
            // row 1
            rhs.m10 * m00 + rhs.m11 * m10 + rhs.m12 * m20,
            rhs.m10 * m01 + rhs.m11 * m11 + rhs.m12 * m21,
            rhs.m10 * m02 + rhs.m11 * m12 + rhs.m12 * m22,
            rhs.m10 * m03 + rhs.m11 * m13 + rhs.m12 * m23 + rhs.m13,
            // row 2
            rhs.m20 * m00 + rhs.m21 * m10 + rhs.m22 * m20,
            rhs.m20 * m01 + rhs.m21 * m11 + rhs.m22 * m21,
            rhs.m20 * m02 + rhs.m21 * m12 + rhs.m22 * m22,
            rhs.m20 * m03 + rhs.m21 * m13 + rhs.m22 * m23 + rhs.m23,
            // row 3
            rhs.m30 * m00 + rhs.m31 * m10 + rhs.m32 * m20,
            rhs.m30 * m01 + rhs.m31 * m11 + rhs.m32 * m21,
            rhs.m30 * m02 + rhs.m31 * m12 + rhs.m32 * m22,
            rhs.m30 * m03 + rhs.m31 * m13 + rhs.m32 * m23 + rhs.m33
            );
#endif // AUL_USE_COORDINATE_HANDEDNESS

#elif AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_RIGHT_HANDED

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return matrix4x4<T>(
            // row 0
            rhs.m00 * m00 + rhs.m01 * m10 + rhs.m02 * m20 + rhs.m03 * m30,
            rhs.m00 * m01 + rhs.m01 * m11 + rhs.m02 * m21 + rhs.m03 * m31,
            rhs.m00 * m02 + rhs.m01 * m12 + rhs.m02 * m22 + rhs.m03 * m32,
            rhs.m03,
            // row 1
            rhs.m10 * m00 + rhs.m11 * m10 + rhs.m12 * m20 + rhs.m13 * m30,
            rhs.m10 * m01 + rhs.m11 * m11 + rhs.m12 * m21 + rhs.m13 * m31,
            rhs.m10 * m02 + rhs.m11 * m12 + rhs.m12 * m22 + rhs.m13 * m32,
            rhs.m13,
            // row 2
            rhs.m20 * m00 + rhs.m21 * m10 + rhs.m22 * m20 + rhs.m23 * m30,
            rhs.m20 * m01 + rhs.m21 * m11 + rhs.m22 * m21 + rhs.m23 * m31,
            rhs.m20 * m02 + rhs.m21 * m12 + rhs.m22 * m22 + rhs.m23 * m32,
            rhs.m23,
            // row 3
            rhs.m30 * m00 + rhs.m31 * m10 + rhs.m32 * m20 + rhs.m33 * m30,
            rhs.m30 * m01 + rhs.m31 * m11 + rhs.m32 * m21 + rhs.m33 * m31,
            rhs.m30 * m02 + rhs.m31 * m12 + rhs.m32 * m22 + rhs.m33 * m32,
            rhs.m33
            );
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return matrix4x4<T>(
            // row 0
            m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20 + m03 * rhs.m30,
            m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21 + m03 * rhs.m31,
            m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22 + m03 * rhs.m32,
            m00 * rhs.m03 + m01 * rhs.m13 + m02 * rhs.m23 + m03 * rhs.m33,
            // row 1
            m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20 + m13 * rhs.m30,
            m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21 + m13 * rhs.m31,
            m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22 + m13 * rhs.m32,
            m10 * rhs.m03 + m11 * rhs.m13 + m12 * rhs.m23 + m13 * rhs.m33,
            // row 2
            m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20 + m23 * rhs.m30,
            m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21 + m23 * rhs.m31,
            m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22 + m23 * rhs.m32,
            m20 * rhs.m03 + m21 * rhs.m13 + m22 * rhs.m23 + m23 * rhs.m33,
            // row 3
            rhs.m30,
            rhs.m31,
            rhs.m32,
            rhs.m33
            );
#endif // AUL_USE_COORDINATE_HANDEDNESS

#endif // AUL_USE_MATRIX_MULTIPLICATION_ORDER
    }

    template<typename T>
    vector3<T> xform<T>::transform_point(const vector3<T>& point) const
    {
#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return vector3<T>(m00 * point.x + m10 * point.y + m20 * point.z + m30,
            m01 * point.x + m11 * point.y + m21 * point.z + m31,
            m02 * point.x + m12 * point.y + m22 * point.z + m32);
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return vector3<T>(m00 * point.x + m01 * point.y + m02 * point.z + m03,
            m10 * point.x + m11 * point.y + m12 * point.z + m13,
            m20 * point.x + m21 * point.y + m22 * point.z + m23);
#endif // AUL_USE_COORDINATE_HANDEDNESS
    }

    template<typename T>
    vector3<T> xform<T>::transform_vector(const vector3<T>& vec) const
    {
#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return vector3<T>(m00 * vec.x + m10 * vec.y + m20 * vec.z,
            m01 * vec.x + m11 * vec.y + m21 * vec.z,
            m02 * vec.x + m12 * vec.y + m22 * vec.z);
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return vector3<T>(m00 * vec.x + m01 * vec.y + m02 * vec.z,
            m10 * vec.x + m11 * vec.y + m12 * vec.z,
            m20 * vec.x + m21 * vec.y + m22 * vec.z);
#endif // AUL_USE_COORDINATE_HANDEDNESS
    }

    template<typename T>
    T xform<T>::determinant() const
    {
        return m00 * (m11 * m22 - m12 * m21) - m01 * (m10 * m22 - m12 * m20) + m02 * (m10 * m21 - m11 * m20);
    }

    template<typename T>
    xform<T> xform<T>::inverse() const
    {
        vector3<T> a(x.xyz.cross(y.xyz));
        vector3<T> b(z.xyz.cross(w.xyz));
        vector3<T> c(z.xyz);

        T inv_det = scalar<T>::ONE / a.dot(c);
        a *= inv_det;
        b *= inv_det;
        c *= inv_det;

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        vector3<T> c0 = y.xyz.cross(c);
        vector3<T> c1 = c.cross(x.xyz);
        vector3<T> c2 = a;

        return xform<T>(
            c0.x, c1.x, c2.x,
            c0.y, c1.y, c2.y,
            c0.z, c1.z, c2.z,
            -(y.xyz.dot(b)), x.xyz.dot(b), -(w.xyz.dot(a))
            );
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        vector3<T> r0 = y.xyz.cross(c);
        vector3<T> r1 = c.cross(x.xyz);
        vector3<T> r2 = a;

        return xform<T>(
            r0.x, r0.y, r0.z, -(y.xyz.dot(b)),
            r1.x, r1.y, r1.z, x.xyz.dot(b),
            r2.x, r2.y, r2.z, -(w.xyz.dot(a))
            );
#endif // AUL_USE_COORDINATE_HANDEDNESS
    }

    template<typename T>
    xform<T>& xform<T>::invert()
    {
        vector3<T> a(x.xyz.cross(y.xyz));
        vector3<T> b(z.xyz.cross(w.xyz));
        vector3<T> c(z.xyz);

        T inv_det = scalar<T>::ONE / a.dot(c);
        a *= inv_det;
        b *= inv_det;
        c *= inv_det;

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        vector3<T> c0 = y.xyz.cross(c);
        vector3<T> c1 = c.cross(x.xyz);
        vector3<T> c2 = a;

        m00 = c0.x; m01 = c1.x; m02 = c2.x;
        m10 = c0.y; m11 = c1.y; m12 = c2.y;
        m20 = c0.z; m21 = c1.z; m22 = c2.z;
        m30 = -(y.xyz.dot(b)); m31 = x.xyz.dot(b); m32 = -(w.xyz.dot(a));
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        vector3<T> r0 = y.xyz.cross(c);
        vector3<T> r1 = c.cross(x.xyz);
        vector3<T> r2 = a;

        m00 = r0.x; m01 = r0.y; m02 = r0.z; m03 = -(y.xyz.dot(b));
        m10 = r1.x; m11 = r1.y; m12 = r1.z; m13 = x.xyz.dot(b);
        m20 = r2.x; m21 = r2.y; m22 = r2.z; m23 = -(w.xyz.dot(a));
#endif // AUL_USE_COORDINATE_HANDEDNESS

        return *this;
    }

    template<typename T>
    xform<T> xform<T>::make_scale(T scale, const vector3<T>& axis)
    {
        scale -= scalar<T>::ONE;
        T axis_x = axis.x * scale;
        T axis_y = axis.y * scale;
        T axis_z = axis.z * scale;
        T axis_xy = axis_x * axis.y;
        T axis_xz = axis_x * axis.z;
        T axis_yz = axis_y * axis.z;

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return xform<T>(
            axis_x * axis.x + scalar<T>::ONE, axis_xy, axis_xz,
            axis_xy, axis_y * axis.y + scalar<T>::ONE, axis_yz,
            axis_xz, axis_yz, axis_z * axis.z + scalar<T>::ONE,
            scalar<T>::ZERO, scalar<T>::ZERO, scalar<T>::ZERO
            );
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return xform<T>(
            axis_x * axis.x + scalar<T>::ONE, axis_xy, axis_xz, scalar<T>::ZERO,
            axis_xy, axis_y * axis.y + scalar<T>::ONE, axis_yz, scalar<T>::ZERO,
            axis_xz, axis_yz, axis_z * axis.z + scalar<T>::ONE, scalar<T>::ZERO
            );
#endif // AUL_USE_COORDINATE_HANDEDNESS
    }

    template<typename T>
    xform<T> xform<T>::make_translation(T x_, T y_, T z_)
    {
#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return xform<T>(
            scalar<T>::ONE, scalar<T>::ZERO, scalar<T>::ZERO,
            scalar<T>::ZERO, scalar<T>::ONE, scalar<T>::ZERO,
            scalar<T>::ZERO, scalar<T>::ZERO, scalar<T>::ONE,
            x_, y_, z_
            );
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return xform<T>(
            scalar<T>::ONE, scalar<T>::ZERO, scalar<T>::ZERO, x_,
            scalar<T>::ZERO, scalar<T>::ONE, scalar<T>::ZERO, y_,
            scalar<T>::ZERO, scalar<T>::ZERO, scalar<T>::ONE, z_
            );
#endif // AUL_USE_COORDINATE_HANDEDNESS
    }

    template<typename T>
    xform<T> xform<T>::make_translation(const vector3<T>& translation)
    {
#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return xform<T>(
            scalar<T>::ONE, scalar<T>::ZERO, scalar<T>::ZERO,
            scalar<T>::ZERO, scalar<T>::ONE, scalar<T>::ZERO,
            scalar<T>::ZERO, scalar<T>::ZERO, scalar<T>::ONE,
            translation.x, translation.y, translation.z
            );
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return xform<T>(
            scalar<T>::ONE, scalar<T>::ZERO, scalar<T>::ZERO, translation.x,
            scalar<T>::ZERO, scalar<T>::ONE, scalar<T>::ZERO, translation.y,
            scalar<T>::ZERO, scalar<T>::ZERO, scalar<T>::ONE, translation.z
            );
#endif // AUL_USE_COORDINATE_HANDEDNESS
    }

    template<typename T>
    xform<T> xform<T>::make_skew(T radians, const vector3<T>& skew_axis, const vector3<T>& normal_axis)
    {
        T tan_rad = tan(radians);
        T axis_x = skew_axis.x * tan_rad;
        T axis_y = skew_axis.y * tan_rad;
        T axis_z = skew_axis.z * tan_rad;

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return xform<T>(
            axis_x * normal_axis.x + scalar<T>::ONE, axis_y * normal_axis.x, axis_z * normal_axis.x,
            axis_x * normal_axis.y, axis_y * normal_axis.y + scalar<T>::ONE, axis_z * normal_axis.y,
            axis_x * normal_axis.z, axis_y * normal_axis.z, axis_z * normal_axis.z + scalar<T>::ONE,
            scalar<T>::ZERO, scalar<T>::ZERO, scalar<T>::ZERO
            );
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return xform<T>(
            axis_x * normal_axis.x + scalar<T>::ONE, axis_x * normal_axis.y, axis_x * normal_axis.z, scalar<T>::ZERO,
            axis_y * normal_axis.x, axis_y * normal_axis.y + scalar<T>::ONE, axis_y * normal_axis.z, scalar<T>::ZERO,
            axis_z * normal_axis.x, axis_z * normal_axis.y, axis_z * normal_axis.z + scalar<T>::ONE, scalar<T>::ZERO
            );
#endif // AUL_USE_COORDINATE_HANDEDNESS
    }

    template<typename T>
    xform<T> xform<T>::make_rotation_x(T radians)
    {
        T sin_rad = sin(radians);
        T cos_rad = cos(radians);

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return xform<T>(
            scalar<T>::ONE, scalar<T>::ZERO, scalar<T>::ZERO,
            scalar<T>::ZERO, cos_rad, sin_rad,
            scalar<T>::ZERO, -sin_rad, cos_rad,
            scalar<T>::ZERO, scalar<T>::ZERO, scalar<T>::ZERO
            );
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return xform<T>(
            scalar<T>::ONE, scalar<T>::ZERO, scalar<T>::ZERO, scalar<T>::ZERO,
            scalar<T>::ZERO, cos_rad, -sin_rad, scalar<T>::ZERO,
            scalar<T>::ZERO, sin_rad, cos_rad, scalar<T>::ZERO
            );
#endif // AUL_USE_COORDINATE_HANDEDNESS
    }

    template<typename T>
    xform<T> xform<T>::make_rotation_y(T radians)
    {
        T sin_rad = sin(radians);
        T cos_rad = cos(radians);

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return xform<T>(
            cos_rad, scalar<T>::ZERO, -sin_rad,
            scalar<T>::ZERO, scalar<T>::ONE, scalar<T>::ZERO,
            sin_rad, scalar<T>::ZERO, cos_rad,
            scalar<T>::ZERO, scalar<T>::ZERO, scalar<T>::ZERO
            );
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return xform<T>(
            cos_rad, scalar<T>::ZERO, sin_rad, scalar<T>::ZERO,
            scalar<T>::ZERO, scalar<T>::ONE, scalar<T>::ZERO, scalar<T>::ZERO,
            -sin_rad, scalar<T>::ZERO, cos_rad, scalar<T>::ZERO
            );
#endif // AUL_USE_COORDINATE_HANDEDNESS
    }

    template<typename T>
    xform<T> xform<T>::make_rotation_z(T radians)
    {
        T sin_rad = sin(radians);
        T cos_rad = cos(radians);

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return xform<T>(
            cos_rad, sin_rad, scalar<T>::ZERO,
            -sin_rad, cos_rad, scalar<T>::ZERO,
            scalar<T>::ZERO, scalar<T>::ZERO, scalar<T>::ONE,
            scalar<T>::ZERO, scalar<T>::ZERO, scalar<T>::ZERO
            );
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return xform<T>(
            cos_rad, -sin_rad, scalar<T>::ZERO, scalar<T>::ZERO,
            sin_rad, cos_rad, scalar<T>::ZERO, scalar<T>::ZERO,
            scalar<T>::ZERO, scalar<T>::ZERO, scalar<T>::ONE, scalar<T>::ZERO
            );
#endif // AUL_USE_COORDINATE_HANDEDNESS
    }

    template<typename T>
    xform<T> xform<T>::make_rotation_xyz(T radians_x, T radians_y, T radians_z)
    {
#if AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_LEFT_HANDED
        return make_rotation_x(radians_x) * make_rotation_y(radians_y) * make_rotation_z(radians_z);
#elif AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_RIGHT_HANDED
        return make_rotation_z(radians_z) * make_rotation_y(radians_y) * make_rotation_x(radians_x);
#endif // AUL_USE_MATRIX_MULTIPLICATION_ORDER
    }

    template<typename T>
    xform<T> xform<T>::make_rotation_xyz(const vector3<T>& radians_xyz)
    {
#if AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_LEFT_HANDED
        return make_rotation_x(radians_xyz.x) * make_rotation_y(radians_xyz.y) * make_rotation_z(radians_xyz.z);
#elif AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_RIGHT_HANDED
        return make_rotation_z(radians_xyz.z) * make_rotation_y(radians_xyz.y) * make_rotation_x(radians_xyz.x);
#endif // AUL_USE_MATRIX_MULTIPLICATION_ORDER
    }

    template<typename T>
    xform<T> xform<T>::make_rotation_ypr(T radians_yaw, T radians_pitch, T radians_roll)
    {
#if AUL_USE_UP_VECTOR == AUL_Y_UP

#if AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_LEFT_HANDED
        return make_rotation_y(radians_yaw) * make_rotation_x(radians_pitch) * make_rotation_z(radians_roll);
#elif AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_RIGHT_HANDED
        return make_rotation_z(radians_roll * make_rotation_x(radians_pitch) * make_rotation_y(radians_yaw);
#endif // AUL_USE_MATRIX_MULTIPLICATION_ORDER

#elif AUL_USE_UP_VECTOR == AUL_Z_UP

#if AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_LEFT_HANDED

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return make_rotation_z(radians_yaw) * make_rotation_y(radians_pitch) * make_rotation_x(radians_roll);
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return make_rotation_z(radians_yaw) * make_rotation_x(radians_pitch) * make_rotation_y(radians_roll);
#endif // AUL_USE_COORDINATE_HANDEDNESS

#elif AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_RIGHT_HANDED

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return make_rotation_x(radians_roll) * make_rotation_y(radians_pitch) * make_rotation_z(radians_yaw);
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return make_rotation_y(radians_roll) * make_rotation_x(radians_pitch) * make_rotation_z(radians_yaw);
#endif // AUL_USE_COORDINATE_HANDEDNESS

#endif // AUL_USE_MATRIX_MULTIPLICATION_ORDER

#endif // AUL_USE_UP_VECTOR
    }

    template<typename T>
    xform<T> xform<T>::make_rotation_ypr(const vector3<T>& radians_ypr)
    {
#if AUL_USE_UP_VECTOR == AUL_Y_UP

#if AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_LEFT_HANDED
        return make_rotation_y(radians_ypr.yaw) * make_rotation_x(radians_ypr.pitch) * make_rotation_z(radians_ypr.roll);
#elif AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_RIGHT_HANDED
        return make_rotation_z(radians_ypr.yaw) * make_rotation_x(radians_ypr.pitch) * make_rotation_y(radians_ypr.roll);
#endif // AUL_USE_MATRIX_MULTIPLICATION_ORDER

#elif AUL_USE_UP_VECTOR == AUL_Z_UP

#if AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_LEFT_HANDED

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return make_rotation_z(radians_ypr.yaw) * make_rotation_y(radians_ypr.pitch) * make_rotation_x(radians_ypr.roll);
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return make_rotation_z(radians_ypr.yaw) * make_rotation_x(radians_ypr.pitch) * make_rotation_y(radians_ypr.roll);
#endif // AUL_USE_COORDINATE_HANDEDNESS

#elif AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_RIGHT_HANDED

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return make_rotation_x(radians_ypr.roll) * make_rotation_y(radians_ypr.pitch) * make_rotation_z(radians_ypr.yaw);
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return make_rotation_y(radians_ypr.roll) * make_rotation_x(radians_ypr.pitch) * make_rotation_z(radians_ypr.yaw);
#endif // AUL_USE_COORDINATE_HANDEDNESS

#endif // AUL_USE_MATRIX_MULTIPLICATION_ORDER

#endif // AUL_USE_UP_VECTOR
    }

    template<typename T>
    xform<T> xform<T>::make_rotation_angle_axis(T radians, const vector3<T>& axis)
    {
        T cos_rad = cos(radians);
        T sin_rad = sin(radians);
        T one_minus_cos_rad = scalar<T>::ONE - cos_rad;
        T axis_x = axis.x * one_minus_cos_rad;
        T axis_y = axis.y * one_minus_cos_rad;
        T axis_z = axis.z * one_minus_cos_rad;
        T axis_xy = axis_x * axis.y;
        T axis_xz = axis_x * axis.z;
        T axis_yz = axis_y * axis.z;

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return xform<T>(
            cos_rad + axis_x * axis.x, axis_xy + sin_rad * axis.z, axis_xz - sin_rad * axis.y,
            axis_xy - sin_rad * axis.z, cos_rad + axis_y * axis.y, axis_yz + sin_rad * axis.x,
            axis_xz + sin_rad * axis.y, axis_yz - sin_rad * axis.x, cos_rad + axis_z * axis.z,
            scalar<T>::ZERO, scalar<T>::ZERO, scalar<T>::ZERO
            );
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return xform<T>(
            cos_rad + axis_x * axis.x, axis_xy - sin_rad * axis.z, axis_xz + sin_rad * axis.y, scalar<T>::ZERO,
            axis_xy + sin_rad * axis.z, cos_rad + axis_y * axis.y, axis_yz - sin_rad * axis.x, scalar<T>::ZERO,
            axis_xz - sin_rad * axis.y, axis_yz + sin_rad * axis.x, cos_rad + axis_z * axis.z, scalar<T>::ZERO
            );
#endif // AUL_USE_COORDINATE_HANDEDNESS
    }

    template<typename T>
    xform<T> xform<T>::make_reflection(const vector3<T>& normal)
    {
        T normal_x = normal.x * -scalar<T>::TWO;
        T normal_y = normal.y * -scalar<T>::TWO;
        T normal_z = normal.z * -scalar<T>::TWO;
        T normal_xy = normal_x * normal.y;
        T normal_xz = normal_x * normal.z;
        T normal_yz = normal_y * normal.z;

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return xform<T>(
            normal_x * normal.x + scalar<T>::ONE, normal_xy, normal_xz,
            normal_xy, normal_y * normal.y + scalar<T>::ONE, normal_yz,
            normal_xz, normal_yz, normal_z * normal.z + scalar<T>::ONE,
            scalar<T>::ZERO, scalar<T>::ZERO, scalar<T>::ZERO
            );
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return xform<T>(
            normal_x * normal.x + scalar<T>::ONE, normal_xy, normal_xz, scalar<T>::ZERO,
            normal_xy, normal_y * normal.y + scalar<T>::ONE, normal_yz, scalar<T>::ZERO,
            normal_xz, normal_yz, normal_z * normal.z + scalar<T>::ONE, scalar<T>::ZERO
            );
#endif // AUL_USE_COORDINATE_HANDEDNESS
    }

    template<typename T>
    xform<T> xform<T>::make_involution(const vector3<T>& vec)
    {
        T vec_x = vec.x * scalar<T>::TWO;
        T vec_y = vec.y * scalar<T>::TWO;
        T vec_z = vec.z * scalar<T>::TWO;
        T vec_xy = vec_x * vec.y;
        T vec_xz = vec_x * vec.z;
        T vec_yz = vec_y * vec.z;

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return xform<T>(
            vec_x * vec.x - scalar<T>::ONE, vec_xy, vec_xz,
            vec_xy, vec_y * vec.y - scalar<T>::ONE, vec_yz,
            vec_xz, vec_yz, vec_z * vec.z - scalar<T>::ONE,
            scalar<T>::ZERO, scalar<T>::ZERO, scalar<T>::ZERO
            );
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return xform<T>(
            vec_x * vec.x - scalar<T>::ONE, vec_xy, vec_xz, scalar<T>::ZERO,
            vec_xy, vec_y * vec.y - scalar<T>::ONE, vec_yz, scalar<T>::ZERO,
            vec_xz, vec_yz, vec_z * vec.z - scalar<T>::ONE, scalar<T>::ZERO
            );
#endif // AUL_USE_COORDINATE_HANDEDNESS
    }

#undef AUL_INTERNAL_ZERO_NON_DIAGONAL
#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
#define AUL_INTERNAL_ZERO_NON_DIAGONAL() m01 = m02 = m10 = m12 = m20 = m21 = m30 = m31 = m32 = scalar<T>::ZERO
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
#define AUL_INTERNAL_ZERO_NON_DIAGONAL() m01 = m02 = m03 = m10 = m12 = m13 = m20 = m21 = m23 = scalar<T>::ZERO
#endif // AUL_USE_COORDINATE_HANDEDNESS

    template<typename T>
    xform<T>& xform<T>::to_uniform_scale(T scale)
    {
        m00 = m11 = m22 = scale;
        m33 = scalar<T>::ONE;
        AUL_INTERNAL_ZERO_NON_DIAGONAL();
        return *this;
    }

    template<typename T>
    xform<T>& xform<T>::to_scale(T scale_x, T scale_y, T scale_z)
    {
        m00 = scale_x;
        m11 = scale_y;
        m22 = scale_z;
        m33 = scalar<T>::ONE;
        AUL_INTERNAL_ZERO_NON_DIAGONAL();
        return *this;
    }

    template<typename T>
    xform<T>& xform<T>::to_scale(const vector3<T>& scale)
    {
        m00 = scale.x;
        m11 = scale.y;
        m22 = scale.z;
        m33 = scalar<T>::ONE;
        AUL_INTERNAL_ZERO_NON_DIAGONAL();
        return *this;
    }

    template<typename T>
    xform<T>& xform<T>::to_scale(T scale, const vector3<T>& axis)
    {
        scale -= scalar<T>::ONE;
        T axis_x = axis.x * scale;
        T axis_y = axis.y * scale;
        T axis_z = axis.z * scale;
        T axis_xy = axis_x * axis.y;
        T axis_xz = axis_x * axis.z;
        T axis_yz = axis_y * axis.z;

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        m00 = axis_x * axis.x + scalar<T>::ONE; m01 = axis_xy; m02 = axis_xz;
        m10 = axis_xy; m11 = axis_y * axis.y + scalar<T>::ONE; m12 = axis_yz;
        m20 = axis_xz; m21 = axis_yz; m22 = axis_z * axis.z + scalar<T>::ONE;
        m30 = scalar<T>::ZERO; m31 = scalar<T>::ZERO; m32 = scalar<T>::ZERO;
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        m00 = axis_x * axis.x + scalar<T>::ONE; m01 = axis_xy; m02 = axis_xz; m03 = scalar<T>::ZERO;
        m10 = axis_xy; m11 = axis_y * axis.y + scalar<T>::ONE; m12 = axis_yz; m13 = scalar<T>::ZERO;
        m20 = axis_xz; m21 = axis_yz; m22 = axis_z * axis.z + scalar<T>::ONE; m23 = scalar<T>::ZERO;
#endif // AUL_USE_COORDINATE_HANDEDNESS
        return *this;
    }

    template<typename T>
    xform<T>& xform<T>::to_translation(T x_, T y_, T z_)
    {
        x.xyz = vector3<T>::X_AXIS;
        y.xyz = vector3<T>::Y_AXIS;
        z.xyz = vector3<T>::Z_AXIS;
        w.x = x_;
        w.y = y_;
        w.z = z_;
        w.w = scalar<T>::ONE;
        return *this;
    }

    template<typename T>
    xform<T>& xform<T>::to_translation(const vector3<T>& translation)
    {
        x.xyz = vector3<T>::X_AXIS;
        y.xyz = vector3<T>::Y_AXIS;
        z.xyz = vector3<T>::Z_AXIS;
        w.xyz = translation;
        w.w = scalar<T>::ONE;
        return *this;
    }

    template<typename T>
    xform<T>& xform<T>::to_skew(T radians, const vector3<T>& skew_axis, const vector3<T>& normal_axis)
    {
        T tan_rad = tan(radians);
        T axis_x = skew_axis.x * tan_rad;
        T axis_y = skew_axis.y * tan_rad;
        T axis_z = skew_axis.z * tan_rad;

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        m00 = axis_x * normal_axis.x + scalar<T>::ONE; m01 = axis_y * normal_axis.x; m02 = axis_z * normal_axis.x;
        m10 = axis_x * normal_axis.y; m11 = axis_y * normal_axis.y + scalar<T>::ONE; m12 = axis_z * normal_axis.y;
        m20 = axis_x * normal_axis.z; m21 = axis_y * normal_axis.z; m22 = axis_z * normal_axis.z + scalar<T>::ONE;
        m30 = scalar<T>::ZERO; m31 = scalar<T>::ZERO; m32 = scalar<T>::ZERO;
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        m00 = axis_x * normal_axis.x + scalar<T>::ONE; m01 = axis_x * normal_axis.y; m02 = axis_x * normal_axis.z; m03 = scalar<T>::ZERO;
        m10 = axis_y * normal_axis.x; m11 = axis_y * normal_axis.y + scalar<T>::ONE; m12 = axis_y * normal_axis.z; m13 = scalar<T>::ZERO;
        m20 = axis_z * normal_axis.x; m21 = axis_z * normal_axis.y; m22 = axis_z * normal_axis.z + scalar<T>::ONE; m23 = scalar<T>::ZERO;
#endif // AUL_USE_COORDINATE_HANDEDNESS

        return *this;
    }

    template<typename T>
    xform<T>& xform<T>::to_rotation_x(T radians)
    {
        T sin_rad = sin(radians);
        T cos_rad = cos(radians);

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        m00 = scalar<T>::ONE; m01 = scalar<T>::ZERO; m02 = scalar<T>::ZERO;
        m10 = scalar<T>::ZERO; m11 = cos_rad; m12 = sin_rad;
        m20 = scalar<T>::ZERO; m21 = -sin_rad; m22 = cos_rad;
        m30 = scalar<T>::ZERO; m31 = scalar<T>::ZERO; m32 = scalar<T>::ZERO;
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        m00 = scalar<T>::ONE; m01 = scalar<T>::ZERO; m02 = scalar<T>::ZERO; m03 = scalar<T>::ZERO;
        m10 = scalar<T>::ZERO; m11 = cos_rad; m12 = -sin_rad; m13 = scalar<T>::ZERO;
        m20 = scalar<T>::ZERO; m21 = sin_rad; m22 = cos_rad; m23 = scalar<T>::ZERO;
#endif // AUL_USE_COORDINATE_HANDEDNESS

        return *this;
    }

    template<typename T>
    xform<T>& xform<T>::to_rotation_y(T radians)
    {
        T sin_rad = sin(radians);
        T cos_rad = cos(radians);

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        m00 = cos_rad; m01 = scalar<T>::ZERO; m11 = -sin_rad;
        m10 = scalar<T>::ZERO; m11 = scalar<T>::ONE; m12 = scalar<T>::ZERO;
        m20 = sin_rad; m21 = scalar<T>::ZERO; m22 = cos_rad;
        m30 = scalar<T>::ZERO; m31 = scalar<T>::ZERO; m32 = scalar<T>::ZERO;
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        m00 = cos_rad; m01 = scalar<T>::ZERO; m02 = sin_rad; m03 = scalar<T>::ZERO;
        m10 = scalar<T>::ZERO; m11 = scalar<T>::ONE; m12 = scalar<T>::ZERO; m13 = scalar<T>::ZERO;
        m20 = -sin_rad; m21 = scalar<T>::ZERO; m22 = cos_rad; m23 = scalar<T>::ZERO;
#endif // AUL_USE_COORDINATE_HANDEDNESS

        return *this;
    }

    template<typename T>
    xform<T>& xform<T>::to_rotation_z(T radians)
    {
        T sin_rad = sin(radians);
        T cos_rad = cos(radians);

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        m00 = cos_rad; m01 = sin_rad; m02 = scalar<T>::ZERO;
        m10 = -sin_rad; m11 = cos_rad; m12 = scalar<T>::ZERO;
        m20 = scalar<T>::ZERO; m21 = scalar<T>::ZERO; m22 = scalar<T>::ONE;
        m30 = scalar<T>::ZERO; m31 = scalar<T>::ZERO; m32 = scalar<T>::ZERO;
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        m00 = cos_rad; m01 = -sin_rad; m02 = scalar<T>::ZERO; m03 = scalar<T>::ZERO;
        m10 = sin_rad; m11 = cos_rad; m12 = scalar<T>::ZERO; m13 = scalar<T>::ZERO;
        m20 = scalar<T>::ZERO; m21 = scalar<T>::ZERO; m22 = scalar<T>::ONE; m23 = scalar<T>::ZERO;
#endif // AUL_USE_COORDINATE_HANDEDNESS

        return *this;
    }

    template<typename T>
    xform<T>& xform<T>::to_rotation_xyz(T radians_x, T radians_y, T radians_z)
    {
#if AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_LEFT_HANDED
        return *this = make_rotation_x(radians_x) * make_rotation_y(radians_y) * make_rotation_z(radians_z);
#elif AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_RIGHT_HANDED
        return *this = make_rotation_z(radians_z) * make_rotation_y(radians_y) * make_rotation_x(radians_x);
#endif // AUL_USE_MATRIX_MULTIPLICATION_ORDER
    }

    template<typename T>
    xform<T>& xform<T>::to_rotation_xyz(const vector3<T>& radians_xyz)
    {
#if AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_LEFT_HANDED
        return *this = make_rotation_x(radians_xyz.x) * make_rotation_y(radians_xyz.y) * make_rotation_z(radians_xyz.z);
#elif AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_RIGHT_HANDED
        return *this = make_rotation_z(radians_xyz.z) * make_rotation_y(radians_xyz.y) * make_rotation_x(radians_xyz.x);
#endif // AUL_USE_MATRIX_MULTIPLICATION_ORDER
    }

    template<typename T>
    xform<T>& xform<T>::to_rotation_ypr(T radians_yaw, T radians_pitch, T radians_roll)
    {
#if AUL_USE_UP_VECTOR == AUL_Y_UP

#if AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_LEFT_HANDED
        return *this = make_rotation_y(radians_yaw) * make_rotation_x(radians_pitch) * make_rotation_z(radians_roll);
#elif AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_RIGHT_HANDED
        return *this = make_rotation_z(radians_roll) * make_rotation_x(radians_pitch) * make_rotation_y(radians_yaw);
#endif // AUL_USE_MATRIX_MULTIPLICATION_ORDER

#elif AUL_USE_UP_VECTOR == AUL_Z_UP

#if AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_LEFT_HANDED

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return *this = make_rotation_z(radians_yaw) * make_rotation_y(radians_pitch) * make_rotation_x(radians_roll);
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return *this = make_rotation_z(radians_yaw) * make_rotation_x(radians_pitch) * make_rotation_y(radians_roll);
#endif // AUL_USE_COORDINATE_HANDEDNESS

#elif AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_RIGHT_HANDED

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return *this = make_rotation_x(radians_roll) * make_rotation_y(radians_pitch) * make_rotation_z(radians_yaw);
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return *this = make_rotation_y(radians_roll) * make_rotation_x(radians_pitch) * make_rotation_z(radians_yaw);
#endif // AUL_USE_COORDINATE_HANDEDNESS

#endif // AUL_USE_MATRIX_MULTIPLICATION_ORDER

#endif // AUL_USE_UP_VECTOR
    }

    template<typename T>
    xform<T>& xform<T>::to_rotation_ypr(vector3<T>& radians_ypr)
    {
#if AUL_USE_UP_VECTOR == AUL_Y_UP

#if AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_LEFT_HANDED
        return *this = make_rotation_y(radians_ypr.yaw) * make_rotation_x(radians_ypr.pitch) * make_rotation_z(radians_ypr.roll);
#elif AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_RIGHT_HANDED
        return *this = make_rotation_z(radians_ypr.roll) * make_rotation_x(radians_ypr.pitch) * make_rotation_y(radians_ypr.yaw);
#endif // AUL_USE_MATRIX_MULTIPLICATION_ORDER

#elif AUL_USE_UP_VECTOR == AUL_Z_UP

#if AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_LEFT_HANDED

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return *this = make_rotation_z(radians_ypr.yaw) * make_rotation_y(radians_ypr.pitch) * make_rotation_x(radians_ypr.roll);
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return *this = make_rotation_z(radians_ypr.yaw) * make_rotation_x(radians_ypr.pitch) * make_rotation_y(radians_ypr.roll);
#endif // AUL_USE_COORDINATE_HANDEDNESS

#elif AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_RIGHT_HANDED

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return *this = make_rotation_x(radians_ypr.roll) * make_rotation_y(radians_ypr.pitch) * make_rotation_z(radians_ypr.yaw);
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return *this = make_rotation_y(radians_ypr.roll) * make_rotation_x(radians_ypr.pitch) * make_rotation_z(radians_ypr.yaw);
#endif // AUL_USE_COORDINATE_HANDEDNESS

#endif // AUL_USE_MATRIX_MULTIPLICATION_ORDER

#endif // AUL_USE_UP_VECTOR
    }

    template<typename T>
    xform<T>& xform<T>::to_rotation_angle_axis(T radians, const vector3<T>& axis)
    {
        T cos_rad = cos(radians);
        T sin_rad = sin(radians);
        T one_minus_cos_rad = scalar<T>::ONE - cos_rad;
        T axis_x = axis.x * one_minus_cos_rad;
        T axis_y = axis.y * one_minus_cos_rad;
        T axis_z = axis.z * one_minus_cos_rad;
        T axis_xy = axis_x * axis.y;
        T axis_xz = axis_x * axis.z;
        T axis_yz = axis_y * axis.z;

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        m00 = cos_rad + axis_x * axis.x; m01 = axis_xy + sin_rad * axis.z; m02 = axis_xz - sin_rad * axis.y;
        m10 = axis_xy - sin_rad * axis.z; m11 = cos_rad + axis_y * axis.y; m12 = axis_yz + sin_rad * axis.x;
        m20 = axis_xz + sin_rad * axis.y; m21 = axis_yz - sin_rad * axis.x; m22 = cos_rad + axis_z * axis.z;
        m30 = scalar<T>::ZERO; m31 = scalar<T>::ZERO; m32 = scalar<T>::ZERO;
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        m00 = cos_rad + axis_x * axis.x; m01 = axis_xy - sin_rad * axis.z; m02 = axis_xz + sin_rad * axis.y; m03 = scalar<T>::ZERO;
        m10 = axis_xy + sin_rad * axis.z; m11 = cos_rad + axis_y * axis.y; m12 = axis_yz - sin_rad * axis.x; m13 = scalar<T>::ZERO;
        m20 = axis_xz - sin_rad * axis.y; m21 = axis_yz + sin_rad * axis.x; m22 = cos_rad + axis_z * axis.z; m23 = scalar<T>::ZERO;
#endif // AUL_USE_COORDINATE_HANDEDNESS

        return *this;
    }

    template<typename T>
    xform<T>& xform<T>::to_reflection(const vector3<T>& normal)
    {
        T normal_x = normal.x * -scalar<T>::TWO;
        T normal_y = normal.y * -scalar<T>::TWO;
        T normal_z = normal.z * -scalar<T>::TWO;
        T normal_xy = normal_x * normal.y;
        T normal_xz = normal_x * normal.z;
        T normal_yz = normal_y * normal.z;

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        m00 = normal_x * normal.x + scalar<T>::ONE; m01 = normal_xy; m02 = normal_xz;
        m10 = normal_xy; m11 = normal_y * normal.y + scalar<T>::ONE; m12 = normal_yz;
        m20 = normal_xz; m21 = normal_yz; m22 = normal_z * normal.z + scalar<T>::ONE;
        m30 = scalar<T>::ZERO; m31 = scalar<T>::ZERO; m32 = scalar<T>::ZERO;
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        m00 = normal_x * normal.x + scalar<T>::ONE; m01 = normal_xy; m02 = normal_xz; m03 = scalar<T>::ZERO;
        m10 = normal_xy; m11 = normal_y * normal.y + scalar<T>::ONE; m12 = normal_yz; m13 = scalar<T>::ZERO;
        m20 = normal_xz; m21 = normal_yz; m22 = normal_z * normal.z + scalar<T>::ONE; m23 = scalar<T>::ZERO;
#endif // AUL_USE_COORDINATE_HANDEDNESS

        return *this;
    }

    template<typename T>
    xform<T>& xform<T>::to_involution(const vector3<T>& vec)
    {
        T vec_x = vec.x * scalar<T>::TWO;
        T vec_y = vec.y * scalar<T>::TWO;
        T vec_z = vec.z * scalar<T>::TWO;
        T vec_xy = vec_x * vec.y;
        T vec_xz = vec_x * vec.z;
        T vec_yz = vec_y * vec.z;

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        m00 = vec_x * vec.x - scalar<T>::ONE; m01 = vec_xy; m02 = vec_xz;
        m10 = vec_xy; m11 = vec_y * vec.y - scalar<T>::ONE; m12 = vec_yz;
        m20 = vec_xz; m21 = vec_yz; m22 = vec_z * vec.z - scalar<T>::ONE;
        m30 = scalar<T>::ZERO; m31 = scalar<T>::ZERO; m32 = scalar<T>::ZERO;
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        m00 = vec_x * vec.x - scalar<T>::ONE; m01 = vec_xy; m02 = vec_xz; m03 = scalar<T>::ZERO;
        m10 = vec_xy; m11 = vec_y * vec.y - scalar<T>::ONE; m12 = vec_yz; m13 = scalar<T>::ZERO;
        m20 = vec_xz; m21 = vec_yz; m22 = vec_z * vec.z - scalar<T>::ONE; m23 = scalar<T>::ZERO;
#endif // AUL_USE_COORDINATE_HANDEDNESS

        return *this;
    }

    template<typename T>
    xform<T>::operator wide_string() const
    {
        wide_stringstream stream;
        stream << *this;
        return stream.str();
    }

    template<typename T>
    wide_ostream& operator<<(wide_ostream& out, const xform<T>& mat)
    {
#if AUL_USE_MATRIX_MULTI_LINE_STRING_REPRESENTATION
        out << L"[" << mat.m00 << L", " << mat.m01 << L", " << mat.m02 << L", " << mat.m03 << L"]\n"
            << L"[" << mat.m10 << L", " << mat.m11 << L", " << mat.m12 << L", " << mat.m13 << L"]\n"
            << L"[" << mat.m20 << L", " << mat.m21 << L", " << mat.m22 << L", " << mat.m23 << L"]\n"
            << L"[" << mat.m30 << L", " << mat.m31 << L", " << mat.m32 << L", " << mat.m33 << L"]";
#else // AUL_USE_MATRIX_MULTI_LINE_STRING_REPRESENTATION
        out << L"[" << mat.x << L", " << mat.y << L", " << mat.z << L", " << mat.w << L"]";
#endif // AUL_USE_MATRIX_MULTI_LINE_STRING_REPRESENTATION
        return out;
    }

    template<typename T>
    xform<T>::operator mb_string() const
    {
        mb_stringstream stream;
        stream << *this;
        return stream.str();
    }

    template<typename T>
    mb_ostream& operator<<(mb_ostream& out, const xform<T>& mat)
    {
#if AUL_USE_MATRIX_MULTI_LINE_STRING_REPRESENTATION
        out << "[" << mat.m00 << ", " << mat.m01 << ", " << mat.m02 << ", " << mat.m03 << "]\n"
            << "[" << mat.m10 << ", " << mat.m11 << ", " << mat.m12 << ", " << mat.m13 << "]\n"
            << "[" << mat.m20 << ", " << mat.m21 << ", " << mat.m22 << ", " << mat.m23 << "]\n"
            << "[" << mat.m30 << ", " << mat.m31 << ", " << mat.m32 << ", " << mat.m33 << "]";
#else // AUL_USE_MATRIX_MULTI_LINE_STRING_REPRESENTATION
        out << "[" << mat.x << ", " << mat.y << ", " << mat.z << mat.w << "]";
#endif // AUL_USE_MATRIX_MULTI_LINE_STRING_REPRESENTATION
        return out;
    }

#undef AUL_INTERNAL_ZERO_NON_DIAGONAL

    //////////////////////////////////////////////////////////////////////////
    // Conversion
    //////////////////////////////////////////////////////////////////////////

#define AUL_INTERNAL_CONVERT_DEF_PARTIAL(T, U) template<> \
xform<T>& convert<xform<T>, xform<U>>(xform<T>& to, const xform<U>& from) \
{ \
to.m00 = (T)from.m00; to.m01 = (T)from.m01; to.m02 = (T)from.m02; to.m03 = (T)from.m03; \
to.m10 = (T)from.m10; to.m11 = (T)from.m11; to.m12 = (T)from.m12; to.m13 = (T)from.m13; \
to.m20 = (T)from.m20; to.m21 = (T)from.m21; to.m22 = (T)from.m22; to.m23 = (T)from.m23; \
to.m30 = (T)from.m30; to.m31 = (T)from.m31; to.m32 = (T)from.m32; to.m33 = (T)from.m33; \
return to; \
}

#define AUL_INTERNAL_CONVERT_DEF(T, U) AUL_INTERNAL_CONVERT_DEF_PARTIAL(T, U) \
AUL_INTERNAL_CONVERT_DEF_PARTIAL(U, T)

    AUL_INTERNAL_CONVERT_DEF(float, double);

#undef AUL_INTERNAL_CONVERT_DEF
#undef AUL_INTERNAL_CONVERT_DEF_PARTIAL
}