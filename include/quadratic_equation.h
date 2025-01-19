#pragma once

#include <optional>
#include <ostream>
#include <vector>

class QuadraticEquation
{
public:
    struct Result
    {
        // The equation has infinite number of roots
        bool infinite{};
        // Real roots of the equation
        std::vector<double> roots;
        // X-coordinate of the extremum (does not exist for linear functions)
        std::optional<double> extremum_x;
        // Sequence number for consistently arranged output
        size_t number;
        // Provides sorting by number
        bool operator>(const Result &r) const { return number > r.number; }
    };

    QuadraticEquation(double a, double b, double c);

    // Solve the equation (find its roots) and find X-coordinate of the extremum
    Result solve();

private:
    // ax^2 + bx + c = 0 (`a` must be non-zero)
    Result solveQuadratic(double a, double b, double c) const;

    // bx + c = 0
    Result solveLinear(double b, double c) const;

    // Helper method to check if a floating point number effectively equals zero
    bool almostZero(double value) const;

    friend std::ostream &operator<<(std::ostream &os, const Result &result);

private:
    // Coefficients of ax² + bx + c
    double a, b, c;
};
