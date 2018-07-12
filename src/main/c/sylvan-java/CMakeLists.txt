cmake_minimum_required(VERSION 3.1)

project(sylvan-java)

enable_language(C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(CMakeDependentOption)

set(LIBRARY_OUTPUT_PATH lib)
set(SYLVAN_SOVERSION 1)

include_directories(${SYLVAN_SRC_ROOT}/src/sylvan/include)

if(USE_NATIVE_JNI)
    find_package(JNI REQUIRED)
    include_directories(${JNI_INCLUDE_DIRS})
else()
    include_directories(/java/include)
    include_directories(/java/include/linux)
    include_directories(/java/include/darwin)
endif()

set(JNI_SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/jsylvan.c
    ${PROJECT_SOURCE_DIR}/src/mc_help.c)

add_library(sylvan-java SHARED ${JNI_SOURCE_FILES})
target_link_libraries(sylvan-java ${SYLVAN_BUILD_ROOT}/src/libsylvan${CMAKE_STATIC_LIBRARY_SUFFIX})
