
cmake_minimum_required(VERSION 3.11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "/std:c++17 /clr /W4 /WX /permissive")
set(CMAKE_CXX_FLAGS_RELEASE "/Ob2 /O1")
set(CMAKE_CXX_FLAGS_DEBUG "/Zi")

include(FetchContent)
FetchContent_Populate(mavlink
  GIT_REPOSITORY https://github.com/mavlink/c_library_v2)

project(mavlink-clr-exporter)

include_directories(${mavlink_SOURCE_DIR})

add_executable(mavlink-clr-exporter
  mavlink-clr-exporter.cc)

add_custom_target(run
  DEPENDS mavlink-clr-exporter
  COMMAND mavlink-clr-exporter
  DEPENDS mavlink-clr.dll
  COMMAND ildasm mavlink-clr.dll)
