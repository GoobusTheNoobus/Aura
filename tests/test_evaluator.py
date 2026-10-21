import random
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
AURA = ROOT / "build-debug" / "aura"

RED = "\033[31m"
GREEN = "\033[32m"
RESET = "\033[0m"

# Test Case: (category, test_name, input, expected_output)
TESTS = [
    # Literals
    ("Literals", "Integer literal", "42", "42"),
    ("Literals", "Zero", "0", "0"),
    ("Literals", "Negative integer", "-42", "-42"),
    ("Literals", "Float literal", "3.14", "3.140000"),
    ("Literals", "Boolean true", "true", "true"),
    ("Literals", "Boolean false", "false", "false"),

    # Arithmetic
    ("Arithmetic", "Addition", "2 + 3", "5"),
    ("Arithmetic", "Subtraction", "9 - 4", "5"),
    ("Arithmetic", "Multiplication", "6 * 7", "42"),
    ("Arithmetic", "Division", "8 / 2", "4.000000"),
    ("Arithmetic", "Fractional division", "5 / 4", "1.250000"),
    ("Arithmetic", "Modulo", "17 % 5", "2"),
    ("Arithmetic", "Unary plus", "+5", "5"),
    ("Arithmetic", "Unary minus", "-5", "-5"),
    ("Arithmetic", "Repeated unary operators", "+-+-5", "5"),
    ("Arithmetic", "Multiplication precedence", "2 + 3 * 4", "14"),
    ("Arithmetic", "Parentheses", "(2 + 3) * 4", "20"),
    ("Arithmetic", "Nested parentheses", "((2 + 3) * 4)", "20"),
    ("Arithmetic", "Left associativity", "20 / 5 / 2", "2.000000"),
    ("Arithmetic", "Mixed arithmetic",
     "4 / 3 + (5.5 + 12 % 3) - 3.5", "3.333333"),
    ("Arithmetic", "Unary minus preserves integer", "-1", "-1"),
    ("Arithmetic", "Unary minus on float", "-1.5", "-1.500000"),
    ("Arithmetic", "Negative multiplication", "-3 * 4", "-12"),
    ("Arithmetic", "Negative division", "-8 / 2", "-4.000000"),

    # Comparisons
    ("Comparisons", "Equal integers", "3 == 3", "true"),
    ("Comparisons", "Unequal integers", "3 == 4", "false"),
    ("Comparisons", "Not equal", "3 != 4", "true"),
    ("Comparisons", "Greater than true", "8 > 4", "true"),
    ("Comparisons", "Greater than false", "3 > 4", "false"),
    ("Comparisons", "Less than", "2 < 5", "true"),
    ("Comparisons", "Less than or equal true", "5 <= 5", "true"),
    ("Comparisons", "Less than or equal false", "6 <= 5", "false"),
    ("Comparisons", "Greater than or equal", "5 >= 5", "true"),
    ("Comparisons", "Arithmetic before comparison", "2 + 3 > 4", "true"),
    ("Comparisons", "Comparison with parentheses", "(2 + 3) == 5", "true"),

    # Logic
    ("Logic", "NOT true", "!true", "false"),
    ("Logic", "NOT false", "!false", "true"),
    ("Logic", "AND true", "true && true", "true"),
    ("Logic", "AND false", "true && false", "false"),
    ("Logic", "OR true", "true || false", "true"),
    ("Logic", "OR false", "false || false", "false"),
    ("Logic", "Repeated NOT", "!!true", "true"),
    ("Logic", "NOT comparison", "!(1 > 2)", "true"),
    ("Logic", "Comparison and AND", "3 > 2 && 5 > 4", "true"),
    ("Logic", "Comparison and OR", "3 < 2 || 5 > 4", "true"),
    ("Logic", "AND before OR", "true || false && false", "true"),
    ("Logic", "Parenthesized logic", "(true || false) && false", "false"),
    ("Logic", "Complex logical expression",
     "1 + 1 == 2 && 3 + 4 == 8 || 3 * 4 == 12", "true"),

    # Strings
    ("Strings", "Empty string", '""', ""),
    ("Strings", "Simple string", '"hello"', "hello"),
    ("Strings", "String concatenation",
     '"hello" + " world"', "hello world"),
    ("Strings", "Concatenation with integer", '1 + "Hi"', "1Hi"),
    ("Strings", "Concatenation with empty string", '1 + ""', "1"),
    ("Strings", "Single-quoted string", "'hello'", "hello"),
    ("Strings", "Escaped newline", '"Hello\\nworld!"', "Hello\nworld!"),
    ("Strings", "Escaped tab", '"Hello\\tworld!"', "Hello\tworld!"),
    ("Strings", "Escaped double quote",
     '"She said \\"hello\\""', 'She said "hello"'),
    ("Strings", "Escaped backslash", '"C:\\\\Aura"', "C:\\Aura"),
    ("Strings", "Concatenation precedence", '"a" + "b" + "c"', "abc"),
    ("Strings", "Parenthesized concatenation",
     '("hello" + " ") + "world"', "hello world"),

    # Sequences
    ("Sequences", "Semicolon separator", "1;2", "1\n2"),
    ("Sequences", "Three expressions", "1;2;3", "1\n2\n3"),
    ("Sequences", "Independent expressions",
     "1 + 2; 3 * 4; 5 - 1", "3\n12\n4"),
    ("Sequences", "Complex expression sequence",
     '1 + 2 * 3; !(4 > 5); "a" + "b"', "7\ntrue\nab"),

    # Declarations
    ("Declarations", "Variable declaration", "let num = 3", "3"),
    ("Declarations", "Expression declaration",
     "let num = 4 / 3 + (5.5 + 12 % 3) - 3.5", "3.333333"),
    ("Declarations", "Variable usage",
     'let str = "Hello, "; str + "world!"',
     "Hello, \nHello, world!"),
    ("Declarations", "Boolean variable",
     "let yes = true || false; !yes", "true\nfalse"),

    # Assignments
    ("Assignments", "Normal assignment",
     "let num = 3; num = 12", "3\n12"),
    ("Assignments", "Different type assignment",
     "let num = 3; num = 'Hello, world!'", "3\nHello, world!"),
    ("Assignments", "Chain assignments",
     "let a = 0; let b = 0; a = b = 12", "0\n0\n12"),

    # Printing
    ("Printing", "Printing", "print 'Hello world!'", "Hello world!"),
    ("Printing", "Println", "println 'Hello world!'", "Hello world!"),
    ("Printing", "Chained printing",
     "println 'Number: ', 2", "Number: 2"),
    ("Printing", "Nested printing",
     "println println 'Number: ', 2", "Number: 2\n"),
    ("Printing", "More nested printing",
     "println (println 'Number: ', 2), 'wow that was pretty nested!'",
     "Number: 2\nwow that was pretty nested!"),

    # Blocks 
    ("Blocks", "Empty Block", "{}", ""),
    ("Blocks", "Variable Scope", "{let i = 0; println i}", "0"),
    ("Blocks", "Block With Multiple Statements", "{let i = 1; println i; let j = 2; println j}", "1\n2"),
    ("Blocks", "Nested Blocks", "{let i = 1; {let j = 2; println j}; println i}", "2\n1"),
    ("Blocks", "Inner Scope Accessing Outer Variables", "{let i = 42; {println i}}", "42"), 
    ("Blocks", "Inner Scope Accessing Changing Variables", "{let i = 42; {i = 67}; println i}", "67"), 
    ("Blocks", "Variable Shadowing", "let n = 5; {let n = 'String'; println n}; println n", "5\nString\n5"), 
]

# Ragebait messages
# Very effective if you are already mad that the evaluator isn't working
MEAN_FAIL_MESSAGES = [
    "never let bro code again",
    "how do you fail on basic math",
    "my blind deaf mute grandma can do better btw",
    "this is why we can't be friends.",
    "the AST isn't angry, its just disappointed",
    "the parser would like to speak to your manager",
    "bro has one job",
    "ts doesn't even work on my machine, why would it work on yours",
    "segmentation fault: `*brain` <- attempted to dereference a null pointer",
    "segmentation fault: `*intelligence` <- attempted to dereference a null pointer",
    "segmentation fault: `*competence` <- attempted to dereference a null pointer",
    "its not a bug, its a feature my ass",
    "if i am the pope, this test passed.",
    "aura has more aura than you",
    "gcc optimizing your dad leaving you",
    "gcc optimizing you coping (-O3)",
    "gcc optimizes you getting good: FAILED",
    "clang optimizes you getting a girlfriend: exited with code 1",
    "have you tried turning it off and on?",
    "world.execute(me)                  (niche reference)",
    "Your code is an argument against dynamic typing.",
    "Your code is why they have instructions on shampoo bottles",
    "Jesus did not die for ts",
    "The stack overflowed from disappointment",
]


def normalize_output(output: str) -> str:
    # Remove the final newline added by the REPL's output formatting.
    if output.endswith("\n"):
        output = output[:-1]

    return output


def run_test(category: str, name: str, source: str, expected: str) -> bool:
    try:
        result = subprocess.run(
            [str(AURA), "--eval", source],
            capture_output=True,
            text=True,
            cwd=ROOT,
            timeout=5,
        )
    except subprocess.TimeoutExpired:
        print(f"{RED}FAIL{RESET} [{category}] {name}: timed out")
        return False

    actual = normalize_output(result.stdout)

    if result.returncode != 0 or actual != expected:
        print(f"{RED}FAIL{RESET} [{category}] {name}")
        print(f"  Source:   {source!r}")
        print(f"  Expected: {expected!r}")
        print(f"  Actual:   {actual!r}")

        if result.returncode != 0:
            print(f"  Exit code: {result.returncode}")

        if result.stderr:
            print(f"  Stderr: {result.stderr!r}")

        return False

    print(f"{GREEN}PASS{RESET} [{category}] {name}")
    return True


def main() -> int:
    if not AURA.is_file():
        print(f"Error: Aura executable not found at {AURA}")
        print("Build Aura first with: cmake --build build-debug")
        return 2

    passed = 0
    failed = 0

    for category, name, source, expected in TESTS:
        if run_test(category, name, source, expected):
            passed += 1
        else:
            failed += 1

    print("\n================================================\n")
    print(f"Results: {passed} passed, {failed} failed, "
          f"{len(TESTS)} total")

    if failed > 0:
        print(f"{RED}Not all tests passed! {random.choice(MEAN_FAIL_MESSAGES)}{RESET}")
    else:
        print(f"{GREEN}All tests passed! {RESET}")

    return 1 if failed else 0


if __name__ == "__main__":
    raise SystemExit(main())
