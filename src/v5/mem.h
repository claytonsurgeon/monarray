#pragma once
#include "common.h"


// WARNING!! care must be taken to insure that log2 is never called on 0, log2 of 0 is undefined
idx gcc_log2(u32 n){
	return 31 - __builtin_clz(n);
}

struct {
	idx table[100];
	idx count;

	u8  mem[500*KB];
	idx end;

} GLOBAL = {0};



// align beginning of points
void ALIGN_MEM() {
	GLOBAL.end = UPPER_ROUND(GLOBAL.end, idx_size);
}


CODE * get_code(idx index) {
	return (CODE *)(GLOBAL.mem+index);
}

inline ARRAY * get_array(idx index) {
	return (ARRAY *)(GLOBAL.mem+index);
}


inline bool * get_bools(idx index, idx element) {
	return (
		(bool*) (GLOBAL.mem + get_array(index)->idx[ gcc_log2(element+1) ])
	) + (
		(element+1) - (1<<gcc_log2(element+1))
	);
}

inline f32 * get_f32s(idx index, idx element) {
	return (
		(f32*) (GLOBAL.mem + get_array(index)->idx[ gcc_log2(element+1) ])
	) + (
		(element+1) - (1<<gcc_log2(element+1))
	);
}
f32*get_f32s_start(idx index, idx E) {
	return (f32*) (((ARRAY *)(GLOBAL.mem+index))->idx[E] + GLOBAL.mem);
}

inline c64 * get_c64s(idx index, idx element) {
	return (
		(c64*) (GLOBAL.mem + get_array(index)->idx[ gcc_log2(element+1) ])
	) + (
		(element+1) - (1<<gcc_log2(element+1))
	);
}




inline VALUE * get_value(idx index) {
	return (VALUE *)(GLOBAL.mem+index);
}

inline VALUE_f32 * get_value_f32(idx index) {
	return (VALUE_f32 *)(GLOBAL.mem+index);
}

inline VALUE_c64 * get_value_c64(idx index) {
	return (VALUE_c64 *)(GLOBAL.mem+index);
}

inline VALUE_bool * get_value_bool(idx index) {
	return (VALUE_bool *)(GLOBAL.mem+index);
}







idx make_array(Kind kind) {
	ALIGN_MEM();
	
	idx index = GLOBAL.end;
	GLOBAL.table[GLOBAL.count++] = index;

	ARRAY*self = get_array(index);
	self->type = ARRAY_;
	self->rank = 1;
	self->kind = kind;
	
	GLOBAL.end += ARRAY_size;
	return index;
}





void allot(uz size, idx index) {
	ALIGN_MEM();
	ARRAY*array = get_array(index);
	array->len = size;

	for (uz max = 1, i = 0; size >= max; max <<= 1, i++) {
		if (!array->idx[i]) {
			array->idx[i] = GLOBAL.end;

			switch(array->kind) {
				case b_1:
					GLOBAL.end += bools_sizes[i];
					break;

				case f_32:
					GLOBAL.end += f32s_sizes[i];
					break;

				case c_64:
					GLOBAL.end += c64s_sizes[i];
					break;

				default:
					panic("allot default");
			}
		}
	}
}



void allot_fixed(uz size, idx index) {
	ALIGN_MEM();
	ARRAY*array = get_array(index);
	array->len = size;

	for (uz max = 1, i = 0; size >= max; max <<= 1, i++) {
		if (!array->idx[i]) {
			array->idx[i] = GLOBAL.end;

			switch(array->kind) {
				case b_1:
					// GLOBAL.end += bools_sizes[i];
					GLOBAL.end += MIN(bools_sizes[i], (size-(1<<i)+1)*x8_size);
					break;

				case f_32:
					// printf("%lu ", MIN(f32s_sizes[i], (size-(1<<i)+1)*x32_size) );
					// GLOBAL.end += f32s_sizes[i];
					GLOBAL.end += MIN(f32s_sizes[i], (size-(1<<i)+1)*x32_size);
					break;

				case c_64:
					GLOBAL.end += MIN(c64s_sizes[i], (size-(1<<i)+1)*x64_size);
					// GLOBAL.end += c64s_sizes[i];
					break;

				default:
					panic("allot default");
			}
		}
	}

	printf("\n");
	
}










idx make_value_f32(f32 x) {
	ALIGN_MEM();
	
	idx index = GLOBAL.end;
	GLOBAL.table[GLOBAL.count++] = index;

	VALUE_f32*self = get_value_f32(index);
	self->type = VALUE_;
	self->kind = f_32;
	self->data = x;

	GLOBAL.end += VALUE_f32_size;
	return index;
}

idx make_value_c64(c64 x) {
	ALIGN_MEM();
	
	idx index = GLOBAL.end;
	GLOBAL.table[GLOBAL.count++] = index;

	VALUE_c64*self = get_value_c64(index);
	self->type = VALUE_;
	self->kind = c_64;
	self->data.real = x.real;
	self->data.imag = x.imag;

	GLOBAL.end += VALUE_c64_size;
	return index;
}


idx make_value_bool(bool x) {
	ALIGN_MEM();
	
	idx index = GLOBAL.end;
	GLOBAL.table[GLOBAL.count++] = index;

	VALUE_bool*self = get_value_bool(index);
	self->type = VALUE_;
	self->kind = b_1;
	self->data = x;

	GLOBAL.end += VALUE_bool_size;
	return index;
}

