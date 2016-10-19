#pragma once

#include <lug/Config.hpp>

#if defined(LUG_MAIN_EXPORT)

    #define LUG_MAIN_API LUG_API_EXPORT

#else

    #define LUG_MAIN_API LUG_API_IMPORT

#endif
