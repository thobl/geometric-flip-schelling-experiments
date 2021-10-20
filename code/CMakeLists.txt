cmake_minimum_required(VERSION 3.3)

project(Schelling)

get_filename_component(build_dir ${CMAKE_CURRENT_BINARY_DIR} NAME)
if (${build_dir} MATCHES "debug")
  set(CMAKE_BUILD_TYPE Debug)
else()
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

set(HEADERS
  lib/CLI11.hpp
  include/graph.hpp
  include/rgg.hpp
  include/er.hpp
  include/ipe.hpp
  include/voting.hpp
  include/visualization.hpp)

set(SOURCE_FILES
  source/graph.cpp
  source/rgg.cpp
  source/er.cpp
  source/ipe.cpp
  source/voting.cpp
  source/visualization.cpp)

set(EXECUTABLE_FILES
  source/run.cpp)

# this is some windows stuff; not sure whether we need it...
find_library(PSAPI psapi)
if (NOT PSAPI)
  set(PSAPI "")
endif()

include_directories(include lib)
add_library(code STATIC ${SOURCE_FILES} ${HEADERS})
foreach(file ${EXECUTABLE_FILES})
    get_filename_component(executable ${file} NAME_WE)
    add_executable(${executable} ${file})
    target_link_libraries(${executable} code ${PSAPI})
endforeach()
