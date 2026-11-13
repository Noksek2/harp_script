# Harp Script v0.1.1

**문서 작업 중**

*HarpScript for simple programming*

<img alt="Image" height=150 src="harpscriptlogo.jpg" />

## 정보
- [한국어/영문 README](README.md)
- 최초 개발 (v0.1.0): 2018년 12월경
- 리메이크 아이디어: 2026년 4월 이후
- v0.1.1 : 2026년 8월 ~ 2026년 12월 (계획)
- 상태: 실험적

## 소개

Harp Script는 2018년경에 개발된 간단한 스크립트 스타일 프로그래밍 언어입니다.

Harp Script의 목표는 다음과 같습니다.
- 단순한 문법, 덜 엄격한 구문, 덜 엄격한 파서
- 나에게 잘 맞는 언어
- VM, 메모리 친화적인 개발 (가비지 컬렉션 최소화)
- 가볍고 빠른 VM

## 공지
이 저장소는 주로 소개와 설명용이며, 코드는 업로드되지 않았습니다. 코드베르그(Codeberg)에 별도로 보관되어 있습니다.
https://codeberg.org/Noksek/harp_script

## 철학?
- 쉽게 구현하고 오래 사용할 수 있어야 함
- 제한된 기능은 생각의 힘을 요구함
- 기본 기능 위주, 복잡한 기능은 배제
- 코드 스타일 논쟁은 무의미함
- 타입 오류는 개발자의 책임

## 최신 버전 (v0.1.1)
- 프로그램 구조 수정 (VM, Compiler, Types 등)
- 문법 수정

## 기능

- 제한된 기능

- 간단한 수학 연산

- 일부 내장 함수

- 콘솔 I/O

## Codeberg, 부탁드립니다...
- **Codeberg 호스팅** : 무단 AI 데이터 수집을 방지하기 위해, 기여하거나 포크할 때는 Codeberg를 이용해 주세요. GitHub를 사용할 경우, 포크는 비공개로 유지해 주세요.

## 예제 및 문법

예제 폴더 안의 파일을 참고하면 문법 및 예시를 확인할 수 있습니다.
- [LANG_SPEC/English](LANG_SPEC_EN.md)
- [LANG_SPEC/한국어](LANG_SPEC_KR.md)

## 구현 기능

* **직접 바이트코드 생성 또는 간단한 AST**: 별도의 추상 구문 트리(AST)를 만들지 않습니다. 연산자 우선순위 스택을 사용하는 재귀 하강 파서가 파싱 중에 바이트코드를 직접 생성합니다. (`express.cpp` 참조)
* **단순한 FFI (Foreign Function Interface)**: WinAPI (`LoadLibrary`/`GetProcAddress`)를 사용해 `.dll` 파일 (예: `DxLib.dll`)을 동적으로 로드합니다. 이들은 VM에서 직접 호출할 수 있도록 내장 함수 (`infunc`)로 래핑됩니다. (`memory.h`, `infunc.cpp` 참조)
* **스택 기반 VM**: 간단한 스택 기반 가상 머신이 C++로 작성되어 있으며, 바이트코드를 해석하고 실행합니다. (`execute.cpp` 참조)

## 사용 방법

1. 코드를 작성하고 경로와 파일 이름을 기억합니다.

2. `harp.exe`를 실행하거나 명령을 입력합니다.
```bash
$ harp run src/main.harp
```
바이트코드가 생성되고 VM이 실행됩니다. 행복한 코딩 ;)

## 라이선스

v0.1.0까지: Apache License 2.0 <br> v0.1.1 이후: HarpScript License (추후 설명 예정)
<img alt="Image" height=100 src="hsl_logo.jpg" /> ```
