# Arbitrary Precision Calculator (APC)

A high-performance command-line Arbitrary Precision Calculator written in C. This tool performs arithmetic operations on integers of virtually unlimited magnitude without standard integer overflow (`int`, `long long`) by dynamically representing numbers using doubly linked lists.

---

## Features

- **Arbitrary Size Handling**: Calculate numbers with tens, hundreds, or thousands of digits.
- **Core Arithmetic Operations**:
  - **Addition (`+`)**: Column-wise addition with carry propagation.
  - **Subtraction (`-`)**: Digit-by-digit difference with borrow mechanics.
  - **Multiplication (`x`)**: Long multiplication with positional zero-shifting and intermediate accumulation.
  - **Division (`/`)**: Large-integer quotient evaluation via repeated subtraction and magnitude comparison.
- **Sign Evaluation**: Complete support for positive and negative integers across all operations.
- **Leading Zero Normalization**: Automatically trims redundant leading zeros (e.g., `007` -> `7`, `0000` -> `0`).
- **Terminal UI**: Styled terminal interface with ANSI color frames, input validation, and clear error reporting.

---

## Data Structure & Design

The core data structure is a bidirectional doubly linked list (`Dlist`):

## File Structure

├── main.c              # Application entry point, argument parsing, sign handling, and UI
├── main.h              # Struct definitions, ANSI macros, and function prototypes
├── list_functions.c    # Doubly linked list operations, parsing, zero removal, and comparison
├── list_operations.c   # Core arithmetic engines (add, subtract, multiply, divide)
├── Makefile            # Build configuration
└── README.md           # Project documentation

Getting Started

## Prerequisites
GCC compiler (gcc) & GNU Make (make)

## Building the Project
Compile the project using the provided Makefile: make

To clean the compiled binary: make clean

## Usage
Run the compiled executable passing three positional arguments: ./calc.exe <number1> <operator> <number2>

## Supported Operators

+ : Addition
- : Subtraction
x : Multiplication (use x, not *, to avoid shell wildcard expansion)
/ : Integer Division

## Error Handling

1. Invalid Number of Arguments: Enforces exact <operand1> <operator> <operand2> syntax.
2. Invalid Number Strings: Rejects non-digit characters and bare sign tokens (+, -).
3. Unsupported Operators: Restricts operator choices strictly to +, -, x, /.
4. Division by Zero: Detects zero divisors and halts execution safely without crashes.



