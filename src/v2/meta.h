#pragma once
#include "common.h"
// #include "array.h"
#include "mem.h"

typedef char * str;

// max 18446744073709551615, len 20
void print_U(u64 x) {
	printf("%lu", x);
}
void print_idx(idx index) {
	printf("@%.4x", index);
}

void print_program(mem*X, idx index) {
	PROGRAM*program = get_program(X);
	print_U(program->z);
}




void print_data(mem*X, idx base, void* data) {
	Code*code = get_code(X, base);

	switch (code->k) {
		case U8_K:
			print_U(((u8*) data)[0]);
			break;
		case U32_K:
			print_U(((u32*) data)[0]);
			break;

		default:
			printf("ERROR ON %s\t AT ", KIND_str[code->k]);
			panic("print_data/op default");
			break;
	}
}




void print_graph(mem*X, idx index) {
	GRAPH*graph = get_graph(X, index);
	printf("{ ");
	for (uz i = 0; i < graph->rank; i++) {
		print_idx(graph->data[i]);
		printf(" ");
	}
	printf("}");
}

void print_shape(mem*X, idx index) {
	SHAPE*shape = get_shape(X, index);
	printf("[");
	for (uz i = 0; i < shape->rank; i++) {
		print_U(shape->data[i]);
		printf(" ");
	}
	print_idx(shape->base);
	printf("]");
}


void print_array(mem*X, idx index) {
	ARRAY*array = get_array(X, index);
	SHAPE*shape = get_shape(X, array->shape);
	idx base = shape->base;
	print_idx(array->shape); printf(" ");
	print_idx(array->graph); printf(" ");
	printf("{ ");
	for (uz i = 0; i < array->count && i < 11; i++) {
		if (i == 10) {
			printf("...");
		}
		else {
			print_data(X, base, (void*) (array->data+(i * calc_size(X, base))) );
		}
		printf(" ");
	}
	// print_idx(array->shape);
	printf("}");
}


void print_switch_k (mem*X, idx index) {
	switch(get_code(X, index)->k) {
		case U8_K: {
			print_U(get_Op0_u8(X, index)->z);
			break;
		}
		case U32_K: {
			print_U(get_Op0_u32(X, index)->z);
			break;
		}

		case GRAPH_K: {
			print_graph(X, index);
			break;
		}

		case SHAPE_K: {
			print_shape(X, index);
			break;
		}

		case ARRAY_K: {
			print_array(X, index);
			break;
		}

		default:
			panic("meta/print_switch_k default");
			break;
	}
}


void print_code1(mem*X, idx index) {
	Code * code = get_code(X, index);
	key * key = get_key(X, index);
	printf(
		"%-2u %10s%s  @%.4x: %-4s%-6s @%.4x %s: "
		// "%10s @%.4x: %-4s%-6s @%.4x %s: "
		,	key->index ? key->shell : 0
		,	key->index ? key->label : ""
		,	key->index ? ":" : " "
		,	index
		,	OP_str[code->o]
		,	KIND_str[code->k]
		,	code->parent
		,	code->variable ? "variable" : "constant"
	);
}

void print_code2(mem*X, idx index) {
	Code * code = get_code(X, index);
	key * key = get_key(X, index);
	printf(
		"%-2u %10s%s  @%.4x: %-10s @%.4x %s: "
		// "%10s @%.4x: %-10s @%.4x %s: "
		// "@%.4x: %-10s @%.4x %s: "
		,	key->index ? key->shell : 0
		,	key->index ? key->label : " "
		,	key->index ? ":" : " "
		,	index
		,	OP_str[code->o]
		,	code->parent
		,	code->variable ? "variable" : "constant"
	);
}

void print_point(mem*X, idx index) {
	
	switch(get_code(X, index)->o) {
		case VALUE_O: {
			print_code1(X, index);
			print_switch_k(X, index);
			break;
		}

		case PROGRAM_O: {
			print_code2(X, index);
			print_program(X, index);
			break;
		}


		default:
			printf("%u", get_code(X, index)->o);
			panic("meta/print_point default");
			break;
	}
}


void print_points(mem*X) {
	for (uz i = 0; i < X->count; i++) {
		print_point(X, X->lookup[i]);
		printf("\n");
	}
}



void print_mem(mem*X) {
	printf("%p\t%lu %lu\n%p\t%u\n", X->byte, X->byte_max, X->end, X->lookup, X->count);
}