set(INCROOT ${PROJECT_SOURCE_DIR}/include/lug/System)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/lug/System)

# all source files
set(SRC
    ${SRCROOT}/Exception.cpp
    ${SRCROOT}/Memory/Allocator/Basic.cpp
    ${SRCROOT}/Memory/Allocator/Linear.cpp
    ${SRCROOT}/Memory/Allocator/Stack.cpp
    ${SRCROOT}/Memory/FreeList.cpp
)
source_group("src" FILES ${SRC})

# all header files
set(INC
    ${INCROOT}/Debug.hpp
    ${INCROOT}/Exception.hpp
    ${INCROOT}/Export.hpp
    ${INCROOT}/Memory.hpp
    ${INCROOT}/Memory.inl
    ${INCROOT}/Memory/Allocator/Basic.hpp
    ${INCROOT}/Memory/Allocator/Chunk.hpp
    ${INCROOT}/Memory/Allocator/Chunk.inl
    ${INCROOT}/Memory/Allocator/Linear.hpp
    ${INCROOT}/Memory/Allocator/Pool.hpp
    ${INCROOT}/Memory/Allocator/Stack.hpp
    ${INCROOT}/Memory/Area/IArea.hpp
    ${INCROOT}/Memory/Area/Heap.hpp
    ${INCROOT}/Memory/Area/Heap.inl
    ${INCROOT}/Memory/Area/Stack.hpp
    ${INCROOT}/Memory/Area/Stack.inl
    ${INCROOT}/Memory/Arena.hpp
    ${INCROOT}/Memory/Arena.inl
    ${INCROOT}/Memory/FreeList.hpp
    ${INCROOT}/Memory/Policies/Thread.hpp
    ${INCROOT}/Memory/Policies/Thread.inl
    ${INCROOT}/Memory/Policies/BoundsChecker.hpp
    ${INCROOT}/Memory/Policies/BoundsChecker.inl
    ${INCROOT}/Memory/Policies/MemoryMarker.hpp
    ${INCROOT}/Memory/Policies/MemoryMarker.inl
)
source_group("inc" FILES ${INC})

# define the target
lug_add_library(lug-system
                SOURCES ${SRC} ${INC}
)
