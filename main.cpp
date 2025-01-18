#include <cmath>
#include <iostream>
#include <optional>
#include <vector>

using namespace std;

enum class ParabolaOrientation
{
    Up,
    Down,
};

struct Extremum
{
    double x;
    ParabolaOrientation orientation;
};

struct Result
{
    // Real roots of the equation
    vector<double> roots;
    // X-coordinate of the extremum (does not exist for linear functions)
    optional<Extremum> extremum{};
};

ostream &operator<<(ostream &os, const Result &result)
{
    if (result.roots.empty())
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

    if (result.extremum.has_value())
    {
        auto &extremum = result.extremum.value();
        cout << " X";
        if (extremum.orientation == ParabolaOrientation::Up)
            cout << "min";
        else
            cout << "max";
        cout << " " << extremum.x;
    }

    return os;
}

Result solveLinearEquation(double b, double c)
{
    if (b == 0)
        return {};
    // Single real root
    return {.roots = {-c / b}};
}

Result solveQuadraticEquation(double a, double b, double c)
{
    // a is guaranteed to be non-zero

    // Extremum always exists for quadratic functions
    auto result = Result{.extremum = Extremum{
                             .x = -static_cast<double>(b) / (2 * a),
                             .orientation = a > 0 ? ParabolaOrientation::Up : ParabolaOrientation::Down}};

    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) // No roots
        return result;

    if (discriminant == 0) // Single real root
    {
        result.roots.push_back(-b / (2 * a));
        return result;
    }

    // (discriminant > 0) Two real roots
    discriminant = sqrt(discriminant);
    double denominator = 2 * a;
    result.roots = {(-b - discriminant) / denominator, (-b + discriminant) / denominator};

    return result;
}

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

    if (a == 0 && b == 0 && c == 0) // infinite roots
    {
        cout << "error: infinite roots (all parameters equal zero)\n";
        return 0;
    }
    else if (a == 0) // bx + c = 0
    {
        auto result = solveLinearEquation(b, c);
        cout << result << '\n';
        return 0;
    }
    else // ax^2 + bx + c = 0
    {
        auto result = solveQuadraticEquation(a, b, c);
        cout << result << '\n';
    }

    return 0;
}
