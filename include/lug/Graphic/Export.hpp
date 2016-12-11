#pragma once

#include <lug/Config.hpp>

#if defined(LUG_GRAPHIC_EXPORT)

    #define LUG_GRAPHIC_API LUG_API_EXPORT

#else

    #define LUG_GRAPHIC_API LUG_API_IMPORT

#endif
