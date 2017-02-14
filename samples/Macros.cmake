set(SHADERS_DIR "${LUG_ROOT}/resources/shaders")

# shaders compilation commands
macro(compile_shader shader)
    add_custom_command(OUTPUT ${shader}.spv
                    COMMAND glslangValidator -V ${SHADERS_DIR}/${shader} -o ${shader}.spv
                    DEPENDS ${SHADERS_DIR}/${shader}
                    COMMENT "Compiling ${shader}")
    list(APPEND SHADERS_SPV ${shader}.spv)
endmacro()

macro(compile_shaders target)

    foreach(shader ${ARGN})
        compile_shader(${shader})
    endforeach(shader)

    add_custom_target(shaders DEPENDS ${SHADERS_SPV})
    add_dependencies(${target} shaders)
endmacro()
