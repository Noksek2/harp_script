# Harp Script

**Documentation In progress**

## NOTICE
This repository is mainly for introduction and explanation, and the code has not been uploaded. Codeberg Instead.
https://codeberg.org/Noksek/harp_script

## Info
- [Korean/한국어판 README_KR](README_KR.md)
- First Developed: ~December 2018
- Remake : April 2026~
- Status: Experimental

## Introduction

Harp Script is a simple, experimental scripting language developed around 2018.

<img width="200" height="200" alt="Image" src="https://github.com/user-attachments/assets/6a9b426f-38df-4145-a1be-634d9ce833e7" />

The goal of Harp Script is
- Simple grammar, Flexible syntax.
- A not-so-strict parser.
- Fast VM, Memory-friendly development (no Garbage Collection).
- Light and fast VM.

## Philosophy?
- Easy to implement, use it for a long time.
- Limited functions require thinking ability. Therefore **Foolish language** creates good developers. MAybe😛

## Complain
- I dont like Compiler theory 
- Modern language is complicated. Even Python.
- Code style debate awful.
- Typing errors are the developer’s responsibility


## Latest Version (v0.1.1)
- Modify Program Structure (VM, Compiler, Types... else)
- Modify grammar
- Escape from Toy Language

## Features

- Limited functionality

- Simple math operations

- Console I/O

- Some built-in functions related to dxlib



## Codeberg, Please...
- **Hosted on Codeberg** : To prevent unauthorized AI data harvesting, please contribute or fork via Codeberg. If using GitHub, keep your forks private.


## Examples & Syntax

Please refer to the files inside the example folder for syntax references and examples.

- Flexible, Free style syntax. But be careful to use it. ()
```
~same grammar~
n=100 n1=100.1
n
=100
n1 =
100.1


~ same grammar ~
print n, n1
print n,
n1

```



## Implementation Features

* ** Direct-to-Bytecode (No AST):** Does not build an explicit Abstract Syntax Tree (AST). A recursive descent parser with an operator-precedence stack generates bytecode directly during parsing. (See `express.cpp`)
* **Simple FFI (Foreign Function Interface):** Uses WinAPI (`LoadLibrary`/`GetProcAddress`) to dynamically load `.dll` files (e.g., `DxLib.dll`). These are wrapped as built-in functions (infunc) that can be called directly from the VM. (See `memory.h`, `infunc.cpp`)
* **Stack-Based VM:** A simple stack-based virtual machine, written in C++, interprets and executes the bytecode. (See `execute.cpp`)
* 
## Usage

1. Write your code in a.harp.

2. Run harp.exe.

3. Bytecode will be generated as the output file.

4. And you can see the result. (Console or Dxlib program)

## LICENSE
Until `v0.1.0` : `Apache License 2.0` <br>
After `v0.1.1` : `HarpScript License` (will explain it later.)
