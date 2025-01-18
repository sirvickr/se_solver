cmake_minimum_required(VERSION 3.10)
project(SquareEquationSolver VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add library for QuadraticEquation
add_library(quadratic_equation
    src/quadratic_equation.cpp
)

# Set include directories for the library
target_include_directories(quadratic_equation PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Add the executable
add_executable(se_solver src/main.cpp)

# Link the executable with the library
target_link_libraries(se_solver PRIVATE quadratic_equation)
