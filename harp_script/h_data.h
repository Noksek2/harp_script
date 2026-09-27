#ifndef __HARPDATA_H__
#define __HARPDATA_H__

#define __TESTMODE


#include "h_head.h"


enum vartype {
    TFloat = 0U,

    TError = 1U,
    TUnq = 1U | 8,//unique type
    TInt = 2U | 8,
    TObj = 3U | 8,
    TStr = 4U | 8,
    TFnc = 5U | 8,
    TInFnc = 6U | 8,
};

enum optype :uint8_t {
    op_push, //push lit_idx , stack <== lit[lit_idx]
    op_pop, //pop 
    op_call, // call [para_len], stack.pop()(stack.pop() * para_len)
    op_incall, 
    op_lit, //
    op_lload,
    op_gload,
    //op_lvar,
    //op_gvar,
    op_lstore,
    op_gstore,
    op_pushinfunc,
    op_pushfunc,
    //op_pushprev,

    op_setsize,
    op_setsizes,
    op_set,
    op_sets,
    op_at,
    op_atm,//at member
    op_array,
    op_jmp,
    op_ujmp,
    op_return,

    op_or,
    op_and,
    op_neq,
    op_eq,
    op_gte,
    op_gt,
    op_lte,
    op_lt,
    op_add,
    op_sub,
    op_mul,
    op_div,
    op_mod,
    op_pow,
    op_min,
    op_not,
    op_print,
    op_out,
    op_dbgprint,

    op_addeq,
    op_subeq,
    op_muleq,
    op_diveq,
    op_modeq,
    op_poweq,
    op_checktyp,
    op_mkstr,
    op_ang = 99,

    rank_brack = 100,
    OP_MAX = 127,
};

enum harpobjtype {
    objt_null,
    objt_str,
    objt_list,
    objt_dict,
};


typedef struct harpdata harpdata;
typedef struct harpdict_d harpdict_d;
typedef struct harplist_d harplist_d;
typedef struct harpvtable harpvtable;

typedef struct harpobjpool harpobjpool;
typedef struct harpobj harpobj;

typedef struct harpstr_d harpstr_d;
typedef harpstr_d* harpstr;
typedef harplist_d* harplist;
static harpstr harpstr_copy(harpstr str);

struct harpstr_d {
    uint32_t len;
    uint32_t hash;
    
    uint32_t rc;
    wchar ptr[];
    const bool Compare(const harpstr& s1) const {
        if (s1 == this) return true;
        if (s1->len != this->len) return false;
        return memcmp(s1->ptr, this->ptr, s1->len * sizeof(wchar)) == 0;
    }
    harpstr Copy() { return harpstr_copy(this); }
    void Print() const {
        for (uint32_t i = 0; i < len; i++) {
            putwchar(ptr[i]);
        }
    }
    void Dump() {
        wprintf(L"[harpstr] (len=%u hash=%X val=", len, hash);
        Print();
        printf(")\n");
    }
    void ToWstring(wchar_t* buf) {
        memcpy(buf, ptr, sizeof((len) * sizeof(wchar)));
        buf[len] = 0;
    }
};

static void harpstr_sethash(harpstr str) {
    uint32_t len = str->len;
    uint32_t hash = 0u;
    for (uint32_t idx = 0; idx < len; idx++)
    {
        hash ^= str->ptr[idx];
        hash *= 16777619; // FNV prime
    }
    if (hash < 2u) hash = 2u;
    str->hash = hash;
}
static harpstr harpstr_new(const void* ptr, const wchar_t* buf, const uint32_t len) {
    harpstr str = (harpstr)ptr;
    str->len = len;
    memcpy(str->ptr, buf, len * sizeof(wchar));
    harpstr_sethash(str);
    return str;
}
static harpstr harpstr_new(const wchar_t* buf, const uint32_t len) {
    //uint32_t str_sz = harp_align();
    harpstr str = (harpstr)malloc(sizeof(harpstr_d) + len * sizeof(wchar));
    str->len = len;
    memcpy(str->ptr, buf, len * sizeof(wchar));
    harpstr_sethash(str);
    return str;
}
static harpstr harpstr_copy(harpstr str) {
    uint32_t memsz = sizeof(harpstr_d) + str->len * sizeof(wchar);
    harpstr newstr =(harpstr)calloc(1, memsz);
    if (newstr == NULL) {
        Harp_assert(newstr != NULL, "harpstr_copy failed : NULL RETURNED");
    }
    memcpy(newstr, str, memsz);
    return newstr;
}

#if __cplusplus
struct harpobj {
    enum {
        MARKED = 0x80000000, 
    };
    union {
        harpstr s;
        harplist li;
        void* p;
    }u;
    uint32_t next;
    uint32_t prev;
    //uint8_t ptr[];
    uint32_t refcnt;
    harpobjtype objtype;
    harpvtable* vtable;
    void Init() { u.p = NULL; next = 0u; prev = 0u; refcnt = 0u; vtable = NULL; objtype = objt_null; }
    inline void Delete();
    void Print() const;
    void DebugPrint() const;
    inline void IncRC() { refcnt++; }
    inline void DecRC() { 
        Harp_assert_dbg(refcnt > 0u, "[ERROR] try to dec refcnt, but refcnt == 0");
        refcnt--; 
        if (refcnt == 0u) { this->Delete(); } 
    }

    void Mark() {

    }
    void SetMark() { refcnt = refcnt | MARKED; }
    bool IsMark() { return refcnt& MARKED; }
    bool IsNull() { return u.p == NULL; }
};
enum {
    method_init_idx,
    method_deinit_idx,
    method_oppow_idx,
    method_opmul_idx,
    method_opdiv_idx,
    method_opmod_idx,
    method_opadd_idx,
    method_opsub_idx,
    method_oplt_idx,
    method_oplte_idx,
    method_opgt_idx,
    method_opgte_idx,
    method_opeq_idx,
    method_opneq_idx,
};

struct harpvtable {
    
    
    //런타임 단계
    MyStack<harpfunc_p, 256u> methods; //연산자, 생성자 등 오버라이드는 맨앞에서 자동 생성.
    MyStack<harpdata, 256u> members; //멤버는 고정되어 있음.

    //compile 단계에서 이렇게 번역...?
    //harpfunc_p fn_op_add;
    //harpfunc_p fn_op_sub;
    //harpfunc_p fn_op_mul;
    //harpfunc_p fn_op_mod;
    //harpfunc_p fn_op_div;
    //harpfunc_p fn_op_pow;
    //harpfunc_p fn_op_lt;
    //harpfunc_p fn_op_lte;
    //harpfunc_p fn_op_gt;
    //harpfunc_p fn_op_gte;
    //harpfunc_p fn_op_eq;
    //harpfunc_p fn_op_neq;
    //harpfunc_p fn_op_and;
    //harpfunc_p fn_op_or;
};



struct harpdata {
    union {
        uintptr_t byte;
        int64_t i64;
        double f64;
        char s8[8];
        void* v;
        struct harpobj* obj;
    };
    
    inline void SetInt(int64_t i);
    inline void SetFloat(double d);
    inline void SetObj(harpobj* obj);
    inline void SetBool(bool b);
    inline void SetErr(rterrtype);


    inline const int64_t GetInt()const;
    inline const harpstr GetStr()const;
    const bool isTrue() const;
    inline void IncRC();
    inline void DecRC();
    void Print() const;
    void DebugPrint() const ;
    inline const uint32_t GetType() const;
};

struct harptype {
    uint8_t TagType;
    uint8_t _;
    uint16_t ObjType;//obj 
};
typedef const harpdata(*harpfunc_p) (const harpdata*, const uint32_t);
struct harpfunc {
    harpfunc_p fn;
    harpstr name;
    uint8_t frame_len;
    uint8_t para_len;
    uint8_t types[8];//0 1 2 3 4 5 6 7 8 p=parent() c=child() s:parent
    harptype types_etc[];
};
struct harpvtableinfo {
    //그전에는 이렇게?
    MyHashTable<harpstr, harpdata> members;
    MyHashTable<harpstr, harpdata> methods;

};
struct harpclass {
    //harpfunc_p fn;
    //harpfunc_p fn_op_add;
    //harpfunc_p fn_op_sub;
    //harpfunc_p fn_op_mul;
    //harpfunc_p fn_op_div;
    //harpfunc_p fn_op_mod;
    //harpfunc_p fn_op_pow;
    //harpfunc_p fn_op_lt;
    //harpfunc_p fn_op_lte;
    //harpfunc_p fn_op_gt;
    //harpfunc_p fn_op_gte;
    //harpfunc_p fn_op_;
    union {
        harpvtable* vtable;
        harpvtableinfo* vtableinfo;//컴파일 타임에만 사용
    };
    harpstr name;
    uint8_t member_len;
    uint8_t method_len;
    uint8_t enum_len;
    uint8_t types[8];//0 1 2 3 4 5 6 7 8 p=parent() c=child() s:parent
    harptype types_etc[];
};


//dtype objtype
//인수 최대 4개

constexpr uint64_t TAG_INT = (NAN_MARK | ((uintptr_t)TInt << 48));
constexpr uint64_t TAG_OBJ = (NAN_MARK | ((uintptr_t)TObj << 48));
constexpr uint64_t TAG_STR = (NAN_MARK | ((uintptr_t)TStr << 48));
constexpr uint64_t TAG_FNC = (NAN_MARK | ((uintptr_t)TFnc << 48));
constexpr uint64_t TAG_UNQ = (NAN_MARK | ((uintptr_t)TUnq << 48));
constexpr uint64_t TAG_ERR = (SNAN_MARK | ((uintptr_t)TError << 48));

constexpr uint32_t GET_TAGNO(const harpdata V) {
    return ((((V).byte) & 0x000F000000000000ULL) >> 48);
}
constexpr uint64_t CHECK_TAG(const harpdata V) {
    return (((V).byte) & BIT_16_MARK);
}
constexpr uint64_t CHECK_TAG15(const harpdata V) {
    return (((V).byte) & BIT_15_MARK);
}
constexpr uint64_t CHECK_EXPONENT(const harpdata V) {
    return (((V).byte) & INF_MARK);
}
constexpr uint64_t CHECK_INF(const harpdata V) {
    return (((V).byte & 0x7FFFFFFFFFFFFFFFULL) == INF_MARK);
}
constexpr bool IF_INT(const harpdata V) { return ((CHECK_TAG(V)) == TAG_INT); }
constexpr bool IF_OBJ(const harpdata V) { return ((CHECK_TAG(V)) == TAG_OBJ); }
constexpr bool IF_UNQ(const harpdata V) {
    return ((CHECK_TAG(V)) == TAG_UNQ);
}

constexpr bool IF_FLOAT(const harpdata V) {
    return ((CHECK_EXPONENT(V) != INF_MARK) || CHECK_INF(V));
}
//prev : 7FF7 == 7FF0
constexpr bool IF_NAN(const harpdata V) {
    return (((V).byte & 0x7FFFFFFFFFFFFFFFLLU) == 0x7FF0000000000000LLU);
}
constexpr bool IF_ERR(const harpdata V) {
    return (IF_NAN(V) || ((CHECK_TAG(V)) == TAG_ERR));
}

#define IF_INT_NEG(V) ((V).byte&0x0000800000000000)
constexpr uint64_t HARP_TRUE = (TAG_UNQ | 1);
constexpr uint64_t HARP_FALSE = (TAG_UNQ | 0);

#define ENCODE_BOOL(t) (t?HARP_TRUE:HARP_FALSE)


constexpr uint64_t ENCODE_INT32(uint32_t i) {
    return 	(TAG_INT | (uint64_t)(i));
}
constexpr uint64_t ENCODE_INT(uint64_t i) { return  (TAG_INT | ((uint64_t)(i)&BIT_48)); }
constexpr uint64_t ENCODE_OBJ(void* p) { return  (TAG_OBJ | ((uint64_t)(p)&BIT_48)); }
//constexpr uint64_t ENCODE_STR(const harpdata d) { return (TAG_STR | (d.byte)); }
constexpr uint64_t ENCODE_ERR(rterrtype i) { return  (TAG_ERR | ((uint32_t)(i))); }


constexpr uint32_t DECODE_INT32(const harpdata V) {
    return ((int32_t)((V).byte & 0xFFFFFFFF));
}

#define DECODE_VAL(V) ((V).byte&BIT_48)
#define DECODE_INT(V) ((int64_t)((V).byte&BIT_48))
#define DECODE_INT_V(I) ((int64_t)((I)&BIT_48))

#define DEC_NORM_INT(V) IF_INT_NEG(V)?\
    (((int64_t)((V).byte&BIT_48))|0xFFFF000000000000)\
    :((int64_t)((V).byte&BIT_48))
#define DECODE_FLOAT(V) ((V).f64)
constexpr harpobj* DECODE_OBJ(const harpdata V) { return 	((harpobj*)(void*)((V.byte) & BIT_48)); }

#define INT_CALC_NEW(N1, S, N2) ENCODE_INT((DECODE_INT(N1)) S (DECODE_INT(N2)))
//#define INT_CALC_IN(N1, S, N2) DECODE_INT(N1) S DECODE_INT(N2)

void harpdata::SetInt(int64_t i)  {
    byte = ENCODE_INT(i);
}
inline void harpdata::SetFloat(double d) {
    f64 = d;
}
inline void harpdata::SetObj(harpobj* obj) {
    byte = ENCODE_OBJ(obj);
}
inline void harpdata::SetBool(bool b) {
    byte = ENCODE_INT(b);
}
inline void harpdata::SetErr(rterrtype e) {
    byte = ENCODE_ERR(e);
}

inline const int64_t harpdata::GetInt()const {
    return DEC_NORM_INT(*this);
}
inline const harpstr harpdata::GetStr() const {
    return DECODE_OBJ(*this)->u.s;
}
inline const bool harpdata::isTrue() const {
    //if (byte == HARP_TRUE) {
    //    return true;
    //}
    if (byte == HARP_FALSE) {
        return false;
    }
    if (IF_OBJ(*this)) {
        return DECODE_OBJ(*this)->u.p != NULL;
    }
    return true;
}
inline void harpdata::IncRC() {
    if (IF_OBJ(*this)) DECODE_OBJ(*this)->IncRC();
}
inline void harpdata::DecRC() {
    if (IF_OBJ(*this)) DECODE_OBJ(*this)->DecRC();
}

inline const uint32_t harpdata::GetType() const {
    uint32_t tag;
    if ((byte & INF_MARK) == INF_MARK) {
        tag = GET_TAGNO(*this);
    }
    else tag = TFloat;

    return tag;
}
static harpobj harpobj_new(void* ptr, harpobjtype obj_t) {
    harpobj obj;
    obj.Init();
    obj.refcnt = 1u;
    obj.u.p = ptr;
    obj.objtype = obj_t;
    return obj;
}

typedef struct harppair {
    harpdata key;
    harpdata val;
} harppair;
constexpr uint32_t HARPDICT_CAPA_MAX=15u;
constexpr uint32_t HARPDICT_CAPA[HARPDICT_CAPA_MAX] = {

    17u, 37u, 67u, 131u, 257u,
    521u, 1031u, 2053u, 4099u, 8209u,
    16411u, 32771u, 65537u, 131101u, 262147u,
};
struct harpdict_d {
    uint32_t len;//점유 중
    uint32_t capa;
    uint32_t tomb_cnt;
    uint32_t head_len;
    uint8_t* head;
    harppair dict[];

    
    uint32_t GetHeadSizeFrom(uint32_t _capa) {
        return (capa + 7u)& (~7u);
    }
    void Resort() {

    }
    void Resize() {
        
    }

    void Insert() {
        if ((len + tomb_cnt) * 4 > capa * 3) {
            if (tomb_cnt * 3 > capa) {
                //resort
            }
            else {
                //resize
            }
        }
    }
};
struct harplist_d {
    uint32_t len;
    uint32_t capa;
    harpdata arr[];
    void Print() const {
        putchar('[');
        for (uint32_t i = 0; i < len; i++) {
            arr[i].Print();
            putchar(',');
            putchar(' ');
        }
        putchar(']');
    }
    void DebugPrint() const {
        printf("[list 0x%llX (%u/%u)]\n", (size_t)this->arr, len, capa);
        putchar('[');
        for (uint32_t i = 0; i < len; i++) {
            arr[i].DebugPrint();
            putchar(',');
            putchar(' ');
        }
        putchar(']');
    }
    void Delete() {
        for (uint32_t i = 0u; i < len; i++) {
            arr[i].DecRC();
        }
    }
};


inline void harpobj::Delete() {
    switch (objtype) {
    case objt_str:
        free(u.p);
        break;
    case objt_list:
        u.li->Delete();
        break;
    }
    u.p = NULL;
    objtype = objt_null;
    //원래 더 해야되는데.

}


#else
#define TAG_INT   (NAN_MARK  | ((uintptr_t)TInt<<48))
#define TAG_OBJ   (NAN_MARK  | ((uintptr_t)TObj<<48))
#define TAG_STR (NAN_MARK  | ((uintptr_t)TStr<<48))
#define TAG_FNC   (NAN_MARK  | ((uintptr_t)TFnc<<48))
#define TAG_NULL  (NAN_MARK  | ((uintptr_t)TNull<<48))
#define TAG_ERR   (SNAN_MARK | ((uintptr_t)TError<<48))


#define GET_TAGNO(V)      ((((V).byte)&0x000F000000000000ULL) >> 48)
#define CHECK_TAG(V)      (((V).byte)&BIT_16_MARK)
#define CHECK_TAG15(V)    (((V).byte)&BIT_15_MARK)
#define CHECK_EXPONENT(V) (((V).byte)&INF_MARK)
#define CHECK_INF(V)      (((V).byte&0x7FFFFFFFFFFFFFFFULL)==INF_MARK)
#define IF_INT(V)   ((CHECK_TAG(V))   == TAG_INT)
#define IF_OBJ(V)   ((CHECK_TAG(V))   == TAG_OBJ)
#define IF_NULL(V)  ((CHECK_TAG(V))   == TAG_NULL)

// ((CHECK_TAG15(V)==NAN_MARK)|| ())

#define IF_FLOAT(V) ((CHECK_EXPONENT(V)!= INF_MARK)||CHECK_INF(V))
//prev : 7FF7 == 7FF0
#define IF_NAN(V) ((((V).byte & (BIT_15_MARK|BIT_48)) > INF_MARK) && (((V).byte & (BIT_15_MARK|BIT_48)) <= (NAN_MARK | BIT_48)))
#define IF_ERR(V)  (IF_NAN(V) || ((CHECK_TAG(V))   == TAG_ERR))

#define IF_INT_NEG(V) ((V).byte&0x0000800000000000)

#define ENCODE_INT32(i)	(TAG_INT | ((uint32_t)(i)))
#define ENCODE_INT(i)   (TAG_INT | ((uint64_t)(i) & BIT_48))
#define ENCODE_OBJ(p)   (TAG_OBJ | ((uint64_t)(p) & BIT_48))
#define ENCODE_STR(d) (TAG_STR | (d.byte))
#define ENCODE_ERR(i)   (TAG_ERR | ((uint32_t)(i)))


#define DECODE_INT32(V)	((int32_t)((V).byte&0xFFFFFFFF))

#define DECODE_INT(V) ((int64_t)((V).byte&BIT_48))

#define DEC_NORM_INT(V) IF_INT_NEG(V)?\
    (((int64_t)((V).byte&BIT_48))|0xFFFF000000000000)\
    :((int64_t)((V).byte&BIT_48))
#define DECODE_FLOAT(V) ((V).f64)
#define DECODE_OBJ(V)	((harpobj*)(void*)((V)&BIT_48))

#define INT_CALC_NEW(N1, S, N2) ENCODE_INT((DECODE_INT(N1)) S (DECODE_INT(N2)))
//#define INT_CALC_IN(N1, S, N2) DECODE_INT(N1) S DECODE_INT(N2)

#endif
static const harpdata harpdata_calc_1(const harpdata n1, const uint8_t op) {
    harpdata res;
    switch (op) {
    case op_min:
        if (n1.GetType() == TInt) {
            res.byte = ENCODE_INT(n1.byte ^ 0x800000000000llu + 1u);
        }
        else if (n1.GetType() == TFloat) { res.f64 = -n1.f64; }
        else if (n1.GetType() == TObj) {}
        else goto l_err;
        break;
    case op_not:
        if (n1.GetType() == TInt) {}
        else if (n1.GetType() == TFloat) {}
        else if (n1.GetType() == TObj) {}
        else goto l_err;
        break;
    default:
        res.byte = 0u;
        break;
    }
    
    return res;
l_err:
    res.SetErr(rte_calc_failed);
    return res;
}
static const harpdata harpdata_calc(const harpdata n1, const harpdata n2, const uint8_t op) {
    harpdata res;
    // val & 7FF == inf?{
    // taf==7FF -> inf or nan (qnan/snan and type)
    // else float
    //   val & 7FFF == 7FF8
    //	    -> float
    //   }
    //	 val & (0008|0001) == INT
    //	 val & (0008|0002) == OBJ
    //	 val & (0008|0003) == INSTR
    //	 val & (0008|0004) == FNC
    // }
    // -> float
    // spec -> type
#define CT(N1, N2) ((N1<<4) | (N2))

    const uint32_t Type = (n1.GetType() << 4) | (n2.GetType());
    switch (Type) {
    case CT(TInt, TInt): {
        switch (op) {
        case op_add: res.byte = INT_CALC_NEW(n1, +, n2); break;
        case op_sub: res.byte = INT_CALC_NEW(n1, -, n2); break;
        case op_mul: res.byte = INT_CALC_NEW(n1, *, n2); break;
        case op_div: res.byte = INT_CALC_NEW(n1, / , n2); break;
        case op_mod: res.byte = INT_CALC_NEW(n1, %, n2); break;
        case op_lt: res.byte = ENCODE_BOOL(n1.byte < n2.byte); break;
        case op_lte: res.byte = ENCODE_BOOL(n1.byte <= n2.byte); break;
        case op_gt: res.byte = ENCODE_BOOL(n1.byte > n2.byte); break;
        case op_gte: res.byte = ENCODE_BOOL(n1.byte >= n2.byte); break;
        case op_eq: res.byte = ENCODE_BOOL(n1.byte == n2.byte); break;
        case op_neq: res.byte = ENCODE_BOOL(n1.byte != n2.byte); break;
        case op_pow: res.byte = ENCODE_INT((int64_t)pow((double)DECODE_INT(n1), (double)DECODE_INT(n2)));
            break;
        default: goto l_err;
        }
        goto l_ret;
    }break;
                       //
    case CT(TInt, TFloat): {
        switch (op) {
        case op_add:   res.f64 = (double)DECODE_INT(n1) + n2.f64; break;
        case op_sub:   res.f64 = (double)DECODE_INT(n1) - n2.f64; break;
        case op_mul:   res.f64 = (double)DECODE_INT(n1) * n2.f64; break;
        case op_div:   res.f64 = (double)DECODE_INT(n1) / n2.f64; break;
        case op_mod:   res.f64 = fmod((double)DECODE_INT(n1), n2.f64); break;
        case op_lt:  res.byte = ENCODE_BOOL((double)DECODE_INT(n1) < n2.f64); break;
        case op_lte: res.byte = ENCODE_BOOL((double)DECODE_INT(n1) <= n2.f64); break;
        case op_gt:  res.byte = ENCODE_BOOL((double)DECODE_INT(n1) > n2.f64); break;
        case op_gte: res.byte = ENCODE_BOOL((double)DECODE_INT(n1) >= n2.f64); break;
        case op_eq:    res.byte = HARP_FALSE; break;
        case op_neq:   res.byte = HARP_TRUE; break;
        case op_pow:   res.f64 = pow((double)DECODE_INT(n1), n2.f64); break;
        default: goto l_err;
        }
        goto l_ret;
    }break;
    case CT(TFloat, TFloat): {
        switch (op) {
        case op_add:res.f64 = n1.f64 + n2.f64; break;
        case op_sub:res.f64 = n1.f64 - n2.f64; break;
        case op_mul:res.f64 = n1.f64 * n2.f64; break;
        case op_div:res.f64 = n1.f64 / n2.f64; break;
        case op_mod:   res.f64 = fmod(n1.f64, n2.f64); break;
        case op_lt:  res.byte = ENCODE_BOOL(n1.f64 < n2.f64); break;
        case op_lte: res.byte = ENCODE_BOOL(n1.f64 <= n2.f64);  break;
        case op_gt:  res.byte = ENCODE_BOOL(n1.f64 > n2.f64); break;
        case op_gte: res.byte = ENCODE_BOOL(n1.f64 >= n2.f64);  break;
        case op_eq:    res.byte = ENCODE_BOOL(n1.f64 == n2.f64);  break;
        case op_neq:   res.byte = ENCODE_BOOL(n1.f64 != n2.f64);  break;
        case op_pow:   res.f64 = pow(n1.f64, n2.f64);  break;
        default: goto l_err;
        }
        goto l_ret;
    }break;
    case CT(TFloat, TInt): {
        switch (op) {
        case op_add:   res.f64 = n1.f64 + (double)DECODE_INT(n2); break;
        case op_sub:   res.f64 = n1.f64 - (double)DECODE_INT(n2); break;
        case op_mul:   res.f64 = n1.f64 * (double)DECODE_INT(n2); break;
        case op_div:   res.f64 = n1.f64 / (double)DECODE_INT(n2); break;
        case op_mod:   res.f64 = fmod(n1.f64, (double)DECODE_INT(n2)); break;
        case op_lt:  res.byte = ENCODE_BOOL(n1.f64 < (double)DECODE_INT(n2)); break;
        case op_lte: res.byte = ENCODE_BOOL(n1.f64 <= (double)DECODE_INT(n2)); break;
        case op_gt:  res.byte = ENCODE_BOOL(n1.f64 > (double)DECODE_INT(n2)); break;
        case op_gte: res.byte = ENCODE_BOOL(n1.f64 >= (double)DECODE_INT(n2)); break;
        case op_eq:    res.byte = HARP_FALSE; break;
        case op_neq:   res.byte = HARP_TRUE; break;
        case op_pow:   res.f64 = pow(n1.f64, (double)DECODE_INT(n2)); break;
        default:       goto l_err;
        }
        goto l_ret;
    }break;
    default: {
        {
            const uint32_t n1_type = GET_TAGNO(n1);
            const uint32_t n2_type = GET_TAGNO(n2);
            if (n1_type == n2_type) {
                switch (n1_type) {
                case TAG_INT:
                default:
                    goto l_err;
                    //goto l_err;
                }
            }
            goto l_err;
        }
    }break;
    }
l_err:
    res.SetErr(rte_calc_failed);
l_ret:
    return res;
}

////static const harpdata harpdata_calc2(const harpdata n1, const harpdata n2, const uint8_t op) {
  //  harpdata res;
  //  // val & 7FF == inf?{
  //  // taf==7FF -> inf or nan (qnan/snan and type)
  //  // else float
  //  //   val & 7FFF == 7FF8
  //  //	    -> float
  //  //   }
  //  //	 val & (0008|0001) == INT
  //  //	 val & (0008|0002) == OBJ
  //  //	 val & (0008|0003) == INSTR
  //  //	 val & (0008|0004) == FNC
  //  // }
  //  // -> float
  //  // spec -> type
  //  const uint64_t n1_tag = (n1.byte) >> 48;
  //  const uint64_t n2_tag = (n1.byte) >> 48;
  //  if (n1_tag == TAG_INT) {
  //      if (n2_tag == TAG_INT) {
  //          switch (op) {
  //          case op_add: res.byte = INT_CALC_NEW(n1, +, n2); break;
  //          case op_sub: res.byte = INT_CALC_NEW(n1, -, n2); break;
  //          case op_mul: res.byte = INT_CALC_NEW(n1, *, n2); break;
  //          case op_div: res.byte = INT_CALC_NEW(n1, / , n2); break;
  //          case op_mod: res.byte = INT_CALC_NEW(n1, %, n2); break;
  //          case op_lt: res.byte = INT_CALC_NEW(n1, < , n2); break;
  //          case op_lte: res.byte = INT_CALC_NEW(n1, <= , n2); break;
  //          case op_gt: res.byte =  INT_CALC_NEW(n1, > , n2); break;
  //          case op_gte: res.byte = INT_CALC_NEW(n1, >= , n2); break;
  //          case op_eq: res.byte =  INT_CALC_NEW(n1, == , n2); break;
  //          case op_neq: res.byte = INT_CALC_NEW(n1, != , n2); break;
  //          case op_pow: res.byte = ENCODE_INT(pow((double)DECODE_INT(n1), (double)DECODE_INT(n2)));
  //              break;
  //          default: goto l_err;
  //          }
  //          goto l_ret;
  //      }
  //      //
  //      else if (IF_FLOAT(n2)) {
  //          switch (op) {
  //          case op_add:   res.f64 = (double)DECODE_INT(n1) + n2.f64; break;
  //          case op_sub:   res.f64 = (double)DECODE_INT(n1) - n2.f64; break;
  //          case op_mul:   res.f64 = (double)DECODE_INT(n1) * n2.f64; break;
  //          case op_div:   res.f64 = (double)DECODE_INT(n1) / n2.f64; break;
  //          case op_mod:   res.f64 = fmod((double)DECODE_INT(n1), n2.f64); break;
  //          case op_lt:  res.byte = ENCODE_INT((double)DECODE_INT(n1) < n2.f64); break;
  //          case op_lte: res.byte = ENCODE_INT((double)DECODE_INT(n1) <= n2.f64); break;
  //          case op_gt:  res.byte = ENCODE_INT((double)DECODE_INT(n1) > n2.f64); break;
  //          case op_gte: res.byte = ENCODE_INT((double)DECODE_INT(n1) >= n2.f64); break;
  //          case op_eq:    res.byte = ENCODE_INT((double)DECODE_INT(n1) == n2.f64); break;
  //          case op_neq:   res.byte = ENCODE_INT((double)DECODE_INT(n1) != n2.f64); break;
  //          case op_pow:   res.f64 = pow((double)DECODE_INT(n1), n2.f64); break;
  //          default: goto l_err;
  //          }
  //          goto l_ret;
  //      }
  //  }
  //  else if (IF_FLOAT(n1)) {
  //      if (IF_FLOAT(n2)) {
  //          switch (op) {
  //          case op_add:res.f64 = n1.f64 + n2.f64; break;
  //          case op_sub:res.f64 = n1.f64 - n2.f64; break;
  //          case op_mul:res.f64 = n1.f64 * n2.f64; break;
  //          case op_div:res.f64 = n1.f64 / n2.f64; break;
  //          case op_mod:   res.f64 = fmod(n1.f64, n2.f64); break;
  //          case op_lt:  res.byte = ENCODE_BOOL(n1.f64 < n2.f64); break;
  //          case op_lte: res.byte = ENCODE_BOOL(n1.f64 <= n2.f64);  break;
  //          case op_gt:  res.byte = ENCODE_BOOL(n1.f64 > n2.f64); break;
  //          case op_gte: res.byte = ENCODE_BOOL(n1.f64 >= n2.f64);  break;
  //          case op_eq:    res.byte = ENCODE_BOOL(n1.f64 == n2.f64);  break;
  //          case op_neq:   res.byte = ENCODE_BOOL(n1.f64 != n2.f64);  break;
  //          case op_pow:   res.f64 = pow(n1.f64, n2.f64);  break;
  //          default: goto l_err;
  //          }
  //          goto l_ret;
  //      }
  //      else if (n2_tag == TAG_INT) {
  //
  //          switch (op) {
  //          case op_add:   res.f64 = n1.f64 + (double)DECODE_INT(n2); break;
  //          case op_sub:   res.f64 = n1.f64 - (double)DECODE_INT(n2); break;
  //          case op_mul:   res.f64 = n1.f64 * (double)DECODE_INT(n2); break;
  //          case op_div:   res.f64 = n1.f64 / (double)DECODE_INT(n2); break;
  //          case op_mod:   res.f64 = fmod(n1.f64, (double)DECODE_INT(n2)); break;
  //          case op_lt:  res.byte = ENCODE_INT(n1.f64 < (double)DECODE_INT(n2)); break;
  //          case op_lte: res.byte = ENCODE_INT(n1.f64 <= (double)DECODE_INT(n2)); break;
  //          case op_gt:  res.byte = ENCODE_INT(n1.f64 > (double)DECODE_INT(n2)); break;
  //          case op_gte: res.byte = ENCODE_INT(n1.f64 >= (double)DECODE_INT(n2)); break;
  //          case op_eq:    res.byte = ENCODE_INT(n1.f64 == (double)DECODE_INT(n2)); break;
  //          case op_neq:   res.byte = ENCODE_INT(n1.f64 != (double)DECODE_INT(n2)); break;
  //          case op_pow:   res.f64 = pow(n1.f64, (double)DECODE_INT(n2)); break;
  //          default:       goto l_err;
  //          }
  //          goto l_ret;
  //      }
  //  }
  //  else {
  //      {
  //          const uint32_t n1_type = GET_TAGNO(n1);
  //          const uint32_t n2_type = GET_TAGNO(n2);
  //          if (n1_type == n2_type) {
  //              switch (n1_type) {
  //              default:
  //                  goto l_err;
  //                  //goto l_err;
  //              }
  //          }
  //          goto l_err;
  //      }
  //  }
//l_//err:
//  //  res.byte = ENCODE_ERR(rte_calc_failed);
//l_//ret:
//  //  return res;
//} //
////
//static void harpdata_print(const harpdata n1) {
//    printf("%llX ", n1.byte);
//    if (IF_INT(n1)) { printf("[int] %lld", DECODE_INT_C(n1)); }
//    else if (IF_ERR(n1)) {
//        printf("[err] %lld", DECODE_INT_C(n1)); 
//    }
//    else if (IF_FLOAT(n1)) { printf("[float] %lf", (n1)); }
//    putchar('\n');
//}
#ifdef __TESTMODE
extern void harpdata_test();
#endif
#endif