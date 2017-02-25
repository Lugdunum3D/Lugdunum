set(SHADERS_DIR "shaders")

# shaders compilation commands
macro(compile_shader shader)
    set(shader_path  "${LUG_RESOURCES_DIR}/${SHADERS_DIR}/${shader}")
    set(shader_spv_path  "${CMAKE_CURRENT_BINARY_DIR}/${shader}.spv")
    add_custom_command(OUTPUT ${shader_spv_path}
                    COMMAND glslangValidator -V ${shader_path} -o ${shader_spv_path}
                    DEPENDS ${shader_path}
                    COMMENT "Compiling ${shader_path} to ${shader_spv_path}")
    list(APPEND SHADERS_SPV ${shader_spv_path})
endmacro()

macro(compile_shaders target)
    foreach(shader ${ARGN})
        compile_shader(${shader})
    endforeach(shader)

    add_custom_target(shaders DEPENDS ${SHADERS_SPV})
    add_dependencies(${target} shaders)
endmacro()
