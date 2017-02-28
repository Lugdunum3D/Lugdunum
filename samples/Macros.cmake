include(CMakeParseArguments)

set(LUG_SHADERS_DIR "shaders")
set(PROJECT_SHADERS_ROOT "shaders")

# shaders
macro(target_shader shader)
    # Select where to copy the resource
    if(LUG_OS_ANDROID)
        set(new_path ${ANDROID_PROJECT_SHADERS}/${shader})
    else()
        set(new_path ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_SHADERS_ROOT}/${shader}.spv)
    endif()

    set(old_path ${LUG_RESOURCES_DIR}/${LUG_SHADERS_DIR}/${shader})
    get_filename_component(new_path_directory ${new_path} DIRECTORY)

    if(LUG_OS_ANDROID)
        # Just copy to the good place, Android will compile the shader for us
        add_custom_command(
            OUTPUT ${new_path}
            DEPENDS ${old_path}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${new_path_directory}
            COMMAND ${CMAKE_COMMAND} -E copy ${old_path} ${new_path}

            COMMENT "Copying shader ${old_path} to ${new_path}"
        )
    else()
        # Compile the shader
        add_custom_command(
            OUTPUT ${new_path}
            DEPENDS ${old_path}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${new_path_directory}
            COMMAND glslangValidator -V ${old_path} -o ${new_path}

            COMMENT "Compiling shader ${old_path} to ${new_path}"
        )
    endif()

    list(APPEND SHADERS_DEPENDS ${new_path})
endmacro()

macro(target_shaders target)
    foreach(shader ${ARGN})
        target_shader(${shader})
    endforeach(shader)

    add_custom_target(shaders DEPENDS ${SHADERS_DEPENDS})
    add_dependencies(${target} shaders)
endmacro()

# resources
macro(target_resource other_resource)
    # Select where to copy the resource
    if(LUG_OS_ANDROID)
        set(new_path ${ANDROID_PROJECT_ASSETS}/${other_resource})
    else()
        set(new_path ${CMAKE_CURRENT_BINARY_DIR}/${other_resource})
    endif()

    set(old_path ${LUG_RESOURCES_DIR}/${other_resource})
    get_filename_component(new_path_directory ${new_path} DIRECTORY)

    # Custom command to create the directory and copy the resource
    add_custom_command(
        OUTPUT ${new_path}
        DEPENDS ${old_path}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${new_path_directory}
        COMMAND ${CMAKE_COMMAND} -E copy ${old_path} ${new_path}

        COMMENT "Copying ${old_path} to ${new_path}"
    )

    list(APPEND OTHER_RESOURCES_DEPENDS ${new_path})
endmacro()

macro(target_resources target)
    # Copy all resources
    foreach(other_resource ${ARGN})
        target_resource(${other_resource})
    endforeach(other_resource)

    # Link new resources files to the target
    add_custom_target(resources DEPENDS ${OTHER_RESOURCES_DEPENDS})
    add_dependencies(${target} resources)
endmacro()

# macro to add a sample
macro(lug_add_sample target)
    # parse the arguments
    cmake_parse_arguments(THIS "" "" "SOURCES;DEPENDS;SHADERS;OTHER_RESOURCES" ${ARGN})

    # find vulkan
    find_package(Vulkan)

    if (NOT VULKAN_INCLUDE_DIR)
        if (NOT EXISTS "${CMAKE_SOURCE_DIR}/../../../thirdparty/vulkan")
            message(FATAL_ERROR "Can't find vulkan, call `git submodule update --recursive`")
        endif()

        set(VULKAN_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/../../../thirdparty/vulkan/include)
    endif()

    include_directories(${VULKAN_INCLUDE_DIR})

    # find fmt
    find_package(Fmt)

    if (NOT FMT_INCLUDE_DIR)
        if (NOT EXISTS "${CMAKE_SOURCE_DIR}/../../../thirdparty/fmt")
            message(FATAL_ERROR "Can't find fmt, call `git submodule update --recursive`")
        endif()

        set(FMT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/../../../thirdparty/fmt/include)
    endif()

    include_directories(${FMT_INCLUDE_DIR})

    # find Lugdunum
    find_package(LUG REQUIRED ${THIS_DEPENDS})

    # create target
    if(LUG_OS_ANDROID)
        add_library(${target} SHARED ${THIS_SOURCES})

        set(ANDROID_PROJECT_PATH ${PROJECT_SOURCE_DIR}/../compiler/android/${target}App)
        set(ANDROID_PROJECT_ASSETS ${ANDROID_PROJECT_PATH}/src/main/assets)
        set(ANDROID_PROJECT_SHADERS ${ANDROID_PROJECT_PATH}/src/main/shaders)
    else()
        add_executable(${target} ${THIS_SOURCES})
    endif()

    lug_add_compile_options(${target})

    # use lugdunum
    include_directories(${LUG_INCLUDE_DIR})
    target_link_libraries(${target} ${LUG_LIBRARIES})

    # copy / build shaders
    if(THIS_SHADERS)
        target_shaders(${target} ${THIS_SHADERS})
    endif()

    # copy resources
    if(THIS_OTHER_RESOURCES)
        target_resources(${target} ${THIS_OTHER_RESOURCES})
    endif()
endmacro()
