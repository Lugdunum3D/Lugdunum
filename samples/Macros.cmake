set(LUG_SHADERS_DIR "shaders")
set(SHADERS_ROOT "shaders")

# shaders compilation commands
macro(compile_shader shader)
    set(shader_path ${LUG_RESOURCES_DIR}/${LUG_SHADERS_DIR}/${shader})
    set(shader_spv_path ${CMAKE_CURRENT_BINARY_DIR}/${SHADERS_ROOT}/${shader}.spv)

    add_custom_command(OUTPUT ${shader_spv_path}
                    COMMAND glslangValidator -V ${shader_path} -o ${shader_spv_path}
                    DEPENDS ${shader_path}
                    COMMENT "Compiling ${shader_path} to ${shader_spv_path}")

    list(APPEND SHADERS_SPV ${shader_spv_path})
endmacro()

macro(compile_shaders target)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${SHADERS_ROOT})

    foreach(shader ${ARGN})
        compile_shader(${shader})
    endforeach(shader)

    add_custom_target(shaders DEPENDS ${SHADERS_SPV})
    add_dependencies(${target} shaders)
endmacro()
