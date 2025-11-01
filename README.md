# Harp Script

- Developed: Until ~December 2018

- Status: Experimental / Discontinued

## Introduction

Harp Script is a simple, experimental scripting language developed around 2018.

## Features

- Limited functionality

- Simple math operations

- Console I/O

- Some built-in functions related to dxlib

## Implementation Features

* **Direct-to-Bytecode (No AST):** Does not build an explicit Abstract Syntax Tree (AST). A recursive descent parser with an operator-precedence stack generates bytecode directly during parsing. (See `express.cpp`)
* **Simple FFI (Foreign Function Interface):** Uses WinAPI (`LoadLibrary`/`GetProcAddress`) to dynamically load `.dll` files (e.g., `DxLib.dll`). These are wrapped as built-in functions (infunc) that can be called directly from the VM. (See `memory.h`, `infunc.cpp`)
* **Stack-Based VM:** A simple stack-based virtual machine, written in C++, interprets and executes the bytecode. (See `execute.cpp`)
* 
## Usage

1. Write your code in a.harp.

2. Run harp.exe.

3. Bytecode will be generated as the output file.

4. And you can see the result. (Console or Dxlib program)

## Examples & Syntax

Please refer to the files inside the example folder for syntax references and examples.

## Issues

This project is discontinued. Issues or pull requests may not be fixed or will be handled non-periodically.
