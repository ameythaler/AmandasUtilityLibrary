#pragma once

#include <aul/math.h>
#include <aul/macros.h>
#include <aul/impl/simd_math_types.h>

// Disable the warning about negating unsigned values - that it doesn't do anything is expected behavior.
// #TODO: Do the same for LLVM and GCC
#if AUL_MSVC
#pragma warning(disable:4146) 
#endif // AUL_MSVC

namespace aul
{
    template<typename T> struct vector4;
    template<typename T, typename U> struct vector4_int;

    template<typename T>
    wide_ostream& operator<< (wide_ostream& out, const vector4<T>& vec);

    template<typename T>
    mb_ostream& operator<< (mb_ostream& out, const vector4<T>& vec);

    template<typename T, typename U>
    wide_ostream& operator<< (wide_ostream& out, const vector4_int<T, U>& vec);

    template<typename T, typename U>
    mb_ostream& operator<< (mb_ostream& out, const vector4_int<T, U>& vec);

    // These two specialization keep the uint8 and int8 components from being interpreted as characters.
    template<>
    mb_ostream& operator<< (mb_ostream& out, const vector4_int<uint8, float>& vec);

    template<>
    mb_ostream& operator<< (mb_ostream& out, const vector4_int<int8, float>& vec);

    template<typename T>
    struct vector4
    {
        union
        {
            T data[4];
            struct
            {
                T x;
                T y;
                T z;
                T w;
            };
        };

        static const vector4 ZERO;
        static const vector4 ONE;
        static const vector4 X_AXIS;
        static const vector4 Y_AXIS;
        static const vector4 Z_AXIS;
        static const vector4 W_AXIS;

        vector4(T x_ = scalar<T>::ZERO, T y_ = scalar<T>::ZERO, T z_ = scalar<T>::ZERO, T w_ = scalar<T>::ZERO) : x(x_), y(y_), z(z_), w(w_) { }
        vector4(const T* arr_data);
        vector4& operator=(const vector4& rhs);
        vector4& operator=(const T* arr_data);

        inline bool operator==(const vector4& rhs) const { return scalar<T>::equal(x, rhs.x) && scalar<T>::equal(y, rhs.y) && scalar<T>::equal(z, rhs.z) && scalar<T>::equal(w, rhs.w); }
        inline bool operator!=(const vector4& rhs) const { return scalar<T>::not_equal(x, rhs.x) || scalar<T>::not_equal(y, rhs.y) || scalar<T>::not_equal(z, rhs.z) || scalar<T>::not_equal(w, rhs.w); }
        inline vector4 operator-() const { return vector4(-x, -y, -z, -w); }
        inline vector4 operator+(const vector4& rhs) const { return vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
        inline vector4 operator-(const vector4& rhs) const { return vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
        inline vector4 operator*(T rhs) const { return vector4(x * rhs, y * rhs, z * rhs, w * rhs); }
        inline friend vector4 operator*(T lhs, const vector4<T>& rhs) { return vector4<T>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w); }
        inline vector4 operator/(T rhs) const { return vector4(x / rhs, y / rhs, z / rhs, w / rhs); }
        inline friend vector4 operator/(T lhs, const vector4<T>& rhs) { return vector4<T>(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z, lhs / rhs.w); }
        inline vector4 operator*(const vector4& rhs) const { return vector4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w); }
        inline vector4 operator/(const vector4& rhs) const { return vector4(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w); }
        inline vector4& operator+=(const vector4& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this; }
        inline vector4& operator-=(const vector4& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this; }
        inline vector4& operator*=(T rhs) { x *= rhs; y *= rhs; z *= rhs; w *= rhs; return *this; }
        inline vector4& operator/=(T rhs) { x /= rhs; y /= rhs; z /= rhs; w /= rhs; return *this; }
        inline vector4& operator*=(const vector4& rhs) { x *= rhs.x; y *= rhs.y; z *= rhs.z; w *= rhs.w; return *this; }
        inline vector4& operator/=(const vector4& rhs) { x /= rhs.x; y /= rhs.y; z /= rhs.z; z /= rhs.z; return *this; }

        inline T length() const { return sqrt(x * x + y * y + z * z + w * w); }
        inline T length_sq() const { return x * x + y * y + z * z + w * w; }
        vector4& normalize();
        vector4 normalized() const;
        inline T dot(const vector4& rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w; }
        inline vector4 cross(const vector4& rhs) const;
        vector4 project(const vector4& axis) const;

        operator wide_string() const;
        friend wide_ostream& operator<< <> (wide_ostream& out, const vector4<T>& vec);

        operator mb_string() const;
        friend mb_ostream& operator<< <> (mb_ostream& out, const vector4<T>& vec);
    };

    template<typename T, typename U>
    struct vector4_int
    {
        union
        {
            T data[3];
            struct
            {
                T x;
                T y;
                T z;
                T w;
            };
        };

        static const vector4_int ZERO;
        static const vector4_int ONE;
        static const vector4_int X_AXIS;
        static const vector4_int Y_AXIS;
        static const vector4_int Z_AXIS;
        static const vector4_int W_AXIS;

        vector4_int(T x_ = scalar_int<T>::ZERO, T y_ = scalar_int<T>::ZERO, T z_ = scalar_int<T>::ZERO, T w_ = scalar_int<T>::ZERO) : x(x_), y(y_), z(z_), w(w_) { }
        vector4_int(const T* arr_data);
        vector4_int& operator=(const vector4_int& rhs);
        vector4_int& operator=(const T* arr_data);

        inline bool operator==(const vector4_int& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w; }
        inline bool operator!=(const vector4_int& rhs) const { return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w; }
        inline vector4_int operator-() const { return vector4_int(-x, -y, -z, -w); }
        inline vector4_int operator+(const vector4_int& rhs) const { return vector4_int(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
        inline vector4_int operator-(const vector4_int& rhs) const { return vector4_int(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
        inline vector4_int operator*(T rhs) const { return vector4_int(x * rhs, y * rhs, z * rhs, w * rhs); }
        inline friend vector4_int operator*(T lhs, const vector4_int<T, U>& rhs) { return vector4_int<T, U>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w); }
        inline vector4_int operator/(T rhs) const { return vector4_int(x / rhs, y / rhs, z / rhs, w / rhs); }
        inline friend vector4_int operator/(T lhs, const vector4_int<T, U>& rhs) { return vector4_int<T, U>(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z, lhs / rhs.w); }
        inline vector4_int operator*(const vector4_int& rhs) const { return vector4_int(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w); }
        inline vector4_int operator/(const vector4_int& rhs) const { return vector4_int(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w); }
        inline vector4_int& operator+=(const vector4_int& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this; }
        inline vector4_int& operator-=(const vector4_int& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; z -= rhs.z; return *this; }
        inline vector4_int& operator*=(T rhs) { x *= rhs; y *= rhs; z *= rhs; w *= rhs; return *this; }
        inline vector4_int& operator/=(T rhs) { x /= rhs; y /= rhs; z /= rhs; z /= rhs; return *this; }
        inline vector4_int& operator*=(const vector4_int& rhs) { x *= rhs.x; y *= rhs.y; z *= rhs.z; w *= rhs.w; return *this; }
        inline vector4_int& operator/=(const vector4_int& rhs) { x /= rhs.x; y /= rhs.y; z /= rhs.z; w /= rhs.w; return *this; }

        inline U length() const { return sqrt((U)x * (U)x + (U)y * (U)y + (U)z * (U)z + (U)w * (U)w); }
        inline U length_sq() const { return (U)x * (U)x + (U)y * (U)y + (U)z * (U)z + (U)w * (U)w; }

        operator wide_string() const;
        friend wide_ostream& operator<< <> (wide_ostream& out, const vector4_int<T, U>& vec);

        operator mb_string() const;
        friend mb_ostream& operator<< <> (mb_ostream& out, const vector4_int<T, U>& vec);
    };

#define AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC(T, extension) typedef vector4<T> vector4 ## extension; \
extern template struct vector4<T>

#define AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC_INT(T, U, extension) typedef vector4_int<T, U> vector4 ## extension; \
extern template struct vector4_int<T, U>

    AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC(float, f);
    AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC(double, d);
    AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC_INT(int32, float, i32);
    AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC_INT(uint32, float, u32);
    AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC_INT(int64, double, i64);
    AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC_INT(uint64, double, u64);
    AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC_INT(int16, float, i16);
    AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC_INT(uint16, float, u16);
    AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC_INT(int8, float, i8);
    AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC_INT(uint8, float, u8);

#undef AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC
#undef AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC_INT

    // SIMD set and get
#if AUL_USE_SSE
    inline vec4f vec4f_set(const vector4f& vec4)
    {
        return _mm_set_ps(vec4.w, vec4.z, vec4.y, vec4.x);
    }

    inline vec4f vec4f_set(vector4f&& vec4)
    {
        return _mm_set_ps(vec4.w, vec4.z, vec4.y, vec4.x);
    }

    inline vector4f vec4f_get_vector4f(vec4f vec)
    {
        AUL_ALIGN(16) float vec_array[4];
        _mm_store_ps(vec_array, vec);
        return vector4f(vec_array[3], vec_array[2], vec_array[1], vec_array[0]);
    }
#endif // AUL_USE_SSE
}

#if AUL_MSVC
#pragma warning(default:4146)
#endif // AUL_MSVC