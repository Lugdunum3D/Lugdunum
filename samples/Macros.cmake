# shaders compilation commands
macro(compile_shader shader)
    add_custom_command(OUTPUT ${shader}.spv
                    COMMAND glslangValidator -V ${CMAKE_SOURCE_DIR}/${shader} -o ${shader}.spv
                    DEPENDS ${CMAKE_SOURCE_DIR}/${shader}
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
