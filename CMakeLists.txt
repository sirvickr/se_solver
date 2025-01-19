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
add_executable(se_solver src/main.cpp src/task.cpp)

# Link the executable with the library
target_link_libraries(se_solver PRIVATE quadratic_equation)

# Enable testing
enable_testing()

# Add custom target to run Python tests
add_custom_target(run_tests
    COMMAND ${CMAKE_COMMAND} -E echo "Running Python tests..."
    COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.py
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS se_solver
)

# Find Python (optional)
find_package(Python COMPONENTS Interpreter)
if(Python_FOUND)
    message(STATUS "Python found: ${Python_EXECUTABLE}")
    # Add test that runs the Python test suite
    add_test(NAME PythonTests
             COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.py
             WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
else()
    message(WARNING "Python not found - testing will be disabled")
endif()
