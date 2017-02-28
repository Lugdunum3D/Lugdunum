# detect os
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(LUG_OS_WINDOWS 1)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(LUG_OS_LINUX 1)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    set(LUG_OS_ANDROID 1)
else()
    message(FATAL_ERROR "Unsupported operating system or environment")
    return()
endif()

# detect compiler
if(CMAKE_CXX_COMPILER MATCHES ".*clang[+][+]" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang++")
    set(LUG_COMPILER_CLANG 1)
elseif(CMAKE_COMPILER_IS_GNUCXX)
    set(LUG_COMPILER_GCC 1)
elseif(MSVC)
    set(LUG_COMPILER_MSVC 1)
else()
    message(FATAL_ERROR "Unsupported compiler")
    return()
endif()

# define the install directory for miscellaneous files
if(LUG_OS_WINDOWS)
    set(INSTALL_MISC_DIR .)
elseif(LUG_OS_LINUX)
    set(INSTALL_MISC_DIR share/lug)
elseif(LUG_OS_ANDROID)
    set(INSTALL_MISC_DIR .)
endif()

# define the architecture
if (CMAKE_CL_64)
  set(ARCH_DIR "x64")
else()
  set(ARCH_DIR "x86")
endif()

# use c++14
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)
