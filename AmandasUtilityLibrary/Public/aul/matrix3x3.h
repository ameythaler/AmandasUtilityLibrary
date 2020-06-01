#pragma once

#include <aul/scalar_math.h>
#include <aul/macros.h>
#include <aul/vector3.h>

namespace aul
{
    template<typename T> struct matrix3x3;

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
#if AUL_USE_MATRIX_ORDER == AUL_ROW_MAJOR_ORDER
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
#elif AUL_USE_MATRIX_ORDER == AUL_COLUMN_MAJOR_ORDER
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
#endif // AUL_USE_MATRIX_ORDER
            };
            struct  
            {
                vector3<T> x;
                vector3<T> y;
                vector3<T> z;
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
        matrix3x3(const T* arr_data);
        matrix3x3& operator=(const T* arr_data);

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
        inline matrix3x3& operator*=(T rhs) { x *= rhs; y *= rhs; z *= rhs; return *this; }
        inline matrix3x3& operator/=(T rhs) { x /= rhs; y /= rhs; z /= rhs; return *this; }
        inline matrix3x3& operator*=(matrix3x3& rhs) { *this = *this * rhs; return *this; }

        matrix3x3& transpose();
        matrix3x3 transposed() const;
        inline vector3<T> transform_point(const vector3<T>& point) const { return *this * point; }
        inline vector3<T> transform_vector(const vector3<T>& vec) const { return (*this * vec).normalized(); }
        T determinant() const;

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
}