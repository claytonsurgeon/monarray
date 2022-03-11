#pragma once
#include "common.h"
#include "mem.h"
#include "meta.h"



// allow constant time array extension
idx Size(uz size, idx index) {
	ARRAY*array = get_array(index);

	if (size < array->len) {
		array->len = size;
		return index;
	}
	else if (size == array->len) {
		return index;
	}
	else {
		allot(size, index);
		return index;
	}
}

// contiguous allocation for interop (like FFT)
idx Fixed(uz size, idx index) {
	ARRAY*array = get_array(index);

	allot_fixed(size, index);
	return index;
}




idx F32S() {
	return make_array(f_32);
}

idx C64S() {
	return make_array(c_64);
}

idx BOOLS() {
	return make_array(b_1);
}


idx Iota(idx index) {
	ARRAY*array = get_array(index);
	for (uz i = 0; i < array->len; i++) {
		switch (get_array(index)->kind) {

			case f_32:
				get_f32s(index, i)[0] = (f32) i;			
				break;

			case c_64:
				get_c64s(index, i)[0].real = (f32) i;		
			
				break;

			default:
				panic("Iota default");
		}
	}
	return index;
}




idx F32(f32 x) {
	return make_value_f32(x);
}

idx C64(c64 x) {
	return make_value_c64(x);
}

idx BOOL(bool x) {
	return make_value_bool(x);
}







// idx Add_Scalar(idx a_idx, idx b_idx, idx c_idx) {
// 	CODE*a_array = get_code(a_idx);
// 	CODE*b_value = get_code(b_idx);
// 	CODE*c_code = get_code(c_idx);


// 	if (a_code->type == VALUE_) {
		
// 		if (b_code->type == VALUE_) {
// 			if (c_code->type == VALUE_) {

// 			}
// 			else {
// 				printf("ADD a/VALUE, b/VALUE, c/%s\n", Type_str[c_code->type]);
// 				panic("Type mismatch");
// 			}
// 		}
// 	}
// }



void add_scalar(uz max, f32*a, f32 b, f32*c) {
	printf("max %lu\n", max);
	for (uz i = 0; i < max; i++) {
		c[i] = a[i] + b;
	}
}

idx Add_scalar(idx a_idx, idx b_idx, idx c_idx) {
	ARRAY*a = get_array(a_idx);
	VALUE*b = get_value(b_idx);

	// if (a->len != c->len) {
	// 	printf("%u ≠ %u\n", a->len, c->len);
	// 	panic("Shape mismatch\n");
	// }

	ARRAY*c = get_array(c_idx);

	idx len = a->len;
	Size(len, c_idx);


// fall through intentional
	switch (gcc_log2(len)) {
		case 0xF:	add_scalar(MIN(1<<0xF, len-(1<<0xF)+1), get_f32s_start(a_idx, 0xF), get_value_f32(b_idx)->data, get_f32s_start(c_idx, 0xF));
		case 0xE:	add_scalar(MIN(1<<0xE, len-(1<<0xE)+1), get_f32s_start(a_idx, 0xE), get_value_f32(b_idx)->data, get_f32s_start(c_idx, 0xE));
		case 0xD:	add_scalar(MIN(1<<0xD, len-(1<<0xD)+1), get_f32s_start(a_idx, 0xD), get_value_f32(b_idx)->data, get_f32s_start(c_idx, 0xD));
		case 0xC:	add_scalar(MIN(1<<0xC, len-(1<<0xC)+1), get_f32s_start(a_idx, 0xC), get_value_f32(b_idx)->data, get_f32s_start(c_idx, 0xC));
		case 0xB:	add_scalar(MIN(1<<0xB, len-(1<<0xB)+1), get_f32s_start(a_idx, 0xB), get_value_f32(b_idx)->data, get_f32s_start(c_idx, 0xB));
		case 0xA:	add_scalar(MIN(1<<0xA, len-(1<<0xA)+1), get_f32s_start(a_idx, 0xA), get_value_f32(b_idx)->data, get_f32s_start(c_idx, 0xA));
		case 0x9:	add_scalar(MIN(1<<0x9, len-(1<<0x9)+1), get_f32s_start(a_idx, 0x9), get_value_f32(b_idx)->data, get_f32s_start(c_idx, 0x9));
		case 0x8:	add_scalar(MIN(1<<0x8, len-(1<<0x8)+1), get_f32s_start(a_idx, 0x8), get_value_f32(b_idx)->data, get_f32s_start(c_idx, 0x8));
		case 0x7:	add_scalar(MIN(1<<0x7, len-(1<<0x7)+1), get_f32s_start(a_idx, 0x7), get_value_f32(b_idx)->data, get_f32s_start(c_idx, 0x7));
		case 0x6:	add_scalar(MIN(1<<0x6, len-(1<<0x6)+1), get_f32s_start(a_idx, 0x6), get_value_f32(b_idx)->data, get_f32s_start(c_idx, 0x6));
		case 0x5:	add_scalar(MIN(1<<0x5, len-(1<<0x5)+1), get_f32s_start(a_idx, 0x5), get_value_f32(b_idx)->data, get_f32s_start(c_idx, 0x5));
		case 0x4:	add_scalar(MIN(1<<0x4, len-(1<<0x4)+1), get_f32s_start(a_idx, 0x4), get_value_f32(b_idx)->data, get_f32s_start(c_idx, 0x4));
		case 0x3:	add_scalar(MIN(1<<0x3, len-(1<<0x3)+1), get_f32s_start(a_idx, 0x3), get_value_f32(b_idx)->data, get_f32s_start(c_idx, 0x3));
		case 0x2:	add_scalar(MIN(1<<0x2, len-(1<<0x2)+1), get_f32s_start(a_idx, 0x2), get_value_f32(b_idx)->data, get_f32s_start(c_idx, 0x2));
		case 0x1:	add_scalar(MIN(1<<0x1, len-(1<<0x1)+1), get_f32s_start(a_idx, 0x1), get_value_f32(b_idx)->data, get_f32s_start(c_idx, 0x1));
		case 0x0:	add_scalar(1, 									 get_f32s_start(a_idx, 0x0), get_value_f32(b_idx)->data, get_f32s_start(c_idx, 0x0));
			break;

		default:
			panic("Add default");
	}
	
	return 0;

}

void add(uz max, f32*a, f32*b, f32*c) {
	printf("max %lu\n", max);
	for (uz i = 0; i < max; i++) {
		c[i] = a[i] + b[i];
	}
}

idx Add(idx a_idx, idx b_idx, idx c_idx) {
	ARRAY*a = get_array(a_idx);
	ARRAY*b = get_array(b_idx);

	if (a->len != b->len) {
		printf("%u ≠ %u\n", a->len, b->len);
		panic("Shape mismatch\n");
	}

	ARRAY*c = get_array(c_idx);

	idx len = a->len;
	Size(len, c_idx);


// fall through intentional
	switch (gcc_log2(len)) {
		case 0xF:	add(MIN(1<<0xF, len-(1<<0xF)+1), get_f32s_start(a_idx, 0xF), get_f32s_start(b_idx, 0xF), get_f32s_start(c_idx, 0xF));
		case 0xE:	add(MIN(1<<0xE, len-(1<<0xE)+1), get_f32s_start(a_idx, 0xE), get_f32s_start(b_idx, 0xE), get_f32s_start(c_idx, 0xE));
		case 0xD:	add(MIN(1<<0xD, len-(1<<0xD)+1), get_f32s_start(a_idx, 0xD), get_f32s_start(b_idx, 0xD), get_f32s_start(c_idx, 0xD));
		case 0xC:	add(MIN(1<<0xC, len-(1<<0xC)+1), get_f32s_start(a_idx, 0xC), get_f32s_start(b_idx, 0xC), get_f32s_start(c_idx, 0xC));
		case 0xB:	add(MIN(1<<0xB, len-(1<<0xB)+1), get_f32s_start(a_idx, 0xB), get_f32s_start(b_idx, 0xB), get_f32s_start(c_idx, 0xB));
		case 0xA:	add(MIN(1<<0xA, len-(1<<0xA)+1), get_f32s_start(a_idx, 0xA), get_f32s_start(b_idx, 0xA), get_f32s_start(c_idx, 0xA));
		case 0x9:	add(MIN(1<<0x9, len-(1<<0x9)+1), get_f32s_start(a_idx, 0x9), get_f32s_start(b_idx, 0x9), get_f32s_start(c_idx, 0x9));
		case 0x8:	add(MIN(1<<0x8, len-(1<<0x8)+1), get_f32s_start(a_idx, 0x8), get_f32s_start(b_idx, 0x8), get_f32s_start(c_idx, 0x8));
		case 0x7:	add(MIN(1<<0x7, len-(1<<0x7)+1), get_f32s_start(a_idx, 0x7), get_f32s_start(b_idx, 0x7), get_f32s_start(c_idx, 0x7));
		case 0x6:	add(MIN(1<<0x6, len-(1<<0x6)+1), get_f32s_start(a_idx, 0x6), get_f32s_start(b_idx, 0x6), get_f32s_start(c_idx, 0x6));
		case 0x5:	add(MIN(1<<0x5, len-(1<<0x5)+1), get_f32s_start(a_idx, 0x5), get_f32s_start(b_idx, 0x5), get_f32s_start(c_idx, 0x5));
		case 0x4:	add(MIN(1<<0x4, len-(1<<0x4)+1), get_f32s_start(a_idx, 0x4), get_f32s_start(b_idx, 0x4), get_f32s_start(c_idx, 0x4));
		case 0x3:	add(MIN(1<<0x3, len-(1<<0x3)+1), get_f32s_start(a_idx, 0x3), get_f32s_start(b_idx, 0x3), get_f32s_start(c_idx, 0x3));
		case 0x2:	add(MIN(1<<0x2, len-(1<<0x2)+1), get_f32s_start(a_idx, 0x2), get_f32s_start(b_idx, 0x2), get_f32s_start(c_idx, 0x2));
		case 0x1:	add(MIN(1<<0x1, len-(1<<0x1)+1), get_f32s_start(a_idx, 0x1), get_f32s_start(b_idx, 0x1), get_f32s_start(c_idx, 0x1));
		case 0x0:	add(1, 									get_f32s_start(a_idx, 0x0), get_f32s_start(b_idx, 0x0), get_f32s_start(c_idx, 0x0));
			break;

		default:
			panic("Add default");
	}
	
	return 0;

}



void mul(uz max, f32*a, f32*b, f32*c) {
	printf("max %lu\n", max);
	for (uz i = 0; i < max; i++) {
		c[i] = a[i] * b[i];
	}
}

idx Mul(idx a_idx, idx b_idx, idx c_idx) {
	ARRAY*a = get_array(a_idx);
	ARRAY*b = get_array(b_idx);

	if (a->len != b->len) {
		printf("%u ≠ %u\n", a->len, b->len);
		panic("Shape mismatch\n");
	}

	ARRAY*c = get_array(c_idx);

	idx len = a->len;
	Size(len, c_idx);


// fall through intentional
	switch (gcc_log2(len)) {
		case 0xF:	mul(MIN(1<<0xF, len-(1<<0xF)+1), get_f32s_start(a_idx, 0xF), get_f32s_start(b_idx, 0xF), get_f32s_start(c_idx, 0xF));
		case 0xE:	mul(MIN(1<<0xE, len-(1<<0xE)+1), get_f32s_start(a_idx, 0xE), get_f32s_start(b_idx, 0xE), get_f32s_start(c_idx, 0xE));
		case 0xD:	mul(MIN(1<<0xD, len-(1<<0xD)+1), get_f32s_start(a_idx, 0xD), get_f32s_start(b_idx, 0xD), get_f32s_start(c_idx, 0xD));
		case 0xC:	mul(MIN(1<<0xC, len-(1<<0xC)+1), get_f32s_start(a_idx, 0xC), get_f32s_start(b_idx, 0xC), get_f32s_start(c_idx, 0xC));
		case 0xB:	mul(MIN(1<<0xB, len-(1<<0xB)+1), get_f32s_start(a_idx, 0xB), get_f32s_start(b_idx, 0xB), get_f32s_start(c_idx, 0xB));
		case 0xA:	mul(MIN(1<<0xA, len-(1<<0xA)+1), get_f32s_start(a_idx, 0xA), get_f32s_start(b_idx, 0xA), get_f32s_start(c_idx, 0xA));
		case 0x9:	mul(MIN(1<<0x9, len-(1<<0x9)+1), get_f32s_start(a_idx, 0x9), get_f32s_start(b_idx, 0x9), get_f32s_start(c_idx, 0x9));
		case 0x8:	mul(MIN(1<<0x8, len-(1<<0x8)+1), get_f32s_start(a_idx, 0x8), get_f32s_start(b_idx, 0x8), get_f32s_start(c_idx, 0x8));
		case 0x7:	mul(MIN(1<<0x7, len-(1<<0x7)+1), get_f32s_start(a_idx, 0x7), get_f32s_start(b_idx, 0x7), get_f32s_start(c_idx, 0x7));
		case 0x6:	mul(MIN(1<<0x6, len-(1<<0x6)+1), get_f32s_start(a_idx, 0x6), get_f32s_start(b_idx, 0x6), get_f32s_start(c_idx, 0x6));
		case 0x5:	mul(MIN(1<<0x5, len-(1<<0x5)+1), get_f32s_start(a_idx, 0x5), get_f32s_start(b_idx, 0x5), get_f32s_start(c_idx, 0x5));
		case 0x4:	mul(MIN(1<<0x4, len-(1<<0x4)+1), get_f32s_start(a_idx, 0x4), get_f32s_start(b_idx, 0x4), get_f32s_start(c_idx, 0x4));
		case 0x3:	mul(MIN(1<<0x3, len-(1<<0x3)+1), get_f32s_start(a_idx, 0x3), get_f32s_start(b_idx, 0x3), get_f32s_start(c_idx, 0x3));
		case 0x2:	mul(MIN(1<<0x2, len-(1<<0x2)+1), get_f32s_start(a_idx, 0x2), get_f32s_start(b_idx, 0x2), get_f32s_start(c_idx, 0x2));
		case 0x1:	mul(MIN(1<<0x1, len-(1<<0x1)+1), get_f32s_start(a_idx, 0x1), get_f32s_start(b_idx, 0x1), get_f32s_start(c_idx, 0x1));
		case 0x0:	mul(1, 									get_f32s_start(a_idx, 0x0), get_f32s_start(b_idx, 0x0), get_f32s_start(c_idx, 0x0));
			break;

		default:
			panic("mul default");
	}
	
	return 0;

}







void AB() {

	// idx A = Size(100, F32S());


	idx A = Size(1, F32S());
	idx B = Size(1, F32S());

	Size(10, A);
	Size(10, B);

	Size(20, A);
	idx A_fixed = Iota(Fixed(100, F32S()));
	Size(20, B);

	Size(63, A);
	Size(63, B);


	// idx C = F32(20.2);

	Add_scalar(A, F32(20.2), A);
	Add_scalar(B, F32(66.6), B);
	
	// idx A = Iota(1000, C64S());
	// idx B = Size(1000, Iota(25, F32S()));
	// idx C = Size(1, F32S());
	// idx bools = Size(100, BOOLS());

	print_points();

	printf("mem end %u\n", GLOBAL.end);


	// printf("ARRAY  %lu\n", ARRAY_size);
	// printf("f32  %lu\n", f32s_sizes[1]);
	// printf("c64  %lu\n", c64s_sizes[1]);
	// printf("bool %lu\n", VALUE_bool_size);
}