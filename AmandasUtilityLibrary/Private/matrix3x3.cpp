#include "stdafx.h"
#include <aul/matrix3x3.h>

namespace aul
{
    template<typename T>
    const matrix3x3<T> matrix3x3<T>::ZERO = matrix3x3<T>(vector3<T>::ZERO, vector3<T>::ZERO, vector3<T>::ZERO);

    template<typename T>
    const matrix3x3<T> matrix3x3<T>::IDENTITY = matrix3x3<T>(scalar<T>::ONE, scalar<T>::ZERO, scalar<T>::ZERO
        , scalar<T>::ZERO, scalar<T>::ONE, scalar<T>::ZERO
        , scalar<T>::ZERO, scalar<T>::ZERO, scalar<T>::ONE);

    template<typename T>
    const matrix3x3<T> matrix3x3<T>::NEGATIVE_IDENTITY = matrix3x3<T>(-scalar<T>::ONE, scalar<T>::ZERO, scalar<T>::ZERO
        , scalar<T>::ZERO, -scalar<T>::ONE, scalar<T>::ZERO
        , scalar<T>::ZERO, scalar<T>::ZERO, -scalar<T>::ONE);

#define AUL_INTERNAL_EXPLICIT_INST_DEF(T) template struct matrix3x3<T>

    AUL_INTERNAL_EXPLICIT_INST_DEF(float);
    AUL_INTERNAL_EXPLICIT_INST_DEF(double);

#undef AUL_INTERNAL_EXPLICIT_INST_DEF

    template<typename T>
    matrix3x3<T>::matrix3x3(const T* arr_data)
    {
        memcpy(data, arr_data, sizeof(T) * 9);
    }

    template<typename T>
    matrix3x3<T>& matrix3x3<T>::operator=(const T* arr_data)
    {
        memcpy(data, arr_data, sizeof(T) * 9);
        return *this;
    }

    template<typename T>
    matrix3x3<T> matrix3x3<T>::operator*(const matrix3x3<T>& rhs) const
    {
        return matrix3x3<T>(
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
            m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22
            );
    }

    template<typename T>
    vector3<T> matrix3x3<T>::operator*(const vector3<T>& rhs) const
    {
        return vector3<T>(m00 * rhs.x + m10 * rhs.y + m20 * rhs.z,
            m01 * rhs.x + m11 * rhs.y + m21 * rhs.z,
            m02 * rhs.x + m12 * rhs.y + m22 * rhs.z);
    }

    template<typename T>
    matrix3x3<T>& matrix3x3<T>::transpose()
    {
        std::swap(m01, m10);
        std::swap(m02, m20);
        std::swap(m12, m21);
        return *this;
    }

    template<typename T>
    matrix3x3<T> matrix3x3<T>::transposed() const
    {
        return matrix3x3<T>(m00, m10, m20,
            m01, m11, m21,
            m02, m12, m22);
    }

    template<typename T>
    T matrix3x3<T>::determinant() const
    {
        return m00 * (m11 * m22 - m12 * m21) - m01 * (m10 * m22 - m12 * m20) + m02 * (m10 * m21 - m11 * m20);
    }

    template<typename T>
    matrix3x3<T> matrix3x3<T>::inverse() const
    {
        vector3<T> r0 = y.cross(z);
        vector3<T> r1 = z.cross(x);
        vector3<T> r2 = x.cross(y);

        T invDet = scalar<T>::ONE / r2.dot(z);
        matrix3x3<T> retVal(r0, r1, r2);
        return retVal * invDet;
    }

    template<typename T>
    matrix3x3<T> matrix3x3<T>::rotation_x(T radians)
    {
        return matrix3x3<T>(
            scalar<T>::ONE, scalar<T>::ZERO, scalar<T>::ZERO,
            scalar<T>::ZERO, cos(radians), -sin(radians),
            scalar<T>::ZERO, sin(radians), cos(radians)
            );
    }

    template<typename T>
    matrix3x3<T> matrix3x3<T>::rotation_y(T radians)
    {
        return matrix3x3<T>(
            cos(radians), scalar<T>::ZERO, sin(radians),
            scalar<T>::ZERO, scalar<T>::ONE, scalar<T>::ZERO,
            -sin(radians), scalar<T>::ZERO, cos(radians)
            );
    }

    template<typename T>
    matrix3x3<T> matrix3x3<T>::rotation_z(T radians)
    {
        return matrix3x3<T>(
            cos(radians), -sin(radians), scalar<T>::ZERO,
            sin(radians), cos(radians), scalar<T>::ZERO,
            scalar<T>::ZERO, scalar<T>::ZERO, scalar<T>::ONE
            );
    }

    template<typename T>
    matrix3x3<T>::operator wide_string() const
    {
        wide_stringstream stream;
        stream << *this;
        return stream.str();
    }

    template<typename T>
    wide_ostream& operator<<(wide_ostream& out, const matrix3x3<T>& mat)
    {
#if AUL_USE_MATRIX_MULTI_LINE_STRING_REPRESENTATION
        out << L"[" << mat.m00 << L", " << mat.m01 << L", " << mat.m02 << L"]\n"
            << L"[" << mat.m10 << L", " << mat.m11 << L", " << mat.m12 << L"]\n"
            << L"[" << mat.m20 << L", " << mat.m21 << L", " << mat.m22 << L"]";
#else // AUL_USE_MATRIX_MULTI_LINE_STRING_REPRESENTATION
        out << L"[" << mat.x << L", " << mat.y << L", " << mat.z << L"]";
#endif // AUL_USE_MATRIX_MULTI_LINE_STRING_REPRESENTATION
        return out;
    }

    template<typename T>
    matrix3x3<T>::operator mb_string() const
    {
        mb_stringstream stream;
        stream << *this;
        return stream.str();
    }

    template<typename T>
    mb_ostream& operator<<(mb_ostream& out, const matrix3x3<T>& mat)
    {
#if AUL_USE_MATRIX_MULTI_LINE_STRING_REPRESENTATION
        out << "[" << mat.m00 << ", " << mat.m01 << ", " << mat.m02 << "]\n"
            << "[" << mat.m10 << ", " << mat.m11 << ", " << mat.m12 << "]\n"
            << "[" << mat.m20 << ", " << mat.m21 << ", " << mat.m22 << "]";
#else // AUL_USE_MATRIX_MULTI_LINE_STRING_REPRESENTATION
        out << "[" << mat.x << ", " << mat.y << ", " << mat.z << "]";
#endif // AUL_USE_MATRIX_MULTI_LINE_STRING_REPRESENTATION
        return out;
    }
}