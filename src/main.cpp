#include "task.h"

#include <iostream>

using namespace std;

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
        QuadraticEquationsTask task;
        task.run(argc, argv);
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
