#pragma once

#include <vector>

struct Task
{
    static constexpr int COEFFICIENTS_COUNT = 3;
    // {a, b, c} treys of coefficients for ax² + bx + c = 0 equations
    double a, b, c;
    // sequential number of the task
    int number;
};

using Tasks = std::vector<Task>;
