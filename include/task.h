#pragma once

#include <array>
#include <ios>
#include <vector>

class QuadraticEquationsTask
{
public:
    QuadraticEquationsTask();
    ~QuadraticEquationsTask();
    // Solve the equations and print the results
    void run(int argc, char *argv[]);

private:
    // Output precision
    static constexpr std::streamsize PRECISION = 15;
    const std::streamsize prev_precision;

    // {a, b, c} treys of coefficients for ax² + bx + c = 0 equations
    static constexpr int COEFFICIENTS_COUNT = 3;
    using Coefficients = std::array<double, COEFFICIENTS_COUNT>;
    using CoefficientTreys = std::vector<Coefficients>;

    // Convert command line arguments to a vector of coefficients treys
    CoefficientTreys parseArguments(int argc, char *argv[]);
};
