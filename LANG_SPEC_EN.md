# Harp Script v0.1.1 Language Specification

## Overview

**Harp Script** is a bytecode language with script-like syntax. It supports both static and dynamic typing simultaneously. While appearing as an interpreter language on the surface, future expansion to a compiled language is planned. JIT compilation will not be supported.

> **Note:** Source code is not publicly available on GitHub; it is only available on Codeberg.

---

## 1. Implementation Features

### 1.1 Core Design Principles

- **Reference Counting (RC)**: Memory management for `str` and `strbuf` types
- **Lightweight Implementation**: Avoids complex implementation patterns
- **Minimal GC Overhead**: Currently uses RC only; no global garbage collection
- **Developer Freedom**: Changes are decided at the discretion of the repository maintainer

### 1.2 Code Style Principles

Code style is up to the developer, but the following recommended style is provided:

- Avoid unnecessary style debates.

---

## 2. Program Structure

### 2.1 Entry Point

Programs start with the `main` keyword. Code declared outside of `main` causes a compilation error.

**Rules:**
- `main` is not a function name.
- All code must be written inside the `main` keyword.

**Basic Example:**
```harp
main
  var n = 0
  print(n)
  var msg = 'Hello World'
  print(msg)
;
```

> **Note:** This syntax may change in the future (e.g., when passing arguments to main is needed).

---

## 3. Identifier Rules

- No duplicate identifiers are allowed in the same scope (node).
- Redeclaration of identifiers is forbidden (as of v0.1.1).

**Example:**
```harp
main
  var n = 30
  var m = 500
;
```

---

## 4. Block Structure

### 4.1 Basic Block Syntax

Harp Script uses `:` and `;` to define blocks.

**Rules:**
- `:` can be omitted if not necessary.
- `;` terminates a block.

**Example:**
```harp
main
  var n = 10
  print(n)
;
```

### 4.2 Alternative Block Syntax (do-end)

Developers who prefer not to use `:` and `;` can use `do` and `end`.

```harp
main do
  var n = 10
  print(n)
end
```

> **Note:** `do-end` may be considered redundant, but developer choice is respected.

---

## 5. Function Declaration

### 5.1 Basic Function Structure

```harp
func sum(a, b)
  return a + b
;
```

### 5.2 Function Features

- **Type Checking**: Function type checking is supported.
- **Function Overloading**: Not currently supported (due to complex implementation).
  - May be supported in the future if a simple implementation method is devised.

### 5.3 Type Annotation (Not Yet Implemented)

The following syntax is expected to be supported in the future:

```harp
func int sum(a int, b int)
  return a + b
;
```

### 5.4 Return Statement

`return` can be abbreviated as `ret` (decided at the discretion of the repository maintainer).

```harp
func getValue()
  ret 42
;
```

---

## 6. Control Statements

### 6.1 Loop Control

- **redo**: Restart the current iteration.
- **break**: Exit the loop.

### 6.2 Labels

Labels are currently under testing and are declared with `@`.

**Expected Syntax (Subject to Change):**
```harp
@hahabreak
while i < 100
  i += 1
  if i == 30
    break_to @hahabreak
  ;
;
```

---

## 7. Macros

Macros start with `#`.

### 7.1 Current Usage

- **#ver**: Version specification
  ```harp
  #ver 0.1 -- Only usable in version 0.1
  ```

---

## 8. Modules

### 8.1 include Keyword

The `import` keyword is not currently supported.
Modules are loaded relative to the directory of the file.

### 8.2 Module Characteristics

- Module analysis is performed **recursively**.
- **Circular references are not allowed**.
- Using `as` is **mandatory**.

### 8.3 Usage Examples

```harp
include 'a.harp' as A
include 'ex/a.harp' as EX

main
  A.someFunction()
  EX.anotherFunction()
;
```

---

## 9. Comments

Harp Script supports two comment formats:

- **Single-line Comment**: `--`
  ```harp
  -- This is a single-line comment
  ```

- **Multi-line Comment**: `--[ ]--`
  ```harp
  --[ This is a
      multi-line
      comment ]--
  ```

---

## 10. Variable Declaration

### 10.1 Basic Variable Declaration

```harp
var n = 30, m = 50
```

### 10.2 Type Annotation (Experimental)

```harp
var n int = 300, m num = 500.0
```

### 10.3 Type Checking Characteristics

- **Difference from Python**: Type mismatch causes a compilation error.
- **Difference from Other Languages**: Type checking provides meaningful performance improvements at runtime:
  - Minimized dynamic typing checks
  - Minimized reference counting checks

> **Current Issue**: Ambiguity exists when type annotation is omitted.

---

## 11. Type System

### 11.1 Supported Types (v0.1.1)

| Type | Description | Memory Management |
|------|-------------|------------------|
| `void` (or `none`) | None | Stack |
| `bool` | Boolean (`true`, `false`) | Stack |
| `char` | Unicode character (~3 bytes) | Stack |
| `int` | Integer (i48: up to 48-byte range) | Stack |
| `num` | Floating-point (equivalent to f64) | Stack |
| `str` | String (immutable) | Heap (RC) |
| `strbuf` | Mutable string (experimental) | Heap (RC) |

> **Note:** `int` currently supports i48 (48-byte range), but may be limited to 32-byte range in the future.

### 11.2 Void Type Special Case

```harp
var n = none  -- void type
```

The following is **not allowed**:
```harp
var n void = none  -- Compilation error
```

**Reason**: A void variable has no meaning and is logically incorrect.

### 11.3 Mutability

Current support status:
- Only `immut` types are supported.
- `mut` is not supported except for `strbuf`.

### 11.4 Future Type System Plans

- **NaN Boxing**: Possible extension in v0.2 (subject to change)

---

## 12. Operators

### 12.1 Assignment Operators

Only frequently used assignment operators are supported:

| Operator | Description |
|----------|-------------|
| `=` | Basic assignment |
| `+=` | Addition assignment |
| `-=` | Subtraction assignment |
| `*=` | Multiplication assignment |

**Currently Not Supported**: `/=`, `%=`

> **Performance Tip:** `n = n + 500` and `n += 500` have performance differences at compile time. Use `+=` is recommended.

### 12.2 Operator Precedence (High → Low)

1. Unary `-`, `!`
2. `**` (Exponentiation)
3. `*`, `/`, `%`
4. `+`, `-`
5. `<`, `<=`, `>`, `>=`
6. `<=>`
7. `==`, `!=`
8. `and` (or `&&`)
9. `or` (or `||`)

### 12.3 Arithmetic Operator Type Rules

Supported operations: `+`, `-`, `*`, `/`, `%`, `**`

| Left | Right | Result |
|-----|-------|--------|
| `num` | `num` | `num` |
| `int` | `num` | `num` |
| `num` | `int` | `num` |
| `int` | `int` | `int` |

**On Operation Failure**: Returns `void` (none) or `error` (subject to change)

### 12.4 Comparison Operators

| Operator | Return Value | Description |
|----------|--------------|-------------|
| `<`, `<=`, `>`, `>=` | `bool` or `none` | Returns `none` if comparison is not possible |
| `==`, `!=` | `bool` or `none` | Returns `none` if comparison is not possible |
| `<=>` | `int` | Spaceship operator |

### 12.5 Logical Operators

| Operator | Alternative | Return Value |
|----------|-------------|--------------|
| `and` | `&&` | `bool` or `none` |
| `or` | `\|\|` | `bool` or `none` |

### 12.6 Bitwise Operators (Not Yet Implemented, Planned for Future)

| Operator | Description | Applicable Types | Return Value |
|----------|-------------|------------------|--------------|
| `&` | Bitwise AND | `int`, `bool` | `int` (expected) |
| `\|` | Bitwise OR | `int`, `bool` | `int` (expected) |
| `^` | Bitwise XOR | `int`, `bool` | `int` (expected) |
| `~` | Bitwise NOT | `int`, `bool` | `int` (expected) |

---

## 13. Optimization

### 13.1 Currently Supported Optimization

Harp Script supports limited optimization:

- **Constant Folding (-O1)**: Successfully implemented; additional optimizations planned based on implementation capacity.

**Example:**
```harp
main
  n = 10 + 400
  -- After -O1
  -- n = 410
;
```

### 13.2 Experimental Optimization

Removal of unnecessary identifiers and statements is under experimentation.

**Example:**
```harp
main
  m = 100
  n = 400
  print(100)
  -- m, n are unnecessary; removed
;
```

---

## 14. Additional Notes

### 14.1 Current Limitations

- Function overloading not supported
- Bitwise operations not implemented
- Labels and break_to syntax under testing
- `/=`, `%=` operators not supported
- `mut` types (except `strbuf`) not supported

### 14.2 Future Plans

- Function overloading (if a simple implementation method is devised)
- Expansion to compiled language
- Type system expansion (NaN Boxing, etc.)
- Bitwise operation implementation

---

## 15. Examples

### 15.1 Simple Program

```harp
main
  var greeting = 'Hello, Harp Script!'
  print(greeting)
;
```

### 15.2 Functions and Variables

```harp
func multiply(a, b)
  return a * b
;

main
  var result = multiply(5, 10)
  print(result)
;
```

### 15.3 Control Statements

```harp
main
  var i = 0
  while i < 10
    print(i)
    i += 1
  ;
;
```

### 15.4 Using Modules

```harp
include 'utils.harp' as Utils

main
  Utils.doSomething()
;
```

---

## 16. License

- **Up to v0.1.0**: Apache License 2.0
- **v0.1.1 and Later**: HarpScript License (to be explained)

---

**Document Version**: v0.1.1  
**Last Updated**: October 2026
