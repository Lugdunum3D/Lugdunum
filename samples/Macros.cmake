set(SHADERS_DIR "shaders")

# shaders compilation commands
macro(compile_shader shader)
    add_custom_command(OUTPUT ${shader}.spv
                    COMMAND glslangValidator -V ${LUG_RESOURCES_DIR}/${SHADERS_DIR}/${shader} -o ${shader}.spv
                    DEPENDS ${LUG_RESOURCES_DIR}/${SHADERS_DIR}/${shader}
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
