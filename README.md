## Introduction

Harp Script is a simple, dynamically-typed scripting language first created in 2018.

<img width="200" height="200" alt="Image" src="https://github.com/user-attachments/assets/6a9b426f-38df-4145-a1be-634d9ce833e7" />

The goals of Harp Script are as follows:
- Simple syntax, flexible syntax usage
- Uncomplicated parser
- Lightweight and extensible VM
- Mark sweep, no global memory GC

## Philosophy
- Implement easily, use for a long time.
- Limited features require critical thinking.
- Pursue basic and uncomplicated features
- Code style is up to the developer
- Typing errors are the developer's responsibility

## v0.2 Goals
- Redesign the entire VM and parser from v0.1
- Modify syntax and some features. Abandon some syntax compatibility with v0.1
- Move beyond Toy Script

## Features (v0.2)

- Limited functionality

- Simple mathematical operations

- Console I/O support, minimal built-in function support

- Basic data types provided: integers, floats, characters, booleans, None, and strings

- Dynamic array List and hash table Dictionary objects provided

- As of v0.2, object declaration is not possible. Classes are planned for v0.3 and later

- Function (Func) declaration and Enum constant support

- DLL loading functionality for C language interoperability

- Basic GUI programming support (libraries like DxLib provided by default)

- Simple language management system inspired by Go

## Other Differences from v0.1

- Starts from the main function
- Added basic data types (`list`, `dict`, `bool`, `none`)
- Runtime errors trigger panic for safety (`error`) - v0.2 terminates immediately without exception handling
- Changed some built-in function specifications and syntax specs
- And more

## Examples & Syntax

Please refer to [`/docs/`](./docs/README.md), or the files inside the [`/ex/`](./ex/) folder for syntax references and examples.

### Basic Syntax Explanation

0. Flexible and free syntax.
- Of course, freedom comes with responsibility.

Flexible syntax itself is much easier to implement in interpreters/compilers and provides better performance.

```
~same grammar~
n=100 n1=100.1
n
=100
n1 =
100.1
// Line Comment

~ same 
grammar 
~
print n, n1
print n,
n1


n=100 n1=100.1
n =100
n1 =
100.1


~ same grammar ~
print n, n1
print n,
n1

```

1. Block start is `do`, end is `end` or `;`. However, block start can be omitted.
- Since this is not Python, tabs are optional.
```
// v0.2 = ruby style
if true do
	print('true haha')
end

// also ok 
if true
	print('true haha')
end

// no ok. it's not python
if true
	print('true haha')
	

// v0.1 style. ok. 
// but don't use `:` for starting block
if true do
	print(100)
;
if true
	print(100)
;
if true do print(100) ;
if true print(100) ;

	if true
print(100)
					;
// i dont care tab : ) 
```


2. For other details, see the examples [`/ex/`](./ex/)

```
// DxLib Example
use dxlib as *
func main
	Dxlib_init()
	x=250
	y=200
	img = Loadgraph('./image/yuka.png')
	t=0
	while Msgloop() and !Keypress(vk_esc)
		loop i,5
			Drawstr('와 정말 신기한걸?',
				i*2,
				i*2,
				rgb(0,i*50,i*50))
		;
		loop i,20
			Rotagraph(img,
				i*10+cos(t)*200+250,
				sin(t)*i*10+200,
				sin(t/2)*360,
				(sin(i*10)+sin(t))/3)
		;
		t+=1
	;
	Dxlib_end()
;
```
Code

## Implementation Features

* **Direct-to-Bytecode (No AST):** v0.2 does not use AST for fast parsing and simple compiler implementation. Uses recursive descent parser and generates bytecode immediately on the stack. (See `h_expr.cpp`)
* **Simple FFI (Foreign Function Interface):** Uses WinAPI functionality (`LoadLibrary`/`GetProcAddress`) to load `.dll` files (e.g., `DxLib.dll`). Built-in features are immediately usable without additional parsing or file checking delays. (See `h_mem.h`, `h_infunc.cpp`)
* **Stack-Based VM:** Simple stack-based virtual machine written in C-level C++, interprets and executes bytecode directly. (See `h_exec.cpp`)

## Usage

1. Add the path of harp.exe to PATH.

2. Create a source file with a .harp extension and write your code.

3. Open a console and type `harp sourcefile`.
   - 3-1. If the source file does not have a .harp extension, use a specific flag (`-r`) and include the extension.
   
   `harp -r a.txt`
   
   - 3-2. If both a.txt and a.harp exist in the same directory, only a.harp will run.
   
   - 3-3. If you type `harp a.harp`, it will look for `a.harp.harp`, so just type `harp a`.
   
   - 3-4. If you type `harp a.txt` without the `-r` flag, it will load `a.txt.harp`.

4. Available flags can be checked with `-h` or `--help`.

5. Check your results. Happy programming :)

## LICENSE
- Until `v0.1.0` : `Apache License 2.0` -> Private
- After `v0.2.0` : `HarpScript License` (will explain it later.)
