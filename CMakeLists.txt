cmake_minimum_required(VERSION 3.22)
project(scary-baboon-mod)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(COMPILE_ID scary-baboon-mod)

# QPM generates these files on restore - they handle ALL include paths
include(${CMAKE_CURRENT_LIST_DIR}/qpm_defines.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/extern.cmake)

file(GLOB_RECURSE SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp
)

message(STATUS "Source files: ${SOURCE_FILES}")

add_library(${COMPILE_ID} SHARED ${SOURCE_FILES})

target_include_directories(${COMPILE_ID} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${EXTERN_INC_DIR}
)

target_link_libraries(${COMPILE_ID}
    ${EXTERN_LIB_DIR}
    -llog
    -lz
    -landroid
)

target_compile_options(${COMPILE_ID} PRIVATE
    -O2
    -fPIC
    -fvisibility=hidden
    -DANDROID
    -Wno-unused-value
)

target_compile_definitions(${COMPILE_ID} PRIVATE
    MOD_ID="${COMPILE_ID}"
    VERSION="0.1.0"
)
