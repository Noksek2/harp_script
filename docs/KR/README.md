# Harp Script Docs 한글판

`HARP SCRIPT is the programming language`

현재 한글만 지원

```
추후에 이렇게 변경 예정
/docs/ : EN 파일만 모음
/docs/KR/ : 해석본

/docs/g/ 언어 문법 설명
/docs/infunc/ 내장 라이브러리(내장 기능) 설명
/docs/export/ 라이브러리 배포 설명
/docs/dev/ 개발 일지, 언어 개발 관련 내용 모음 (Github 공개 안함)
```


# Grammar/Syntax
## macro
### version
```
#ver 0.2.0
```

## Be careful
### Space
```
a = 300
b = 500 // ok
a= 300 b=500 // ok. 
a=300 b=500 // ok. 
a=500	b=300 // ok

a
=
500 b
= 500 // ok but not recommended.

a=300b=500 // NO. 컴파일이 성공할 가능성도 있으나 절대로 권장하지 않음.

```

### Line
개행을 한 번 한다고 구분되지 않음. 
문장을 구분하고 싶으면, 두 번 연속으로 개행해야 함.
```

b (20 
30 40) // it's func call

a
(10 20) // it's also function call expr. be careful.

a // a = none if a is not declared
// or call expr `a()` if a is function type (but not recommended)
// or skipped

(10 20)// but it's ok

// 개행 문자가 연속으로 두번 이상일 경우 다른 문장으로 취급됨.

```
## Comment (주석)
### Block Comment
```
~ Block Comment ~
~
  Block Comment
	Yeah : ))
~
```

### Line Comment
```
//Line
a = 100 // Comment

// b=c 3030 Ok
```


## Var Declaration
```
//var keyword ok
var a = 100
//also
var do
	a = 300
	b = 500
	c = 100
end
a = 100 // var a = 100
b // b = none
a // a just declared, skipped.
```
## Harp Fundamental Types

### Basic Type (none, bool, int, float, str, list, dict)
Basic Type is `immut` (immutable type)

```

//ok
a = 100 // b is int
b = "str" // b is str
b = 'str2' // b is also str
c // c = none
a // meaningless syntax
a = none 
a = true or false // true
a = true and false // false
a = 

```
- int, float
```
op : + - * / ** % 
int op int => int
int op float => float
float op int => float
int op float => float

op_l : < <= > >= == !=

type <=> type : none or int(-1, 0, 1)

4 < 3 <=> 5 == more less same

int + str => str
str + int => str 
str * int => str
str / str => str
str - str => str
str % str => str
str <=> str => -1 0 1

etc : error

abcdefghijklmnop % abc = abc
abcdef / abc = def

'abc'['a'] = [0]
'dddddd'['dd'] = [0,1,2,3,4,5]

```

### operations
- `-` op1min
- `**` oppow
- `*` opmul
- `/` opdiv
- `%` opmod


- `+` opadd
- `-` opsub

- `<` oplt
- `<=` oplte

- `>` opgt
- `>=` opgte

- `==` opeq
- `!=` opneq

- `&&` `and` opand
- `||` `or` opor

- `!` opnot

- `&` opband
- `|` opbor
- `^` opbxor

- `min` opkmin
- `max` opkmax
- `abs` opkabs


### may be later, or not (maybe v0.3)
- `^^` `xor` opxor : test ver
- `..` op op3rng
- `...` op3rng2 : `3..5 inc 1` `3..5 sub 1`
