#pragma once

#include <aul/scalar_math.h>
#include <aul/macros.h>
#include <aul/vector3.h>

namespace aul
{
    template<typename T> struct matrix3x3;
    template<typename T> struct matrix4x4;

    template<typename T>
    wide_ostream& operator<< (wide_ostream& out, const matrix3x3<T>& mat);

    template<typename T>
    mb_ostream& operator<< (mb_ostream& out, const matrix3x3<T>& mat);
    
    template<typename T>
    struct matrix3x3
    {
        union
        {
            T data[9];
            struct 
            {
#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
                // Row 0
                T m00;
                T m01;
                T m02;
                // Row 1
                T m10;
                T m11;
                T m12;
                // Row 2
                T m20;
                T m21;
                T m22;
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
                // Column 0
                T m00;
                T m10;
                T m20;
                // Column 1
                T m01;
                T m11;
                T m21;
                // Column 2
                T m02;
                T m12;
                T m22;
#endif // AUL_USE_COORDINATE_HANDEDNESS
            };
            struct  
            {
                vector3<T> x;
                vector3<T> y;
                vector3<T> z;
            };
            struct  
            {
#if AUL_USE_UP_VECTOR == AUL_Y_UP
                vector3<T> right;
                vector3<T> up;
                vector3<T> forward;
#elif AUL_USE_UP_VECTOR == AUL_Z_UP
#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
                vector3<T> forward;
                vector3<T> right;
                vector3<T> up;
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
                vector3<T> right;
                vector3<T> forward;
                vector3<T> up;
#endif // AUL_USE_COORDINATE_HANDEDNESS
#endif
            };
        };

        static const matrix3x3 ZERO;
        static const matrix3x3 IDENTITY;
        static const matrix3x3 NEGATIVE_IDENTITY;
        // TODO: Add additional constants

        matrix3x3(T m00_ = scalar<T>::ONE, T m01_ = scalar<T>::ZERO, T m02_ = scalar<T>::ZERO
            , T m10_ = scalar<T>::ZERO, T m11_ = scalar<T>::ONE, T m12_ = scalar<T>::ZERO
            , T m20_ = scalar<T>::ZERO, T m21_ = scalar<T>::ZERO, T m22_ = scalar<T>::ONE)
            : m00(m00_), m01(m01_), m02(m02_)
            , m10(m10_), m11(m11_), m12(m12_)
            , m20(m20_), m21(m21_), m22(m22_)
        { }
        matrix3x3(const vector3<T>& x_, const vector3<T>& y_, const vector3<T>& z_)
            : x(x_), y(y_), z(z_)
        { }
        matrix3x3(const vector3<T>& diagonal)
            : m00(diagonal.x), m01(scalar<T>::ZERO), m02(scalar<T>::ZERO)
            , m10(scalar<T>::ZERO), m11(diagonal.y), m12(scalar<T>::ZERO)
            , m20(scalar<T>::ZERO), m21(scalar<T>::ZERO), m22(diagonal.z)
        { }
        matrix3x3(const T* arr_data);
        matrix3x3(const matrix4x4<T>& rhs);
        matrix3x3& operator=(const T* arr_data);
        matrix3x3& operator=(const vector3<T>& diagonal);
        matrix3x3& operator=(const matrix4x4<T>& rhs);

        inline bool operator==(const matrix3x3& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
        inline bool operator!=(const matrix3x3& rhs) const { return x != rhs.x || y != rhs.y || z != rhs.z; }
        inline matrix3x3 operator-() const { return matrix3x3(-x, -y, -z); }
        inline matrix3x3 operator+(const matrix3x3& rhs) const { return matrix3x3(x + rhs.x, y + rhs.y, z + rhs.z); }
        inline matrix3x3 operator-(const matrix3x3& rhs) const { return matrix3x3(x - rhs.x, y - rhs.y, z - rhs.z); }
        inline matrix3x3 operator*(T rhs) const { return matrix3x3(x * rhs, y * rhs, z * rhs); }
        inline friend matrix3x3 operator*(T lhs, const matrix3x3<T>& rhs) { return matrix3x3<T>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z); }
        inline matrix3x3 operator/(T rhs) const { return matrix3x3(x / rhs, y / rhs, z / rhs); }
        inline friend matrix3x3 operator/(T lhs, const matrix3x3<T>& rhs) { return matrix3x3<T>(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z); }
        matrix3x3 operator*(const matrix3x3& rhs) const;
        vector3<T> operator*(const vector3<T>& rhs) const;
        inline friend vector3<T> operator* (const vector3<T>& lhs, const matrix3x3<T>& rhs) { return rhs * lhs; }
        inline matrix3x3& operator*=(T rhs) { x *= rhs; y *= rhs; z *= rhs; return *this; }
        inline matrix3x3& operator/=(T rhs) { x /= rhs; y /= rhs; z /= rhs; return *this; }
        inline matrix3x3& operator*=(const matrix3x3& rhs);

        inline vector3<T> major_diagonal() const { return vector3<T>(m00, m11, m22); }
        inline matrix3x3& major_diagonal(T diagonal_x, T diagonal_y, T diagonal_z) { m00 = diagonal_x; m11 = diagonal_y; m22 = diagonal_z; return *this; }
        inline matrix3x3& major_diagonal(const vector3<T>& diagonal) { m00 = diagonal.x; m11 = diagonal.y; m22 = diagonal.z; return *this; }
        matrix3x3& transpose();
        matrix3x3 transposed() const;
        inline vector3<T> transform_point(const vector3<T>& point) const { return *this * point; }
        inline vector3<T> transform_vector(const vector3<T>& vec) const { return (*this * vec).normalized(); }
        T determinant() const;
        matrix3x3 inverse() const;
        matrix3x3& invert();

        inline static matrix3x3 make_uniform_scale(T scale) { return matrix3x3(vector3<T>(scale, scale, scale)); }
        inline static matrix3x3 make_scale(T scale_x, T scale_y, T scale_z) { return matrix3x3(vector3<T>(scale_x, scale_y, scale_z)); }
        inline static matrix3x3 make_scale(const vector3<T>& scale) { return matrix3x3(scale); }
        static matrix3x3 make_scale(T scale, const vector3<T>& axis);
        static matrix3x3 make_skew(T radians, const vector3<T>& skew_axis, const vector3<T>& normal_axis);
        static matrix3x3 make_rotation_x(T radians);
        static matrix3x3 make_rotation_y(T radians);
        static matrix3x3 make_rotation_z(T radians);
        static matrix3x3 make_rotation_xyz(T radians_x, T radians_y, T radians_z);
        static matrix3x3 make_rotation_xyz(const vector3<T>& radians_xyz);
        static matrix3x3 make_rotation_ypr(T radians_yaw, T radians_pitch, T radians_roll);
        static matrix3x3 make_rotation_ypr(const vector3<T>& radians_ypr);
        static matrix3x3 make_rotation_angle_axis(T radians, const vector3<T>& axis);
        inline static matrix3x3 make_rotation_axis_angle(const vector3<T>& axis, T radians) { return make_rotation_angle_axis(radians, axis); }
        static matrix3x3 make_reflection(const vector3<T>& normal);
        static matrix3x3 make_involution(const vector3<T>& vec);

        matrix3x3& to_uniform_scale(T scale);
        matrix3x3& to_scale(T scale_x, T scale_y, T scale_z);
        inline matrix3x3& to_scale(const vector3<T>& scale);
        matrix3x3& to_scale(T scale, const vector3<T>& axis);
        matrix3x3& to_skew(T radians, const vector3<T>& skew_axis, const vector3<T>& normal_axis);
        matrix3x3& to_rotation_x(T radians);
        matrix3x3& to_rotation_y(T radians);
        matrix3x3& to_rotation_z(T radians);
        matrix3x3& to_rotation_xyz(T radians_x, T radians_y, T radians_z);
        matrix3x3& to_rotation_xyz(const vector3<T>& radians_xyz);
        matrix3x3& to_rotation_ypr(T radians_yaw, T radians_pitch, T radians_roll);
        matrix3x3& to_rotation_ypr(vector3<T>& radians_ypr);
        matrix3x3& to_rotation_angle_axis(T radians, const vector3<T>& axis);
        inline matrix3x3& to_rotation_axis_angle(const vector3<T>& axis, T radians) { return to_rotation_angle_axis(radians, axis); }
        matrix3x3& to_reflection(const vector3<T>& normal);
        matrix3x3& to_involution(const vector3<T>& vec);

        inline matrix3x3& scale_uniformly(T scale) { return *this *= make_uniform_scale(scale); }
        inline matrix3x3& scale(T scale_x, T scale_y, T scale_z) { return *this *= make_scale(scale_x, scale_y, scale_z); }
        inline matrix3x3& scale(const vector3<T>& scale) { return *this *= make_scale(scale); }
        inline matrix3x3& scale(T scale, const vector3<T>& axis) { return *this *= make_scale(scale, axis); }
        inline matrix3x3& skew(T radians, const vector3<T>& skew_axis, const vector3<T>& normal_axis) { return *this *= make_skew(radians, skew_axis, normal_axis); }
        inline matrix3x3& rotate_x(T radians) { return *this *= make_rotation_x(radians); }
        inline matrix3x3& rotate_y(T radians) { return *this *= make_rotation_y(radians); }
        inline matrix3x3& rotate_z(T radians) { return *this *= make_rotation_z(radians); }
        inline matrix3x3& rotate_xyz(T radians_x, T radians_y, T radians_z) { return *this *= make_rotation_xyz(radians_x, radians_y, radians_z); }
        inline matrix3x3& rotate_xyz(const vector3<T>& radians_xyz) { return *this *= make_rotation_xyz(radians_xyz); }
        inline matrix3x3& rotate_ypr(T radians_yaw, T radians_pitch, T radians_roll) { return *this *= make_rotation_ypr(radians_yaw, radians_pitch, radians_roll); }
        inline matrix3x3& rotate_ypr(const vector3<T>& radians_ypr) { return *this *= make_rotation_ypr(radians_ypr); }
        inline matrix3x3& rotate_angle_axis(T radians, const vector3<T>& axis) { return *this *= make_rotation_angle_axis(radians, axis); }
        inline matrix3x3& rotate_axis_angle(const vector3<T>& axis, T radians) { return *this *= make_rotation_angle_axis(radians, axis); }
        inline matrix3x3& reflect(const vector3<T>& normal) { return *this *= make_reflection(normal); }

        operator wide_string() const;
        friend wide_ostream& operator<< <> (wide_ostream& out, const matrix3x3<T>& mat);

        operator mb_string() const;
        friend mb_ostream& operator<< <> (mb_ostream& out, const matrix3x3<T>& mat);
    };

#define AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC(T, extension) typedef matrix3x3<T> matrix3x3 ## extension; \
extern template struct matrix3x3<T>

    AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC(float, f);
    AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC(double, d);

#undef AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC

    //////////////////////////////////////////////////////////////////////////
    // Conversion
    //////////////////////////////////////////////////////////////////////////

#define AUL_INTERNAL_CONVERT_DEC(T, U) template<> matrix3x3<T>& convert<matrix3x3<T>, matrix3x3<U>>(matrix3x3<T>& to, const matrix3x3<U>& from); \
template<> matrix3x3<U>& convert<matrix3x3<U>, matrix3x3<T>>(matrix3x3<U>& to, const matrix3x3<T>& from)

    AUL_INTERNAL_CONVERT_DEC(float, double);

#undef AUL_INTERNAL_CONVERT_DEC
}