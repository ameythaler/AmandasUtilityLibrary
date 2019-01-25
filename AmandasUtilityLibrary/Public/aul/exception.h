#pragma once

#include "aul/platform.h"

#if AUL_THROW_EXCEPTIONS

#include <exception>

namespace aul
{
    using exception = std::exception;
}

#define AUL_THROW(EXCEPTION) throw aul::exception(EXCEPTION)

#else // AUL_THROW_EXCEPTIONS

#define AUL_THROW(EXCEPTION)

#endif // AUL_THROW_EXCEPTIONS