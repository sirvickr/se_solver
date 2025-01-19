#include "quadratic_equation.h"

#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        cerr << "Usage: " << argv[0] << " <space-separated list of integers>\n";
        return 1;
    }

    double a = stod(argv[1]);
    double b = stod(argv[2]);
    double c = stod(argv[3]);

    try
    {
        QuadraticEquation equation(a, b, c);
        equation.solve();
    }
    catch (const exception &e)
    {
        cerr << "Unexpected error: " << e.what() << "\n";
    }

    return 0;
}
