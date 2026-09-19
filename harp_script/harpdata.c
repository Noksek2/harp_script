#include "harpdata.h"
void harpdata_test_calc(const harpdata n1, const harpdata n2) {
	harpdata h;
	printf("+  "); h = harpdata_calc(n1, n2, op_add); harpdata_print(h);
	printf("-  "); h = harpdata_calc(n1, n2, op_sub); harpdata_print(h);
	printf("*  "); h = harpdata_calc(n1, n2, op_mul); harpdata_print(h);
	printf("/  "); h = harpdata_calc(n1, n2, op_div); harpdata_print(h);
	printf("%%  "); h = harpdata_calc(n1, n2, op_mod); harpdata_print(h);
	printf("** "); h = harpdata_calc(n1, n2, op_pow); harpdata_print(h);
	printf("<  "); h = harpdata_calc(n1, n2, op_less); harpdata_print(h);
	printf("<= "); h = harpdata_calc(n1, n2, op_less2); harpdata_print(h);
	printf(">  "); h = harpdata_calc(n1, n2, op_more); harpdata_print(h);
	printf(">= "); h = harpdata_calc(n1, n2, op_more2); harpdata_print(h);
	printf("== "); h = harpdata_calc(n1, n2, op_eq); harpdata_print(h);
	printf("!= "); h = harpdata_calc(n1, n2, op_neq); harpdata_print(h);

}
void harpdata_test() {

	puts("=== CALC TEST ===");
	volatile harpdata h = { .f64 = INFINITY / INFINITY };
	harpdata_print(h);

	h.byte = (NAN_MARK | 1);
	harpdata_print(h);
	h.f64 *= 2;
	harpdata_print(h);
	h.f64 = -INFINITY;
	harpdata_print(h);
	h.byte = (SNAN_MARK | 1);
	harpdata_print(h);
	h.byte = (h.byte | 0x0001000000000000ULL);
	harpdata_print(h);
	h.f64 *= 2.0;
	harpdata_print(h);
	h.byte = ENCODE_INT(2LL);
	harpdata_print(h);

	volatile harpdata n1;
	volatile harpdata n2;
	n1.byte = ENCODE_INT(200);
	n2.byte = ENCODE_INT(3);


	puts("INT & INT");
	harpdata_test_calc(n1, n2);

	n1.byte = ENCODE_INT(200);
	n2.byte = 3.0;

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