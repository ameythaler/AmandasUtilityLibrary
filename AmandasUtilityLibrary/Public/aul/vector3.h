#pragma once

#include <aul/scalar_math.h>
#include <aul/macros.h>
#include <aul/impl/simd_math_types.h>
#include <aul/vector2.h>

// Disable the warning about negating unsigned values - that it doesn't do anything is expected behavior.
// #TODO: Do the same for LLVM and GCC
#if AUL_MSVC
#pragma warning(disable:4146) 
#endif // AUL_MSVC

namespace aul
{
    template<typename T> struct vector3;
    template<typename T, typename U> struct vector3_int;

    template<typename T>
    wide_ostream& operator<< (wide_ostream& out, const vector3<T>& vec);

    template<typename T>
    mb_ostream& operator<< (mb_ostream& out, const vector3<T>& vec);

    template<typename T, typename U>
    wide_ostream& operator<< (wide_ostream& out, const vector3_int<T, U>& vec);

    template<typename T, typename U>
    mb_ostream& operator<< (mb_ostream& out, const vector3_int<T, U>& vec);

    // These two specialization keep the uint8 and int8 components from being interpreted as characters.
    template<>
    mb_ostream& operator<< (mb_ostream& out, const vector3_int<uint8, float>& vec);

    template<>
    mb_ostream& operator<< (mb_ostream& out, const vector3_int<int8, float>& vec);

    template<typename T>
    struct vector3
    {
        union
        {
            T data[3];
            struct
            {
                T x;
                T y;
                T z;
            };
            struct
            {
                T r;
                T g;
                T b;
            };
            struct
            {
                T u;
                T v;
                T w;
            };
            struct
            {
                T yaw;
                T pitch;
                T roll;
            };
            vector2<T> xy;
        };

        static const vector3 ZERO;
        static const vector3 ONE;
        static const vector3 X_AXIS;
        static const vector3 Y_AXIS;
        static const vector3 Z_AXIS;

        vector3(T x_ = scalar<T>::ZERO, T y_ = scalar<T>::ZERO, T z_ = scalar<T>::ZERO) : x(x_), y(y_), z(z_) { }
        vector3(const T* arr_data);
        vector3(const vector2<T>& rhs, T _z = scalar<T>::ZERO);
        vector3& operator=(const T* arr_data);
        vector3& operator=(const vector2<T>& rhs);

        inline bool operator==(const vector3& rhs) const { return scalar<T>::equal(x, rhs.x) && scalar<T>::equal(y, rhs.y) && scalar<T>::equal(z, rhs.z); }
        inline bool operator!=(const vector3& rhs) const { return scalar<T>::not_equal(x, rhs.x) || scalar<T>::not_equal(y, rhs.y) || scalar<T>::not_equal(z, rhs.z); }
        inline vector3 operator-() const { return vector3(-x, -y, -z); }
        inline vector3 operator+(const vector3& rhs) const { return vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
        inline vector3 operator-(const vector3& rhs) const { return vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
        inline vector3 operator*(T rhs) const { return vector3(x * rhs, y * rhs, z * rhs); }
        inline friend vector3 operator*(T lhs, const vector3<T>& rhs) { return vector3<T>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z); }
        inline vector3 operator/(T rhs) const { return vector3(x / rhs, y / rhs, z / rhs); }
        inline friend vector3 operator/(T lhs, const vector3<T>& rhs) { return vector3<T>(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z); }
        inline vector3 operator*(const vector3& rhs) const { return vector3(x * rhs.x, y * rhs.y, z * rhs.z); }
        inline vector3 operator/(const vector3& rhs) const { return vector3(x / rhs.x, y / rhs.y, z / rhs.z); }
        inline vector3& operator+=(const vector3& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
        inline vector3& operator-=(const vector3& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
        inline vector3& operator*=(T rhs) { x *= rhs; y *= rhs; z *= rhs; return *this; }
        inline vector3& operator/=(T rhs) { x /= rhs; y /= rhs; z /= rhs; return *this; }
        inline vector3& operator*=(const vector3& rhs) { x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this; }
        inline vector3& operator/=(const vector3& rhs) { x /= rhs.x; y /= rhs.y; z /= rhs.z; return *this; }

        inline T length() const { return sqrt(x * x + y * y + z * z); }
        inline T length_sq() const { return x * x + y * y + z * z; }
        vector3& normalize();
        vector3 normalized() const;
        inline T dot(const vector3& rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z; }
        inline vector3 cross(const vector3& rhs) const;
        vector3 project(const vector3& axis) const;

        operator wide_string() const;
        friend wide_ostream& operator<< <> (wide_ostream& out, const vector3<T>& vec);

        operator mb_string() const;
        friend mb_ostream& operator<< <> (mb_ostream& out, const vector3<T>& vec);
    };

    template<typename T, typename U>
    struct vector3_int
    {
        union
        {
            T data[3];
            struct
            {
                T x;
                T y;
                T z;
            };
            struct  
            {
                T r;
                T g;
                T b;
            };
            struct  
            {
                T u;
                T v;
                T s;
            };
            struct  
            {
                T yaw;
                T pitch;
                T roll;
            };
            vector2_int<T, U> xy;
        };

        static const vector3_int ZERO;
        static const vector3_int ONE;
        static const vector3_int X_AXIS;
        static const vector3_int Y_AXIS;
        static const vector3_int Z_AXIS;

        vector3_int(T x_ = scalar_int<T>::ZERO, T y_ = scalar_int<T>::ZERO, T z_ = scalar_int<T>::ZERO) : x(x_), y(y_), z(z_) { }
        vector3_int(const T* arr_data);
        vector3_int(const vector2_int<T, U>& rhs, T z_ = scalar_int<T>::ZERO);
        vector3_int& operator=(const T* arr_data);
        vector3_int& operator=(const vector2_int<T, U>& rhs);

        inline bool operator==(const vector3_int& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
        inline bool operator!=(const vector3_int& rhs) const { return x != rhs.x || y != rhs.y || z != rhs.z; }
        inline vector3_int operator-() const { return vector3_int(-x, -y, -z); }
        inline vector3_int operator+(const vector3_int& rhs) const { return vector3_int(x + rhs.x, y + rhs.y, z + rhs.z); }
        inline vector3_int operator-(const vector3_int& rhs) const { return vector3_int(x - rhs.x, y - rhs.y, z - rhs.z); }
        inline vector3_int operator*(T rhs) const { return vector3_int(x * rhs, y * rhs, z * rhs); }
        inline friend vector3_int operator*(T lhs, const vector3_int<T, U>& rhs) { return vector3_int<T, U>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z); }
        inline vector3_int operator/(T rhs) const { return vector3_int(x / rhs, y / rhs, z / rhs); }
        inline friend vector3_int operator/(T lhs, const vector3_int<T, U>& rhs) { return vector3_int<T, U>(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z); }
        inline vector3_int operator*(const vector3_int& rhs) const { return vector3_int(x * rhs.x, y * rhs.y, z * rhs.z); }
        inline vector3_int operator/(const vector3_int& rhs) const { return vector3_int(x / rhs.x, y / rhs.y, z / rhs.z); }
        inline vector3_int& operator+=(const vector3_int& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
        inline vector3_int& operator-=(const vector3_int& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
        inline vector3_int& operator*=(T rhs) { x *= rhs; y *= rhs; z *= rhs; return *this; }
        inline vector3_int& operator/=(T rhs) { x /= rhs; y /= rhs; z /= rhs; return *this; }
        inline vector3_int& operator*=(const vector3_int& rhs) { x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this; }
        inline vector3_int& operator/=(const vector3_int& rhs) { x /= rhs.x; y /= rhs.y; z /= rhs.z; return *this; }

        inline U length() const { return sqrt((U)x * (U)x + (U)y * (U)y + (U)z * (U)z); }
        inline U length_sq() const { return (U)x * (U)x + (U)y * (U)y + (U)z * (U)z; }

        operator wide_string() const;
        friend wide_ostream& operator<< <> (wide_ostream& out, const vector3_int<T, U>& vec);

        operator mb_string() const;
        friend mb_ostream& operator<< <> (mb_ostream& out, const vector3_int<T, U>& vec);
    };

#define AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC(T, extension) typedef vector3<T> vector3 ## extension; \
extern template struct vector3<T>

#define AUL_INTERNAL_EXPLICIT_SPEC_INST_DEC_INT(T, U, extension) typedef vector3_int<T, U> vector3 ## extension; \
extern template struct vector3_int<T, U>

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

    //////////////////////////////////////////////////////////////////////////
    // Conversion
    //////////////////////////////////////////////////////////////////////////

#define AUL_INTERNAL_CONVERT_DEC_PARTIAL(T, U, V) template<> vector3<T>& convert<vector3<T>, vector3_int<U, V>>(vector3<T>& to, const vector3_int<U, V>& from); \
template<> vector3_int<U, V>& convert<vector3_int<U, V>, vector3<T>>(vector3_int<U, V>& to, const vector3<T>& from)

#define AUL_INTERNAL_CONVERT_DEC(T, U, V, W) AUL_INTERNAL_CONVERT_DEC_PARTIAL(T, V, W); \
AUL_INTERNAL_CONVERT_DEC_PARTIAL(U, V, W); \
AUL_INTERNAL_CONVERT_DEC_PARTIAL(T, u ## V, W); \
AUL_INTERNAL_CONVERT_DEC_PARTIAL(U, u ## V, W)

#define AUL_INTERNAL_CONVERT_DEC_INT(T, U, V, W) template<> vector3_int<T, U>& convert<vector3_int<T, U>, vector3_int<V, W>>(vector3_int<T, U>& to, const vector3_int<V, W>& from)

#define AUL_INTERNAL_CONVERT_DEC_INT_CROSS(T, U, V, W) AUL_INTERNAL_CONVERT_DEC_INT(T, U, V, W); \
AUL_INTERNAL_CONVERT_DEC_INT(u ## T, U, V, W); \
AUL_INTERNAL_CONVERT_DEC_INT(T, U, u ## V, W); \
AUL_INTERNAL_CONVERT_DEC_INT(u ## T, U, u ## V, W)

#define AUL_INTERNAL_CONVERT_DEC_INT_FAMILY(S, T, U, V, W, X, Y, Z) AUL_INTERNAL_CONVERT_DEC_INT(S, T, u ## S, T); \
AUL_INTERNAL_CONVERT_DEC_INT(u ## S, T, S, T); \
AUL_INTERNAL_CONVERT_DEC_INT_CROSS(S, T, U, V); \
AUL_INTERNAL_CONVERT_DEC_INT_CROSS(S, T, W, X); \
AUL_INTERNAL_CONVERT_DEC_INT_CROSS(S, T, Y, Z);

#define AUL_INTERNAL_CONVERT_DEC_FLOAT(T, U) template<> vector3<T>& convert<vector3<T>, vector3<U>>(vector3<T>& to, const vector3<U>& from); \
template<> vector3<U>& convert<vector3<U>, vector3<T>>(vector3<U>& to, const vector3<T>& from)

    AUL_INTERNAL_CONVERT_DEC(float, double, int32, float);
    AUL_INTERNAL_CONVERT_DEC(float, double, int64, double);
    AUL_INTERNAL_CONVERT_DEC(float, double, int16, float);
    AUL_INTERNAL_CONVERT_DEC(float, double, int8, float);

    AUL_INTERNAL_CONVERT_DEC_INT_FAMILY(int32, float, int64, double, int16, float, int8, float);
    AUL_INTERNAL_CONVERT_DEC_INT_FAMILY(int64, double, int32, float, int16, float, int8, float);
    AUL_INTERNAL_CONVERT_DEC_INT_FAMILY(int16, float, int32, float, int64, double, int8, float);
    AUL_INTERNAL_CONVERT_DEC_INT_FAMILY(int8, float, int32, float, int64, double, int16, float);

    AUL_INTERNAL_CONVERT_DEC_FLOAT(float, double);

#undef AUL_INTERNAL_CONVERT_DEC_FLOAT
#undef AUL_INTERNAL_CONVERT_DEC_INT_FAMILY
#undef AUL_INTERNAL_CONVERT_DEC_INT_CROSS
#undef AUL_INTERNAL_CONVERT_DEC_INT
#undef AUL_INTERNAL_CONVERT_DEC
#undef AUL_INTERNAL_CONVERT_DEC_PARTIAL

    // SIMD set and get
#if AUL_USE_SSE
    inline vec4f vec4f_set(const vector3f& vec3, float w = 0.0f)
    {
        return _mm_set_ps(w, vec3.z, vec3.y, vec3.x);
    }

    inline vector3f vec4f_get_vector3f(vec4f vec)
    {
        AUL_ALIGN(16) float vec_array[4];
        _mm_store_ps(vec_array, vec);
        return vector3f(vec_array[0], vec_array[1], vec_array[2]);
    }
#endif // AUL_USE_SSE
}

#if AUL_MSVC
#pragma warning(default:4146)
#endif // AUL_MSVC