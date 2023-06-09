cmake_minimum_required(VERSION 3.24)

project(himawari_dl C)

set(CMAKE_C_STANDARD 90)

# All files in source and include directory
file(GLOB_RECURSE SRC_DIR RELATIVE ${CMAKE_SOURCE_DIR} src/*.c)
file(GLOB_RECURSE INC_DIR RELATIVE ${CMAKE_SOURCE_DIR} include/*.h)

# Set output directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set required packages
find_package(CURL REQUIRED)
find_package(PNG REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include ${CURL_INCLUDE_DIR}
        ${PNG_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SRC_DIR} ${INC_DIR})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
target_compile_options(${PROJECT_NAME} PRIVATE -Werror)
target_compile_options(${PROJECT_NAME} PRIVATE -pedantic-errors)
target_compile_options(${PROJECT_NAME} PRIVATE -pedantic)
target_compile_options(${PROJECT_NAME} PRIVATE -Wextra)
target_compile_options(${PROJECT_NAME} PRIVATE -Wshadow)
target_compile_options(${PROJECT_NAME} PRIVATE -Wundef)
target_compile_options(${PROJECT_NAME} PRIVATE -Wfloat-equal)
target_compile_options(${PROJECT_NAME} PRIVATE -Wpointer-arith)
target_compile_options(${PROJECT_NAME} PRIVATE -Wcast-align)
target_compile_options(${PROJECT_NAME} PRIVATE -Wstrict-prototypes)
target_compile_options(${PROJECT_NAME} PRIVATE -Wstrict-overflow)
target_compile_options(${PROJECT_NAME} PRIVATE -Wno-misleading-indentation)

target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARIES} ${PNG_LIBRARIES})
