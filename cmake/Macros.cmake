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
        target_compile_options(${target} PUBLIC /W4 /WX /MP)
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
            LIBRARY DESTINATION lib${LIB_SUFFIX} COMPONENT bin
            ARCHIVE DESTINATION lib${LIB_SUFFIX} COMPONENT devel
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

function(lug_download_thirdparty)
    lug_set_option(LUG_THIRDPARTY_URL "https://thirdparty-dl.lugbench.eu" STRING "Choose the server from which to download the thirdparty directory")
    lug_set_option(LUG_ACCEPT_DL OFF BOOL "Choose whether to accept or not the download of the thirdparty directory")

    string(TOLOWER "${CMAKE_SYSTEM_NAME}" THIRDPARTY_PLATFORM)

    if (NOT EXISTS "${CMAKE_SOURCE_DIR}/.git")
        message(FATAL_ERROR "Can't find the .git directory")
    endif()

    find_package(Git)
    if (GIT_FOUND)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} log -n 1 --pretty=format:%h -- thirdparty.yml
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
            OUTPUT_VARIABLE "THIRDPARTY_LATEST_HASH"
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        message(STATUS "Thirdparty hash: ${THIRDPARTY_LATEST_HASH}")
    else()
        message(FATAL_ERROR "Can't find the latest revision of `thirdparty.yml`")
    endif()

    if (EXISTS "${LUG_THIRDPARTY_DIR}")
        if (NOT EXISTS "${LUG_THIRDPARTY_DIR}/version")
            message(STATUS "Thirdparty dir exists and version is UNKNOWN, using as-is")
            return()
        endif()

        file(STRINGS "${LUG_THIRDPARTY_DIR}/version" LUG_THIRDPARTY_VERSION)
        if (${LUG_THIRDPARTY_VERSION} STREQUAL "${THIRDPARTY_PLATFORM}_${THIRDPARTY_LATEST_HASH}")
            message(STATUS "Thirdparty dir exists and version is OK")
            return()
        endif()
        message(STATUS "Thirdparty dir exists but is outdated (${LUG_THIRDPARTY_VERSION}, needs ${THIRDPARTY_PLATFORM}_${THIRDPARTY_LATEST_HASH}), updating...")
        file(REMOVE_RECURSE "${LUG_THIRDPARTY_DIR}")
    endif()

    if (NOT "${LUG_ACCEPT_DL}")
        message(
            FATAL_ERROR
            "\n"
            "NOT FOUND: ${LUG_THIRDPARTY_DIR}\n"
            "---\n"
            "The thirdparty directory was NOT found, "
            "but we can download it for your architecture. "
            "To protect your privacy the download is not automatic.\n"
            "To proceed and accept the download, re-run cmake with -DLUG_ACCEPT_DL=ON\n"
            "---\n"
            "You can also compile it from source: see https://github.com/Lugdunum3D/ThirdParty-Builder\n"
            "---\n"
            "Thanks,\n"
            "The Lugdunum3D Team.\n"
            "\n"
        )
    endif()

    set(SHOULD_DOWNLOAD ON)
    set(DL_FILE "${CMAKE_BINARY_DIR}/thirdparty-${THIRDPARTY_LATEST_HASH}.zip")
    set(DL_URL "${LUG_THIRDPARTY_URL}/${THIRDPARTY_LATEST_HASH}/${THIRDPARTY_PLATFORM}.zip")

    # Checksums
    set(DL_FILE_EXPECTED_MD5 "${CMAKE_BINARY_DIR}/thirdparty-${THIRDPARTY_LATEST_HASH}.md5")
    set(DL_URL_EXPECTED_MD5 "${LUG_THIRDPARTY_URL}/${THIRDPARTY_LATEST_HASH}/${THIRDPARTY_PLATFORM}.md5")

    file(DOWNLOAD "${DL_URL_EXPECTED_MD5}" "${DL_FILE_EXPECTED_MD5}")
    file(STRINGS "${DL_FILE_EXPECTED_MD5}" DL_EXPECTED_MD5)
    message(STATUS "Expected MD5 will be '${DL_EXPECTED_MD5}'")

    # Check MD5 if file exists
    if (EXISTS "${DL_FILE}")
        file(MD5 "${DL_FILE}" DL_FILE_MD5)
        if (DL_FILE_MD5 STREQUAL DL_EXPECTED_MD5)
            message(STATUS "Already exists and checks valid: ${DL_FILE}")
            set(SHOULD_DOWNLOAD OFF)
        else()
            message(STATUS "Already exists but not valid: ${DL_FILE}")
        endif()
    endif()

    if(SHOULD_DOWNLOAD)
        message(STATUS "Downloading ${DL_URL}")
        file(
            DOWNLOAD
            "${DL_URL}" "${DL_FILE}"
            EXPECTED_MD5 "${DL_EXPECTED_MD5}"
        )
    endif()

    # Extract zip
    message(STATUS "Extracting...")
    file(MAKE_DIRECTORY "${LUG_THIRDPARTY_DIR}")
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xfz "${DL_FILE}"
        WORKING_DIRECTORY "${LUG_THIRDPARTY_DIR}"
        RESULT_VARIABLE rv
    )
    if (NOT rv EQUAL 0)
        file(REMOVE_RECURSE "${LUG_THIRDPARTY_DIR}")
        message(
            FATAL_ERROR
            "Extract of '${DL_FILE}' failed"
            "Try to remove it to download it again"
        )
    endif()

    file(WRITE "${LUG_THIRDPARTY_DIR}/version" "${THIRDPARTY_PLATFORM}_${THIRDPARTY_LATEST_HASH}")
    message(STATUS "Done!")

endfunction()
