cmake_minimum_required(VERSION 3.20)
project(UpmemCostModel CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(UpmemCostModel STATIC
    src/ProgramBuilder.cpp
    src/JsonEmitter.cpp
    src/Simulator.cpp
    src/ScatterGatherCm.cpp
)

target_include_directories(UpmemCostModel PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

find_package(OpenMP REQUIRED COMPONENTS CXX)
target_link_libraries(UpmemCostModel PRIVATE OpenMP::OpenMP_CXX)
