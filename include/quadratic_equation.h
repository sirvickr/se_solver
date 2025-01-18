#pragma once

#include <optional>
#include <ostream>
#include <vector>

class QuadraticEquation
{
public:
    QuadraticEquation(int a, int b, int c);

    // Solve the equation (find its roots)
    // and find the axis of symmetry (X-coordinate of the extremum)
    void solve();

private:
    struct Result
    {
        // Real roots of the equation
        std::vector<double> roots;
        // X-coordinate of the extremum (does not exist for linear functions)
        std::optional<double> extremum_x;
    };

    // ax^2 + bx + c = 0 (`a` must be non-zero)
    Result solveQuadratic(double a, double b, double c) const;

    // bx + c = 0
    Result solveLinear(double b, double c) const;

    // Helper method to check if a floating point number effectively equals zero
    bool almostZero(double value) const;

    friend std::ostream &operator<<(std::ostream &os, const Result &result);

private:
    // Coefficients of ax² + bx + c
    int a, b, c;
};
