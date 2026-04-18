cmake_minimum_required(VERSION 3.22)
project(scary-baboon-mod)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)
set(COMPILE_ID scary-baboon-mod)
set(EXTERN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern)

file(GLOB_RECURSE SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp
)

message(STATUS "Source files: ${SOURCE_FILES}")

add_library(${COMPILE_ID} SHARED ${SOURCE_FILES})

target_include_directories(${COMPILE_ID} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${EXTERN_DIR}
    ${EXTERN_DIR}/beatsaber-hook/shared
    ${EXTERN_DIR}/beatsaber-hook/shared/utils
    ${EXTERN_DIR}/scotland2/shared
    ${EXTERN_DIR}/paper/shared
    ${EXTERN_DIR}/paper/shared/utfcpp/source
    ${EXTERN_DIR}/fmt/include
    ${EXTERN_DIR}/libil2cpp/il2cpp
)

target_link_libraries(${COMPILE_ID}
    -llog
    -lz
    -landroid
)

target_compile_options(${COMPILE_ID} PRIVATE
    -O2
    -fPIC
    -fvisibility=hidden
    -DANDROID
)

target_compile_definitions(${COMPILE_ID} PRIVATE
    MOD_ID="${COMPILE_ID}"
    VERSION="0.1.0"
)
