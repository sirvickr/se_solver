import unittest
import subprocess
import re
from typing import List, Tuple, Optional
from dataclasses import dataclass


@dataclass
class QuadraticResult:
    """
    Class to store the parsed results of a quadratic/linear equation solution.

    Attributes:
        infinite: True if equation has infinite solutions (all coefficients are 0)
        roots: Tuple of roots. For quadratic equations, can be (x1, x2), (x1,) for repeated root,
              or None for no real roots. For linear equations, will be (x1,)
        extremum_x: x-coordinate of the parabola's vertex. None for linear equations
    """

    infinite: bool
    roots: Optional[Tuple[float, ...]]
    extremum_x: Optional[float]


@dataclass
class QuadraticParams:
    """
    Class to store the equation parameters and expected behavior.

    Attributes:
        a: coefficient of x²
        b: coefficient of x
        c: constant term
        infinite: True if equation should have infinite solutions
        is_linear: True if equation is linear (a=0, b≠0)
    """

    a: float
    b: float
    c: float
    infinite: bool
    is_linear: bool = False

    def __post_init__(self):
        # Automatically determine if equation is linear
        self.is_linear = self.a == 0 and self.b != 0


class QuadraticSolverTests(unittest.TestCase):
    def setUp(self):
        # Path to the compiled binary
        self.binary_path = "./se_solver"
        # Common test cases
        self.cases = [
            # x² - 5x + 6 = 0: (2, 3) Xmin 2.5
            QuadraticParams(a=1, b=-5, c=6, infinite=False),
            # x² - 4 = 0: (-2, 2) Xmin -0
            QuadraticParams(a=1, b=0, c=-4, infinite=False),
            # 2x² - 4x + 2 = 0: (1) Xmin 1
            QuadraticParams(a=2, b=-4, c=2, infinite=False),
            # -x² - 2x - 3 = 0: no roots Xmin -1
            QuadraticParams(a=-1, b=-2, c=-3, infinite=False),
            # 4x - 4 = 0: (1)
            QuadraticParams(a=0, b=4, c=-4, infinite=False),
            # 11x² + 2x = 0: (-0.181818181818182, 0) Xmin -0.0909090909090909
            QuadraticParams(a=11, b=2, c=0, infinite=False),
            # x² - 3x + 2 = 0: (1, 2) Xmin 1.5
            QuadraticParams(a=1, b=-3, c=2, infinite=False),
            # -x + 6 = 0: (6)
            QuadraticParams(a=0, b=-1, c=6, infinite=False),
            # 3x² + 2x - 4 = 0: no roots Xmin 0.333333333333333
            QuadraticParams(a=-3, b=2, c=-4, infinite=False),
            # 0x² - 0x + 0 = 0: (inf)
            QuadraticParams(a=0, b=0, c=0, infinite=True),
            # -x² + x - 1 = 0: no roots Xmin 0.5
            QuadraticParams(a=-1, b=1, c=-1, infinite=False),
        ]

    def parse_output(self, output: str) -> List[QuadraticResult]:
        """Parse the program output and return structured results"""
        results = []
        print(f"output:\n{output}")

        # Pattern for regular roots and Xmin
        pattern = r"\(([-\d\., ]+)\)\s*(?:Xmin\s*([-\d\.]+))?"
        # Special case patterns
        no_roots_pattern = r"no roots\s*Xmin\s*([-\d\.]+)"
        infinite_roots_pattern = r"\(inf\)"

        for line in output.split("\n"):
            if not line.strip():
                continue
            print(f"line: '{line}' len {len(line)}")

            result = QuadraticResult(
                infinite=False,
                roots=None,
                extremum_x=None,  # Initialize as None to handle linear cases
            )

            # Try regular case first
            match = re.match(pattern, line)
            if match:
                print(f"  match: regular")
                roots_str = match.group(1)
                xmin_str = match.group(2)  # May be None for linear equations

                # Handle single root case
                if "," not in roots_str:
                    result.roots = (float(roots_str),)
                else:
                    # Handle two roots case
                    result.roots = tuple(float(x) for x in roots_str.split(","))

                # Set extremum_x only if provided (quadratic case)
                if xmin_str is not None:
                    result.extremum_x = float(xmin_str)

            # Check for no roots case
            match = re.match(no_roots_pattern, line)
            if match:
                print(f"  match: no roots")
                result.extremum_x = float(match.group(1))

            # Check for infinite roots case
            if re.match(infinite_roots_pattern, line):
                print(f"  match: inf")
                result.infinite = True

            print(f"  result: {result}")
            results.append(result)

        return results

    def run_solver(self, *args) -> List[QuadraticResult]:
        """Run the binary with given arguments and return parsed results"""
        try:
            process = subprocess.run(
                [self.binary_path, *map(str, args)],
                capture_output=True,
                text=True,
                check=True,
            )
            return self.parse_output(process.stdout)
        except subprocess.CalledProcessError as e:
            print(f"Error running solver: {e}")
            print(f"Standard error: {e.stderr}")
            return []

    def verify_roots(self, a, b, c, result: QuadraticResult):
        """Verify that the roots are correct for the given equation"""

        self.assertIsNotNone(result.roots)

        for x in result.roots:
            # Verify each root satisfies the equation: ax² + bx + c = 0
            value = a * x**2 + b * x + c
            print(f"x: {x} value: {value}")
            self.assertAlmostEqual(
                value,
                0,
                places=4,
                msg=f"Root {x} does not satisfy equation {a}x² + {b}x + {c} = 0",
            )

    def verify_extremum(self, a, b, result: QuadraticResult):
        """Verify that the extremum is correct for quadratic equations"""
        if a == 0:  # Linear equation - should not have extremum
            self.assertIsNone(
                result.extremum_x, msg="Linear equation should not have an extremum"
            )
            return

        # Verify x-coordinate of extremum: x = -b/(2a)
        expected_x = -b / (2 * a)
        self.assertAlmostEqual(
            result.extremum_x,
            expected_x,
            places=4,
            msg=f"Incorrect extremum x-coordinate",
        )

    def test_single_equations(self):
        """Test basic equations one by one"""

        for case in self.cases:
            results = self.run_solver(case.a, case.b, case.c)
            self.assertEqual(len(results), 1)

            result = results[0]
            self.assertEqual(result.infinite, case.infinite)

            if not case.infinite:
                self.assertFalse(result.infinite)
                if result.roots is not None:
                    self.verify_roots(case.a, case.b, case.c, result)
                if not case.is_linear:
                    self.verify_extremum(case.a, case.b, result)

    def test_multiple_equations(self):
        """Test processing multiple equations in one command"""
        coefficients = []

        for case in self.cases:
            coefficients.extend([case.a, case.b, case.c])

        results = self.run_solver(*coefficients)
        self.assertEqual(len(results), len(self.cases))

        for case, result in zip(self.cases, results):
            self.assertEqual(result.infinite, case.infinite)

            if not case.infinite:
                self.assertFalse(result.infinite)
                if result.roots is not None:
                    self.verify_roots(case.a, case.b, case.c, result)
                if not case.is_linear:
                    self.verify_extremum(case.a, case.b, result)

    def test_large_coefficients(self):
        """Test processing multiple equations in one command with coefficient values far beyond 32-bit integers"""
        K = 10_000_000_000.0
        cases = []
        coefficients = []
        for case in self.cases:
            cases.append(
                QuadraticParams(
                    a=case.a * K,
                    b=case.b * K,
                    c=case.c * K,
                    infinite=case.infinite,
                ),
            )
            coefficients.extend([case.a, case.b, case.c])

        results = self.run_solver(*coefficients)
        self.assertEqual(len(results), len(cases))

        for case, result in zip(cases, results):
            self.assertEqual(result.infinite, case.infinite)

            if not case.infinite:
                self.assertFalse(result.infinite)
                if result.roots is not None:
                    self.verify_roots(case.a, case.b, case.c, result)
                if not case.is_linear:
                    self.verify_extremum(case.a, case.b, result)

    def test_incomplete_sets(self):
        """Test handling of incomplete coefficient sets"""
        process = subprocess.run(
            [self.binary_path, "1", "-5", "6", "2", "-4"],
            capture_output=True,
            text=True,
        )

        # Should process first complete set
        results = self.parse_output(process.stdout)
        self.assertEqual(len(results), 1)

        # Should warn about incomplete set
        self.assertIn("trailing coefficient(s) ignored", process.stderr)

    def test_invalid_input(self):
        """Test handling of incomplete coefficient sets"""
        process = subprocess.run(
            [self.binary_path, "1", "-5", "6", "a", "-4", "-4", "2", "-4", "2"],
            capture_output=True,
            text=True,
        )

        # Should process first complete set
        results = self.parse_output(process.stdout)
        self.assertEqual(len(results), 2)

        # Should warn about incomplete set
        self.assertIn("skipping invalid input", process.stderr)


if __name__ == "__main__":
    unittest.main(verbosity=2)
