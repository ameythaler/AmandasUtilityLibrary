#include "stdafx.h"
#include <aul/vector2.h>

namespace aul
{
    template<typename T>
    const vector2<T> vector2<T>::ZERO = vector2<T>();

    template<typename T>
    const vector2<T> vector2<T>::ONE = vector2<T>(scalar<T>::ONE, scalar<T>::ONE);

    template<typename T>
    const vector2<T> vector2<T>::X_AXIS = vector2<T>(scalar<T>::ONE, scalar<T>::ZERO);

    template<typename T>
    const vector2<T> vector2<T>::Y_AXIS = vector2<T>(scalar<T>::ZERO, scalar<T>::ONE);

    template<typename T, typename U>
    const vector2_int<T, U> vector2_int<T, U>::ZERO = vector2_int<T, U>();

    template<typename T, typename U>
    const vector2_int<T, U> vector2_int<T, U>::ONE = vector2_int<T, U>(scalar_int<T>::ONE, scalar_int<T>::ONE);

    template<typename T, typename U>
    const vector2_int<T, U> vector2_int<T, U>::X_AXIS = vector2_int<T, U>(scalar_int<T>::ONE, scalar_int<T>::ZERO);

    template<typename T, typename U>
    const vector2_int<T, U> vector2_int<T, U>::Y_AXIS = vector2_int<T, U>(scalar_int<T>::ZERO, scalar_int<T>::ONE);

#define AUL_INTERNAL_EXPLICIT_INST_DEF(T) template struct vector2<T>
#define AUL_INTERNAL_EXPLICIT_INST_DEF_INT(T, U) template struct vector2_int<T, U>

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
    // vector2
    //////////////////////////////////////////////////////////////////////////

    template<typename T>
    vector2<T>::vector2(const T* arr_data)
    {
        memcpy(data, arr_data, sizeof(T) * 2);
    }

    template<typename T>
    vector2<T>& vector2<T>::operator =(const T* arr_data)
    {
        memcpy(data, arr_data, sizeof(T) * 2);
        return *this;
    }

    template<typename T>
    vector2<T>& vector2<T>::normalize()
    {
        return *this /= length();
    }

    template<typename T>
    vector2<T> vector2<T>::normalized() const
    {
        return *this / length();
    }

    template<typename T>
    vector2<T> vector2<T>::project(const vector2<T>& axis) const
    {
        return dot(axis) * axis;
    }

    template<typename T>
    vector2<T>::operator wide_string() const
    {
        wide_stringstream stream;
        stream << *this;
        return stream.str();
    }

    template<typename T>
    wide_ostream& operator<<(wide_ostream& out, const vector2<T>& vec)
    {
        out << L"<" << vec.x << L", " << vec.y << L">";
        return out;
    }

    template<typename T>
    vector2<T>::operator mb_string() const
    {
        mb_stringstream stream;
        stream << *this;
        return stream.str();
    }

    template<typename T>
    mb_ostream& operator<<(mb_ostream& out, const vector2<T>& vec)
    {
        out << "<" << vec.x << ", " << vec.y << ">";
        return out;
    }

    //////////////////////////////////////////////////////////////////////////
    // vector2_int
    //////////////////////////////////////////////////////////////////////////

    template<typename T, typename U>
    vector2_int<T, U>::vector2_int(const T* arr_data)
    {
        memcpy(data, arr_data, sizeof(T) * 2);
    }

    template<typename T, typename U>
    vector2_int<T, U>& vector2_int<T, U>::operator =(const T* arr_data)
    {
        memcpy(data, arr_data, sizeof(T) * 2);
        return *this;
    }

    template<typename T, typename U>
    vector2_int<T, U>::operator wide_string() const
    {
        wide_stringstream stream;
        stream << *this;
        return stream.str();
    }

    template<typename T, typename U>
    wide_ostream& operator<<(wide_ostream& out, const vector2_int<T, U>& vec)
    {
        out << L"<" << vec.x << L", " << vec.y << L">";
        return out;
    }

    template<typename T, typename U>
    vector2_int<T, U>::operator mb_string() const
    {
        mb_stringstream stream;
        stream << *this;
        return stream.str();
    }

    template<typename T, typename U>
    mb_ostream& operator<<(mb_ostream& out, const vector2_int<T, U>& vec)
    {
        out << "<" << vec.x << ", " << vec.y << ">";
        return out;
    }

    template<>
    mb_ostream& operator<<(mb_ostream& out, const vector2_int<uint8, float>& vec)
    {
        out << "<" << (uint32)vec.x << ", " << (uint32)vec.y << ">";
        return out;
    }

    template<>
    mb_ostream& operator<<(mb_ostream& out, const vector2_int<int8, float>& vec)
    {
        out << "<" << (int32)vec.x << ", " << (int32)vec.y << ">";
        return out;
    }

    //////////////////////////////////////////////////////////////////////////
    // Conversion
    //////////////////////////////////////////////////////////////////////////

#define AUL_INTERNAL_CONVERT_DEF_PARTIAL(T, U, V) \
template<> vector2<T>& convert<vector2<T>, vector2_int<U, V>>(vector2<T>& to, const vector2_int<U, V>& from) { to.x = (T)from.x; to.y = (T)from.y; return to; } \
template<> vector2_int<U, V>& convert<vector2_int<U, V>, vector2<T>>(vector2_int<U, V>& to, const vector2<T>& from) { to.x = (U)from.x; to.y = (U)from.y; return to; }

#define AUL_INTERNAL_CONVERT_DEF(T, U, V, W) AUL_INTERNAL_CONVERT_DEF_PARTIAL(T, V, W) \
AUL_INTERNAL_CONVERT_DEF_PARTIAL(U, V, W) \
AUL_INTERNAL_CONVERT_DEF_PARTIAL(T, u ## V, W) \
AUL_INTERNAL_CONVERT_DEF_PARTIAL(U, u ## V, W)

#define AUL_INTERNAL_CONVERT_DEF_INT(T, U, V, W) \
template<> vector2_int<T, U>& convert<vector2_int<T, U>, vector2_int<V, W>>(vector2_int<T, U>& to, const vector2_int<V, W>& from) { to.x = (T)from.x; to.y = (T)from.y; return to; }

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
template<> vector2<T>& convert<vector2<T>, vector2<U>>(vector2<T>& to, const vector2<U>& from) { to.x = (T)from.x; to.y = (T)from.y; return to; } \
template<> vector2<U>& convert<vector2<U>, vector2<T>>(vector2<U>& to, const vector2<T>& from) { to.x = (U)from.x; to.y = (U)from.y; return to; }

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