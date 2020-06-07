#pragma once

#include <aul/scalar_math.h>
#include <aul/macros.h>
#include <aul/vector4.h>

namespace aul
{
    template<typename T> struct xform;
    template<typename T> struct matrix3x3;
    template<typename T> struct matrix4x4;

    template<typename T>
    wide_ostream& operator<< (wide_ostream& out, const xform<T>& mat);

    template<typename T>
    mb_ostream& operator<< (mb_ostream& out, const xform<T>& mat);

    template<typename T>
    struct xform
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

        static const xform ZERO;
        static const xform IDENTITY;
        static const xform NEGATIVE_IDENTITY;
        // TODO: Add additional constants
#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        xform(T m00_ = scalar<T>::ONE, T m01_ = scalar<T>::ZERO, T m02_ = scalar<T>::ZERO
            , T m10_ = scalar<T>::ZERO, T m11_ = scalar<T>::ONE, T m12_ = scalar<T>::ZERO
            , T m20_ = scalar<T>::ZERO, T m21_ = scalar<T>::ZERO, T m22_ = scalar<T>::ONE
            , T m30_ = scalar<T>::ZERO, T m31_ = scalar<T>::ZERO, T m32_ = scalar<T>::ZERO)
            : m00(m00_), m01(m01_), m02(m02_), m03(scalar<T>::ZERO)
            , m10(m10_), m11(m11_), m12(m12_), m13(scalar<T>::ZERO)
            , m20(m20_), m21(m21_), m22(m22_), m23(scalar<T>::ZERO)
            , m30(m30_), m31(m31_), m32(m32_), m33(scalar<T>::ONE)
        { }
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        xform(T m00_ = scalar<T>::ONE, T m01_ = scalar<T>::ZERO, T m02_ = scalar<T>::ZERO, T m03_ = scalar<T>::ZERO
            , T m10_ = scalar<T>::ZERO, T m11_ = scalar<T>::ONE, T m12_ = scalar<T>::ZERO, T m13_ = scalar<T>::ZERO
            , T m20_ = scalar<T>::ZERO, T m21_ = scalar<T>::ZERO, T m22_ = scalar<T>::ONE, T m23_ = scalar<T>::ZERO)
            : m00(m00_), m01(m01_), m02(m02_), m03(m03_)
            , m10(m10_), m11(m11_), m12(m12_), m13(m13_)
            , m20(m20_), m21(m21_), m22(m22_), m23(m23_)
            , m30(scalar<T>::ZERO), m31(scalar<T>::ZERO), m32(scalar<T>::ZERO), m33(scalar<T>::ONE)
        { }
#endif // AUL_USE_COORDINATE_HANDEDNESS
        xform(const vector3<T>& x_, const vector3<T>& y_, const vector3<T>& z_, const vector3<T>& w_)
            : x(x_, scalar<T>::ZERO), y(y_, scalar<T>::ZERO), z(z_, scalar<T>::ZERO), w(w_, scalar<T>::ONE)
        { }
        xform(const vector3<T>& diagonal)
            : m00(diagonal.x), m01(scalar<T>::ZERO), m02(scalar<T>::ZERO), m03(scalar<T>::ZERO)
            , m10(scalar<T>::ZERO), m11(diagonal.y), m12(scalar<T>::ZERO), m13(scalar<T>::ZERO)
            , m20(scalar<T>::ZERO), m21(scalar<T>::ZERO), m22(diagonal.z), m23(scalar<T>::ZERO)
            , m30(scalar<T>::ZERO), m31(scalar<T>::ZERO), m32(scalar<T>::ZERO), m33(scalar<T>::ONE)
        { }
        xform(const matrix4x4<T>& rhs);
        xform(const matrix3x3<T>& rhs);
        xform(const matrix3x3<T>& mat, const vector3<T>& translation)
            : x(mat.x, scalar<T>::ZERO), y(mat.y, scalar<T>::ZERO), z(mat.z, scalar<T>::ZERO), w(translation, scalar<T>::ONE)
        { }
        xform(const T* arr_data);
        xform& operator=(const T* arr_data);
        xform& operator=(const vector3<T>& diagonal);
        xform& operator=(const matrix4x4<T>& rhs);
        xform& operator=(const matrix3x3<T>& rhs);

        // xform operators
        inline bool operator==(const xform& rhs) const { return x.xyz == rhs.x.xyz && y.xyz == rhs.y.xyz && z.xyz == rhs.z.xyz && w.xyz == rhs.w.xyz; }
        inline bool operator!=(const xform& rhs) const { return x.xyz != rhs.x.xyz || y.xyz != rhs.y.xyz || z.xyz != rhs.z.xyz || w.xyz != rhs.w.xyz; }
        inline xform operator-() const { return xform(-x.xyz, -y.xyz, -z.xyz, -w.xyz); }
        inline xform operator+(const xform& rhs) const { return xform(x.xyz + rhs.x.xyz, y.xyz + rhs.y.xyz, z.xyz + rhs.z.xyz, w.xyz + rhs.w.xyz); }
        inline xform operator-(const xform& rhs) const { return xform(x.xyz - rhs.x.xyz, y.xyz - rhs.y.xyz, z.xyz - rhs.z.xyz, w.xyz + rhs.w.xyz); }
        inline xform operator*(T rhs) const { return xform(x.xyz * rhs, y.xyz * rhs, z.xyz * rhs, w.xyz * rhs); }
        inline friend xform operator*(T lhs, const xform<T>& rhs) { return xform<T>(lhs * rhs.x.xyz, lhs * rhs.y.xyz, lhs * rhs.z.xyz, lhs * rhs.w.xyz); }
        inline xform operator/(T rhs) const { return xform(x.xyz / rhs, y.xyz / rhs, z.xyz / rhs, w.xyz / rhs); }
        inline friend xform operator/(T lhs, const xform<T>& rhs) { return xform<T>(lhs / rhs.x.xyz, lhs / rhs.y.xyz, lhs / rhs.z.xyz, lhs / rhs.w.xyz); }
        xform operator*(const xform& rhs) const;
        vector4<T> operator*(const vector4<T>& rhs) const;
        inline friend vector4<T> operator* (const vector4<T>& lhs, const xform<T>& rhs) { return rhs * lhs; }
        inline xform& operator*=(T rhs) { x.xyz *= rhs; y.xyz *= rhs; z.xyz *= rhs; w.xyz *= rhs; return *this; }
        inline xform& operator/=(T rhs) { x.xyz /= rhs; y.xyz /= rhs; z.xyz /= rhs; w.xyz /= rhs; return *this; }
        inline xform& operator*=(const xform& rhs);

        // matrix4x4 operators
        inline bool operator==(const matrix4x4<T>& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w; }
        inline bool operator!=(const matrix4x4<T>& rhs) const { return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w; }
        inline matrix4x4<T> operator+(const matrix4x4<T>& rhs) const { return matrix4x4<T>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
        inline matrix4x4<T> operator-(const matrix4x4<T>& rhs) const { return matrix4x4<T>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
        matrix4x4<T> operator*(const matrix4x4<T>& rhs) const;

        inline vector4<T> major_diagonal() const { return vector4<T>(m00, m11, m22, m33); }
        inline xform& major_diagonal(T diagonal_x, T diagonal_y, T diagonal_z) { m00 = diagonal_x; m11 = diagonal_y; m22 = diagonal_z; return *this; }
        inline xform& major_diagonal(const vector3<T>& diagonal) { m00 = diagonal.x; m11 = diagonal.y; m22 = diagonal.z; return *this; }
        inline vector4<T> transform(const vector4<T>& rhs) const { return *this * rhs; }
        vector3<T> transform_point(const vector3<T>& point) const;
        vector3<T> transform_vector(const vector3<T>& vec) const;
        T determinant() const;
        xform inverse() const;
        xform& invert();

        inline static xform make_uniform_scale(T scale) { return xform(vector3<T>(scale, scale, scale)); }
        inline static xform make_scale(T scale_x, T scale_y, T scale_z) { return xform(vector3<T>(scale_x, scale_y, scale_z)); }
        inline static xform make_scale(const vector3<T>& scale) { return xform(scale); }
        static xform make_scale(T scale, const vector3<T>& axis);
        static xform make_translation(T x_, T y_, T z_);
        static xform make_translation(const vector3<T>& translation);
        static xform make_skew(T radians, const vector3<T>& skew_axis, const vector3<T>& normal_axis);
        static xform make_rotation_x(T radians);
        static xform make_rotation_y(T radians);
        static xform make_rotation_z(T radians);
        static xform make_rotation_xyz(T radians_x, T radians_y, T radians_z);
        static xform make_rotation_xyz(const vector3<T>& radians_xyz);
        static xform make_rotation_ypr(T radians_yaw, T radians_pitch, T radians_roll);
        static xform make_rotation_ypr(const vector3<T>& radians_ypr);
        static xform make_rotation_angle_axis(T radians, const vector3<T>& axis);
        inline static xform make_rotation_axis_angle(const vector3<T>& axis, T radians) { return make_rotation_angle_axis(radians, axis); }
        static xform make_reflection(const vector3<T>& normal);
        static xform make_involution(const vector3<T>& vec);

        xform& to_uniform_scale(T scale);
        xform& to_scale(T scale_x, T scale_y, T scale_z);
        inline xform& to_scale(const vector3<T>& scale);
        xform& to_scale(T scale, const vector3<T>& axis);
        xform& to_translation(T x_, T y_, T z_);
        xform& to_translation(const vector3<T>& translation);
        xform& to_skew(T radians, const vector3<T>& skew_axis, const vector3<T>& normal_axis);
        xform& to_rotation_x(T radians);
        xform& to_rotation_y(T radians);
        xform& to_rotation_z(T radians);
        xform& to_rotation_xyz(T radians_x, T radians_y, T radians_z);
        xform& to_rotation_xyz(const vector3<T>& radians_xyz);
        xform& to_rotation_ypr(T radians_yaw, T radians_pitch, T radians_roll);
        xform& to_rotation_ypr(vector3<T>& radians_ypr);
        xform& to_rotation_angle_axis(T radians, const vector3<T>& axis);
        inline xform& to_rotation_axis_angle(const vector3<T>& axis, T radians) { return to_rotation_angle_axis(radians, axis); }
        xform& to_reflection(const vector3<T>& normal);
        xform& to_involution(const vector3<T>& vec);

#if AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_LEFT_HANDED
        inline xform& scale_uniformly(T scale) { return *this *= make_uniform_scale(scale); }
        inline xform& scale(T scale_x, T scale_y, T scale_z) { return *this *= make_scale(scale_x, scale_y, scale_z); }
        inline xform& scale(const vector3<T>& scale) { return *this *= make_scale(scale); }
        inline xform& scale(T scale, const vector3<T>& axis) { return *this *= make_scale(scale, axis); }
        inline xform& translate(T x_, T y_, T z_) { return *this *= make_translation(x_, y_, z_); }
        inline xform& translate(vector3<T>& translation) { return *this *= make_translation(translation); }
        inline xform& skew(T radians, const vector3<T>& skew_axis, const vector3<T>& normal_axis) { return *this *= make_skew(radians, skew_axis, normal_axis); }
        inline xform& rotate_x(T radians) { return *this *= make_rotation_x(radians); }
        inline xform& rotate_y(T radians) { return *this *= make_rotation_y(radians); }
        inline xform& rotate_z(T radians) { return *this *= make_rotation_z(radians); }
        inline xform& rotate_xyz(T radians_x, T radians_y, T radians_z) { return *this *= make_rotation_xyz(radians_x, radians_y, radians_z); }
        inline xform& rotate_xyz(const vector3<T>& radians_xyz) { return *this *= make_rotation_xyz(radians_xyz); }
        inline xform& rotate_ypr(T radians_yaw, T radians_pitch, T radians_roll) { return *this *= make_rotation_ypr(radians_yaw, radians_pitch, radians_roll); }
        inline xform& rotate_ypr(const vector3<T>& radians_ypr) { return *this *= make_rotation_ypr(radians_ypr); }
        inline xform& rotate_angle_axis(T radians, const vector3<T>& axis) { return *this *= make_rotation_angle_axis(radians, axis); }
        inline xform& rotate_axis_angle(const vector3<T>& axis, T radians) { return *this *= make_rotation_angle_axis(radians, axis); }
        inline xform& reflect(const vector3<T>& normal) { return *this *= make_reflection(normal); }
#elif AUL_USE_MATRIX_MULTIPLICATION_ORDER == AUL_RIGHT_HANDED
        inline xform& scale_uniformly(T scale) { return *this = make_uniform_scale(scale) * *this; }
        inline xform& scale(T scale_x, T scale_y, T scale_z) { return *this = make_scale(scale_x, scale_y, scale_z) * *this; }
        inline xform& scale(const vector3<T>& scale) { return *this = make_scale(scale) * *this; }
        inline xform& scale(T scale, const vector3<T>& axis) { return *this = make_scale(scale, axis) * *this; }
        inline xform& translate(T x_, T y_, T z_) { return *this = make_translation(x_, y_, z_) * *this; }
        inline xform& translate(vector3<T>& translation) { return *this = make_translation(translation) * *this; }
        inline xform& skew(T radians, const vector3<T>& skew_axis, const vector3<T>& normal_axis) { return *this = make_skew(radians, skew_axis, normal_axis) * *this; }
        inline xform& rotate_x(T radians) { return *this = make_rotation_x(radians) * *this; }
        inline xform& rotate_y(T radians) { return *this = make_rotation_y(radians) * *this; }
        inline xform& rotate_z(T radians) { return *this = make_rotation_z(radians) * *this; }
        inline xform& rotate_xyz(T radians_x, T radians_y, T radians_z) { return *this = make_rotation_xyz(radians_x, radians_y, radians_z) * *this; }
        inline xform& rotate_xyz(const vector3<T>& radians_xyz) { return *this = make_rotation_xyz(radians_xyz) * *this; }
        inline xform& rotate_ypr(T radians_yaw, T radians_pitch, T radians_roll) { return *this = make_rotation_ypr(radians_yaw, radians_pitch, radians_roll) * *this; }
        inline xform& rotate_ypr(const vector3<T>& radians_ypr) { return *this = make_rotation_ypr(radians_ypr) * *this; }
        inline xform& rotate_angle_axis(T radians, const vector3<T>& axis) { return *this = make_rotation_angle_axis(radians, axis) * *this; }
        inline xform& rotate_axis_angle(const vector3<T>& axis, T radians) { return *this = make_rotation_angle_axis(radians, axis) * *this; }
        inline xform& reflect(const vector3<T>& normal) { return *this = make_reflection(normal) * *this; }
#endif // AUL_USE_MATRIX_MULTIPLICATION_ORDER

        operator wide_string() const;
        friend wide_ostream& operator<< <> (wide_ostream& out, const xform<T>& mat);

        operator mb_string() const;
        friend mb_ostream& operator<< <> (mb_ostream& out, const xform<T>& mat);
    };

#define AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC(T, extension) typedef xform<T> xform ## extension; \
extern template struct xform<T>

    AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC(float, f);
    AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC(double, d);

#undef AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC

    //////////////////////////////////////////////////////////////////////////
    // Conversion
    //////////////////////////////////////////////////////////////////////////

#define AUL_INTERNAL_CONVERT_DEC(T, U) template<> xform<T>& convert<xform<T>, xform<U>>(xform<T>& to, const xform<U>& from); \
template<> xform<U>& convert<xform<U>, xform<T>>(xform<U>& to, const xform<T>& from)

    AUL_INTERNAL_CONVERT_DEC(float, double);

#undef AUL_INTERNAL_CONVERT_DEC
}