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

# ScatterGatherCm.cpp's scatterSgCostMs builds an mlpack::FFN internally;
# PRIVATE since that's confined to the .cpp -- ScatterGatherCm.h's
# declarations don't expose any mlpack types to consumers.
target_link_libraries(UpmemCostModel PRIVATE mlpack::mlpack)

find_package(OpenMP REQUIRED COMPONENTS CXX)
target_link_libraries(UpmemCostModel PRIVATE OpenMP::OpenMP_CXX)
