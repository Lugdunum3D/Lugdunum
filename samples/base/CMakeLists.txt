cmake_minimum_required(VERSION 3.1)

if(NOT ANDROID)
    # determine the build type
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
    endif()
    set(EXECUTABLE_NAME "base")
    add_executable(${EXECUTABLE_NAME} main.cpp)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../cmake/modules")
endif()

if(ANDROID_ABI)

    # build native_app_glue as a static lib
    set(APP_GLUE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android/app/src/main/cpp/native_app_glue)
    include_directories(${APP_GLUE_DIR})
    add_library( app-glue STATIC ${APP_GLUE_DIR}/android_native_app_glue.c)


    set(LUG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android/app/src/main/lugdunum)
    include_directories(${LUG_DIR}/include)



    add_library( # Sets the name of the library.
                 sample

                 # Sets the library as a shared library.
                 SHARED
                 # Provides a relative path to your source file(s).
                 # Associated headers in the same location as their source
                 # file are automatically included.
                 main.cpp)

    if(ANDROID_NATIVE_API_LEVEL EQUAL 23)
    target_link_libraries(sample
                          app-glue
                          log
                          android
                          ${LUG_DIR}/lib/liblug-system-s-d.a)
    else()
    target_link_libraries(sample
                              app-glue
                              log
                              android
                              ${LUG_DIR}/lib/liblug-system-s-d.so)
    endif()
endif()

# find Lugdunum and link with it
if(NOT ANDROID)
    find_package(LUG REQUIRED system)
    if(LUG_FOUND)
        include_directories(${LUG_INCLUDE_DIR})
        target_link_libraries(${EXECUTABLE_NAME} ${LUG_LIBRARIES})
    endif()
endif()
