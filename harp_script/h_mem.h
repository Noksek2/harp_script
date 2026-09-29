
#pragma once

#include "h_def.h"



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
		objs.Init(_4MB);
		freeobj = NULL;
		free_len = 0u;
		objs.m_len = 1u;
		objs[0].Init();
	}
	//void ResetPool() {
	//}
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
			ptr->next = (uint32_t)(freeobj - &objs[0]);
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