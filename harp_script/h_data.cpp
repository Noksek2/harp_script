
#include "h_data.h"

void harpdata::Print() const {
	const uint64_t n1_tag = CHECK_TAG15(*this);
	if (IF_FLOAT(*this)) {
		printf("%lf", f64);
	}
	switch (n1_tag) {
	case TAG_INT:
		printf("%lld", DEC_NORM_INT(*this));
		break;
	case TAG_OBJ:
		DECODE_OBJ(*this)->Print();
		break;
	}
}
void harpdata::DebugPrint() const {
	const uint64_t n1_tag = CHECK_TAG15(*this);
	if (IF_FLOAT(*this)) {
		printf("[fload %lf]", f64);
	}
	switch (n1_tag) {
	case TAG_INT:
		printf("[int %lld]", DEC_NORM_INT(*this));
		break;
	case TAG_OBJ:
		DECODE_OBJ(*this)->DebugPrint();
		break;
	}
}

void harpobj::Print()const {
	switch (objtype) {
	case objt_list:
		u.li->Print();
		break;
	case objt_str:
		wprintf(L"%.*s", u.s->len, u.s->ptr);
		break;
	}
}

void harpobj::DebugPrint() const {
	printf("[obj 0x%llX rc=%u obj_t=%u]\n", (size_t)this, refcnt, objtype);
	switch (objtype) {
	case objt_list:
		putchar('[');
		for (uint32_t i = 0; i < u.li->len; i++) {
			u.li->DebugPrint();
			putchar(',');
			putchar(' ');
		}
		putchar(']');
		break;
	case objt_str:
		wprintf(L"%.*s", u.s->len, u.s->ptr);
		break;
	}
}
void harpdata_test_calc(const harpdata& n1, const harpdata& n2) {
	harpdata h;
	printf("+  "); h = harpdata_calc(n1, n2, op_add); h.Print();
	printf("-  "); h = harpdata_calc(n1, n2, op_sub); h.Print();
	printf("*  "); h = harpdata_calc(n1, n2, op_mul); h.Print();
	printf("/  "); h = harpdata_calc(n1, n2, op_div); h.Print();
	printf("%%  "); h = harpdata_calc(n1, n2, op_mod); h.Print();
	printf("** "); h = harpdata_calc(n1, n2, op_pow); h.Print();
	printf("<  "); h = harpdata_calc(n1, n2, op_lt); h.Print();
	printf("<= "); h = harpdata_calc(n1, n2, op_lte); h.Print();
	printf(">  "); h = harpdata_calc(n1, n2, op_gt); h.Print();
	printf(">= "); h = harpdata_calc(n1, n2, op_gte); h.Print();
	printf("== "); h = harpdata_calc(n1, n2, op_eq); h.Print();
	printf("!= "); h = harpdata_calc(n1, n2, op_neq); h.Print();

}
void harpdata_test() {

	puts("=== CALC TEST ===");
	harpdata h;
	h.f64 = INFINITY / INFINITY;
	h.Print();

	h.byte = (NAN_MARK | 1);
	h.Print();
	h.f64 *= 2;
	h.Print();
	h.f64 = -INFINITY;
	h.Print();
	h.byte = (SNAN_MARK | 1);
	h.Print();
	h.byte = (h.byte | 0x0001000000000000ULL);
	h.Print();
	h.f64 *= 2.0;
	h.Print();
	h.byte = ENCODE_INT(2LL);
	h.Print();

	harpdata n1;
	harpdata n2;
	n1.byte = ENCODE_INT(200);
	n2.byte = ENCODE_INT(3);


	puts("INT & INT");
	harpdata_test_calc(n1, n2);

	n1.byte = ENCODE_INT(200);
	n2.f64 = 3.0;

	puts("INT & FLOAT");
	harpdata_test_calc(n1, n2);




	n1.f64 = 200.0;
	n2.f64 = 3.0;
	puts("FLOAT & FLOAT");
	harpdata_test_calc(n1, n2);

	n1.f64 = 200.0;
	n2.byte = ENCODE_INT(3);
	puts("FLOAT & INT");
	harpdata_test_calc(n1, n2);


	n1.f64 = INFINITY;
	n2.byte = ENCODE_INT(3);
	puts("inf & INT");
	harpdata_test_calc(n1, n2);






}