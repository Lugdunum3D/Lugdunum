#pragma once

#include <lug/Config.hpp>

#if defined(LUG_SYSTEM_EXPORT)

    #define LUG_SYSTEM_API LUG_API_EXPORT

#else

    #define LUG_SYSTEM_API LUG_API_IMPORT

#endif
