# Harp Script 한글판 README

## dev v0.2
**문서 작성 중**

## NOTICE
- Codeberg, Please...
현재 Github의 레포지토리는 개인 계정의 정책에 따라서 설명 및 소개만을 위해 존재합니다. 코드는 기본적으로 아래 Codeberg에서 업로드될 예정이므로 양해부탁드립니다.
 
- **Hosted on Codeberg** : To prevent unauthorized AI data harvesting, please contribute or fork via Codeberg. If using GitHub, keep your forks private.
https://codeberg.org/Noksek/harp_script



## Introduction

Harp Script(하프 스크립트)는 2018에 처음 만들어진 간편한 동적 타입의 스크립트 언어입니다.

<img width="200" height="200" alt="Image" src="https://github.com/user-attachments/assets/6a9b426f-38df-4145-a1be-634d9ce833e7" />

Harp Script의 목표는 이렇습니다.
- 간단한 문법, 유연한 문법 사용
- 복잡하지 않은 파서
- 가볍고 확장성 있는 VM
- 마크 스윕, 전역 메모리 GC 금지

## 철학
- 쉽게 구현하고, 오래 쓴다. 
- 제한된 기능은 사고 능력을 요구한다.
- 기본 기능, 복잡하지 않은 기능 추구
- 코드 스타일은 개발자 마음
- 타이핑 에러는 개발자의 책임


## v0.2의 목표
- v0.1의 VM, 파서, 전부 개조, 구조 재설계
- 문법 수정, 일부 기능 수정. v0.1과 문법 호환성 일부 포기
- Toy Script 벗어나기

## 특징 (v0.2)

- 한정된 기능

- 간단한 수학 연산

- 콘솔 I/O 지원, 최소한의 함수 지원

- 정수, 실수, 문자, 부울, None, 문자열은 기본 자료형으로 제공. 

- 동적 배열인 리스트(List)와 해시 테이블 딕셔너리(Dict) 객체 제공.

- v0.2 기준 객체 선언은 불가능. 클래스는 v0.3 부터 지원 예정

- 함수(Func) 선언, 열거형 상수(Enum) 지원

- C언어 연동을 위한 DLL 로드 기능 제공

- 기본적인 GUI 프로그래밍 지원 (Dxlib 등의 라이브러리를 기본 제공)

- Go 언어를 참고한 간편한 언어 관리 시스템 제공

## 그 외 v0.1과의 차이점

- main 함수에서 시작함.
- 기본적인 자료형 추가 (`list`, `dict`, `bool`, `none`)
- 안전성을 위해 런타임 에러 시 패닉 발생 (`error`) - v0.2는 예외 처리 없이 즉시 종료.
- 내장 함수 명세, 문법 스펙 등 일부 변경
- 기타 등등

## Examples & Syntax


Please refer to [`/docs/`](./docs/README.md), or the files inside the `/ex/` folder for syntax references and examples.

### 기본적인 구문 설명

0. 유연하고, 자유로운 구문. 
- 물론, 자유에는 책임이 따릅니다.

구문 자체는 엄격하지 않은 편이 인터프리터/컴파일러 구현에 훨씬 용이하고 성능도 빨라집니다.

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

```


```
func 
n=100 n1=100.1
n =100
n1 =
100.1


~ same grammar ~
print n, n1
print n,
n1

```

1. 블록 시작은 `do`, 종료는 `end` 혹은 ';'. 단, 블록 시작은 생략 가능.
- 파이썬이 아니므로 탭은 무시 가능.
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

2. 그 외에는 예제(/ex/) 참고

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

## 구현 특징

* ** Direct-to-Bytecode (No AST):** v0.2는 빠른 파싱 및 간단한 컴파일러 구현을 위해 AST을 사용하지 않음. 재귀 하향 파서 및 스택에서 즉시 바이트코드를 생성함. (See `h_expr.cpp`)
* **Simple FFI (Foreign Function Interface):** WinAPI (`LoadLibrary`/`GetProcAddress`)의 기능을 이용, `.dll` files (e.g., `DxLib.dll`)을 로드하는데 사용. 내장 기능은 별도의 파싱/파일 검사 지연없이 즉시 사용가능. (See `h_mem.h`, `h_infunc.cpp`)
* **Stack-Based VM:** 간단한 스택 기반 가상 머신 VM, C 수준의 C++로 작성, 바이트코드를 그대로 해석하며 실행. (See `h_exec.cpp`)
* 
## 사용 방법

1. harp.exe의 경로를 PATH에 등록한다. 

2. 이름.harp 소스파일을 만들어서 코드를 작성한다.

3. 콘솔을 실행하여, `harp 소스파일`을 입력한다. 
3-1. 만일 소스파일이 .harp 확장자가 아닌 경우, 특정 플래그(`-r`)를 사용하고, 확장자까지 입력해야 함. 

`harp -r a.txt`

3-2. a.txt와 a.harp가 같은 경로에 있다면, a.harp만 실행됨.

3-3. `harp a.harp`를 입력하면 `a.harp.harp`를 조사하게 되므로, `harp a`만 입력할것.

3-3. `-r`플래그를 빼고 `harp a.txt`를 입력하면 `a.txt.harp`를 불러오게 됨. 

4. 사용 가능한 플래그는 `-h`, `--help`로 조사 가능함.

5. 결과 확인. 즐거운 프로그래밍 :)

## LICENSE
- Until `v0.1.0` : `Apache License 2.0` -> 비공개
- After `v0.2.0` : `HarpScript License` (will explain it later.)
