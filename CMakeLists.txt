cmake_minimum_required(VERSION 3.22)
project(scary-baboon-mod)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(COMPILE_ID scary-baboon-mod)
set(EXTERN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern)

# ── QPM extern cmake includes ──────────────────────────────────────────────
include(${EXTERN_DIR}/beatsaber-hook/cmake/beatsaber-hook.cmake OPTIONAL)

# ── Collect source files ───────────────────────────────────────────────────
file(GLOB_RECURSE SOURCE_FILES
    src/*.cpp
    src/**/*.cpp
)

add_library(${COMPILE_ID} SHARED ${SOURCE_FILES})

# ── Include directories ────────────────────────────────────────────────────
target_include_directories(${COMPILE_ID} PRIVATE
    src
    ${EXTERN_DIR}/beatsaber-hook/shared
    ${EXTERN_DIR}/scotland2/shared
    ${EXTERN_DIR}/paper/shared
    ${EXTERN_DIR}/paper/shared/utfcpp/source
    ${EXTERN_DIR}/fmt/include
    ${EXTERN_DIR}/libil2cpp/il2cpp
)

# ── Link libraries ─────────────────────────────────────────────────────────
target_link_libraries(${COMPILE_ID}
    -llog
    -lz
    -landroid
    ${EXTERN_DIR}/libs/libbeatsaber-hook_5_1_9.so
    ${EXTERN_DIR}/libs/libsl2.so
    ${EXTERN_DIR}/libs/libpaperlog.so
)

# ── Compiler flags ─────────────────────────────────────────────────────────
target_compile_options(${COMPILE_ID} PRIVATE
    -O2
    -fPIC
    -fvisibility=hidden
    -DUNITY_2021
    -DANDROID
)

target_compile_definitions(${COMPILE_ID} PRIVATE
    MOD_ID="${COMPILE_ID}"
    VERSION="0.1.0"
)
