#pragma once
#include "common.h"
#include "array.h"
#include "mem.h"

typedef char * str;

// max 18446744073709551615, len 20
void print_U(u64 x) {
	printf("%lu", x);
}
void print_idx(idx index) {
	printf("@%.4x", index);
}

void print_program(idx index) {
	PROGRAM*program = get_program(index);
	print_U(program->z);
}




void print_data(idx base, void* data) {
	Code*code = get_code(base);

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




void print_graph(idx index) {
	GRAPH*graph = get_graph(index);
	printf("{ ");
	for (uz i = 0; i < graph->rank; i++) {
		print_idx(graph->data[i]);
		printf(" ");
	}
	printf("}");
}

void print_shape(idx index) {
	SHAPE*shape = get_shape(index);
	printf("[");
	for (uz i = 0; i < shape->rank; i++) {
		print_U(shape->data[i]);
		printf(" ");
	}
	print_idx(shape->base);
	printf("]");
}


void print_array(idx index) {
	ARRAY*array = get_array(index);
	SHAPE*shape = get_shape(array->shape);
	idx base = shape->base;
	print_idx(array->shape); printf(" ");
	print_idx(array->graph); printf(" ");
	printf("{ ");
	for (uz i = 0; i < array->count && i < 11; i++) {
		if (i == 10) {
			printf("...");
		}
		else {
			print_data(base, (void*) (array->data+(i * calc_size(base))) );
		}
		printf(" ");
	}
	// print_idx(array->shape);
	printf("}");
}


void print_switch_k (idx index) {
	switch(get_code(index)->k) {
		case U8_K: {
			print_U(get_Op0_u8(index)->z);
			break;
		}
		case U32_K: {
			print_U(get_Op0_u32(index)->z);
			break;
		}

		case GRAPH_K: {
			print_graph(index);
			break;
		}

		case SHAPE_K: {
			print_shape(index);
			break;
		}

		case ARRAY_K: {
			print_array(index);
			break;
		}

		default:
			panic("meta/print_switch_k default");
			break;
	}
}


void print_code1(idx index) {
	Code * code = get_code(index);
	KEY * key = get_key(index);
	printf(
		"%-2u %10s%s  @%.4x: %-4s%-6s @%.4x %s: "
		// "@%.4x: %-4s%-6s @%.4x %s: "
		,	key->index ? get_key(index)->shell : 0
		,	key->index ? get_key(index)->label : ""
		,	key->index ? ":" : " "
		,	index
		,	OP_str[code->o]
		,	KIND_str[code->k]
		,	code->parent
		,	code->variable ? "variable" : "constant"
	);
}

void print_code2(idx index) {
	Code * code = get_code(index);
	KEY * key = get_key(index);
	printf(
		"%-2u %10s%s  @%.4x: %-10s @%.4x %s: "
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

void print_point(idx index) {
	
	switch(get_code(index)->o) {
		case VALUE_O: {
			print_code1(index);
			print_switch_k(index);
			break;
		}

		case PROGRAM_O: {
			print_code2(index);
			print_program(index);
			break;
		}


		default:
			panic("meta/print_point default");
			break;
	}
}


void print_points() {
	for (uz i = 0; i < LOOKUP_COUNT; i++) {
		print_point(LOOKUP[i]);
		printf("\n");
	}
}