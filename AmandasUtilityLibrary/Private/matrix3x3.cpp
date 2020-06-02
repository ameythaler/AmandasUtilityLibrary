#include "stdafx.h"
#include <aul/matrix3x3.h>

namespace aul
{
#define AUL_INTERNAL_ZERO_NON_DIAGONAL() m01 = m02 = m10 = m12 = m20 = m21 = scalar<T>::ZERO

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
    matrix3x3<T>& matrix3x3<T>::operator=(const vector3<T>& diagonal)
    {
        m00 = diagonal.x;
        m11 = diagonal.y;
        m22 = diagonal.z;
        AUL_INTERNAL_ZERO_NON_DIAGONAL();
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
#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return vector3<T>(m00 * rhs.x + m10 * rhs.y + m20 * rhs.z,
            m01 * rhs.x + m11 * rhs.y + m21 * rhs.z,
            m02 * rhs.x + m12 * rhs.y + m22 * rhs.z);
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return vector3<T>(m00 * rhs.x + m01 * rhs.y + m02 * rhs.z,
            m10 * rhs.x + m11 * rhs.y + m12 * rhs.z,
            m20 * rhs.x + m21 * rhs.y + m22 * rhs.z);
#endif // AUL_USE_COORDINATE_HANDEDNESS
    }

    template<typename T>
    matrix3x3<T>& matrix3x3<T>::operator*=(const matrix3x3<T>& rhs)
    {
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

        m00 = t00; m01 = t01; m02 = t02;
        m10 = t10; m11 = t11; m12 = t12;
        m20 = t20; m21 = t21; m22 = t22;

        return *this;
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
#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        vector3<T> c0 = y.cross(z);
        vector3<T> c1 = z.cross(x);
        vector3<T> c2 = x.cross(y);

        T invDet = scalar<T>::ONE / c2.dot(z);
        return matrix3x3<T>(
            c0.x * invDet, c1.x * invDet, c2.x * invDet,
            c0.y * invDet, c1.y * invDet, c2.y * invDet,
            c0.z * invDet, c1.z * invDet, c2.z * invDet
        );
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        vector3<T> r0 = y.cross(z);
        vector3<T> r1 = z.cross(x);
        vector3<T> r2 = x.cross(y);

        T invDet = scalar<T>::ONE / r2.dot(z);
        return matrix3x3<T>(
            r0.x * invDet, r0.y * invDet, r0.z * invDet,
            r1.x * invDet, r1.y * invDet, r1.z * invDet,
            r2.x * invDet, r2.y * invDet, r2.z * invDet
        );
#endif // AUL_USE_COORDINATE_HANDEDNESS
    }

    template<typename T>
    matrix3x3<T>& matrix3x3<T>::invert()
    {
#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        vector3<T> c0 = y.cross(z);
        vector3<T> c1 = z.cross(x);
        vector3<T> c2 = x.cross(y);

        T invDet = scalar<T>::ONE / c2.dot(z);
        m00 = c0.x * invDet; m01 = c1.x * invDet; m02 = c2.x * invDet;
        m10 = c0.y * invDet; m11 = c1.y * invDet; m12 = c2.y * invDet;
        m20 = c0.z * invDet; m21 = c1.z * invDet; m22 = c2.z * invDet;
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        vector3<T> r0 = y.cross(z);
        vector3<T> r1 = z.cross(x);
        vector3<T> r2 = x.cross(y);

        T invDet = scalar<T>::ONE / r2.dot(z);
        m00 = r0.x * invDet; m01 = r0.y * invDet; m02 = r0.z * invDet;
        m10 = r1.x * invDet; m11 = r1.y * invDet; m12 = r1.z * invDet;
        m20 = r2.x * invDet; m21 = r2.y * invDet; m22 = r2.z * invDet;
#endif // AUL_USE_COORDINATE_HANDEDNESS

        return *this;
    }

    template<typename T>
    matrix3x3<T> matrix3x3<T>::make_scale(T scale, const vector3<T>& axis)
    {
        scale -= scalar<T>::ONE;
        T axis_x = axis.x * scale;
        T axis_y = axis.y * scale;
        T axis_z = axis.z * scale;
        T axis_xy = axis_x * axis.y;
        T axis_xz = axis_x * axis.z;
        T axis_yz = axis_y * axis.z;
        
        return matrix3x3<T>(
            axis_x * axis.x + scalar<T>::ONE, axis_xy, axis_xz,
            axis_xy, axis_y * axis.y + scalar<T>::ONE, axis_yz,
            axis_xz, axis_yz, axis_z * axis.z + scalar<T>::ONE
            );
    }

    template<typename T>
    matrix3x3<T> matrix3x3<T>::make_skew(T radians, const vector3<T>& skew_axis, const vector3<T>& normal_axis)
    {
        T tan_rad = tan(radians);
        T axis_x = skew_axis.x * tan_rad;
        T axis_y = skew_axis.y * tan_rad;
        T axis_z = skew_axis.z * tan_rad;

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return matrix3x3<T>(
            axis_x * normal_axis.x + scalar<T>::ONE, axis_y * normal_axis.x, axis_z * normal_axis.x,
            axis_x * normal_axis.y, axis_y * normal_axis.y + scalar<T>::ONE, axis_z * normal_axis.y,
            axis_x * normal_axis.z, axis_y * normal_axis.z, axis_z * normal_axis.z + scalar<T>::ONE
            );
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return matrix3x3<T>(
            axis_x * normal_axis.x + scalar<T>::ONE, axis_x * normal_axis.y, axis_x * normal_axis.z,
            axis_y * normal_axis.x, axis_y * normal_axis.y + scalar<T>::ONE, axis_y * normal_axis.z,
            axis_z * normal_axis.x, axis_z * normal_axis.y, axis_z * normal_axis.z + scalar<T>::ONE
            );
#endif // AUL_USE_COORDINATE_HANDEDNESS
    }

    template<typename T>
    matrix3x3<T> matrix3x3<T>::make_rotation_x(T radians)
    {
        T sin_rad = sin(radians);
        T cos_rad = cos(radians);

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return matrix3x3<T>(
            scalar<T>::ONE, scalar<T>::ZERO, scalar<T>::ZERO,
            scalar<T>::ZERO, cos_rad, sin_rad,
            scalar<T>::ZERO, -sin_rad, cos_rad
            );
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return matrix3x3<T>(
            scalar<T>::ONE, scalar<T>::ZERO, scalar<T>::ZERO,
            scalar<T>::ZERO, cos_rad, -sin_rad,
            scalar<T>::ZERO, sin_rad, cos_rad
            );
#endif // AUL_USE_COORDINATE_HANDEDNESS
    }

    template<typename T>
    matrix3x3<T> matrix3x3<T>::make_rotation_y(T radians)
    {
        T sin_rad = sin(radians);
        T cos_rad = cos(radians);

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return matrix3x3<T>(
            cos_rad, scalar<T>::ZERO, -sin_rad,
            scalar<T>::ZERO, scalar<T>::ONE, scalar<T>::ZERO,
            sin_rad, scalar<T>::ZERO, cos_rad
            );
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return matrix3x3<T>(
            cos_rad, scalar<T>::ZERO, sin_rad,
            scalar<T>::ZERO, scalar<T>::ONE, scalar<T>::ZERO,
            -sin_rad, scalar<T>::ZERO, cos_rad
            );
#endif // AUL_USE_COORDINATE_HANDEDNESS
    }

    template<typename T>
    matrix3x3<T> matrix3x3<T>::make_rotation_z(T radians)
    {
        T sin_rad = sin(radians);
        T cos_rad = cos(radians);

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return matrix3x3<T>(
            cos_rad, sin_rad, scalar<T>::ZERO,
            -sin_rad, cos_rad, scalar<T>::ZERO,
            scalar<T>::ZERO, scalar<T>::ZERO, scalar<T>::ONE
            );
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return matrix3x3<T>(
            cos_rad, -sin_rad, scalar<T>::ZERO,
            sin_rad, cos_rad, scalar<T>::ZERO,
            scalar<T>::ZERO, scalar<T>::ZERO, scalar<T>::ONE
            );
#endif // AUL_USE_COORDINATE_HANDEDNESS
    }

    template<typename T>
    matrix3x3<T> matrix3x3<T>::make_rotation_xyz(T radians_x, T radians_y, T radians_z)
    {
        return make_rotation_x(radians_x) * make_rotation_y(radians_y) * make_rotation_z(radians_z);
    }

    template<typename T>
    matrix3x3<T> matrix3x3<T>::make_rotation_xyz(const vector3<T>& radians_xyz)
    {
        return make_rotation_x(radians_xyz.x) * make_rotation_y(radians_xyz.y) * make_rotation_z(radians_xyz.z);
    }

    template<typename T>
    matrix3x3<T> matrix3x3<T>::make_rotation_ypr(T radians_yaw, T radians_pitch, T radians_roll)
    {
#if AUL_USE_UP_VECTOR == AUL_Y_UP
        return make_rotation_y(radians_yaw) * make_rotation_x(radians_pitch) * make_rotation_z(radians_roll);
#elif AUL_USE_UP_VECTOR == AUL_Z_UP
#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return make_rotation_z(radians_yaw) * make_rotation_y(radians_pitch) * make_rotation_x(radians_roll);
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return make_rotation_z(radians_yaw) * make_rotation_x(radians_pitch) * make_rotation_y(radians_roll);
#endif // AUL_USE_COORDINATE_HANDEDNESS
#endif // AUL_USE_UP_VECTOR
    }

    template<typename T>
    matrix3x3<T> matrix3x3<T>::make_rotation_ypr(const vector3<T>& radians_ypr)
    {
#if AUL_USE_UP_VECTOR == AUL_Y_UP
        return make_rotation_y(radians_ypr.yaw) * make_rotation_x(radians_ypr.pitch) * make_rotation_z(radians_ypr.roll);
#elif AUL_USE_UP_VECTOR == AUL_Z_UP
#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return make_rotation_z(radians_ypr.yaw) * make_rotation_y(radians_ypr.pitch) * make_rotation_x(radians_ypr.roll);
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return make_rotation_z(radians_ypr.yaw) * make_rotation_x(radians_ypr.pitch) * make_rotation_y(radians_ypr.roll);
#endif // AUL_USE_COORDINATE_HANDEDNESS
#endif // AUL_USE_UP_VECTOR
    }

    template<typename T>
    matrix3x3<T> matrix3x3<T>::make_rotation_angle_axis(T radians, const vector3<T>& axis)
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
        return matrix3x3<T>(
            cos_rad + axis_x * axis.x, axis_xy + sin_rad * axis.z, axis_xz - sin_rad * axis.y,
            axis_xy - sin_rad * axis.z, cos_rad + axis_y * axis.y, axis_yz + sin_rad * axis.x,
            axis_xz + sin_rad * axis.y, axis_yz - sin_rad * axis.x, cos_rad + axis_z * axis.z
            );
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return matrix3x3<T>(
            cos_rad + axis_x * axis.x, axis_xy - sin_rad * axis.z, axis_xz + sin_rad * axis.y,
            axis_xy + sin_rad * axis.z, cos_rad + axis_y * axis.y, axis_yz - sin_rad * axis.x,
            axis_xz - sin_rad * axis.y, axis_yz + sin_rad * axis.x, cos_rad + axis_z * axis.z
            );
#endif // AUL_USE_COORDINATE_HANDEDNESS
    }

    template<typename T>
    matrix3x3<T> matrix3x3<T>::make_reflection(const vector3<T>& normal)
    {
        T normal_x = normal.x * -scalar<T>::TWO;
        T normal_y = normal.y * -scalar<T>::TWO;
        T normal_z = normal.z * -scalar<T>::TWO;
        T normal_xy = normal_x * normal.y;
        T normal_xz = normal_x * normal.z;
        T normal_yz = normal_y * normal.z;

        return matrix3x3<T>(
            normal_x * normal.x + scalar<T>::ONE, normal_xy, normal_xz,
            normal_xy, normal_y * normal.y + scalar<T>::ONE, normal_yz,
            normal_xz, normal_yz, normal_z * normal.z + scalar<T>::ONE
            );
    }

    template<typename T>
    matrix3x3<T> matrix3x3<T>::make_involution(const vector3<T>& vec)
    {
        T vec_x = vec.x * scalar<T>::TWO;
        T vec_y = vec.y * scalar<T>::TWO;
        T vec_z = vec.z * scalar<T>::TWO;
        T vec_xy = vec_x * vec.y;
        T vec_xz = vec_x * vec.z;
        T vec_yz = vec_y * vec.z;

        return matrix3x3<T>(
            vec_x * vec.x - scalar<T>::ONE, vec_xy, vec_xz,
            vec_xy, vec_y * vec.y - scalar<T>::ONE, vec_yz,
            vec_xz, vec_yz, vec_z * vec.z - scalar<T>::ONE
            );
    }

    template<typename T>
    matrix3x3<T>& matrix3x3<T>::to_uniform_scale(T scale)
    {
        m00 = m11 = m22 = scale;
        AUL_INTERNAL_ZERO_NON_DIAGONAL();
        return *this;
    }

    template<typename T>
    matrix3x3<T>& matrix3x3<T>::to_scale(T scale_x, T scale_y, T scale_z)
    {
        m00 = scale_x;
        m11 = scale_y;
        m22 = scale_z;
        AUL_INTERNAL_ZERO_NON_DIAGONAL();
        return *this;
    }

    template<typename T>
    matrix3x3<T>& matrix3x3<T>::to_scale(const vector3<T>& scale)
    {
        m00 = scale.x;
        m11 = scale.y;
        m22 = scale.z;
        AUL_INTERNAL_ZERO_NON_DIAGONAL();
        return *this;
    }

    template<typename T>
    matrix3x3<T>& matrix3x3<T>::to_scale(T scale, const vector3<T>& axis)
    {
        scale -= scalar<T>::ONE;
        T axis_x = axis.x * scale;
        T axis_y = axis.y * scale;
        T axis_z = axis.z * scale;
        T axis_xy = axis_x * axis.y;
        T axis_xz = axis_x * axis.z;
        T axis_yz = axis_y * axis.z;

        m00 = axis_x * axis.x + scalar<T>::ONE; m01 = axis_xy; m02 = axis_xz;
        m10 = axis_xy; m11 = axis_y * axis.y + scalar<T>::ONE; m12 = axis_yz;
        m20 = axis_xz; m21 = axis_yz; m22 = axis_z * axis.z + scalar<T>::ONE;
        return *this;
    }

    template<typename T>
    matrix3x3<T>& matrix3x3<T>::to_skew(T radians, const vector3<T>& skew_axis, const vector3<T>& normal_axis)
    {
        T tan_rad = tan(radians);
        T axis_x = skew_axis.x * tan_rad;
        T axis_y = skew_axis.y * tan_rad;
        T axis_z = skew_axis.z * tan_rad;

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        m00 = axis_x * normal_axis.x + scalar<T>::ONE; m01 = axis_y * normal_axis.x; m02 = axis_z * normal_axis.x;
        m10 = axis_x * normal_axis.y; m11 = axis_y * normal_axis.y + scalar<T>::ONE; m12 = axis_z * normal_axis.y;
        m20 = axis_x * normal_axis.z; m21 = axis_y * normal_axis.z; m22 = axis_z * normal_axis.z + scalar<T>::ONE;
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        m00 = axis_x * normal_axis.x + scalar<T>::ONE; m01 = axis_x * normal_axis.y; m02 = axis_x * normal_axis.z;
        m10 = axis_y * normal_axis.x; m11 = axis_y * normal_axis.y + scalar<T>::ONE; m12 = axis_y * normal_axis.z;
        m20 = axis_z * normal_axis.x; m21 = axis_z * normal_axis.y; m22 = axis_z * normal_axis.z + scalar<T>::ONE;
#endif // AUL_USE_COORDINATE_HANDEDNESS

        return *this;
    }

    template<typename T>
    matrix3x3<T>& matrix3x3<T>::to_rotation_x(T radians)
    {
        T sin_rad = sin(radians);
        T cos_rad = cos(radians);

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        m00 = scalar<T>::ONE; m01 = scalar<T>::ZERO; m02 = scalar<T>::ZERO;
        m10 = scalar<T>::ZERO; m11 = cos_rad; m12 = sin_rad;
        m20 = scalar<T>::ZERO; m21 = -sin_rad; m22 = cos_rad;
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        m00 = scalar<T>::ONE; m01 = scalar<T>::ZERO; m02 = scalar<T>::ZERO;
        m10 = scalar<T>::ZERO; m11 = cos_rad; m12 = -sin_rad;
        m20 = scalar<T>::ZERO; m21 = sin_rad; m22 = cos_rad;
#endif // AUL_USE_COORDINATE_HANDEDNESS

        return *this;
    }

    template<typename T>
    matrix3x3<T>& matrix3x3<T>::to_rotation_y(T radians)
    {
        T sin_rad = sin(radians);
        T cos_rad = cos(radians);

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        m00 = cos_rad; m01 = scalar<T>::ZERO; m11 = -sin_rad;
        m10 = scalar<T>::ZERO; m11 = scalar<T>::ONE; m12 = scalar<T>::ZERO;
        m20 = sin_rad; m21 = scalar<T>::ZERO; m22 = cos_rad;
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        m00 = cos_rad; m01 = scalar<T>::ZERO; m02 = sin_rad;
        m10 = scalar<T>::ZERO; m11 = scalar<T>::ONE; m12 = scalar<T>::ZERO;
        m20 = -sin_rad; m21 = scalar<T>::ZERO; m22 = cos_rad;
#endif // AUL_USE_COORDINATE_HANDEDNESS

        return *this;
    }

    template<typename T>
    matrix3x3<T>& matrix3x3<T>::to_rotation_z(T radians)
    {
        T sin_rad = sin(radians);
        T cos_rad = cos(radians);

#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        m00 = cos_rad; m01 = sin_rad; m02 = scalar<T>::ZERO;
        m10 = -sin_rad; m11 = cos_rad; m12 = scalar<T>::ZERO;
        m20 = scalar<T>::ZERO; m21 = scalar<T>::ZERO; m22 = scalar<T>::ONE;
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        m00 = cos_rad; m01 = -sin_rad; m02 = scalar<T>::ZERO;
        m10 = sin_rad; m11 = cos_rad; m12 = scalar<T>::ZERO;
        m20 = scalar<T>::ZERO; m21 = scalar<T>::ZERO; m22 = scalar<T>::ONE;
#endif // AUL_USE_COORDINATE_HANDEDNESS

        return *this;
    }

    template<typename T>
    matrix3x3<T>& matrix3x3<T>::to_rotation_xyz(T radians_x, T radians_y, T radians_z)
    {
        return *this = make_rotation_x(radians_x) * make_rotation_y(radians_y) * make_rotation_z(radians_z);
    }

    template<typename T>
    matrix3x3<T>& matrix3x3<T>::to_rotation_xyz(const vector3<T>& radians_xyz)
    {
        return *this = make_rotation_x(radians_xyz.x) * make_rotation_y(radians_xyz.y) * make_rotation_z(radians_xyz.z);
    }

    template<typename T>
    matrix3x3<T>& matrix3x3<T>::to_rotation_ypr(T radians_yaw, T radians_pitch, T radians_roll)
    {
#if AUL_USE_UP_VECTOR == AUL_Y_UP
        return *this = make_rotation_y(radians_yaw) * make_rotation_x(radians_pitch) * make_rotation_z(radians_roll);
#elif AUL_USE_UP_VECTOR == AUL_Z_UP
#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return *this = make_rotation_z(radians_yaw) * make_rotation_y(radians_pitch) * make_rotation_x(radians_roll);
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return *this = make_rotation_z(radians_yaw) * make_rotation_x(radians_pitch) * make_rotation_y(radians_roll);
#endif // AUL_USE_COORDINATE_HANDEDNESS
#endif // AUL_USE_UP_VECTOR
    }

    template<typename T>
    matrix3x3<T>& matrix3x3<T>::to_rotation_ypr(vector3<T>& radians_ypr)
    {
#if AUL_USE_UP_VECTOR == AUL_Y_UP
        return *this = make_rotation_y(radians_ypr.yaw) * make_rotation_x(radians_ypr.pitch) * make_rotation_z(radians_ypr.roll);
#elif AUL_USE_UP_VECTOR == AUL_Z_UP
#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        return *this = make_rotation_z(radians_ypr.yaw) * make_rotation_y(radians_ypr.pitch) * make_rotation_x(radians_ypr.roll);
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        return *this = make_rotation_z(radians_ypr.yaw) * make_rotation_x(radians_ypr.pitch) * make_rotation_y(radians_ypr.roll);
#endif // AUL_USE_COORDINATE_HANDEDNESS
#endif // AUL_USE_UP_VECTOR
    }

    template<typename T>
    matrix3x3<T>& matrix3x3<T>::to_rotation_angle_axis(T radians, const vector3<T>& axis)
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
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        m00 = cos_rad + axis_x * axis.x; m01 = axis_xy - sin_rad * axis.z; m02 = axis_xz + sin_rad * axis.y;
        m10 = axis_xy + sin_rad * axis.z; m11 = cos_rad + axis_y * axis.y; m12 = axis_yz - sin_rad * axis.x;
        m20 = axis_xz - sin_rad * axis.y; m21 = axis_yz + sin_rad * axis.x; m22 = cos_rad + axis_z * axis.z;
#endif // AUL_USE_COORDINATE_HANDEDNESS

        return *this;
    }

    template<typename T>
    matrix3x3<T>& matrix3x3<T>::to_reflection(const vector3<T>& normal)
    {
        T normal_x = normal.x * -scalar<T>::TWO;
        T normal_y = normal.y * -scalar<T>::TWO;
        T normal_z = normal.z * -scalar<T>::TWO;
        T normal_xy = normal_x * normal.y;
        T normal_xz = normal_x * normal.z;
        T normal_yz = normal_y * normal.z;

        m00 = normal_x * normal.x + scalar<T>::ONE; m01 = normal_xy; m02 = normal_xz;
        m10 = normal_xy; m11 = normal_y * normal.y + scalar<T>::ONE; m12 = normal_yz;
        m20 = normal_xz; m21 = normal_yz; m22 = normal_z * normal.z + scalar<T>::ONE;

        return *this;
    }

    template<typename T>
    matrix3x3<T>& matrix3x3<T>::to_involution(const vector3<T>& vec)
    {
        T vec_x = vec.x * scalar<T>::TWO;
        T vec_y = vec.y * scalar<T>::TWO;
        T vec_z = vec.z * scalar<T>::TWO;
        T vec_xy = vec_x * vec.y;
        T vec_xz = vec_x * vec.z;
        T vec_yz = vec_y * vec.z;

        m00 = vec_x * vec.x - scalar<T>::ONE; m01 = vec_xy; m02 = vec_xz;
        m10 = vec_xy; m11 = vec_y * vec.y - scalar<T>::ONE; m12 = vec_yz;
        m20 = vec_xz; m21 = vec_yz; m22 = vec_z * vec.z - scalar<T>::ONE;

        return *this;
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

#undef AUL_INTERNAL_ZERO_NON_DIAGONAL
}