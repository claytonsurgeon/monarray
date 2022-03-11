#pragma once
#include "common.h"
#include "mem.h"




void print_value(idx index) {
	switch (get_code(index)->kind) {
		case b_1:
			printf("%u ", get_value_bool(index)->data);
			break;

		case f_32:
			printf("%.2f ", get_value_f32(index)->data);
			break;

		case c_64: {
			c64 x = get_value_c64(index)->data;
			printf("%.2fj%0.2f ", x.real, x.imag);
			break;
		}

		default:
			panic("print_array default");
	}

}


void print_array(idx index) {
	ARRAY*array = get_array(index);
	
	printf(
		"len %u%s "
	, array->len
	, array->len ? ":" : ""
	);


	for (uz i = 0; i < array->len && i < 11; i++) {
		if (i == 10) {
			printf("...");
		}
		else {
			switch (array->kind) {
				case b_1:
					printf("%u ", *get_bools(index, i));
					break;

				case f_32:
					printf("%.2f ", *get_f32s(index, i));
					break;

				case c_64: {
					c64* x = get_c64s(index, i);
					printf("%.2fj%0.2f ", x[0].real, x[0].imag);
					break;
				}

				default:
					panic("print_array default");
			}
		}
	}

}



void print_point(idx index) {
	CODE*code = get_code(index);
	
	printf(
		"@%.4x\t%s %s "
	,	index
	, Type_str[code->type]
	, Kind_str[code->kind]
	);

	switch (code->type) {
		case VALUE_:
			print_value(index);
			break;

		case ARRAY_:
			print_array(index);
			break;

		default:
			panic("print_array default");
	}

	printf("\n");
}



void print_points() {
	for (uz i = 0; i < GLOBAL.count; i++) {
		print_point(GLOBAL.table[i]);
	}
}
