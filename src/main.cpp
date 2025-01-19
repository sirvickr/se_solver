#include "processor.h"
#include "task.h"

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

    // Convert command line arguments to a vector of coefficients treys
    Tasks parseArguments(int argc, char *argv[])
    {
        Tasks tasks;

        for (int i = 1; i < argc; i += Task::COEFFICIENTS_COUNT)
        {
            if (i + Task::COEFFICIENTS_COUNT > argc)
            {
                cerr << "Warning: trailing coefficient(s) ignored\n";
                break;
            }

            bool bad_input = false;
            for (int j = 0; j < Task::COEFFICIENTS_COUNT; ++j)
            {
                if (!isInteger(argv[i + j]))
                {
                    cerr << "Warning: skipping invalid input for equation #" << i / Task::COEFFICIENTS_COUNT << ": '" << argv[i + j] << "'\n";
                    bad_input = true;
                    break;
                }
            }
            if (bad_input)
                continue;

            // Although we expect integer coefficients, all calculations are performed with floating point
            tasks.push_back(Task{stod(argv[i]), stod(argv[i + 1]), stod(argv[i + 2]), i});
        }

        return tasks;
    }

}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        cerr << "Usage: " << argv[0] << " <space-separated list of integers>\n";
        cerr << "Provide sets of treys (a b c) for equations ax² + bx + c = 0\n";
        cerr << "Example: " << argv[0] << " 1 -2 -3\n";
        return 1;
    }

    try
    {
        QuadraticEquationsProcessor solver;
        solver.run(parseArguments(argc, argv));
    }
    catch (const invalid_argument &e)
    {
        cerr << "Error: " << e.what() << "\n";
    }
    catch (const exception &e)
    {
        cerr << "Unexpected error: " << e.what() << "\n";
    }

    return 0;
}
