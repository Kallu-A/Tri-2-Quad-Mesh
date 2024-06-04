project(tri-2-quad-mesh LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(entrypoint entrypoint.cpp)

target_link_libraries(entrypoint ${CMAKE_DL_LIBS} ultimaille $<$<BOOL:${OpenMP_CXX_FOUND}>:OpenMP::OpenMP_CXX>)

IF (NOT WIN32)
    target_link_libraries(entrypoint m)
ENDIF()

