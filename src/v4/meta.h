
#pragma once
#include "common.h"
#include "mem.h"


void print_program(mem*X, idx index) {
	PROGRAM*program = get_program(X);
	printf("%u", program->size);
}


void print_array(mem*X, idx index) {
	ARRAY*array = get_array(X, index);

	
	printf("kind %s, size %u {", Kind_str[array->kind], array->size);

	switch(array->kind) {

		case u_32: {
			ARRAY_u32*array_u32 = (ARRAY_u32 *) array;
			for (uz i = 0; i < array_u32->size; i++) {
				printf(" %u", array_u32->data[i]);
			}
			break;
		}

		case i_32: {
			ARRAY_i32*array_i32 = (ARRAY_i32 *) array;
			for (uz i = 0; i < array_i32->size; i++) {
				printf(" %d", array_i32->data[i]);
			}
			break;
		}

		case f_32: {
			ARRAY_f32*array_f32 = (ARRAY_f32 *) array;
			for (uz i = 0; i < array_f32->size; i++) {
				printf(" %f", array_f32->data[i]);
			}
			break;
		}

		default:
			printf("%u %s", array->kind, Kind_str[array->kind]);
			panic("meta/print_array default");
			break;
	}

	printf(" }");
}

void print_graph(mem*X, idx index) {
	GRAPH*graph = get_graph(X, index);

	
	printf("size %u {", graph->size);
	for (uz i = 0; i < graph->size; i++) {
		printf(" @%.4x", graph->data[i]);
	}
	printf(" }");
}


void print_ref(mem*X, idx index) {
	REF*ref = get_ref(X, index);
	printf("%s: shell: %u index: @%.4x", MEM.label[ref->label], ref->shell, ref->index);
}

void print_key(mem*X, idx index) {
	KEY*key = get_key(X, index);
	printf("%s: shell: %u index: @%.4x", MEM.label[key->label], key->shell, key->index);
}

void print_op2(mem*X, idx index) {
	OP2*op2 = get_op2(X, index);
	printf("(%s %s %s) @%.4x, @%.4x = @%.4x"
	,	Hop_str[op2->hop]
	,	Op_str[op2->op]
	,	Kind_str[op2->kind]
	,	op2->a
	,	op2->b
	,	op2->z
	);
}



void print_code(mem*X, idx index) {
	Code * code = get_code(X, index);
	printf(
		"@%.4x: %-10s @%.4x: "
		,	index
		,	Type_str[code->type]
		,	code->parent
	);
}



void print_point(mem*X, idx index) {
	
	switch(get_code(X, index)->type) {
		case PROGRAM_: {
			print_code(X, index);
			print_program(X, index);
			break;
		}

		case ARRAY_: {
			print_code(X, index);
			print_array(X, index);
			break;
		}

		case GRAPH_: {
			print_code(X, index);
			print_graph(X, index);
			break;
		}

		case KEY_: {
			print_code(X, index);
			print_key(X, index);
			break;
		}

		case REF_: {
			print_code(X, index);
			print_ref(X, index);
			break;
		}

		case OP2_: {
			print_code(X, index);
			print_op2(X, index);
			break;
		}



		default:
			printf("ERROR: %s\n", Type_str[get_code(X, index)->type]);
			panic("meta/print_point default");
			break;
	}
}




void print_points(mem*X) {
	for (uz i = 0; i < X->lookup_end; i++) {
		print_point(X, X->lookup[i]);
		printf("\n");
	}
}



void print_mem(mem*X) {
	printf("%p\t%u %u\n%p\t%u\n", X->byte, X->max, X->end, X->lookup, X->lookup_end);
}