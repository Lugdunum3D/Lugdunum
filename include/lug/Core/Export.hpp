#pragma once

#include <lug/Config.hpp>

#if defined(LUG_CORE_EXPORT)

    #define LUG_CORE_API LUG_API_EXPORT

#else

    #define LUG_CORE_API LUG_API_IMPORT

#endif
