
#pragma once
#include <map>
using namespace std;
#include "def.h"
#include "errormsg.h"

struct PoolBlock {//64KB
	uint8_t sizelist[8] = {3, }; //[8]
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
	//vector<T>s;
	uint32_t m_len;
	uint32_t m_capa;
	//uint32_t m_reserved;
	MyMemStack(uint32_t m_capa_max=_4KB) {
		m_len = 0u;
		m_capa = m_capa_max;
		m_stack = (T*)MEM_reserve(MEM_MAX);
		m_stack = (T*)MEM_commit(m_stack, m_capa);
	}
	void push(const T& dat) {
		if (m_len == m_capa) {
			m_capa <<= 1;
			Harp_assert(m_capa > MEM_MAX, "[ERROR] stack overflow");
			m_stack = (T*)MEM_commit(m_stack, m_capa);
		}
		m_stack[m_len++] = dat;
	}
	inline void resize(uint32_t re_size) {
		if (re_size > m_capa) {
			while (re_size > m_capa)
				m_capa <<= 1;
			Harp_assert(m_capa > MEM_MAX, "[ERROR] stack overflow");
			m_stack = (T*)MEM_commit(m_stack, m_capa);
		}
		

		m_len = m_capa = re_size;
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
	T& operator[] (const uint32_t idx) { return m_stack[idx]; }
	const T& operator[] (const uint32_t idx) const { return m_stack[idx]; }
	~MyMemStack() {
		if (m_stack) {
			MEM_free(m_stack);
			m_stack = NULL;
		}
	}
};