# Quadratic Equation Solver

This project implements a command-line tool for solving quadratic ("square") equations. It can process multiple equations in a single run, providing both roots and the axis of the parabola's symmetry (X-coordinate of the extremum point) for each equation.

## Features

The solver provides analysis of quadratic equations, including:

* Finding and displaying all roots (if any)
* Calculating axis of symmetry (regardless whether the parabola has minimal or maximal extremum)
* Processing multiple equations in a single run
* Handling invalid input gracefully
* Maintaining high numerical precision

## Mathematical Background

For each quadratic equation in the standard form ax² + bx + c = 0, the program:

1. Calculates roots using the quadratic formula: x = (-b ± √(b² - 4ac)) / (2a)
2. Finds X-coordinate of the the extremum point using calculus: x = -b/(2a)

## Building the Project

The project uses `CMake` as its build system. To build:

```bash
mkdir build
cd build
cmake ..
make
```

This will create the executable `se_solver` in the build directory.

## Usage

Run the solver by providing sets of three integers representing the coefficients `a`, `b` and `c`:

```bash
./se_solver 1 -5 6 1 0 -4 2 -4 2 0 4 -4 -1 1 -1 0 0 0
```

This command will solve six equations:

1. x² - 5x + 6 = 0
2. x² - 4 = 0
3. 2x² - 4x + 2 = 0
4. 4x - 4 = 0
5. -x² + x - 1 = 0
6. 0x² - 0x + 0 = 0

It will print the results as follows:

```bash
(2, 3) Xmin 2.5
(-2, 2) Xmin -0
(1) Xmin 1
(1)
no roots Xmin 0.5
(inf)
```

Comma-separated roots are enclosed within parenthesis and the X-coordinate of the the extremum point (if any) follows after the `Xmin` label.

All quadratic equations have an extremum point and the corresponding `Xmin` value printed. Equations `1` and `2` are quadratic equations with two roots. Equation `3` has a single root.

A linear equation is a special case of a quadratic equation when the coefficient at x² (`a`) is zero. Therefore, it has a single root when the coefficient at x (`b`) is nonzero and no roots otherwise. Equation `4` is an linear equation with a single root. Quadratic equation `5` doesn't have roots (i.e. its parabola doesn't intersect Ox). And the case when all coefficients are equal to zero is a degenerate case of the equation, resulting in an infinite number of solutions - just like in the case of the equation `6`.

The program processes coefficients in groups of three. Any incomplete groups at the end are discarded with a warning:

```bash
./se_solver 1 -5 6 1
Warning: trailing coefficient(s) ignored
(2, 3) Xmin 2.5
```

## Project Structure

The solution follows a modular structure of folders:

```
se_solver/
├── CMakeLists.txt          # Build configuration
├── include/
│   ├── async_queue.h          # Asynchronous tasks queue template
│   ├── task.h                 # Solver task declaration
│   ├── processor.h            # Asynchronous tasks processor declaration
│   ├── result_manager.h       # Sequential results rearrangement manager declaration
│   └── quadratic_equation.h   # Solver class declaration
├── src/
│   ├── main.cpp               # Command-line interface
│   ├── processor.cpp          # Asynchronous tasks processor implementation
│   ├── result_manager.cpp     # Sequential results rearrangement manager implementation
│   └── quadratic_equation.cpp # Solver class implementation
└── tests/
    └── test.py # Tests
```

## Testing

The project includes end-to-end tests written in Python. To run the tests, `cd` to the build directory and run the one of the following commands:

```bash
# Either run CTest
ctest --output-on-failure
```

```bash
# Or use the custom target
make run_tests
```

## Technical Limitations

The solver operates within certain technical constraints to maintain reliability:

1. Numerical Precision
   * Uses double-precision floating-point arithmetic
   * Output precision is set to 15 decimal places
   * Coefficient values should stay within ±10,000,000,000 for reliable results
   * Beyond these limits, numerical precision may degrade

2. Input Validation
   * Input must be valid integers
   * Non-numeric input is discarded with a warning:

```bash
./se_solver 1 -5 6 1 aaa -3 2 -4 2
Warning: skipping invalid input for equation #1: 'aaa'
(2, 3) Xmin 2.5
(1) Xmin 1
```
