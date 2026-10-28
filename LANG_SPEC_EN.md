# Harp Script v0.1.1 Language Specification

**Version**: v0.1.1 (Prototype, planned for release in November)  
**Completion Rate**: 50% or less  
**Last Updated**: October 29, 2026

---

## Table of Contents

1. [Overview](#overview)
2. [Version Numbering](#version-numbering)
3. [Supported Environments](#supported-environments)
4. [Command Line Interface](#command-line-interface)
5. [Type System](#type-system)
6. [Program Structure](#program-structure)
7. [Control Statements](#control-statements)
8. [Operators](#operators)
9. [Functions](#functions)
10. [Built-in Functions](#built-in-functions)
11. [Modules and Comments](#modules-and-comments)
12. [Future Plans](#future-plans)

---

## Overview

**Harp Script** is a bytecode-based scripting language.

### Core Features

- **Type System**: Transitioning from dynamic to explicit static typing
- **Memory Management**: Reference Counting (RC) based, no global GC
- **Implementation Philosophy**: Lightweight implementation, avoid complexity
- **Development Status**: Early prototype stage

---

## Version Numbering

### v0.1.x Series Rules

- **v0.1.1, v0.1.2**: Both must conform to v0.1 specification
- **Compatibility**: If v0.1.1 and v0.1.2 exist, **v0.1.2 is mandatory**
- **v0.2 Compatibility**: Undecided (aiming for compatibility if possible)
- Before v1.0, no guarantee of backward compatibility

### Version Roadmap

| Version | Status | Timeline | Major Changes |
|---------|--------|----------|---------------|
| v0.1.1 | Prototype | November | Current specification |
| v0.1.2 | Planned | TBD | New features to be added |
| v0.2 | Planned | TBD | Major improvements (compatibility undecided) |

---

## Supported Environments

### Architecture

- **Recommended**: Intel, AVX 128 or higher
- **Current Testing Environment**: Windows 11 x64
- **Future Support**: Linux (planned for November)

### Platform Status

| Platform | Status | Notes |
|----------|--------|-------|
| Windows 11 x64 | ✅ Tested | v0.1.1 baseline |
| Linux x64 | 🚧 Planned | Planned for November |

---

## Command Line Interface

### Usage

```bash
harp <command> [options] [file]
```

### Commands

#### Run

```bash
harp run <file>
```
Compiles and executes a Harp Script file.

#### Build (Planned for v0.1.2)

```bash
harp build <file> -o <binary>
```
Compiles Harp Script to binary. (Scheduled for v0.1.2 release)

#### Help

```bash
harp help
harp -h
harp --h
```

#### Version

```bash
harp --v
harp -v
harp ver
harp version
```

### Options

#### `-asm` (Disassembly)

```bash
harp run <file> -asm
```
Outputs disassembly of compiled bytecode.

#### Optimization Options

```bash
harp run <file> -O1  # Partial optimization (constant folding, some statement removal)
harp run <file> -O0  # No optimization
```

| Option | Description |
|--------|-------------|
| `-O0` | No optimization |
| `-O1` | Partial optimization (constant folding, some statement removal) |

---

## Type System

### Currently Supported Types (v0.1.1)

#### Primitive Types

| Type | Description | Memory | Status |
|------|-------------|--------|--------|
| `bool` | Boolean (`true`, `false`) | Stack | ✅ Supported |
| `int` | Integer | Stack | ✅ Supported (may be deprecated in v0.2) |
| `char` | Unicode character | Stack | ⚠️ Partial support (`'a` or `'a'`) |
| `num` | Real number (f64) | Stack | ✅ Supported |
| `str` | String (immutable) | Heap (RC) | ✅ Supported |

#### Type Properties

- **Current**: Only `immut` (immutable) types supported
- **Future**: `mut` (mutable) types to be added

### None and Error

Current handling may change to reduce ambiguity.

#### Invalid (Error)

```
none / error
```
Currently treated as **runtime error** immediately upon detection

#### Valid

All other types

---

## Program Structure

### Program Organization: <Macro> <Head> <Body> <Main>

```harp
-- <Macro>
#ver 0.1

-- <Head>: Module imports
include 'a.harp' as A
include 'b.harp' as B

-- <Body>: Function and identifier declarations
func mysum(a, b)
  ret a + b
;

-- <Main>: main is allowed only at the end
-- main must exist in the top-level module. Compilation error if missing.
main
  print(mysum(10, 20))
;
```

### Entry Point

Programs start with the `main` keyword.

```harp
main
  var n = 0
  print(n)
;
```

### Variable Declaration

#### Basic Form

```harp
var n = 30
var m = 50
```

#### Type Annotation (Currently transitioning to static typing)

```harp
var n int = 300
var m num = 500.0
var b bool = true
var s str = 'hello'
var c char = 'a'
```

### Identifier Rules

- Duplicate identifiers in same scope prohibited
- Redeclaration prohibited

---

## Control Statements

### Conditionals

#### if-elif-else

```harp
if condition
  -- code to execute
;
elif condition2
  -- or this code
;
else
  -- or this code
;
```

### Loops

#### while Loop

```harp
while condition
  -- code to repeat
;
```

### Loop Control

#### redo
Restarts the current iteration.

```harp
while i < 10
  i += 1
  if i == 5
    redo
  ;
;
```

#### break
Exits the loop.

```harp
while i < 100
  if i == 30
    break
  ;
  i += 1
;
```

### Labels (Not Currently Supported)

Labels and `break_to` are not currently supported. (To be added in future)

---

## Operators

### Assignment Operators

| Operator | Description |
|----------|-------------|
| `=` | Basic assignment |
| `+=` | Addition assignment |
| `-=` | Subtraction assignment |
| `*=` | Multiplication assignment |

**Not Supported**: `/=`, `%=`

> **Performance Tip**: Using `+=` is more efficient than `n = n + 500`.

### Arithmetic Operators

Supported: `+`, `-`, `*`, `/`, `%`, `**` (exponentiation)

#### Type Rules

| Left | Right | Result |
|-----|-------|--------|
| `num` | `num` | `num` |
| `int` | `num` | `num` |
| `num` | `int` | `num` |
| `int` | `int` | `int` |

### Comparison Operators

| Operator | Return Type | Description |
|----------|-------------|-------------|
| `<`, `<=`, `>`, `>=` | `bool` | |
| `==`, `!=` | `bool` | |
| `<=>` | `int` | Spaceship operator |

### Logical Operators

| Operator | Alternative | Return Type |
|----------|-------------|-------------|
| `and` | `&&` | `bool` |
| `or` | `\|\|` | `bool` |

### Precedence (High → Low)

1. Unary `-`, `!`
2. `**`
3. `*`, `/`, `%`
4. `+`, `-`
5. `<`, `<=`, `>`, `>=`
6. `<=>`
7. `==`, `!=`
8. `and` (`&&`)
9. `or` (`||`)

### Bitwise Operators (Not Implemented)

Currently not supported. To be added in the future.

---

## Functions

### Function Declaration

```harp
func sum(a, b)
  return a + b
;
```

### Type Annotation (Currently being implemented)

```harp
func int sum(a int, b int)
  return a + b
;
```

### Return Statement

```harp
func getValue()
  return 42
;
```

Can also be written as `ret`:

```harp
func getValue()
  ret 42
;
```

### Function Overloading

**Currently not supported**. May be added in the future if a simple implementation method is devised.

---

## Built-in Functions

### Custom Functions vs Built-in Functions

- **harpfunc**: User-defined functions
- **infunc**: Performance-optimized built-in functions

### Current Basic Built-in Functions

Basic arithmetic operations:
- `floor(num)`: Floor function
- `max(a, b)`: Maximum value
- `min(a, b)`: Minimum value

**Example:**
```harp
floor(15.32717)
=> push [literal index of 15.32717]
=> incall [built-in function number for floor]
```

---

## Modules and Comments

### Modules

#### include Keyword

```harp
include 'a.harp' as A
include 'utils/helper.harp' as Helper

main
  A.someFunction()
  Helper.doSomething()
;
```

**Rules:**
- Recursive module analysis supported
- Circular references not allowed
- `as` keyword mandatory

### Comments

#### Single-line Comment

```harp
-- This is a single-line comment
```

#### Multi-line Comment

```harp
--[ This is a
    multi-line
    comment ]--
```

### Macros

Currently only `#ver` is supported:

```harp
#ver 0.1  -- Only usable in version 0.1
```

---

## Future Plans

### To be Added or Modified in v0.1.2 or Later

#### import Support, node, using
When using strings, `import` has the same functionality as `include`.

```harp
import 'a.harp' as A           -- import 'a' as A also supported if enabled
import dxlib as dx             -- Search globally, then in current project
A.print_something(100) 
dx.putlog('wewek')
```

- For global module access only (subject to change):
```harp
import $something as sm
print(sm.wawa())
```

- For current project only (subject to change):
```harp
import .something as sm
print(sm.wawa())
```

- `node` is equivalent to C++ namespace. `using` keyword can also be added.
- The `using` keyword can only be used inside a node (to prevent name conflicts).

```harp
import dxlib as dx
node funny
  using dx  -- Example only
end
funny.putlog('aksdfmksdmfk')
```

#### Built-in -O1 Optimization
- `-O1` constant folding and unnecessary statement removal
- If `-O0` is specified, no optimization will be performed

#### Recommended Block Syntax Change
- In official documentation, `do` `end` takes precedence over `:` `;` (but old syntax still supported)
- `do` can generally be omitted

#### const or let Keyword
Identifiers not declared with `var` are always treated as constants.

```harp
const i = 300
print(i)
-- i=500
-- Compilation error when attempting to change
```

This allows Harp Script to apply optimizations more easily in the future.

#### loop Keyword
```harp
loop i in 0..10, 1
  print(i, ' ') -- 0 1 2 3 4 5 6 7 8 9
end

loop i in 0...10, 1
  print(i, ' ') -- 0 1 2 3 4 5 6 7 8 9 10
end
```

#### Functions
```harp
-- Subject to change; not recommended
func abc(a, b) 
end

-- Type annotation
func abc int(a int, b int)
  --a = 100 => Error because a is treated as const
  ret a + b
end

-- Partial function overloading support possible
-- Declaring var in arguments allows modification
func abc num(var a num, var b num)
  a+=b
  ret a
end
```

Functions require type annotation. Arguments without `var` are treated as const.
Omit type for functions with no return value.

#### Build Functionality
```bash
harp build <file> -o <binary>
```

#### Dynamic Library Support
- DLL/SO dynamic linking
- Strengthened C language interoperability

```harp
-- sample.dll or .so (Harp Script may recommend extensions)
cfunc('sample') sample_sum int(a int)  -- Parameter names optional, types mandatory

main
  var v = sample_sum(10, 20)
end
```

#### Extended Types
Types for C language interoperability:

```harp
u8, u16, u32, i8, i16, i32, f32, f64, ptr
```

| Type | C Type | Description |
|------|--------|-------------|
| `u8`, `u16`, `u32` | `uint8_t` ~ `uint32_t` | Unsigned integers |
| `i8`, `i16`, `i32` | `int8_t` ~ `int32_t` | Signed integers |
| `f32` | `float` | Single precision |
| `f64` | `double` | Double precision |
| `ptr` | `void*` / `uintptr_t` | Pointer |

#### Alias Feature

```harp
alias mydata = ptr
```

#### Object Support (Limited)

- Harp Script does not implement a separate object system
- Object functionality provided through C language integration
- Memory management handled by external C modules

#### Overflow Detection

Strict arithmetic overflow checking:

```harp
-- Safe mode (planned)
var a int = [int max value]
a += 1  -- Runtime error
```

#### Exception Handling

`try-catch` to be set only for unrecoverable exceptions.

### v0.2 Considerations

- Review of `int` type deprecation (unification to `num`)
- NaN Boxing introduction
- Function overloading
- Bitwise operations
- Expanded static optimization

---

## Major Changes (2026-10-29)

### v0.1.1 Baseline Updates

- ✅ Control statements: if-elif-else, while confirmed
- ✅ Label feature removed (currently not supported)
- ✅ char type partial support clarified
- ✅ Type system transition from dynamic to explicit static typing
- ✅ Build command moved to v0.1.2 release
- ✅ Completion rate specified at 50% or less
- ✅ Platform support status added

### Status Indicators

- ✅ Supported
- ⚠️ Partially supported
- 🚧 Planned
- ❌ Not supported

---

## Notes

All content in this document reflects the prototype stage and may change before the November release.

**Official Repository**: https://codeberg.org/Noksek/harp_script
