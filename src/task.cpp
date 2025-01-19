#include "task.h"
#include "quadratic_equation.h"

#include <iostream>
#include <string_view>

using namespace std;

namespace
{
    // Check if a string represents a valid integer
    bool isInteger(string_view str)
    {
        if (str.empty())
            return false;

        size_t start = 0;
        if (str.front() == '-' || str.front() == '+')
        {
            if (str.length() == 1)
                return false;
            start = 1;
        }

        return str.find_first_not_of("0123456789", start) == string_view::npos;
    }
}

// Save current output precision
QuadraticEquationsTask::QuadraticEquationsTask() : prev_precision(cout.precision())
{
    // Set increased output precision
    cout.precision(PRECISION);
}

QuadraticEquationsTask::~QuadraticEquationsTask()
{
    // Restore output precision
    cout.precision(prev_precision);
}

void QuadraticEquationsTask::run(int argc, char *argv[])
{
    // Convert command line arguments to a vector of coefficients treys
    CoefficientTreys treys = parseArguments(argc, argv);

    // Solve the equations
    for (auto [a, b, c] : treys)
    {
        try
        {
            QuadraticEquation eq(a, b, c);
            eq.solve();
        }
        catch (const invalid_argument &e)
        {
            cerr << "Error: " << e.what() << "\n";
            continue;
        }
        catch (const exception &e)
        {
            cerr << "Unexpected error: " << e.what() << "\n";
            continue;
        }
    }
}

QuadraticEquationsTask::CoefficientTreys QuadraticEquationsTask::parseArguments(int argc, char *argv[])
{
    CoefficientTreys treys;

    for (int i = 1; i < argc; i += COEFFICIENTS_COUNT)
    {
        if (i + COEFFICIENTS_COUNT > argc)
        {
            cerr << "Warning: trailing coefficient(s) ignored\n";
            break;
        }

        bool bad_input = false;
        for (int j = 0; j < COEFFICIENTS_COUNT; ++j)
        {
            if (!isInteger(argv[i + j]))
            {
                cerr << "Warning: skipping invalid input for equation #" << i / COEFFICIENTS_COUNT << ": '" << argv[i + j] << "'\n";
                bad_input = true;
                break;
            }
        }
        if (bad_input)
            continue;

        // Although we expect integer coefficients, all calculations are performed with floating point
        treys.push_back({stod(argv[i]), stod(argv[i + 1]), stod(argv[i + 2])});
    }

    return treys;
}
