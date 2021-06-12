cmake_minimum_required(VERSION 3.4.1)
project(sandhook)
ENABLE_LANGUAGE(ASM)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-narrowing -std=c++17")

include_directories(.)

add_subdirectory(exts/vixl)
include_directories(exts/vixl)

add_subdirectory(exts/distorm)
include_directories(exts/distorm)

add_subdirectory(exts/ucontext)
include_directories(exts/ucontext)

set(ARCH_SRC_AARCH32  instrcache/arm32/translator.cc
        inline/arm32/inline_hook_arm32.cpp)
set(ARCH_SRC_AARCH64  instrcache/arm64/translator.cc
        inline/arm64/inline_hook_arm64.cc)

if (CMAKE_SYSTEM_PROCESSOR MATCHES "^arm")
set(ARCH_SOURCES ${ARCH_SOURCES} ${ARCH_SRC_AARCH32})
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^aarch64")
set(ARCH_SOURCES ${ARCH_SOURCES} ${ARCH_SRC_AARCH64})
endif ()

add_library(sandhook SHARED
        common/types.cc
        common/dlfcn_nougat.cc
        common/elf_util.cc
        inline/inline_hook.cc
        platform/memory.cc
        memory/code_buffer.cc
        main.cc
        ${ARCH_SOURCES})

target_link_libraries(sandhook android log)
target_link_libraries(sandhook distorm)
target_link_libraries(sandhook vixl)
target_link_libraries(sandhook uccontext)

