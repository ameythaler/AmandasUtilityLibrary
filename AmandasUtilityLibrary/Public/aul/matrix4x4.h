#pragma once

#include <aul/scalar_math.h>
#include <aul/macros.h>
#include <aul/vector4.h>

namespace aul
{
    template<typename T> struct matrix4x4;
    template<typename T> struct matrix3x3;

    template<typename T>
    wide_ostream& operator<< (wide_ostream& out, const matrix4x4<T>& mat);

    template<typename T>
    mb_ostream& operator<< (mb_ostream& out, const matrix4x4<T>& mat);

    template<typename T>
    struct matrix4x4
    {
        union
        {
            T data[16];
            struct
            {
#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
                // Row 0
                T m00;
                T m01;
                T m02;
                T m03;
                // Row 1
                T m10;
                T m11;
                T m12;
                T m13;
                // Row 2
                T m20;
                T m21;
                T m22;
                T m23;
                // Row 3
                T m30;
                T m31;
                T m32;
                T m33;
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
                // Column 0
                T m00;
                T m10;
                T m20;
                T m30;
                // Column 1
                T m01;
                T m11;
                T m21;
                T m31;
                // Column 2
                T m02;
                T m12;
                T m22;
                T m32;
                // Column 3
                T m03;
                T m13;
                T m23;
                T m33;
#endif // AUL_USE_COORDINATE_HANDEDNESS
            };
            struct
            {
                vector4<T> x;
                vector4<T> y;
                vector4<T> z;
                vector4<T> w;
            };
            struct
            {
#if AUL_USE_UP_VECTOR == AUL_Y_UP
                vector4<T> right;
                vector4<T> up;
                vector4<T> forward;
                vector4<T> position;
#elif AUL_USE_UP_VECTOR == AUL_Z_UP
#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
                vector4<T> forward;
                vector4<T> right;
                vector4<T> up;
                vector4<T> position;
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
                vector4<T> right;
                vector4<T> forward;
                vector4<T> up;
                vector4<T> position;
#endif // AUL_USE_COORDINATE_HANDEDNESS
#endif
            };
        };

        static const matrix4x4 ZERO;
        static const matrix4x4 IDENTITY;
        static const matrix4x4 NEGATIVE_IDENTITY;
        // TODO: Add additional constants

        matrix4x4(T m00_ = scalar<T>::ONE, T m01_ = scalar<T>::ZERO, T m02_ = scalar<T>::ZERO, T m03_ = scalar<T>::ZERO
            , T m10_ = scalar<T>::ZERO, T m11_ = scalar<T>::ONE, T m12_ = scalar<T>::ZERO, T m13_ = scalar<T>::ZERO
            , T m20_ = scalar<T>::ZERO, T m21_ = scalar<T>::ZERO, T m22_ = scalar<T>::ONE, T m23_ = scalar<T>::ZERO
            , T m30_ = scalar<T>::ZERO, T m31_ = scalar<T>::ZERO, T m32_ = scalar<T>::ZERO, T m33_ = scalar<T>::ONE)
            : m00(m00_), m01(m01_), m02(m02_), m03(m03_)
            , m10(m10_), m11(m11_), m12(m12_), m13(m13_)
            , m20(m20_), m21(m21_), m22(m22_), m23(m23_)
            , m30(m30_), m31(m31_), m32(m32_), m33(m33_)
        { }
        matrix4x4(const vector4<T>& x_, const vector4<T>& y_, const vector4<T>& z_, const vector4<T>& w_)
            : x(x_), y(y_), z(z_), w(w_)
        { }
        matrix4x4(const vector3<T>& diagonal, T w = scalar<T>::ONE)
            : m00(diagonal.x), m01(scalar<T>::ZERO), m02(scalar<T>::ZERO), m03(scalar<T>::ZERO)
            , m10(scalar<T>::ZERO), m11(diagonal.y), m12(scalar<T>::ZERO), m13(scalar<T>::ZERO)
            , m20(scalar<T>::ZERO), m21(scalar<T>::ZERO), m22(diagonal.z), m23(scalar<T>::ZERO)
            , m30(scalar<T>::ZERO), m31(scalar<T>::ZERO), m32(scalar<T>::ZERO), m33(w)
        { }
        matrix4x4(const vector4<T>& diagonal)
            : m00(diagonal.x), m01(scalar<T>::ZERO), m02(scalar<T>::ZERO), m03(scalar<T>::ZERO)
            , m10(scalar<T>::ZERO), m11(diagonal.y), m12(scalar<T>::ZERO), m13(scalar<T>::ZERO)
            , m20(scalar<T>::ZERO), m21(scalar<T>::ZERO), m22(diagonal.z), m23(scalar<T>::ZERO)
            , m30(scalar<T>::ZERO), m31(scalar<T>::ZERO), m32(scalar<T>::ZERO), m33(diagonal.w)
        { }
        matrix4x4(const matrix3x3<T>& rhs);
        matrix4x4(const T* arr_data);
        matrix4x4& operator=(const T* arr_data);
        matrix4x4& operator=(const vector4<T>& diagonal);
        matrix4x4& operator=(const vector3<T>& diagonal);
        matrix4x4& operator=(const matrix3x3<T>& rhs);

        inline bool operator==(const matrix4x4& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w; }
        inline bool operator!=(const matrix4x4& rhs) const { return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w; }
        inline matrix4x4 operator-() const { return matrix4x4(-x, -y, -z, -w); }
        inline matrix4x4 operator+(const matrix4x4& rhs) const { return matrix4x4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
        inline matrix4x4 operator-(const matrix4x4& rhs) const { return matrix4x4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
        inline matrix4x4 operator*(T rhs) const { return matrix4x4(x * rhs, y * rhs, z * rhs, w * rhs); }
        inline friend matrix4x4 operator*(T lhs, const matrix4x4<T>& rhs) { return matrix4x4<T>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w); }
        inline matrix4x4 operator/(T rhs) const { return matrix4x4(x / rhs, y / rhs, z / rhs, w / rhs); }
        inline friend matrix4x4 operator/(T lhs, const matrix4x4<T>& rhs) { return matrix4x4<T>(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z, lhs * rhs.w); }
        matrix4x4 operator*(const matrix4x4& rhs) const;
        vector4<T> operator*(const vector4<T>& rhs) const;
        inline friend vector4<T> operator* (const vector4<T>& lhs, const matrix4x4<T>& rhs) { return rhs * lhs; }
        inline matrix4x4& operator*=(T rhs) { x *= rhs; y *= rhs; z *= rhs; w *= rhs; return *this; }
        inline matrix4x4& operator/=(T rhs) { x /= rhs; y /= rhs; z /= rhs; w /= rhs; return *this; }
        inline matrix4x4& operator*=(const matrix4x4& rhs);

        inline vector4<T> major_diagonal() const { return vector4<T>(m00, m11, m22, m33); }
        inline matrix4x4& major_diagonal(T diagonal_x, T diagonal_y, T diagonal_z, T diagonal_w) { m00 = diagonal_x; m11 = diagonal_y; m22 = diagonal_z; m33 = diagonal_w; return *this; }
        inline matrix4x4& major_diagonal(const vector4<T>& diagonal) { m00 = diagonal.x; m11 = diagonal.y; m22 = diagonal.z; m33 = diagonal.w; return *this; }
        matrix4x4& transpose();
        matrix4x4 transposed() const;
        inline vector4<T> transform_point(const vector4<T>& point) const { return *this * point; }
        inline vector4<T> transform_vector(const vector4<T>& vec) const { return (*this * vec).normalized(); }
        T determinant() const;
        matrix4x4 inverse() const;
        matrix4x4& invert();

        inline static matrix4x4 make_uniform_scale(T scale) { return matrix4x4(vector4<T>(scale, scale, scale, scalar<T>::ONE)); }
        inline static matrix4x4 make_scale(T scale_x, T scale_y, T scale_z) { return matrix4x4(vector4<T>(scale_x, scale_y, scale_z, scalar<T>::ONE)); }
        inline static matrix4x4 make_scale(const vector3<T>& scale) { return matrix4x4(scale, scalar<T>::ONE); }
        static matrix4x4 make_scale(T scale, const vector3<T>& axis);
        static matrix4x4 make_translation(T x_, T y_, T z_);
        static matrix4x4 make_translation(const vector3<T>& translation);
        static matrix4x4 make_skew(T radians, const vector3<T>& skew_axis, const vector3<T>& normal_axis);
        static matrix4x4 make_rotation_x(T radians);
        static matrix4x4 make_rotation_y(T radians);
        static matrix4x4 make_rotation_z(T radians);
        static matrix4x4 make_rotation_xyz(T radians_x, T radians_y, T radians_z);
        static matrix4x4 make_rotation_xyz(const vector3<T>& radians_xyz);
        static matrix4x4 make_rotation_ypr(T radians_yaw, T radians_pitch, T radians_roll);
        static matrix4x4 make_rotation_ypr(const vector3<T>& radians_ypr);
        static matrix4x4 make_rotation_angle_axis(T radians, const vector3<T>& axis);
        inline static matrix4x4 make_rotation_axis_angle(const vector3<T>& axis, T radians) { return make_rotation_angle_axis(radians, axis); }
        static matrix4x4 make_reflection(const vector3<T>& normal);
        static matrix4x4 make_involution(const vector3<T>& vec);

        matrix4x4& to_uniform_scale(T scale);
        matrix4x4& to_scale(T scale_x, T scale_y, T scale_z);
        inline matrix4x4& to_scale(const vector3<T>& scale);
        matrix4x4& to_scale(T scale, const vector3<T>& axis);
        matrix4x4& to_translation(T x_, T y_, T z_);
        matrix4x4& to_translation(const vector3<T>& translation);
        matrix4x4& to_skew(T radians, const vector3<T>& skew_axis, const vector3<T>& normal_axis);
        matrix4x4& to_rotation_x(T radians);
        matrix4x4& to_rotation_y(T radians);
        matrix4x4& to_rotation_z(T radians);
        matrix4x4& to_rotation_xyz(T radians_x, T radians_y, T radians_z);
        matrix4x4& to_rotation_xyz(const vector3<T>& radians_xyz);
        matrix4x4& to_rotation_ypr(T radians_yaw, T radians_pitch, T radians_roll);
        matrix4x4& to_rotation_ypr(vector3<T>& radians_ypr);
        matrix4x4& to_rotation_angle_axis(T radians, const vector3<T>& axis);
        inline matrix4x4& to_rotation_axis_angle(const vector3<T>& axis, T radians) { return to_rotation_angle_axis(radians, axis); }
        matrix4x4& to_reflection(const vector3<T>& normal);
        matrix4x4& to_involution(const vector3<T>& vec);

        inline matrix4x4& scale_uniformly(T scale) { return *this *= make_uniform_scale(scale); }
        inline matrix4x4& scale(T scale_x, T scale_y, T scale_z) { return *this *= make_scale(scale_x, scale_y, scale_z); }
        inline matrix4x4& scale(const vector3<T>& scale) { return *this *= make_scale(scale); }
        inline matrix4x4& scale(T scale, const vector3<T>& axis) { return *this *= make_scale(scale, axis); }
        inline matrix4x4& translate(T x_, T y_, T z_) { return *this *= make_translation(x_, y_, z_); }
        inline matrix4x4& translate(vector3<T>& translation) { return *this *= make_translation(translation); }
        inline matrix4x4& skew(T radians, const vector3<T>& skew_axis, const vector3<T>& normal_axis) { return *this *= make_skew(radians, skew_axis, normal_axis); }
        inline matrix4x4& rotate_x(T radians) { return *this *= make_rotation_x(radians); }
        inline matrix4x4& rotate_y(T radians) { return *this *= make_rotation_y(radians); }
        inline matrix4x4& rotate_z(T radians) { return *this *= make_rotation_z(radians); }
        inline matrix4x4& rotate_xyz(T radians_x, T radians_y, T radians_z) { return *this *= make_rotation_xyz(radians_x, radians_y, radians_z); }
        inline matrix4x4& rotate_xyz(const vector3<T>& radians_xyz) { return *this *= make_rotation_xyz(radians_xyz); }
        inline matrix4x4& rotate_ypr(T radians_yaw, T radians_pitch, T radians_roll) { return *this *= make_rotation_ypr(radians_yaw, radians_pitch, radians_roll); }
        inline matrix4x4& rotate_ypr(const vector3<T>& radians_ypr) { return *this *= make_rotation_ypr(radians_ypr); }
        inline matrix4x4& rotate_angle_axis(T radians, const vector3<T>& axis) { return *this *= make_rotation_angle_axis(radians, axis); }
        inline matrix4x4& rotate_axis_angle(const vector3<T>& axis, T radians) { return *this *= make_rotation_angle_axis(radians, axis); }
        inline matrix4x4& reflect(const vector3<T>& normal) { return *this *= make_reflection(normal); }

        operator wide_string() const;
        friend wide_ostream& operator<< <> (wide_ostream& out, const matrix4x4<T>& mat);

        operator mb_string() const;
        friend mb_ostream& operator<< <> (mb_ostream& out, const matrix4x4<T>& mat);
    };

#define AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC(T, extension) typedef matrix4x4<T> matrix4x4 ## extension; \
extern template struct matrix4x4<T>

    AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC(float, f);
    AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC(double, d);

#undef AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC

    //////////////////////////////////////////////////////////////////////////
    // Conversion
    //////////////////////////////////////////////////////////////////////////

#define AUL_INTERNAL_CONVERT_DEC(T, U) template<> matrix4x4<T>& convert<matrix4x4<T>, matrix4x4<U>>(matrix4x4<T>& to, const matrix4x4<U>& from); \
template<> matrix4x4<U>& convert<matrix4x4<U>, matrix4x4<T>>(matrix4x4<U>& to, const matrix4x4<T>& from)

    AUL_INTERNAL_CONVERT_DEC(float, double);

#undef AUL_INTERNAL_CONVERT_DEC
}