

#pragma once
#include "common.h"
#include "mem.h"


/** 'A', STAGE 1, first pass, etc
 * - Compile AST
 * - Resolve available references 
 * - Determine available types, sizes, dimensions
 */





#define NUMARGS(...)  (sizeof((int[]){__VA_ARGS__})/sizeof(int))
#define SUM(...)  (sum(NUMARGS(__VA_ARGS__), __VA_ARGS__))


idx Program() {
	return Make_Program(&MEM.A);
}


idx U8(u8 x) {
	return Make_Op0_u8(&MEM.A, x);
}


idx U32(u32 x) {
	return Make_Op0_u32(&MEM.A, x);
}




idx _Graph(int count, ...) {
	va_list ptr;
	va_start(ptr, count);

	idx index = Make_Graph(&MEM.A, count);
	GRAPH*self = get_graph(&MEM.A, index);

	for (uz i = 0; i < count; i++) {
		idx child_idx = va_arg(ptr, int);
		Code*child = get_code(&MEM.A, child_idx);

		self->code.variable = self->code.variable || child->variable;
		child->parent = index;

		self->data[i] = child_idx;
	}

	return index;
}
#define Graph(...)  (_Graph(NUMARGS(__VA_ARGS__), __VA_ARGS__))





idx _Shape(int count, ...) {
	va_list ptr;
	va_start(ptr, count);

	idx index = Make_Shape(&MEM.A, count-1); // don't count base
	SHAPE*self = get_shape(&MEM.A, index);

	for (uz i = 0; i < count; i++) {
		idx dimension = va_arg(ptr, int);
		if (i == count-1) {
			self->base = dimension;
		}
		else {
			self->data[i] = dimension;
		}
	}

	return index;
}
#define Shape(...)  (_Shape(NUMARGS(__VA_ARGS__), __VA_ARGS__))



idx Cast(idx shape_idx, idx graph_idx) {
	if(get_code(&MEM.A, shape_idx)->k == GRAPH_K
	&& get_code(&MEM.A, graph_idx)->k == SHAPE_K) {
		idx temp = shape_idx;
		shape_idx = graph_idx;
		graph_idx = temp;
	}

	GRAPH*graph = get_graph(&MEM.A, graph_idx);
	SHAPE*shape = get_shape(&MEM.A, shape_idx);
	
	idx base = shape->base;
	idx base_size = calc_size(&MEM.A, base);

	idx count = 1;
	for (uz i = 0; i < shape->rank; i++) {
		count *= shape->data[i];
	}

	idx byte_size = count * base_size;


	printf("number of elements %u\n", count);


	idx index = Make_Array(&MEM.A, shape_idx, graph_idx, count, byte_size);

	// graph and shape pointers must be recreated in case 'Make_Array' resizes bytes and invalidates pointers
	// this could have been an ugly bug, keep an eye out for such cases
	graph = get_graph(&MEM.A, graph_idx);
	shape = get_shape(&MEM.A, shape_idx);

	ARRAY*array = get_array(&MEM.A, index);

	for (uz i = 0; i < array->count; i++) {
		if (i < graph->rank) {
			Op0*op0 = get_Op0(&MEM.A, graph->data[i]);
			uz copy_size = MIN(calc_size(&MEM.A, graph->data[i]), base_size);
			memcpy(
				array->data+(i * base_size), op0->z, copy_size
			);
		}
	}

	return index;
}





idx Variable(idx index) {
	get_code(&MEM.A, index)->variable = true;
	return index;
}



idx Key(label text, idx point) {
	add_label(&MEM.A, text, point);
	Code*code = get_code(&MEM.A, point);

	if (code->k == GRAPH_K) {
		inc_shells(&MEM.A, point);
	}

	return point;
}
