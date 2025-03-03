cmake_minimum_required(VERSION 3.15)

project(ODE_Solver VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(CLI11 REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(Problem src/Problem.cpp)
add_executable(ode_solver src/ode_solver.cpp)
target_link_libraries(ode_solver Problem CLI11::CLI11)
