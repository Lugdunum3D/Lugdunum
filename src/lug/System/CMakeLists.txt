set(INCROOT ${PROJECT_SOURCE_DIR}/include/lug/System)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/lug/System)

# all source files
set(SRC
    ${SRCROOT}/Exception.cpp
    ${SRCROOT}/Logger/FileHandler.cpp
    ${SRCROOT}/Logger/Formatter.cpp
    ${SRCROOT}/Logger/Handler.cpp
    ${SRCROOT}/Logger/Logger.cpp
    ${SRCROOT}/Logger/LoggingFacility.cpp
    ${SRCROOT}/Logger/OstreamHandler.cpp
    ${SRCROOT}/Memory/Allocator/Basic.cpp
    ${SRCROOT}/Memory/Allocator/Linear.cpp
    ${SRCROOT}/Memory/Allocator/Stack.cpp
    ${SRCROOT}/Memory/FreeList.cpp
)

# all header files
set(INC
    ${INCROOT}/Debug.hpp
    ${INCROOT}/Exception.hpp
    ${INCROOT}/Export.hpp
    ${INCROOT}/Logger.hpp
    ${INCROOT}/Logger/Common.hpp
    ${INCROOT}/Logger/FileHandler.hpp
    ${INCROOT}/Logger/Formatter.hpp
    ${INCROOT}/Logger/Handler.hpp
    ${INCROOT}/Logger/LoggingFacility.hpp
    ${INCROOT}/Logger/Message.hpp
    ${INCROOT}/Logger/OstreamHandler.hpp
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
    ${INCROOT}/Memory/Area/GrowingHeap.hpp
    ${INCROOT}/Memory/Area/GrowingHeap.inl
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

set(EXT_LIBRARIES)
if(LUG_OS_ANDROID)
    list(APPEND EXT_LIBRARIES log)
endif()

if(LUG_OS_ANDROID)
    list(APPEND SRC ${SRCROOT}/Logger/LogCatHandler.cpp)
    list(APPEND INC ${INCROOT}/Logger/LogCatHandler.hpp)
endif()

source_group("src" FILES ${SRC})
source_group("inc" FILES ${INC})

# define the target
lug_add_library(lug-system
                SOURCES ${SRC} ${INC}
                EXTERNAL_LIBS ${EXT_LIBRARIES}
)
