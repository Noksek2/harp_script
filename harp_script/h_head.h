#pragma once
#pragma warning(disable: 4200)
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <cmath>

#include <stdio.h>
#include <Windows.h>
#include <crtdbg.h>
#include <conio.h>
#include <time.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <random>

#define umap unordered_map 
using namespace std;
typedef wchar_t wchar;
typedef unsigned char uchar;

constexpr double MATH_PI = 3.1415926535897932384626;
constexpr double DX_PI = 3.14159265358979323846264338327;


constexpr uint64_t NAN_MARK     = 0x7FF8000000000000ULL;
constexpr uint64_t SNAN_MARK    = 0x7FF0000000000000ULL;
constexpr uint64_t INF_MARK     = 0x7FF0000000000000ULL;
constexpr uint64_t BIT_15_MARK  = 0x7FFF000000000000ULL;
constexpr uint64_t BIT_16_MARK  = 0xFFFF000000000000ULL;
constexpr uint64_t NAN_ALL_MARK = 0x7FF7000000000000ULL;

constexpr uint64_t BIT_48 = 0x0000FFFFFFFFFFFFULL;
constexpr uint64_t INT48_MAX = 0x00007FFFFFFFFFFFULL;
constexpr uint64_t INT48_MIN = 0x0000800000000000ULL;

constexpr int64_t NORM_INT48_MAX = (int64_t)(INT48_MAX );
constexpr int64_t NORM_INT48_MIN = (int64_t)(INT48_MIN | (0xFFFFllu << 48llu));

constexpr uint32_t harp_align(const uint32_t sz) {
	const uint64_t ssz = (uint32_t)sz;
	const uint64_t rsz = (ssz + 7llu) & ~(7llu);
	return (uint32_t)min(rsz, UINT32_MAX);
}

#define r_cast(T, V) reinterpret_cast<T>(V)
#define s_cast(T, V) static_cast<T>(V)
#define d_cast(T, V) dynamic_cast<T>(V)

enum {
	_4KB = 4 * 1024,
	_64KB = 64 * 1024,
	_4MB = 4 * 1024 * 1024,
	_16MB = 16 * 1024 * 1024,
	_64MB = 64 * 1024 * 1024,
	_128MB = _64MB * 2,
	_256MB = _128MB * 2,
	_512MB = _256MB * 2,
	_1024MB = _512MB * 2,
	_1GB = _1024MB,
	MEM_MAX_DEFAULT = _16MB,//16MB
	RESERVE_MAX_DEFAULT = _1GB,//1GB

};

static void Harp_assert(bool statement, const char* buf) {
#ifdef _DEBUG
	if (!statement) {
		puts(buf);
		__debugbreak();
	}

#endif
	if (!statement) {
		puts(buf);
		exit(1);
	}
}

static void Harp_assert_dbg(bool statement, const char* buf) {
#ifdef _DEBUG
	if (!statement) {
		puts(buf);
		__debugbreak();
	}

#endif
}

template<typename T>
static T* harp_malloc(size_t len) {
	return (T*)malloc(len * sizeof(T));
}
template<typename T>
static T* harp_calloc(size_t len) {
	return (T*)calloc(len, sizeof(T));
}
template<typename T>
static T* harp_realloc(T* dat, size_t len) {
	return (T*)realloc(dat, len * sizeof(T));
}
static void harp_free(void* ptr) {
	free(ptr);
}




static void* MEM_reserve(const uint64_t max_capa, const uint64_t T_sz, const uint64_t RESERVE_MAX) {
	uint64_t memsz = max_capa * T_sz;
	Harp_assert(memsz <= _1GB, "[ERROR] MEM_reserve cannot reserve more than 1GB");
	Harp_assert(memsz <= RESERVE_MAX, "[ERROR] MEM_reserve more than RESERVE_MAX");
	void* ptr = VirtualAlloc(
		NULL,
		memsz,
		MEM_RESERVE,
		PAGE_READWRITE
	);
	Harp_assert(ptr != NULL, "[ERROR] memory allocate failed - reserve");
	return ptr;
}
static void* MEM_commit(void* ptr, uint64_t len, uint64_t T_sz, const uint64_t RESERVE_MAX) {
	uint64_t memsz = len * T_sz;
	Harp_assert(memsz <= RESERVE_MAX, "[ERROR] MEM_reserve more than RESERVE_MAX");
	void* nptr = VirtualAlloc(
		ptr,
		memsz,
		MEM_COMMIT,
		PAGE_READWRITE
	);
	Harp_assert(nptr != NULL, "[ERROR] memory allocate failed - commit");
	return nptr;
}
static void MEM_free(void* ptr) {
	VirtualFree(ptr, 0, MEM_RELEASE);
}

enum rterrtype {
	rte_unknown,
	rte_calc_failed, //[line, op_+]
	rte_func_para_no_match, //[(line), funcinfo(para_cnt, types=?)]
	rte_call_failed, //[line, wrong value]
	rte_infunc_unknown, // [line, infunctype]
};



template <typename T, const uint64_t RES_CNT_MAX = _64MB>
class MyMemStack {//성능 쓰레기같은 스택
private:
public:
	T* m_stack;
	uint32_t m_len;
	uint32_t m_capa;
	//uint32_t m_reserved;
	void Init(uint32_t m_capa_cnt = _4KB) {
		m_len = 0u;
		m_capa = max(m_capa_cnt, _4KB / sizeof(T));
		m_stack = r_cast(T*, MEM_reserve(RES_CNT_MAX, sizeof(T), RES_CNT_MAX * sizeof(T)));
		m_stack = r_cast(T*, MEM_commit(m_stack, m_capa, sizeof(T), RES_CNT_MAX * sizeof(T)));
	}
	inline void reserve(uint32_t re_size) {
		if (re_size > m_capa) {

			while (re_size > m_capa)
				m_capa <<= 1;
			if (m_capa > RES_CNT_MAX) {
				m_capa = RES_CNT_MAX;
			}
			m_stack = (T*)MEM_commit(m_stack, m_capa, sizeof(T), RES_CNT_MAX * sizeof(T));
		}
	}
	void push(const T& dat) {
		if (m_len >= m_capa) {
			reserve(m_len + 1);
		}
		m_stack[m_len++] = dat;
	}
	inline void resize(uint32_t re_size) {
		reserve(re_size);

		m_len = re_size;
	}
	inline T pop() {
		return m_stack[--m_len];
	}
	inline void onlypop() {
		m_len--;
	}
	inline T top() {
		return m_stack[m_len - 1];
	}
	inline T* back_ptr() {
		return &m_stack[m_len];
	}
	T& operator[] (const uint32_t idx) { return m_stack[idx]; }
	const T& operator[] (const uint32_t idx) const { return m_stack[idx]; }
	~MyMemStack() {
		if (m_stack) {
			MEM_free(m_stack);
			m_stack = NULL;
		}
	}
};

//Init, ~
template <typename T, const uint64_t MEM_CNT_MAX = _64MB>
class MyStack {
private:
public:
	T* m_stack;
	uint32_t m_len;
	uint32_t m_capa;
	//uint32_t m_reserved;
	void Init(uint32_t _m_capa = _4KB) {
		m_len = 0u;
		m_capa = _m_capa;
		m_stack = harp_calloc<T>(m_capa);
		//m_stack = (T*)MEM_commit(m_stack, m_capa);
	}
	inline void reserve(uint32_t re_size) {
		if (re_size > m_capa) {
			while (re_size > m_capa)
				m_capa <<= 1;
			Harp_assert(m_capa <= MEM_CNT_MAX, "[ERROR] reserve more than MEM_MAX");
			T* newarr = harp_realloc<T>(m_stack, m_capa);
			Harp_assert(newarr != NULL, "[ERROR] realloc failed");
			m_stack = newarr;
		}

	}
	void push() {
		reserve(m_len + 1);
		m_len++;
	}
	void push(const T& dat) {
		if (m_len == m_capa) {
			reserve(m_len + 1);
		}
		m_stack[m_len++] = dat;
	}
	inline void resize(uint32_t re_size) {
		reserve(re_size);
		m_len = re_size;
	}

	inline const uint32_t size() { return m_len; }
	inline T pop() {
		return m_stack[--m_len];
	}
	inline void onlypop() {
		m_len--;
	}
	inline T top() {
		return m_stack[m_len - 1];
	}
	T& operator[] (const uint32_t idx) { return m_stack[idx]; }
	const T& operator[] (const uint32_t idx) const { return m_stack[idx]; }

	inline T* back_ptr() {
		return &m_stack[m_len];
	}
	~MyStack() {
		if (m_stack) {
			free(m_stack);
			m_stack = NULL;
		}
	}
};


template<typename T>
struct KeyComposer {
	static inline bool Compare(const T& k1, const T& k2) {
		return k1 == k2;
	}
	static inline uint32_t GetHash(const T& name) { return 2; }
	static inline void Delete(const T& name) { }
	static inline const T& Copy(const T& key) {
		return key;
	}
};
#include <cwchar>
template<>
static inline uint32_t KeyComposer<std::wstring>::GetHash(const std::wstring& name) {
	uint32_t len = wcslen(name.c_str());
	uint32_t hash = 0u;
	for (uint32_t idx = 0; idx < len; idx++)
	{
		hash ^= name[idx];
		hash *= 16777619; // FNV prime
	}
	if (hash < 2u) return 2u;
	return hash;
}


template<>
static inline const std::wstring& KeyComposer<std::wstring>::Copy(const std::wstring& key) {
	return key;
}

//
//template<typename K, typename V, const uint32_t MAXCAPA = _4MB>
//class MyHashTable {
//	uint32_t len; uint32_t CAPA;
//	uint32_t erased_len; uint32_t _;
//	//연속된 메모리 공간을 keymap, keyval, keyhash가 공유함.
//	K* keymap;
//	V* keyval;
//	uint32_t* keyhash;
//
//	MyHashTable* NewTableMemory() {
//	}
//	void Init(uint32_t _capa) {
//		CAPA = _capa;
//		//일단 챗봇한테 맡겼고 확인해봐야 됨.
//		// 1. 각 배열이 다음 타입의 정렬(alignof)을 만족하도록 패딩된 크기 계산
//		size_t k_bytes = sizeof(K) * CAPA;
//		size_t k_padding = (alignof(V) - (k_bytes % alignof(V))) % alignof(V);
//		size_t k_offset = k_bytes + k_padding;
//
//		size_t v_bytes = sizeof(V) * CAPA;
//		size_t v_padding = (alignof(uint32_t) - (v_bytes % alignof(uint32_t))) % alignof(uint32_t);
//		size_t v_offset = v_bytes + v_padding;
//
//		size_t hash_bytes = sizeof(uint32_t) * CAPA;
//
//		// 2. 패딩을 포함한 총 메모리 단일 할당
//		void* pmem = std::calloc(1, k_offset + v_offset + hash_bytes);
//		uint8_t* pmembuf = s_cast(uint8_t*, pmem);
//
//		// 3. 포인터 캐스팅
//		keymap = r_cast(K*, (pmembuf));
//		keyval = r_cast(V*, (pmembuf + k_offset));
//		keyhash = r_cast(uint32_t*, (pmembuf + k_offset + v_offset));
//
//		// 4. 단일 요소 단위로 Placement New 호출 (루프 사용)
//		//for (int i = 0; i < CAPA; ++i) {
//		//	new (&keymap[i]) K();
//		//	new (&keyval[i]) V();
//		//	// uint32_t는 기본 타입이고 calloc으로 0 초기화되었으므로 new 생략 가능
//		//}
//
//		printf("size of %llu->(%llu / %llu) <K, V, %u>\n",
//			   sizeof(MyHashTable),
//			   (sizeof(K) + sizeof(V) + sizeof(uint32_t)) * CAPA,
//			   k_offset + v_offset + hash_bytes,
//			   CAPA);
//	}
//	void Delete() {
//		
//		for (int i = 0; i < CAPA; ++i) {
//			if (keyhash[i] >= 2u) {
//				Harp_assert(len > 0u, "Something Wrong in HashTable: len==0 but hash not empty");
//				KeyComposer<K>::Delete(keymap[i]);
//				KeyComposer<V>::Delete(keyval[i]);
//				keymap[i].~K();
//				keyval[i].~V();
//				len--;
//			}
//			keyhash[i] = HASH_EMPTY;
//			// uint32_t는 기본 타입이고 calloc으로 0 초기화되었으므로 new 생략 가능
//		}
//
//		std::free(keymap);
//		keymap = NULL;
//		keyval = NULL;
//		keyhash = NULL;
//	}
//	MyHashTable(uint32_t def_capa) { Init(def_capa); }
//	inline void Insert(const K& name, const V& val, const uint32_t kh, const uint32_t idx) {
//		keymap[idx] = name;
//		keyhash[idx] = kh;
//		keyval[idx] = val;
//		len++;
//	}
//	void EraseToEmpty(uint32_t idx) {
//		uint32_t i = idx + 1;//idx+1
//		if (idx + 1 >= CAPA) {
//			i = 0u;
//		}
//		if (keyhash[i] != HASH_EMPTY) return;
//		//HASH_ERASED HASH_EMPTY
//		i = idx;
//		keyhash[i] = HASH_EMPTY;
//		while (i > 0u) {
//			--i;
//			if (keyhash[i] != HASH_ERASED) return;
//			keyhash[i] = HASH_EMPTY;
//		}
//		i = CAPA;
//		while (i > idx) {
//			--i;
//			if (keyhash[i] != HASH_ERASED) return;
//			keyhash[i] = HASH_EMPTY;
//		}
//	}
//	inline void Erase(const uint32_t idx) {
//		KeyComposer<K>::Delete(keymap[idx]);
//		KeyComposer<V>::Delete(keyval[idx]);
//		keymap[idx].~K();
//		keyval[idx].~V();
//		keyhash[idx] = HASH_ERASED;
//		len--;
//		erased_len++;
//	}
//	bool Resort() {
//		K* n_k;
//		V* n_v;
//		uint32_t* n_hash;
//		return true;
//	}
//	bool ResortInPlace() {
//		K* n_k;
//		V* n_v;
//		uint32_t* n_hash;
//		return true;
//	}
//	void Switch(uint32_t erase_idx, uint32_t idx) const {
//		memcpy(&keymap[erase_idx], &keymap[idx], sizeof(K));
//		memcpy(&keyval[erase_idx], &keyval[idx], sizeof(V));
//		keyhash[erase_idx] = keyhash[idx];
//		memset(&keymap[idx], 0, sizeof(K));
//		memset(&keyval[idx], 0, sizeof(V));
//		keyhash[idx] = HASH_ERASED;
//	}
//	
//public:
//	enum {
//		HASH_EMPTY = 0u,
//		HASH_ERASED = 1u,
//	};
//	const uint32_t Len() const { return len; }
//	void Dump() {
//		if (CAPA > 256) return;
//		uint32_t idx = 0;
//		for (uint32_t i = 0u; i < CAPA && idx != len; i++) {
//			if (keyhash[i] == HASH_EMPTY) {
//				std::cout << "    ";
//				//std::cout << '[' << i << ']' << "" << '\n';
//			}
//			else if (keyhash[i] == HASH_ERASED) {
//				std::cout << "  E ";
//				//std::cout << '[' << i << ']' << " ERASED" << '\n';
//			}
//			else {
//				printf("%3d ", keyval[i]);
//				//std::wcout << '[' << i << ']' << '(' << keymap[i] << ':' << keyval[i] << ')' << '\n';
//				idx++;
//			}
//		}
//		std::cout << '\n';
//	}
//	static MyHashTable* New(uint32_t def_capa = 16u) {
//		MyHashTable* tb = new MyHashTable(def_capa);
//		return tb;
//	}
//	~MyHashTable() { Delete(); }
//
//	const uint32_t find_idx(const K& name) const {
//		uint32_t kh = KeyComposer<K>::GetHash(name);
//		const uint32_t o_idx = kh % CAPA;
//		uint32_t idx = o_idx;
//		uint32_t erase_idx = -1u;
//		while (idx < CAPA) {
//			if (keyhash[idx] == HASH_EMPTY) {//empty
//				return -1u;
//			}
//			else if (keyhash[idx] == kh) {
//				if (KeyComposer<K>::Compare(name, keymap[idx])) { goto l_Return; }
//			}
//			//else if(keyhash[idx]==HASH_ERASED && erase_idx ==-1u){
//			//	erase_idx = idx;
//			//}
//			// HASH_ERASED
//			idx++;
//		}
//		idx = 0;
//		while (idx < o_idx) {
//			if (keyhash[idx] == HASH_EMPTY) {//empty
//				return -1u;
//			}
//			else if (keyhash[idx] == kh) {
//				if (KeyComposer<K>::Compare(name, keymap[idx])) { goto l_Return; }
//			}
//			//else if (keyhash[idx] == HASH_ERASED && erase_idx == -1u) {
//			//	erase_idx = idx;
//			//}
//			idx++;
//		}
//		return -1u;
//
//	l_Return:
//		//if (erase_idx != -1u){
//		//	Switch(erase_idx, idx);
//		//	return erase_idx;
//		//}
//		return idx;
//	}
//	uint32_t find_empty_idx(const K& name, uint32_t& key) {
//		if (len == CAPA)return -1u;
//		uint32_t kh = KeyComposer<K>::GetHash(name);
//		const uint32_t o_idx = kh % CAPA;
//		uint32_t idx = o_idx;
//		while (idx < CAPA) {
//			if (keyhash[idx] == HASH_EMPTY || keyhash[idx] == HASH_ERASED) {
//				key = kh;
//				return idx;
//			}
//			idx++;
//		}
//		idx = 0;
//		while (idx < o_idx) {
//			if (keyhash[idx] == HASH_EMPTY || keyhash[idx] == HASH_ERASED) {
//				key = kh;
//				return idx;
//			}
//			idx++;
//		}
//		return -1u;
//	}
//	void emplace(const K& name, const V& val) {
//		if ((len + erased_len) * 3 > CAPA * 2) {
//			Harp_assert(Resort(), "HashTable Resort Failed");
//		}
//		
//		uint32_t idx;
//		idx = find_idx(name);
//		if (idx != -1u) {
//			KeyComposer<K>::Delete(name);
//			name.~K();
//			KeyComposer<V>::Delete(keyval[idx]);
//			keyval[idx].~V();
//			keyval[idx] = val;
//			return;
//		}
//			
//		uint32_t kh; //= KeyComposer<K>::GetHash(name);
//		idx = find_empty_idx(name, kh);
//		if (idx != -1u) {
//			Insert(name, val, kh, idx);
//		}
//	}
//	//void emplace(K&& name, const V& val) { emplace(name, val); }
//	void erase(const K& name) {
//		const uint32_t idx = find_idx(name);
//		if (idx == -1u) return;
//
//		Erase(idx);
//		EraseToEmpty(idx);
//		
//	}
//	const V* find(const K& name) const {
//		const uint32_t idx = find_idx(name);
//		if (idx == -1u) return NULL;
//		return &keyval[idx];
//	}
//};

template<typename K, typename V, const uint32_t MAXCAPA = _4MB>
class MyHashTable : public unordered_map<K, V> {
public:
	void Dump() const {}
	static MyHashTable* New(uint32_t capa) {
		auto T = new MyHashTable();
		if (T != nullptr) T->reserve(capa);
		return T;
	}
	const V* find(const K& name) const {
		auto data = unordered_map<K, V>::find(name);
		if (data == this->end()) {
			return NULL;
		}
		return &data->second;
	}
};