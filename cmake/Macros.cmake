include(CMakeParseArguments)

macro(lug_add_library target)

    # parse the arguments
    cmake_parse_arguments(THIS "" "" "SOURCES;DEPENDS;EXTERNAL_LIBS" ${ARGN})

    # create the target
    add_library(${target} ${THIS_SOURCES})
    set_target_properties(${target} PROPERTIES LINKER_LANGUAGE CXX)

    # define the export symbol
    string(REPLACE "-" "_" NAME_UPPER "${target}")
    string(TOUPPER "${NAME_UPPER}" NAME_UPPER)
    set_target_properties(${target} PROPERTIES DEFINE_SYMBOL ${NAME_UPPER}_EXPORT)

    if(BUILD_SHARED_LIBS)
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
    else()
        set_target_properties(${target} PROPERTIES DEBUG_POSTFIX -s-d)
        set_target_properties(${target} PROPERTIES RELEASE_POSTFIX -s)
        set_target_properties(${target} PROPERTIES MINSIZEREL_POSTFIX -s)
        set_target_properties(${target} PROPERTIES RELWITHDEBINFO_POSTFIX -s)
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
