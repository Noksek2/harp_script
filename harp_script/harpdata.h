#ifndef __HARPDATA_H__
#define __HARPDATA_H__

#define __TESTMODE

#include <stdint.h>
#include <math.h>
#include <stdio.h>
enum optype {
    op_push,
    op_pop,
    op_call,
    op_incall,
    op_lit,
    op_lvar,
    op_gvar,
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
    op_array,
    op_jmp,
    op_ujmp,
    op_return,

    op_or,
    op_and,
    op_neq,
    op_eq,
    op_more2,
    op_more,
    op_less2,
    op_less,
    op_plus,
    op_minus,
    op_multi,
    op_divi,
    op_mod,
    op_pow,
    op_min,
    op_not,
    op_print,
    op_out,

    op_add,
    op_sub,
    op_mul,
    op_div,
    op_modis,
    op_powis,

    op_ang = 50,

    rank_brack = 100,
    OP_MAX = 100,
};
enum vartype {
    TFloat = 0U,
    
    TError = 1U,
    TNull = 1U | 8,
    TInt = 2U | 8,
    TObj = 3U | 8,
    TInStr = 4U | 8,
    TFnc = 5U | 8,

};

enum rterrtype {
    rte_calc_failed,
};

#define MATH_PI 3.1415926535897932384626
#define DX_PI 3.14159265358979323846264338327

#define NAN_MARK     0x7FF8000000000000ULL
#define SNAN_MARK    0x7FF0000000000000ULL
#define INF_MARK     0x7FF0000000000000ULL
#define BIT_15_MARK  0x7FFF000000000000ULL
#define BIT_16_MARK  0xFFFF000000000000ULL
#define NAN_ALL_MARK 0x7FF7000000000000ULL

#define TAG_INT   (NAN_MARK  | ((uintptr_t)TInt<<48))
#define TAG_OBJ   (NAN_MARK  | ((uintptr_t)TObj<<48))
#define TAG_INSTR (NAN_MARK  | ((uintptr_t)TInStr<<48))
#define TAG_FNC   (NAN_MARK  | ((uintptr_t)TFnc<<48))
#define TAG_NULL  (NAN_MARK  | ((uintptr_t)TNull<<48))
#define TAG_ERR   (SNAN_MARK | ((uintptr_t)TError<<48))
#define BIT_48  0x0000FFFFFFFFFFFFULL

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
#define ENCODE_INSTR(d) (TAG_INSTR | (d.byte))
#define ENCODE_ERR(i)   (TAG_ERR | ((uint32_t)(i)))


#define DECODE_INT32(V)	((int32_t)((V).byte&0xFFFFFFFF))

#define DECODE_INT(V) ((int64_t)((V).byte&BIT_48))

#define DECODE_INT_C(V) IF_INT_NEG(V)?\
    (((int64_t)((V).byte&BIT_48))|0xFFFF000000000000)\
    :((int64_t)((V).byte&BIT_48))
#define DECODE_FLOAT(V) ((V).f64)
#define DECODE_OBJ(V)	((void*)((V)&BIT_48))

#define INT_CALC_NEW(N1, S, N2) ENCODE_INT((DECODE_INT(N1)) S (DECODE_INT(N2)))
//#define INT_CALC_IN(N1, S, N2) DECODE_INT(N1) S DECODE_INT(N2)
typedef struct harpobj {
    uint32_t len;
    uint32_t poolidx;
    uint8_t ptr[];
}harpobj;
typedef struct harpdata {
    union {
        uintptr_t byte;
        int64_t i64;
        double f64;
        char s8[8];
        void* v;
        struct harpobj* obj;
    };
}harpdata;

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
    const uint64_t n1_tag = CHECK_TAG15(n1);
    const uint64_t n2_tag = CHECK_TAG15(n2);
    if (n1_tag == (0x7FF0ULL << 48)) {
        if (n2_tag == TAG_INT) {
            switch (op) {
            case op_add: res.byte = INT_CALC_NEW(n1, +, n2); break;
            case op_sub: res.byte = INT_CALC_NEW(n1, -, n2); break;
            case op_mul: res.byte = INT_CALC_NEW(n1, *, n2); break;
            case op_div: res.byte = INT_CALC_NEW(n1, / , n2); break;
            case op_mod: res.byte = INT_CALC_NEW(n1, %, n2); break;
            case op_less: res.byte = INT_CALC_NEW(n1, < , n2); break;
            case op_less2: res.byte = INT_CALC_NEW(n1, <= , n2); break;
            case op_more: res.byte = INT_CALC_NEW(n1, > , n2); break;
            case op_more2: res.byte = INT_CALC_NEW(n1, >= , n2); break;
            case op_eq: res.byte = INT_CALC_NEW(n1, == , n2); break;
            case op_neq: res.byte = INT_CALC_NEW(n1, != , n2); break;
            case op_pow: res.byte = ENCODE_INT(pow((double)DECODE_INT(n1), (double)DECODE_INT(n2)));
                break;
            default: goto l_err;
            }
            goto l_ret;
        }
        //
        else if (IF_FLOAT(n2)) {
            switch (op) {
            case op_add:   res.f64 = (double)DECODE_INT(n1) + n2.f64; break;
            case op_sub:   res.f64 = (double)DECODE_INT(n1) - n2.f64; break;
            case op_mul:   res.f64 = (double)DECODE_INT(n1) * n2.f64; break;
            case op_div:   res.f64 = (double)DECODE_INT(n1) / n2.f64; break;
            case op_mod:   res.f64 = fmod((double)DECODE_INT(n1), n2.f64); break;
            case op_less:  res.byte = ENCODE_INT((double)DECODE_INT(n1) < n2.f64); break;
            case op_less2: res.byte = ENCODE_INT((double)DECODE_INT(n1) <= n2.f64); break;
            case op_more:  res.byte = ENCODE_INT((double)DECODE_INT(n1) > n2.f64); break;
            case op_more2: res.byte = ENCODE_INT((double)DECODE_INT(n1) >= n2.f64); break;
            case op_eq:    res.byte = ENCODE_INT((double)DECODE_INT(n1) == n2.f64); break;
            case op_neq:   res.byte = ENCODE_INT((double)DECODE_INT(n1) != n2.f64); break;
            case op_pow:   res.f64 = pow((double)DECODE_INT(n1), n2.f64); break;
            default: goto l_err;
            }
            goto l_ret;
        }
    }
    else if (IF_FLOAT(n1)) {
        if (IF_FLOAT(n2)) {
            switch (op) {
            case op_add:res.f64 = n1.f64 + n2.f64; break;
            case op_sub:res.f64 = n1.f64 - n2.f64; break;
            case op_mul:res.f64 = n1.f64 * n2.f64; break;
            case op_div:res.f64 = n1.f64 / n2.f64; break;
            case op_mod:   res.f64 = fmod(n1.f64, n2.f64); break;
            case op_less:  res.byte = ENCODE_INT(n1.f64 < n2.f64); break;
            case op_less2: res.byte = ENCODE_INT(n1.f64 <= n2.f64);  break;
            case op_more:  res.byte = ENCODE_INT(n1.f64 > n2.f64); break;
            case op_more2: res.byte = ENCODE_INT(n1.f64 >= n2.f64);  break;
            case op_eq:    res.byte = ENCODE_INT(n1.f64 == n2.f64);  break;
            case op_neq:   res.byte = ENCODE_INT(n1.f64 != n2.f64);  break;
            case op_pow:   res.f64 = pow(n1.f64, n2.f64);  break;
            default: goto l_err;
            }
            goto l_ret;
        }
        else if (n2_tag == TAG_INT) {

            switch (op) {
            case op_add:   res.f64 = n1.f64 + (double)DECODE_INT(n2); break;
            case op_sub:   res.f64 = n1.f64 - (double)DECODE_INT(n2); break;
            case op_mul:   res.f64 = n1.f64 * (double)DECODE_INT(n2); break;
            case op_div:   res.f64 = n1.f64 / (double)DECODE_INT(n2); break;
            case op_mod:   res.f64 = fmod(n1.f64, (double)DECODE_INT(n2)); break;
            case op_less:  res.byte = ENCODE_INT(n1.f64 < (double)DECODE_INT(n2)); break;
            case op_less2: res.byte = ENCODE_INT(n1.f64 <= (double)DECODE_INT(n2)); break;
            case op_more:  res.byte = ENCODE_INT(n1.f64 > (double)DECODE_INT(n2)); break;
            case op_more2: res.byte = ENCODE_INT(n1.f64 >= (double)DECODE_INT(n2)); break;
            case op_eq:    res.byte = ENCODE_INT(n1.f64 == (double)DECODE_INT(n2)); break;
            case op_neq:   res.byte = ENCODE_INT(n1.f64 != (double)DECODE_INT(n2)); break;
            case op_pow:   res.f64 = pow(n1.f64, (double)DECODE_INT(n2)); break;
            default:       goto l_err;
            }
            goto l_ret;
        }
    }
    else {
        {
            const uint32_t n1_type = GET_TAGNO(n1);
            const uint32_t n2_type = GET_TAGNO(n2);
            if (n1_type == n2_type) {
                switch (n1_type) {
                default:
                    goto l_err;
                    //goto l_err;
                }
            }
            goto l_err;
        }
    }
l_err:
    res.byte = ENCODE_ERR(rte_calc_failed);
l_ret:
    return res;
}

static const harpdata harpdata_calc2(const harpdata n1, const harpdata n2, const uint8_t op) {
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
    const uint64_t n1_tag = (n1.byte) >> 48;
    const uint64_t n2_tag = (n1.byte) >> 48;
    if (n1_tag == TAG_INT) {
        if (n2_tag == TAG_INT) {
            switch (op) {
            case op_add: res.byte = INT_CALC_NEW(n1, +, n2); break;
            case op_sub: res.byte = INT_CALC_NEW(n1, -, n2); break;
            case op_mul: res.byte = INT_CALC_NEW(n1, *, n2); break;
            case op_div: res.byte = INT_CALC_NEW(n1, / , n2); break;
            case op_mod: res.byte = INT_CALC_NEW(n1, %, n2); break;
            case op_less: res.byte = INT_CALC_NEW(n1, < , n2); break;
            case op_less2: res.byte = INT_CALC_NEW(n1, <= , n2); break;
            case op_more: res.byte = INT_CALC_NEW(n1, > , n2); break;
            case op_more2: res.byte = INT_CALC_NEW(n1, >= , n2); break;
            case op_eq: res.byte = INT_CALC_NEW(n1, == , n2); break;
            case op_neq: res.byte = INT_CALC_NEW(n1, != , n2); break;
            case op_pow: res.byte = ENCODE_INT(pow((double)DECODE_INT(n1), (double)DECODE_INT(n2)));
                break;
            default: goto l_err;
            }
            goto l_ret;
        }
        //
        else if (IF_FLOAT(n2)) {
            switch (op) {
            case op_add:   res.f64 = (double)DECODE_INT(n1) + n2.f64; break;
            case op_sub:   res.f64 = (double)DECODE_INT(n1) - n2.f64; break;
            case op_mul:   res.f64 = (double)DECODE_INT(n1) * n2.f64; break;
            case op_div:   res.f64 = (double)DECODE_INT(n1) / n2.f64; break;
            case op_mod:   res.f64 = fmod((double)DECODE_INT(n1), n2.f64); break;
            case op_less:  res.byte = ENCODE_INT((double)DECODE_INT(n1) < n2.f64); break;
            case op_less2: res.byte = ENCODE_INT((double)DECODE_INT(n1) <= n2.f64); break;
            case op_more:  res.byte = ENCODE_INT((double)DECODE_INT(n1) > n2.f64); break;
            case op_more2: res.byte = ENCODE_INT((double)DECODE_INT(n1) >= n2.f64); break;
            case op_eq:    res.byte = ENCODE_INT((double)DECODE_INT(n1) == n2.f64); break;
            case op_neq:   res.byte = ENCODE_INT((double)DECODE_INT(n1) != n2.f64); break;
            case op_pow:   res.f64 = pow((double)DECODE_INT(n1), n2.f64); break;
            default: goto l_err;
            }
            goto l_ret;
        }
    }
    else if (IF_FLOAT(n1)) {
        if (IF_FLOAT(n2)) {
            switch (op) {
            case op_add:res.f64 = n1.f64 + n2.f64; break;
            case op_sub:res.f64 = n1.f64 - n2.f64; break;
            case op_mul:res.f64 = n1.f64 * n2.f64; break;
            case op_div:res.f64 = n1.f64 / n2.f64; break;
            case op_mod:   res.f64 = fmod(n1.f64, n2.f64); break;
            case op_less:  res.byte = ENCODE_INT(n1.f64 < n2.f64); break;
            case op_less2: res.byte = ENCODE_INT(n1.f64 <= n2.f64);  break;
            case op_more:  res.byte = ENCODE_INT(n1.f64 > n2.f64); break;
            case op_more2: res.byte = ENCODE_INT(n1.f64 >= n2.f64);  break;
            case op_eq:    res.byte = ENCODE_INT(n1.f64 == n2.f64);  break;
            case op_neq:   res.byte = ENCODE_INT(n1.f64 != n2.f64);  break;
            case op_pow:   res.f64 = pow(n1.f64, n2.f64);  break;
            default: goto l_err;
            }
            goto l_ret;
        }
        else if (n2_tag == TAG_INT) {

            switch (op) {
            case op_add:   res.f64 = n1.f64 + (double)DECODE_INT(n2); break;
            case op_sub:   res.f64 = n1.f64 - (double)DECODE_INT(n2); break;
            case op_mul:   res.f64 = n1.f64 * (double)DECODE_INT(n2); break;
            case op_div:   res.f64 = n1.f64 / (double)DECODE_INT(n2); break;
            case op_mod:   res.f64 = fmod(n1.f64, (double)DECODE_INT(n2)); break;
            case op_less:  res.byte = ENCODE_INT(n1.f64 < (double)DECODE_INT(n2)); break;
            case op_less2: res.byte = ENCODE_INT(n1.f64 <= (double)DECODE_INT(n2)); break;
            case op_more:  res.byte = ENCODE_INT(n1.f64 > (double)DECODE_INT(n2)); break;
            case op_more2: res.byte = ENCODE_INT(n1.f64 >= (double)DECODE_INT(n2)); break;
            case op_eq:    res.byte = ENCODE_INT(n1.f64 == (double)DECODE_INT(n2)); break;
            case op_neq:   res.byte = ENCODE_INT(n1.f64 != (double)DECODE_INT(n2)); break;
            case op_pow:   res.f64 = pow(n1.f64, (double)DECODE_INT(n2)); break;
            default:       goto l_err;
            }
            goto l_ret;
        }
    }
    else {
        {
            const uint32_t n1_type = GET_TAGNO(n1);
            const uint32_t n2_type = GET_TAGNO(n2);
            if (n1_type == n2_type) {
                switch (n1_type) {
                default:
                    goto l_err;
                    //goto l_err;
                }
            }
            goto l_err;
        }
    }
l_err:
    res.byte = ENCODE_ERR(rte_calc_failed);
l_ret:
    return res;
}
static void harpdata_print(const harpdata n1) {
    printf("%llX ", n1.byte);
    if (IF_INT(n1)) { printf("[int] %lld", DECODE_INT_C(n1)); }
    else if (IF_ERR(n1)) {
        printf("[err] %lld", DECODE_INT_C(n1)); 
    }
    else if (IF_FLOAT(n1)) { printf("[float] %lf", (n1)); }
    putchar('\n');
}

#ifdef __TESTMODE
extern void harpdata_test();
#endif
#endif