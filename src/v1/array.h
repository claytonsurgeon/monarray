#pragma once
#include "common.h"
#include "meta.h"
#include "mem.h"

#define NUMARGS(...)  (sizeof((int[]){__VA_ARGS__})/sizeof(int))
#define SUM(...)  (sum(NUMARGS(__VA_ARGS__), __VA_ARGS__))



idx U8(u8 x) {
	return Make_Op0_u8(x);
}


idx U32(u32 x) {
	return Make_Op0_u32(x);
}



idx Program() {
	return Make_Program();
}



idx _Graph(int count, ...) {
	va_list ptr;
	va_start(ptr, count);

	idx index = Make_Graph(count);
	GRAPH*self = get_graph(index);

	for (uz i = 0; i < count; i++) {
		idx child_idx = va_arg(ptr, int);
		Code*child = get_code(child_idx);

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

	idx index = Make_Shape(count-1); // don't count base
	SHAPE*self = get_shape(index);

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
	if(get_code(shape_idx)->k == GRAPH_K && get_code(graph_idx)->k == SHAPE_K) {
		idx temp = shape_idx;
		shape_idx = graph_idx;
		graph_idx = temp;
	}

	GRAPH*graph = get_graph(graph_idx);
	SHAPE*shape = get_shape(shape_idx);
	
	idx base = shape->base;
	idx base_size = calc_size(base);

	idx count = 1;
	for (uz i = 0; i < shape->rank; i++) {
		count *= shape->data[i];
	}

	idx byte_size = count * base_size;


	printf("count %u\n", count);


	idx index = Make_Array(shape_idx, graph_idx, count, byte_size);
	ARRAY*array = get_array(index);

	for (uz i = 0; i < array->count; i++) {
		if (i < graph->rank) {
			Op0*op0 = get_Op0(graph->data[i]);
			uz copy_size = MIN(calc_size(graph->data[i]), base_size);
			memcpy(
				array->data+(i * base_size), op0->z, copy_size
			);
		}
	}

	return index;
}



// idx Reshape(){}

idx Variable(idx index) {
	get_code(index)->variable = true;
	return index;
}




idx Key(LABEL label, idx point) {
	add_label(label, point);
	Code*code = get_code(point);

	if (code->k == GRAPH_K) {
		inc_shells(point);
	}

	return point;
}



void runner () {


	idx head = Program();

	// Cast(
	// 	Shape(2,2500, U32(0)),
	// 	Graph(
	// 		U32(1001), U8(2), U8(3),
	// 		U8(4), U8(255), U8(6),
	// 		U32(738945739), U8(8), U8(9)
	// 	)
	// );

	Key("Monkey", Graph(
		Key("Hamster", Graph(
			Key("duck", Graph(
				Key("a", U32(20)),
				Key("b", U32(30))
			)),
			Key("penguin", Cast(
				Shape(2,2500, U32(0)),
				Graph(
					U32(1001), U8(2), U8(3),
					U8(4), U8(255), U8(6),
					U32(738945739), U8(8), U8(9)
				)
			))
		))
	));


	// printf("%s\n", idx_to_key(0x24)->label);

	idx duck = Graph(
		U8(10),
		U32(10000),
		Graph(
			Variable(U8(10)),
			U32(10000),
			U8(20)
		),
		U8(20)
	);

	// printf("duck %u\n", calc_size(duck));

	get_Op0_u32(head)->z = END;

	// DUMP_MEM(0, 20);
	print_points();

	printf("%lu\n", idx_size);
}