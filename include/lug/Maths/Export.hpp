#pragma once

#include <lug/Config.hpp>

#if defined(LUG_MATH_API)

    #define LUG_MATH_API LUG_API_EXPORT

#else

    #define LUG_MATH_API LUG_API_IMPORT

#endif
