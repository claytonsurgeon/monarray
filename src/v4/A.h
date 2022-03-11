

#pragma once
#include "common.h"
#include "mem.h"

idx Program() {
	return Make_Program(&MEM.A);
}


// idx U8(u8 x) {
// 	return Make_Op0_u8(&MEM.A, x);
// }


idx _U32(int count, ...) {
	va_list ptr;
	va_start(ptr, count);

	idx index = Make_Array_u32(&MEM.A, count);
	ARRAY_u32*self = get_array_u32(&MEM.A, index);

	for (uz i = 0; i < self->size; i++) {
		u32 element = va_arg(ptr, int);
		self->data[i] = element;
	}

	return index;
}
#define U32(...)  (_U32(NUMARGS(__VA_ARGS__), __VA_ARGS__))

idx I32(i32 x) {
	return Make_i32(&MEM.A, x);
}

idx F32(f32 x) {
	return Make_f32(&MEM.A, x);
}





idx _Graph(int count, ...) {
	va_list ptr;
	va_start(ptr, count);

	idx index = Make_Graph(&MEM.A, count);
	GRAPH*self = get_graph(&MEM.A, index);

	for (uz i = 0; i < self->size; i++) {
		idx child_idx = va_arg(ptr, int);
		Code*child = get_code(&MEM.A, child_idx);

		child->parent = index;

		self->data[i] = child_idx;
	}

	inc_shells(&MEM.A, index);

	return index;
}
#define Graph(...)  (_Graph(NUMARGS(__VA_ARGS__), __VA_ARGS__))





idx Key(label text, idx point) {
	idx index = Make_Key(&MEM.A, point, text);
	Code*child = get_code(&MEM.A, point);
	child->parent = index;

	return index;
}

idx Ref(label text) {
	return Make_Ref(&MEM.A, text);
}













idx Add(idx a, idx b) {
	idx index = Make_Op2(&MEM.A, a, b);
	OP2*self = get_op2(&MEM.A, index);

	self->op = add_;

	return index;
}