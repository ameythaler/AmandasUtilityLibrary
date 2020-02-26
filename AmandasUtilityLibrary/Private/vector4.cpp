#include "stdafx.h"
#include <aul/vector4.h>
#include <aul/vector3.h>
#include <aul/vector2.h>

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
}