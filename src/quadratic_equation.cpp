#include "quadratic_equation.h"

#include <cmath>
#include <iostream>

using namespace std;

QuadraticEquation::QuadraticEquation(double a_, double b_, double c_) : a(a_), b(b_), c(c_)
{
}

QuadraticEquation::Result QuadraticEquation::solve()
{
    Result result;
    if (a == 0 && b == 0 && c == 0)
        // 0x^2 + 0x + 0 = 0 yields an infinite number or roots
        result.infinite = true;
    else if (a == 0)
        // bx + c = 0
        result = solveLinear(b, c);
    else
        // ax^2 + bx + c = 0
        result = solveQuadratic(a, b, c);
    return result;
}

QuadraticEquation::Result QuadraticEquation::solveQuadratic(double a, double b, double c) const
{
    if (a == 0)
        throw std::invalid_argument("Coefficient 'a' must be non-zero");

    // Extremum always exists for quadratic functions
    auto result = Result{
        .extremum_x = -static_cast<double>(b) / (2 * a),
    };

    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) // No roots
        return result;

    if (almostZero(discriminant)) // Single root
    {
        result.roots.push_back(-b / (2 * a));
        return result;
    }

    // (discriminant > 0) Two roots
    discriminant = sqrt(discriminant);
    double denominator = 2 * a;
    result.roots.reserve(2);
    result.roots.push_back((-b - discriminant) / denominator);
    result.roots.push_back((-b + discriminant) / denominator);

    return result;
}

QuadraticEquation::Result QuadraticEquation::solveLinear(double b, double c) const
{
    if (b == 0) // No roots
        return {};

    // Single root
    return {.roots = {-c / b}};
}

// Helper method to check if a floating point number effectively equals zero
bool QuadraticEquation::almostZero(double value) const
{
    constexpr double EPSILON = 1e-10;
    return std::abs(value) < EPSILON;
}

ostream &operator<<(ostream &os, const QuadraticEquation::Result &result)
{
    if (result.infinite)
        cout << "(inf)";
    else if (result.roots.empty())
        os << "no roots";
    else
    {
        os << '(';
        bool put_delimeter = false;
        for (double root : result.roots)
        {
            if (put_delimeter)
                os << ", ";
            else
                put_delimeter = true;
            os << root;
        }
        os << ')';
    }

    if (result.extremum_x.has_value())
        cout << " Xmin " << result.extremum_x.value();

    return os;
}
