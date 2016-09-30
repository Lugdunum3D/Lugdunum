#pragma once

#include <lug/Config.hpp>

#if defined(LUG_WINDOW_EXPORT)

    #define LUG_WINDOW_API LUG_API_EXPORT

#else

    #define LUG_WINDOW_API LUG_API_IMPORT

#endif
