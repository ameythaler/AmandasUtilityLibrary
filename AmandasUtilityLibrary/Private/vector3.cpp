#include "stdafx.h"
#include <aul/vector3.h>
#include <aul/matrix3x3.h>

namespace aul
{
    template<typename T>
    const vector3<T> vector3<T>::ZERO = vector3<T>();

    template<typename T>
    const vector3<T> vector3<T>::ONE = vector3<T>(scalar<T>::ONE, scalar<T>::ONE, scalar<T>::ONE);

    template<typename T>
    const vector3<T> vector3<T>::X_AXIS = vector3<T>(scalar<T>::ONE, scalar<T>::ZERO, scalar<T>::ZERO);

    template<typename T>
    const vector3<T> vector3<T>::Y_AXIS = vector3<T>(scalar<T>::ZERO, scalar<T>::ONE, scalar<T>::ZERO);

    template<typename T>
    const vector3<T> vector3<T>::Z_AXIS = vector3<T>(scalar<T>::ZERO, scalar<T>::ZERO, scalar<T>::ONE);

    template<typename T, typename U>
    const vector3_int<T, U> vector3_int<T, U>::ZERO = vector3_int<T, U>();

    template<typename T, typename U>
    const vector3_int<T, U> vector3_int<T, U>::ONE = vector3_int<T, U>(scalar_int<T>::ONE, scalar_int<T>::ONE, scalar_int<T>::ONE);

    template<typename T, typename U>
    const vector3_int<T, U> vector3_int<T, U>::X_AXIS = vector3_int<T, U>(scalar_int<T>::ONE, scalar_int<T>::ZERO, scalar_int<T>::ZERO);

    template<typename T, typename U>
    const vector3_int<T, U> vector3_int<T, U>::Y_AXIS = vector3_int<T, U>(scalar_int<T>::ZERO, scalar_int<T>::ONE, scalar_int<T>::ZERO);

    template<typename T, typename U>
    const vector3_int<T, U> vector3_int<T, U>::Z_AXIS = vector3_int<T, U>(scalar_int<T>::ZERO, scalar_int<T>::ZERO, scalar_int<T>::ONE);

#define AUL_INTERNAL_EXPLICIT_INST_DEF(T) template struct vector3<T>
#define AUL_INTERNAL_EXPLICIT_INST_DEF_INT(T, U) template struct vector3_int<T, U>

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
    // vector3
    //////////////////////////////////////////////////////////////////////////

    template<typename T>
    vector3<T>::vector3(const T* arr_data)
    {
        memcpy(data, arr_data, sizeof(T) * 3);
    }

    template<typename T>
    vector3<T>::vector3(const vector2<T>& rhs, T _z) : z(_z)
    {
        memcpy(data, rhs.data, sizeof(T) * 2);
    }

    template<typename T>
    vector3<T>& vector3<T>::operator =(const T* arr_data)
    {
        memcpy(data, arr_data, sizeof(T) * 3);
        return *this;
    }

    template<typename T>
    vector3<T>& vector3<T>::operator =(const vector2<T>& rhs)
    {
        memcpy(data, rhs.data, 2);
        z = scalar<T>::ZERO;
        return *this;
    }

    template<typename T>
    vector3<T>& vector3<T>::operator *=(const matrix3x3<T>& rhs)
    {
#if AUL_USE_COORDINATE_HANDEDNESS == AUL_LEFT_HANDED
        T tx = rhs.m00 * x + rhs.m10 * y + rhs.m20 * z;
        T ty = rhs.m01 * x + rhs.m11 * y + rhs.m21 * z;
        T tz = rhs.m02 * x + rhs.m12 * y + rhs.m22 * z;
#elif AUL_USE_COORDINATE_HANDEDNESS == AUL_RIGHT_HANDED
        T tx = rhs.m00 * x + rhs.m01 * y + rhs.m02 * z;
        T ty = rhs.m10 * x + rhs.m11 * y + rhs.m12 * z;
        T tz = rhs.m20 * x + rhs.m21 * y + rhs.m22 * z;
#endif // AUL_USE_COORDINATE_HANDEDNESS

        x = tx;
        y = ty;
        z = tz;
        return *this;
    }

    template<typename T>
    vector3<T>& vector3<T>::normalize()
    {
        return *this /= length();
    }

    template<typename T>
    vector3<T> vector3<T>::normalized() const
    {
        return *this / length();
    }

    template<typename T>
    vector3<T> vector3<T>::project(const vector3<T>& axis) const
    {
        return dot(axis) * axis;
    }

    template<typename T>
    vector3<T> vector3<T>::cross(const vector3& rhs) const
    {
        return vector3<T>(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
    }

    template<typename T>
    vector3<T>::operator wide_string() const
    {
        wide_stringstream stream;
        stream << *this;
        return stream.str();
    }

    template<typename T>
    wide_ostream& operator<<(wide_ostream& out, const vector3<T>& vec)
    {
        out << L"<" << vec.x << L", " << vec.y << L", " << vec.z << L">";
        return out;
    }

    template<typename T>
    vector3<T>::operator mb_string() const
    {
        mb_stringstream stream;
        stream << *this;
        return stream.str();
    }

    template<typename T>
    mb_ostream& operator<<(mb_ostream& out, const vector3<T>& vec)
    {
        out << "<" << vec.x << ", " << vec.y << ", " << vec.z << ">";
        return out;
    }

    //////////////////////////////////////////////////////////////////////////
    // vector3_int
    //////////////////////////////////////////////////////////////////////////

    template<typename T, typename U>
    vector3_int<T, U>::vector3_int(const T* arr_data)
    {
        memcpy(data, arr_data, sizeof(T) * 3);
    }

    template<typename T, typename U>
    vector3_int<T, U>::vector3_int(const vector2_int<T, U>& rhs, T _z) : z(_z)
    {
        memcpy(data, rhs.data, sizeof(T) * 2);
    }

    template<typename T, typename U>
    vector3_int<T, U>& vector3_int<T, U>::operator =(const T* arr_data)
    {
        memcpy(data, arr_data, sizeof(T) * 3);
        return *this;
    }

    template<typename T, typename U>
    vector3_int<T, U>& vector3_int<T, U>::operator =(const vector2_int<T, U>& rhs)
    {
        memcpy(data, rhs.data, sizeof(T) * 2);
        z = scalar_int<T>::ZERO;
        return *this;
    }

    template<typename T, typename U>
    vector3_int<T, U>::operator wide_string() const
    {
        wide_stringstream stream;
        stream << *this;
        return stream.str();
    }

    template<typename T, typename U>
    wide_ostream& operator<<(wide_ostream& out, const vector3_int<T, U>& vec)
    {
        out << L"<" << vec.x << L", " << vec.y << L", " << vec.z << L">";
        return out;
    }

    template<typename T, typename U>
    vector3_int<T, U>::operator mb_string() const
    {
        mb_stringstream stream;
        stream << *this;
        return stream.str();
    }

    template<typename T, typename U>
    mb_ostream& operator<<(mb_ostream& out, const vector3_int<T, U>& vec)
    {
        out << "<" << vec.x << ", " << vec.y << ", " << vec.z << ">";
        return out;
    }

    template<>
    mb_ostream& operator<<(mb_ostream& out, const vector3_int<uint8, float>& vec)
    {
        out << "<" << (uint32)vec.x << ", " << (uint32)vec.y << ", " << (uint32)vec.z << ">";
        return out;
    }

    template<>
    mb_ostream& operator<<(mb_ostream& out, const vector3_int<int8, float>& vec)
    {
        out << "<" << (int32)vec.x << ", " << (int32)vec.y << ", " << (int32)vec.z << ">";
        return out;
    }

    //////////////////////////////////////////////////////////////////////////
    // Conversion
    //////////////////////////////////////////////////////////////////////////

#define AUL_INTERNAL_CONVERT_DEF_PARTIAL(T, U, V) \
template<> vector3<T>& convert<vector3<T>, vector3_int<U, V>>(vector3<T>& to, const vector3_int<U, V>& from) { to.x = (T)from.x; to.y = (T)from.y; to.z = (T)from.z; return to; } \
template<> vector3_int<U, V>& convert<vector3_int<U, V>, vector3<T>>(vector3_int<U, V>& to, const vector3<T>& from) { to.x = (U)from.x; to.y = (U)from.y; to.z = (U)from.z; return to; }

#define AUL_INTERNAL_CONVERT_DEF(T, U, V, W) AUL_INTERNAL_CONVERT_DEF_PARTIAL(T, V, W) \
AUL_INTERNAL_CONVERT_DEF_PARTIAL(U, V, W) \
AUL_INTERNAL_CONVERT_DEF_PARTIAL(T, u ## V, W) \
AUL_INTERNAL_CONVERT_DEF_PARTIAL(U, u ## V, W)

#define AUL_INTERNAL_CONVERT_DEF_INT(T, U, V, W) \
template<> vector3_int<T, U>& convert<vector3_int<T, U>, vector3_int<V, W>>(vector3_int<T, U>& to, const vector3_int<V, W>& from) { to.x = (T)from.x; to.y = (T)from.y; to.z = (T)from.z; return to; }

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
template<> vector3<T>& convert<vector3<T>, vector3<U>>(vector3<T>& to, const vector3<U>& from) { to.x = (T)from.x; to.y = (T)from.y; to.z = (T)from.z; return to; } \
template<> vector3<U>& convert<vector3<U>, vector3<T>>(vector3<U>& to, const vector3<T>& from) { to.x = (U)from.x; to.y = (U)from.y; to.z = (U)from.z; return to; }

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