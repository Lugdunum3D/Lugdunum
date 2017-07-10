include(CMakeParseArguments)

set(LUG_SHADERS_DIR "shaders")
set(PROJECT_SHADERS_ROOT "shaders")

# shaders
macro(add_shader shader)
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

macro(add_shaders target)
    lug_set_option(BUILD_SHADERS TRUE BOOL "Compile shaders")

    if(BUILD_SHADERS)
        foreach(shader ${ARGN})
            add_shader(${shader})
        endforeach(shader)
    endif()

    set(target_shaders "shaders-${target}")

    add_custom_target(${target_shaders} DEPENDS ${SHADERS_DEPENDS})
    add_dependencies(${target} ${target_shaders})
endmacro()

# resources
macro(add_resource target_resources directory resource)
    # Select where to copy the resource
    if(LUG_OS_ANDROID)
        set(new_path ${ANDROID_PROJECT_ASSETS}/${resource})
    else()
        set(new_path ${CMAKE_CURRENT_BINARY_DIR}/${resource})
    endif()

    set(old_path ${directory}/${resource})
    get_filename_component(new_path_directory ${new_path} DIRECTORY)

    # Custom command to create the directory and copy the resource
    add_custom_command(
        OUTPUT ${new_path}
        DEPENDS ${old_path}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${new_path_directory}
        COMMAND ${CMAKE_COMMAND} -E copy ${old_path} ${new_path}

        COMMENT "Copying ${old_path} to ${new_path}"
    )

    list(APPEND ${target_resources}_DEPENDS ${new_path})
endmacro()

macro(add_resources target target_resources directory)
    # Copy all resources
    foreach(resource ${ARGN})
        add_resource(${target_resources} ${directory} ${resource})
    endforeach(resource)

    # Link new resources files to the target
    add_custom_target(${target_resources} DEPENDS ${${target_resources}_DEPENDS})
    add_dependencies(${target} ${target_resources})
endmacro()

# macro to add a sample
macro(lug_add_sample target)
    # parse the arguments
    cmake_parse_arguments(THIS "" "" "SOURCES;DEPENDS;SHADERS;EXTERNAL_LIBS;LUG_RESOURCES;OTHER_RESOURCES" ${ARGN})

    # find Vulkan
    find_package(Vulkan)
    if (NOT VULKAN_INCLUDE_DIR)
        if (NOT EXISTS "${LUG_THIRDPARTY_DIR}/vulkan")
            message(FATAL_ERROR "Can't find vulkan in the thirdparty directory")
        endif()

        set(VULKAN_INCLUDE_DIR ${LUG_THIRDPARTY_DIR}/vulkan/include)
        message(STATUS "Found Vulkan: ${VULKAN_INCLUDE_DIR}")
    endif()

    include_directories(${VULKAN_INCLUDE_DIR})

    # find fmt
    find_package(Fmt)

    if (NOT FMT_INCLUDE_DIR)
        if (NOT EXISTS "${LUG_THIRDPARTY_DIR}/fmt")
            message(FATAL_ERROR "Can't find fmt in the thirdparty directory")
        endif()

        set(FMT_INCLUDE_DIR ${LUG_THIRDPARTY_DIR}/fmt/include)
        message(STATUS "Found Fmt: ${FMT_INCLUDE_DIR}")
    endif()

    include_directories(${FMT_INCLUDE_DIR})

    # find imgui
    if (NOT EXISTS "${LUG_THIRDPARTY_DIR}/imgui")
        message(FATAL_ERROR "Can't find imgui in the thirdparty directory")
    endif()

    set(IMGUI_INCLUDE_DIR ${LUG_THIRDPARTY_DIR}/imgui/include)
    message(STATUS "Found Imgui: ${IMGUI_INCLUDE_DIR}")

    include_directories(${IMGUI_INCLUDE_DIR})

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

    # link the target to its external dependencies
    if(THIS_EXTERNAL_LIBS)
        target_link_libraries(${target} ${THIS_EXTERNAL_LIBS})
    endif()

    # use lugdunum
    include_directories(${LUG_INCLUDE_DIR})
    target_link_libraries(${target} ${LUG_LIBRARIES})

    # copy / build shaders
    if(THIS_SHADERS)
        add_shaders(${target} ${THIS_SHADERS})
    endif()

    # copy lugdunum resources
    if(THIS_LUG_RESOURCES)
        add_resources(${target} "lug-resources-${target}" ${LUG_RESOURCES_DIR} ${THIS_LUG_RESOURCES})
    endif()

    # copy resources
    if(THIS_OTHER_RESOURCES)
        add_resources(${target} "sample-resources-${target}" "${CMAKE_SOURCE_DIR}/resources" ${THIS_OTHER_RESOURCES})
    endif()
endmacro()
