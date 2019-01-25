#pragma once

#include "aul/platform.h"

#if AUL_MSVC
#define AUL_ALIGN(x) __declspec(align(x))
#elif AUL_STD // AUL_MSVC
#define AUL_ALIGN(x) __attribute__((aligned(x)))
#if AUL_MAC
#ifdef __OJBC__
#define AUL_OBJC_CLASS(name) @class name
#else // __OBJC__
#define AUL_OBJC_CLASS(name) typedef struct objc_object name
#endif // __OBJC__
#endif // AUL_MAC
#endif // AUL_MSVC

#if AUL_WIN
#define AUL_SYSTEM_PAUSE system("Pause")
#elif AUL_POSIX // AUL_WIN
#define AUL_SYSTEM_PAUSE system("read -n 1 -s -p \"Press any key to continue...\"; echo")
#endif // AUL_WIN

#define AUL_DISABLECOPYING(CLASS_NAME) CLASS_NAME(const CLASS_NAME&) = delete;\
CLASS_NAME(const CLASS_NAME&&) = delete;\
CLASS_NAME& operator =(const CLASS_NAME&) = delete;\
CLASS_NAME& operator =(const CLASS_NAME&&) = delete

#define AUL_ITERATOR_FOR_LOOP(iter, container) for(auto iter = container.begin(); iter != container.end(); ++iter)
#define AUL_ITERATOR_WHILE_LOOP(iter, container) for(auto iter = container.begin(); iter != container.end();)

#define AUL_DECLARE_SINGLETON(CLASS_NAME) public:\
inline static CLASS_NAME& instance()\
{\
	return s_instance;\
}\
inline static CLASS_NAME* instance_ptr()\
{\
	return &s_instance;\
}\
private:\
static CLASS_NAME s_instance;\
CLASS_NAME();\
virtual ~CLASS_NAME();\
CLASS_NAME(const CLASS_NAME&) = delete;\
CLASS_NAME(CLASS_NAME&&) = delete;\
CLASS_NAME& operator=(const CLASS_NAME&) = delete;\
CLASS_NAME& operator=(CLASS_NAME&&) = delete

#define AUL_IMPLEMENT_SINGLETON(CLASS_NAME) CLASS_NAME CLASS_NAME::s_instance

#define AUL_CAT(lhs, rhs) lhs ## rhs

#define AUL_ENUM_FLAG_OP(TYPE, OP, BASE_TYPE) TYPE operator ## OP ## (TYPE lhs, TYPE rhs)\
{\
    return (TYPE)((BASE_TYPE)lhs OP (BASE_TYPE)rhs);\
}