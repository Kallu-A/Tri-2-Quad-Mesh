cmake_minimum_required(VERSION 3.12...3.26)

project(ultimailleBunin LANGUAGES C CXX )
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)

message("OSName: " ${OSName})
message("System name: " ${CMAKE_SYSTEM_NAME})


# Get config
get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(NOT is_multi_config AND NOT (CMAKE_BUILD_TYPE OR DEFINED ENV{CMAKE_BUILD_TYPE}))
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Release default")
endif()

# Get development version of ultimaille directly from git repository 
include(FetchContent)
FetchContent_Declare(
  ultimaille
  GIT_REPOSITORY https://github.com/ssloy/ultimaille
  # GIT_TAG        v1.1.0
  GIT_TAG        geom
)
FetchContent_MakeAvailable(ultimaille)

# Add ultimaille directory to get ultimaille CMakeLists
include_directories(${ultimaille_SOURCE_DIR})

# Add executable binary
add_executable(entrypoint src/entrypoint.cpp)

# Link binary with ultimaille / param-parser
target_link_libraries(entrypoint  ${CMAKE_DL_LIBS} ultimaille $<$<BOOL:${OpenMP_CXX_FOUND}>:OpenMP::OpenMP_CXX>)

IF (NOT WIN32)
	target_link_libraries(entrypoint m)
ENDIF()

if (NOT DEFINED OSName)
  set(OSName ${CMAKE_SYSTEM_NAME})
endif()

set_target_properties(
    entrypoint
        PROPERTIES
            OUTPUT_NAME "entrypoint_exec_${OSName}"
)

# Copy asset files to build directory
configure_file(mesh/catorus.geogram mesh/catorus.geogram COPYONLY)
configure_file(mesh/tri_mesh.geogram mesh/tri_mesh.geogram COPYONLY)
configure_file(mesh/quarterdisk_tri.mesh mesh/quarterdisk_tri.mesh COPYONLY)
