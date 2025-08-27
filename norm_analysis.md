
# 42 Norm Rules Analysis

This document summarizes the key rules from the 42 Norm (Version 4.1) that are relevant to the implementation of the Philosophers project.

## I. General Rules & Philosophy

*   **Simplicity and Clarity:** Code should be simple, clear, and easy to understand. Each piece of code should have a unique, clearly understood task.
*   **No Global Variables (with exceptions):** Global variables that are not `const` or `static` are forbidden unless explicitly allowed by the project. (Philosophers project often requires shared memory, so this might be an exception or require careful handling with mutexes).
*   **No Obfuscation:** Avoid any code obfuscation techniques, such as one-liners or macros designed to bypass the norm.

## II. Naming Conventions

*   **Snake Case:** Identifiers (variables, functions, files, directories) must use `snake_case` (all lowercase, words separated by underscores). No capital letters.
*   **Prefixes for Structures:**
    *   `s_` for structures (e.g., `s_philosopher`)
    *   `t_` for typedefs (e.g., `t_philosopher`)
    *   `u_` for unions
    *   `e_` for enums
    *   `g_` for globals (if allowed)
*   **Explicit Names:** All identifiers should be explicit, mnemonic, and readable in English.

## III. Formatting

*   **Function Length:** Each function must be at most 25 lines long (excluding function braces).
*   **Line Width:** Each line must be at most 80 columns wide (including comments).
*   **Indentation:** Use 4-char-long tabulations (ASCII char 9) for indentation. Blocks within braces must be indented.
*   **Empty Lines:** Functions must be separated by an empty line. No two consecutive empty lines. An empty line must be truly empty (no spaces/tabs).
*   **Spaces:** Each operator/operand must be separated by exactly one space. Each comma/semicolon must be followed by a space (unless it's the end of a line). Each C keyword (except types and `sizeof`) must be followed by a space.
*   **Declarations:** Declarations must be at the beginning of a function. One single variable declaration per line. Declaration and initialization cannot be on the same line (except for globals, statics, and constants). An empty line must separate variable declarations from the rest of the function.
*   **Control Structures:** Only one instruction or control structure per line. Control structures (`if`, `while`, etc.) must use braces, unless they contain a single instruction on a single line.

## IV. Functions

*   **Parameter Count:** A function can take at most 4 named parameters.
*   **`void` for No Arguments:** Functions that don't take arguments must be prototyped with `void` (e.g., `int func(void)`).
*   **Named Parameters:** Parameters in function prototypes must be named.
*   **Variable Count:** You cannot declare more than 5 variables per function.
*   **Return Parentheses:** Return of a function must be between parentheses (e.g., `return (val);`), unless the function returns nothing.

## V. Headers

*   **Allowed Elements:** Header files can contain header inclusions, declarations, defines, prototypes, and macros.
*   **Includes at Beginning:** All includes must be at the beginning of the file.
*   **No C File Includes:** You cannot include a `.c` file in a header or another `.c` file.
*   **Include Guards:** Header files must be protected from double inclusions using include guards (e.g., `#ifndef FT_FOO_H`, `#define FT_FOO_H`, `#endif`).
*   **No Unused Headers:** Inclusion of unused headers is forbidden.
*   **42 Header:** Every `.c` and `.h` file must begin with the standard 42 header (multi-line comment with specific information like creator, creation date, last update).

## VI. Macros and Pre-processors

*   **Literal/Constant Values:** Preprocessor constants (`#define`) must be used only for literal and constant values.
*   **No Multiline Macros:** Multiline macros are forbidden.
*   **Uppercase Names:** Macro names must be all uppercase.

## VII. Forbidden Stuff!

*   **Forbidden Keywords/Operators:**
    *   `for`
    *   `do...while`
    *   `switch`
    *   `case`
    *   `goto`
    *   Ternary operators (`?`)
*   **VLAs:** Variable Length Arrays are forbidden.
*   **Implicit Types:** Implicit types in variable declarations are forbidden.

## VIII. Comments

*   **Placement:** Comments cannot be inside function bodies. They must be at the end of a line or on their own line.
*   **Language & Usefulness:** Comments should be in English and useful. They cannot justify bad or 

