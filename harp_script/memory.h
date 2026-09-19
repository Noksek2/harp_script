
#pragma once

#include "def.h"

static void* MEM_reserve(uint32_t max_capa) {
	void* ptr = VirtualAlloc(
		NULL,
		max_capa,
		MEM_RESERVE,
		PAGE_READWRITE
	);
	Harp_assert(ptr != NULL, "[ERROR] memory allocate failed - reserve");
	return ptr;
}
static void* MEM_commit(void* ptr, uint32_t len) {
	void* nptr = VirtualAlloc(
		ptr,
		len,
		MEM_COMMIT,
		PAGE_READWRITE
	);
	Harp_assert(nptr != NULL, "[ERROR] memory allocate failed - commit");
	return nptr;
}
static void MEM_free(void* ptr) {
	VirtualFree(ptr, 0, MEM_RELEASE);
}
template <typename T>
class MyMemStack {//성능 쓰레기같은 스택
private:
public:
	T* m_stack;
	uint32_t m_len;
	uint32_t m_capa;
	//uint32_t m_reserved;
	MyMemStack(uint32_t m_capa_max=_4KB) {
		m_len = 0u;
		m_capa = m_capa_max;
		m_stack = (T*)MEM_reserve(MEM_MAX);
		m_stack = (T*)MEM_commit(m_stack, m_capa * sizeof(T));
	}
	void push(const T& dat) {
		if (m_len == m_capa) {
			m_capa <<= 1;
			Harp_assert(m_capa*sizeof(T) <= MEM_MAX, "[ERROR] stack overflow");
			m_stack = (T*)MEM_commit(m_stack, m_capa);
		}
		m_stack[m_len++] = dat;
	}
	inline void reserve(uint32_t re_size) {
		if (re_size > m_capa) {
			while (re_size > m_capa)
				m_capa <<= 1;
			Harp_assert(m_capa * sizeof(T) <= MEM_MAX, "[ERROR] stack overflow");
			m_stack = (T*)MEM_commit(m_stack, m_capa);
			Harp_assert(m_stack != NULL, "[ERROR] vmem commit failed");
		}

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

template <typename T>
class MyStack {
private:
public:
	T* m_stack;
	uint32_t m_len;
	uint32_t m_capa;
	//uint32_t m_reserved;
	MyStack(uint32_t _m_capa = _4KB) {
		m_len = 0u;
		m_capa = _m_capa;
		m_stack = (T*)calloc(m_capa,sizeof(T));
		//m_stack = (T*)MEM_commit(m_stack, m_capa);
	}
	inline void reserve(uint32_t re_size) {
		if (re_size > m_capa) {
			while (re_size > m_capa)
				m_capa <<= 1;
			Harp_assert(m_capa * sizeof(T) <= MEM_MAX, "[ERROR] stack overflow");
			T* newarr = (T*)realloc(m_stack, m_capa);
			Harp_assert(newarr == NULL, "[ERROR] realloc failed");
			free(m_stack);
			m_stack = newarr;
		}

	}
	void push(const T& dat) {
		if (m_len == m_capa) {
			m_capa <<= 1;
			Harp_assert(m_capa * sizeof(T) <= MEM_MAX, "[ERROR] stack overflow");
			T* newarr = (T*)realloc(m_stack, m_capa * sizeof(T));
			Harp_assert(newarr == NULL, "[ERROR] realloc failed");
			free(m_stack);
			m_stack = newarr;
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


struct PoolBlock {//64KB
	uint8_t sizelist[8] = { 3, }; //[8]
	uint32_t blocksize;
	uint8_t ptr[];
};
struct MemoryInfo {
	uint32_t len;
	uint32_t capa;
	uint32_t threshold;
	PoolBlock* poolblock;
};
extern MemoryInfo g_meminfo;

//obj의 0번지는 비어있음.
struct harpobjpool {
protected:
	MyMemStack<harpobj> objs;
	harpobj* freeobj;
	uint32_t free_len;//0번지는 빼므로 실제로는
public:
	void InitPool() {
		freeobj = NULL;
		free_len = 0u;
		objs.m_len = 1u;
		objs[0].Init();
	}
	harpobj* GetFreeObj(){
		harpobj* o;
		if (freeobj != NULL) {
			o = freeobj;
			free_len--;
			freeobj = &objs[freeobj->next];
			return o;
		}
		o = objs.back_ptr();
		objs.push(objs[0]);
		return o;
		//memset(&objs[0], 0, sizeof(harpobj));
	}
	harpobj* Insert(harpobj obj) {
		harpobj* pobj = GetFreeObj();
		Harp_assert(pobj != NULL, "harpobj insert error : maybe objectpool full");
		*pobj = obj;
		return pobj;
		//if (pobj == NULL) {  return; }
	}
	
	void DeleteByIdx(uint32_t idx) {
		objs[idx].Delete();
	}
	void DeleteByPtr(harpobj* ptr) {
		if (freeobj == NULL)
			ptr->next = 0u;
		else {
			ptr->next = freeobj - &objs[0];
		}
		
		freeobj = ptr;
		ptr->Delete();
	}
	void DeletePool() {
		for (uint32_t i = 0u; i < objs.m_len; i++) {
			if (objs[i].u.p != NULL) {
				objs[i].Delete();
			}
		}
		objs.~MyMemStack();
	}

	//void RunGC();
};
extern harpobjpool* g_objpool;
