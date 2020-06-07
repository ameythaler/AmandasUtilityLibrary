#include "stdafx.h"
#include <aul/vector4.h>
#include <aul/vector3.h>
#include <aul/vector2.h>
#include <aul/matrix4x4.h>
#include <aul/xform.h>

namespace aul
{
    template<typename T>
    const vector4<T> vector4<T>::ZERO = vector4<T>();

    template<typename T>
    const vector4<T> vector4<T>::ONE = vector4<T>(scalar<T>::ONE, scalar<T>::ONE, scalar<T>::ONE, scalar<T>::ONE);

    template<typename T>
    const vector4<T> vector4<T>::X_AXIS = vector4<T>(scalar<T>::ONE, scalar<T>::ZERO, scalar<T>::ZERO, scalar<T>::ZERO);

    template<typename T>
    const vector4<T> vector4<T>::Y_AXIS = vector4<T>(scalar<T>::ZERO, scalar<T>::ONE, scalar<T>::ZERO, scalar<T>::ZERO);

    template<typename T>
    const vector4<T> vector4<T>::Z_AXIS = vector4<T>(scalar<T>::ZERO, scalar<T>::ZERO, scalar<T>::ONE, scalar<T>::ZERO);

    template<typename T>
    const vector4<T> vector4<T>::W_AXIS = vector4<T>(scalar<T>::ZERO, scalar<T>::ZERO, scalar<T>::ONE, scalar<T>::ONE);

    template<typename T, typename U>
    const vector4_int<T, U> vector4_int<T, U>::ZERO = vector4_int<T, U>();

    template<typename T, typename U>
    const vector4_int<T, U> vector4_int<T, U>::ONE = vector4_int<T, U>(scalar_int<T>::ONE, scalar_int<T>::ONE, scalar_int<T>::ONE, scalar_int<T>::ONE);

    template<typename T, typename U>
    const vector4_int<T, U> vector4_int<T, U>::X_AXIS = vector4_int<T, U>(scalar_int<T>::ONE, scalar_int<T>::ZERO, scalar_int<T>::ZERO, scalar_int<T>::ZERO);

    template<typename T, typename U>
    const vector4_int<T, U> vector4_int<T, U>::Y_AXIS = vector4_int<T, U>(scalar_int<T>::ZERO, scalar_int<T>::ONE, scalar_int<T>::ZERO, scalar_int<T>::ZERO);

    template<typename T, typename U>
    const vector4_int<T, U> vector4_int<T, U>::Z_AXIS = vector4_int<T, U>(scalar_int<T>::ZERO, scalar_int<T>::ZERO, scalar_int<T>::ONE, scalar_int<T>::ZERO);

    template<typename T, typename U>
    const vector4_int<T, U> vector4_int<T, U>::W_AXIS = vector4_int<T, U>(scalar_int<T>::ZERO, scalar_int<T>::ZERO, scalar_int<T>::ONE, scalar_int<T>::ONE);

#define AUL_INTERNAL_EXPLICIT_INST_DEF(T) template struct vector4<T>
#define AUL_INTERNAL_EXPLICIT_INST_DEF_INT(T, U) template struct vector4_int<T, U>

    AUL_INTERNAL_EXPLICIT_INST_DEF(float);
    AUL_INTERNAL_EXPLICIT_INST_DEF(double);
    AUL_INTERNAL_EXPLICIT_INST_DEF_INT(int32, float);
    AUL_INTERNAL_EXPLICIT_INST_DEF_INT(uint32, float);
    AUL_INTERNAL_EXPLICIT_INST_DEF_INT(int64, double);
    AUL_INTERNAL_EXPLICIT_INST_DEF_INT(uint64, double);
    AUL_INTERNAL_EXPLICIT_INST_DEF_INT(int16, float);
    AUL_INTERNAL_EXPLICIT_INST_DEF_INT(uint16, float);
    AUL_INTERNAL_EXPLICIT_INST_DEF_INT(int8, float);
    AUL_INTERNAL_EXPLICIT_INST_DEF_INT(uint8, float);

#undef AUL_INTERNAL_EXPLICIT_INST_DEF
#undef AUL_INTERNAL_EXPLICIT_INST_DEF_INT

    //////////////////////////////////////////////////////////////////////////
    // vector4
    //////////////////////////////////////////////////////////////////////////

    template<typename T>
    vector4<T>::vector4(const T* arr_data)
    {
        memcpy(data, arr_data, sizeof(T) * 4);
    }

    template<typename T>
    vector4<T>::vector4(const vector3<T>& rhs, T w_) : w(w_)
    {
        memcpy(data, rhs.data, sizeof(T) * 3);
    }

    template<typename T>
    vector4<T>::vector4(const vector2<T>& rhs, T z_, T w_) : z(z_), w(w_)
    {
        memcpy(data, rhs.data, sizeof(T) * 2);
    }

    template<typename T>
    vector4<T>& vector4<T>::operator =(const T* arr_data)
    {
        memcpy(data, arr_data, sizeof(T) * 4);
        return *this;
    }

    template<typename T>
    vector4<T>& vector4<T>::operator =(const vector3<T>& rhs)
    {
        memcpy(data, rhs.data, sizeof(T) * 3);
        w = scalar<T>::ZERO;
        return *this;
    }

    template<typename T>
    vector4<T>& vector4<T>::operator =(const vector2<T>& rhs)
    {
        memcpy(data, rhs.data, sizeof(T) * 2);
        z = scalar<T>::ZERO;
        w = scalar<T>::ZERO;
        return *this;
    }

    template<typename T>
    vector4<T>& vector4<T>::operator *=(const matrix4x4<T>& rhs)
    {
#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        T tx = rhs.m00 * x + rhs.m10 * y + rhs.m20 * z + rhs.m30 * w;
        T ty = rhs.m01 * x + rhs.m11 * y + rhs.m21 * z + rhs.m31 * w;
        T tz = rhs.m02 * x + rhs.m12 * y + rhs.m22 * z + rhs.m32 * w;
        T tw = rhs.m03 * x + rhs.m13 * y + rhs.m23 * z + rhs.m33 * w;
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        T tx = rhs.m00 * x + rhs.m01 * y + rhs.m02 * z + rhs.m03 * w;
        T ty = rhs.m10 * x + rhs.m11 * y + rhs.m12 * z + rhs.m13 * w;
        T tz = rhs.m20 * x + rhs.m21 * y + rhs.m22 * z + rhs.m23 * w;
        T tw = rhs.m30 * x + rhs.m31 * y + rhs.m32 * z + rhs.m33 * w;
#endif // AUL_USE_COORDINATE_HANDEDNESS

        x = tx;
        y = ty;
        z = tz;
        w = tw;
        return *this;
    }

    template<typename T>
    vector4<T>& vector4<T>::operator *=(const xform<T>& rhs)
    {
#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        T tx = rhs.m00 * x + rhs.m10 * y + rhs.m20 * z + rhs.m30 * w;
        T ty = rhs.m01 * x + rhs.m11 * y + rhs.m21 * z + rhs.m31 * w;
        T tz = rhs.m02 * x + rhs.m12 * y + rhs.m22 * z + rhs.m32 * w;
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        T tx = rhs.m00 * x + rhs.m01 * y + rhs.m02 * z + rhs.m03 * w;
        T ty = rhs.m10 * x + rhs.m11 * y + rhs.m12 * z + rhs.m13 * w;
        T tz = rhs.m20 * x + rhs.m21 * y + rhs.m22 * z + rhs.m23 * w;
#endif // AUL_USE_COORDINATE_HANDEDNESS

        x = tx;
        y = ty;
        z = tz;
        return *this;
    }

    template<typename T>
    vector4<T>& vector4<T>::normalize()
    {
        return *this /= length();
    }

    template<typename T>
    vector4<T> vector4<T>::normalized() const
    {
        return *this / length();
    }

    template<typename T>
    vector4<T> vector4<T>::project(const vector4<T>& axis) const
    {
        return dot(axis) * axis;
    }

    template<typename T>
    vector4<T> vector4<T>::cross(const vector4& rhs) const
    {
        return vector4<T>(x * rhs.z - z * rhs.y, -(x * rhs.z - z * rhs.x), x * rhs.y - y * rhs.x);
    }

    template<typename T>
    vector4<T>::operator wide_string() const
    {
        wide_stringstream stream;
        stream << *this;
        return stream.str();
    }

    template<typename T>
    wide_ostream& operator<<(wide_ostream& out, const vector4<T>& vec)
    {
        out << L"<" << vec.x << L", " << vec.y << L", " << vec.z << L", " << vec.w << L">";
        return out;
    }

    template<typename T>
    vector4<T>::operator mb_string() const
    {
        mb_stringstream stream;
        stream << *this;
        return stream.str();
    }

    template<typename T>
    mb_ostream& operator<<(mb_ostream& out, const vector4<T>& vec)
    {
        out << "<" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ">";
        return out;
    }

    //////////////////////////////////////////////////////////////////////////
    // vector4_int
    //////////////////////////////////////////////////////////////////////////

    template<typename T, typename U>
    vector4_int<T, U>::vector4_int(const T* arr_data)
    {
        memcpy(data, arr_data, sizeof(T) * 4);
    }

    template<typename T, typename U>
    vector4_int<T, U>::vector4_int(const vector3_int<T, U>& rhs, T w_) : w(w_)
    {
        memcpy(data, rhs.data, sizeof(T) * 3);
    }

    template<typename T, typename U>
    vector4_int<T, U>::vector4_int(const vector2_int<T, U>& rhs, T z_, T w_) : z(z_), w(w_)
    {
        memcpy(data, rhs.data, sizeof(T) * 2);
    }

    template<typename T, typename U>
    vector4_int<T, U>& vector4_int<T, U>::operator =(const T* arr_data)
    {
        memcpy(data, arr_data, sizeof(T) * 4);
        return *this;
    }

    template<typename T, typename U>
    vector4_int<T, U>& vector4_int<T, U>::operator =(const vector3_int<T, U>& rhs)
    {
        memcpy(data, rhs.data, sizeof(T) * 3);
        w = scalar_int<T>::ZERO;
        return *this;
    }

    template<typename T, typename U>
    vector4_int<T, U>& vector4_int<T, U>::operator =(const vector2_int<T, U>& rhs)
    {
        memcpy(data, rhs.data, sizeof(T) * 2);
        z = scalar_int<T>::ZERO;
        w = scalar_int<T>::ZERO;
        return *this;
    }

    template<typename T, typename U>
    vector4_int<T, U>::operator wide_string() const
    {
        wide_stringstream stream;
        stream << *this;
        return stream.str();
    }

    template<typename T, typename U>
    wide_ostream& operator<<(wide_ostream& out, const vector4_int<T, U>& vec)
    {
        out << L"<" << vec.x << L", " << vec.y << L", " << vec.z << L", " << vec.w << L">";
        return out;
    }

    template<typename T, typename U>
    vector4_int<T, U>::operator mb_string() const
    {
        mb_stringstream stream;
        stream << *this;
        return stream.str();
    }

    template<typename T, typename U>
    mb_ostream& operator<<(mb_ostream& out, const vector4_int<T, U>& vec)
    {
        out << "<" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ">";
        return out;
    }

    template<>
    mb_ostream& operator<<(mb_ostream& out, const vector4_int<uint8, float>& vec)
    {
        out << "<" << (uint32)vec.x << ", " << (uint32)vec.y << ", " << (uint32)vec.z << ", " << (uint32)vec.w << ">";
        return out;
    }

    template<>
    mb_ostream& operator<<(mb_ostream& out, const vector4_int<int8, float>& vec)
    {
        out << "<" << (int32)vec.x << ", " << (int32)vec.y << ", " << (int32)vec.z << ", " << (int32)vec.w << ">";
        return out;
    }

    //////////////////////////////////////////////////////////////////////////
    // Conversion
    //////////////////////////////////////////////////////////////////////////

#define AUL_INTERNAL_CONVERT_DEF_PARTIAL(T, U, V) \
template<> vector4<T>& convert<vector4<T>, vector4_int<U, V>>(vector4<T>& to, const vector4_int<U, V>& from) { to.x = (T)from.x; to.y = (T)from.y; to.z = (T)from.z; to.w = (T)from.w; return to; } \
template<> vector4_int<U, V>& convert<vector4_int<U, V>, vector4<T>>(vector4_int<U, V>& to, const vector4<T>& from) { to.x = (U)from.x; to.y = (U)from.y; to.z = (U)from.z; to.w = (U)from.w; return to; }

#define AUL_INTERNAL_CONVERT_DEF(T, U, V, W) AUL_INTERNAL_CONVERT_DEF_PARTIAL(T, V, W) \
AUL_INTERNAL_CONVERT_DEF_PARTIAL(U, V, W) \
AUL_INTERNAL_CONVERT_DEF_PARTIAL(T, u ## V, W) \
AUL_INTERNAL_CONVERT_DEF_PARTIAL(U, u ## V, W)

#define AUL_INTERNAL_CONVERT_DEF_INT(T, U, V, W) \
template<> vector4_int<T, U>& convert<vector4_int<T, U>, vector4_int<V, W>>(vector4_int<T, U>& to, const vector4_int<V, W>& from) { to.x = (T)from.x; to.y = (T)from.y; to.z = (T)from.z; to.w = (T)from.w; return to; }

#define AUL_INTERNAL_CONVERT_DEF_INT_CROSS(T, U, V, W) AUL_INTERNAL_CONVERT_DEF_INT(T, U, V, W) \
AUL_INTERNAL_CONVERT_DEF_INT(u ## T, U, V, W) \
AUL_INTERNAL_CONVERT_DEF_INT(T, U, u ## V, W) \
AUL_INTERNAL_CONVERT_DEF_INT(u ## T, U, u ## V, W)

#define AUL_INTERNAL_CONVERT_DEF_INT_FAMILY(S, T, U, V, W, X, Y, Z) AUL_INTERNAL_CONVERT_DEF_INT(S, T, u ## S, T) \
AUL_INTERNAL_CONVERT_DEF_INT(u ## S, T, S, T) \
AUL_INTERNAL_CONVERT_DEF_INT_CROSS(S, T, U, V) \
AUL_INTERNAL_CONVERT_DEF_INT_CROSS(S, T, W, X) \
AUL_INTERNAL_CONVERT_DEF_INT_CROSS(S, T, Y, Z)

#define AUL_INTERNAL_CONVERT_DEF_FLOAT(T, U) \
template<> vector4<T>& convert<vector4<T>, vector4<U>>(vector4<T>& to, const vector4<U>& from) { to.x = (T)from.x; to.y = (T)from.y; to.z = (T)from.z; to.w = (T)from.w; return to; } \
template<> vector4<U>& convert<vector4<U>, vector4<T>>(vector4<U>& to, const vector4<T>& from) { to.x = (U)from.x; to.y = (U)from.y; to.z = (U)from.z; to.w = (U)from.w; return to; }

    AUL_INTERNAL_CONVERT_DEF(float, double, int32, float);
    AUL_INTERNAL_CONVERT_DEF(float, double, int64, double);
    AUL_INTERNAL_CONVERT_DEF(float, double, int16, float);
    AUL_INTERNAL_CONVERT_DEF(float, double, int8, float);

    AUL_INTERNAL_CONVERT_DEF_INT_FAMILY(int32, float, int64, double, int16, float, int8, float);
    AUL_INTERNAL_CONVERT_DEF_INT_FAMILY(int64, double, int32, float, int16, float, int8, float);
    AUL_INTERNAL_CONVERT_DEF_INT_FAMILY(int16, float, int32, float, int64, double, int8, float);
    AUL_INTERNAL_CONVERT_DEF_INT_FAMILY(int8, float, int32, float, int64, double, int16, float);

    AUL_INTERNAL_CONVERT_DEF_FLOAT(float, double);

#undef AUL_INTERNAL_CONVERT_DEF_FLOAT
#undef AUL_INTERNAL_CONVERT_DEF_INT_FAMILY
#undef AUL_INTERNAL_CONVERT_DEF_INT_CROSS
#undef AUL_INTERNAL_CONVERT_DEF_INT
#undef AUL_INTERNAL_CONVERT_DEF
#undef AUL_INTERNAL_CONVERT_DEF_PARTIAL
}