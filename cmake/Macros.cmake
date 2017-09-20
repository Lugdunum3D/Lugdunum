include(CMakeParseArguments)

# define a macro that helps defining an option
macro(lug_set_option var default type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()

    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

macro(populate_android_infos)
    set(ANDROID_ABI arm64-v8a)
    message(STATUS "ANDROID_ABI: ${ANDROID_ABI}")

    set(ANDROID_TOOLCHAIN clang)
    message(STATUS "ANDROID_TOOLCHAIN: ${ANDROID_TOOLCHAIN}")

    lug_set_option(ANDROID_PLATFORM android-24 STRING "Target specific android version target (default: android-24)")
    if(NOT ANDROID_PLATFORM MATCHES android-2[4-9])
        message(FATAL_ERROR "Invalid android version target: ${ANDROID_PLATFORM}")
    endif()
    message(STATUS "ANDROID_PLATFORM: ${ANDROID_PLATFORM}")

    lug_set_option(ANDROID_STL c++_shared STRING "Choose which Android STL to use (default: c++_shared)")

    if(NOT ANDROID_STL STREQUAL c++_static AND NOT ANDROID_STL STREQUAL c++_shared)
        message(FATAL_ERROR "Invalid STL: ${ANDROID_STL}")
    endif()

    set(LIB_SUFFIX "/${ANDROID_ABI}")
endmacro()

macro(lug_add_compile_options target)
    # c++14 on android
    if(LUG_OS_ANDROID)
        target_compile_options(${target} PUBLIC -std=c++14)
    endif()

    # use warnings and errors
    if(LUG_COMPILER_MSVC)
        target_compile_options(${target} PUBLIC /W4 /WX)
    elseif(LUG_COMPILER_GCC OR LUG_COMPILER_CLANG)
        target_compile_options(${target} PUBLIC -Wall -Wextra -Werror)
    endif()

    # add parallel build for MSVC
    if(LUG_COMPILER_MSVC)
        target_compile_options(${target} PUBLIC /MP)
    endif()
endmacro()

macro(lug_add_library target)
    # parse the arguments
    cmake_parse_arguments(THIS "" "" "SOURCES;DEPENDS;EXTERNAL_LIBS" ${ARGN})

    # create the target
    add_library(${target} ${THIS_SOURCES})
    set_target_properties(${target} PROPERTIES LINKER_LANGUAGE CXX)

    # add compile options
    lug_add_compile_options(${target})

    # define the export symbol
    string(REPLACE "-" "_" NAME_UPPER "${target}")
    string(TOUPPER "${NAME_UPPER}" NAME_UPPER)
    set_target_properties(${target} PROPERTIES DEFINE_SYMBOL ${NAME_UPPER}_EXPORT)

    set_target_properties(${target} PROPERTIES DEBUG_POSTFIX -d)

    if(LUG_OS_WINDOWS)
        # include the major version number in Windows shared library names (but not import library names)
        set_target_properties(${target} PROPERTIES SUFFIX "-${VERSION_MAJOR}${CMAKE_SHARED_LIBRARY_SUFFIX}")
    endif()

    if(LUG_OS_WINDOWS AND LUG_COMPILER_GCC)
        # on Windows/gcc get rid of "lib" prefix for shared libraries,
        # and transform the ".dll.a" suffix into ".a" for import libraries
        set_target_properties(${target} PROPERTIES PREFIX "")
        set_target_properties(${target} PROPERTIES IMPORT_SUFFIX ".a")
    endif()

    # set the target's folder (for IDEs that support it, e.g. Visual Studio)
    set_target_properties(${target} PROPERTIES FOLDER "lug")

    # if using gcc or clang on a non-Windows platform, we must hide public symbols by default
    # (exported ones are explicitly marked)
    if(NOT LUG_OS_WINDOWS AND (LUG_COMPILER_GCC OR LUG_COMPILER_CLANG))
        set_target_properties(${target} PROPERTIES COMPILE_FLAGS -fvisibility=hidden)
    endif()

    # link the target to its lug dependencies
    if(THIS_DEPENDS)
        target_link_libraries(${target} ${THIS_DEPENDS})
    endif()

    # link the target to its external dependencies
    if(THIS_EXTERNAL_LIBS)
        target_link_libraries(${target} ${THIS_EXTERNAL_LIBS})
    endif()

    # setup the install of the library
    install(TARGETS ${target}
            RUNTIME DESTINATION bin COMPONENT bin
            LIBRARY DESTINATION lib${LIB_SUFFIX}/${ANDROID_ABI} COMPONENT bin
            ARCHIVE DESTINATION lib${LIB_SUFFIX}/${ANDROID_ABI} COMPONENT devel
            FRAMEWORK DESTINATION ${CMAKE_INSTALL_FRAMEWORK_PREFIX} COMPONENT bin
    )
endmacro()

macro(lug_add_test name)
    set(target run${name}UnitTests)

    # parse the arguments
    cmake_parse_arguments(THIS "" "" "SOURCES;DEPENDS;EXTERNAL_LIBS;SHADERS" ${ARGN})

    add_executable(${target} ${THIS_SOURCES} ${PROJECT_SOURCE_DIR}/main.cpp)

    # add compile options
    lug_add_compile_options(${target})

    # link the target to its lug dependencies
    if(THIS_DEPENDS)
        target_link_libraries(${target} ${THIS_DEPENDS})
    endif()

    # link the target to its external dependencies
    if(THIS_EXTERNAL_LIBS)
        target_link_libraries(${target} ${THIS_EXTERNAL_LIBS})
    endif()

    if(THIS_SHADERS)
        foreach(shader ${THIS_SHADERS})
            set(new_path ${CMAKE_CURRENT_BINARY_DIR}/shaders/${shader})
            set(old_path ${CMAKE_SOURCE_DIR}/resources/shaders/${shader})

            add_custom_command(
                OUTPUT ${new_path}
                DEPENDS ${old_path}
                COMMAND ${CMAKE_COMMAND} -E make_directory shaders/
                COMMAND ${CMAKE_COMMAND} -E copy ${old_path} ${new_path}

                COMMENT "Copying ${old_path} to ${new_path}"
            )

            list(APPEND SHADERS_DEPENDS ${new_path})
        endforeach(shader)

        add_custom_target(${target}Shaders DEPENDS ${SHADERS_DEPENDS})
        add_dependencies(${target} ${target}Shaders)
    endif()

    target_link_libraries(${target} ${GTEST_LIBRARIES} ${GMOCK_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

    add_test(NAME ${name}UnitTests COMMAND ${target} --gtest_output=xml:${TEST_OUTPUT}/${name}UnitTests.xml)
endmacro()
