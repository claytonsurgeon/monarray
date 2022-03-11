#pragma once
#include <stdlib.h>
#include "./common.h"



typedef char label[100];
typedef struct _key {
	idx index;
	idx shell;
	label label;
} key;
#define key_size (sizeof(key))


typedef struct _mem {
	u8*byte;
	uz end;
	uz byte_max;

	idx*lookup;
	idx count;
	idx lookup_max;
	
	key*key;
	idx key_count;
	idx key_max;
} mem;

typedef struct _global {
	mem A;
	mem B;

} global;


global MEM = {0};

void MEM_init() {
	// MEM.A.byte_max = 100*KB;
	MEM.A.byte_max = 1;
	MEM.A.byte = malloc(MEM.A.byte_max);

	MEM.A.lookup_max = 1;
	MEM.A.lookup = malloc(MEM.A.lookup_max*idx_size);

	MEM.A.key_max = 2;
	// skip first key slot
	MEM.A.key_count = 1;
	MEM.A.key = malloc(MEM.A.key_max * key_size);
	
	memset(MEM.A.byte, 0, MEM.A.byte_max);
	memset(MEM.A.lookup, 0, MEM.A.lookup_max*idx_size);
	memset(MEM.A.key, 0, MEM.A.key_max*key_size);

	

	// MEM.B.byte_max = 100*KB;
	MEM.B.byte_max = 1;
	MEM.B.byte = malloc(MEM.B.byte_max);

	MEM.B.lookup_max = 1;
	MEM.B.lookup = malloc(MEM.B.lookup_max*idx_size);

	MEM.B.key_max = 2;
	// skip first key slot
	MEM.B.key_count = 1;
	MEM.B.key = malloc(MEM.B.key_max * key_size);

	memset(MEM.B.byte, 0, MEM.B.byte_max);
	memset(MEM.B.lookup, 0, MEM.B.lookup_max*idx_size);
	memset(MEM.B.key, 0, MEM.B.key_max*key_size);

}

void MEM_exit() {
	free(MEM.A.byte);
	free(MEM.A.lookup);
	free(MEM.A.key);

	free(MEM.B.byte);
	free(MEM.B.lookup);
	free(MEM.B.key);
}




// check if mem bytes needs resizing
void CHECK_MEM(mem*X, uz next_alloc) {
	// printf("%lu %lu %lu %lu %d\n", X->end, X->byte_max, next_alloc, X->end + next_alloc, X->end + next_alloc >= X->byte_max);

	if (X->end + next_alloc >= X->byte_max) {

		do {
			X->byte_max *= 2;
		} while (X->end + next_alloc >= X->byte_max);


		printf("byte max: %lu\n", X->byte_max);
		u8 * next_byte = malloc(X->byte_max);
		if (next_byte == NULL) panic("CHECK_MEM next_byte malloc failed");

		memset(next_byte, 0, X->byte_max);

		memcpy(next_byte, X->byte, X->end);
		free(X->byte);
		X->byte = next_byte;

	}

	if (X->count + 1 >= X->lookup_max) {
		X->lookup_max *= 2;

		printf("lookup max: %u\n", X->lookup_max);
		idx * next_lookup = malloc(X->lookup_max * idx_size);
		if (next_lookup == NULL) panic("CHECK_MEM next_lookup malloc failed");

		memset(next_lookup, 0, X->lookup_max);

		memcpy(next_lookup, X->lookup, X->count * idx_size);
		free(X->lookup);
		X->lookup = next_lookup;
	}
	
	if (X->key_count + 1 >= X->key_max) {
		X->key_max *= 2;

		printf("key max: %u\n", X->key_max);
		key * next_key = malloc(X->key_max * key_size);
		if (next_key == NULL) panic("CHECK_MEM next_key malloc failed");

		memset(next_key, 0, X->key_max * key_size);

		memcpy(next_key, X->key, X->key_count * key_size);
		free(X->key);
		X->key = next_key;
	}

}

// align beginning of points
void ALIGN_MEM(mem*X){
	uz temp = UPPER_ROUND(X->end, idx_size);
	CHECK_MEM(X, temp - X->end);
	X->end = temp;
}


Code * get_code(mem*X, idx index) {
	return (Code *)(X->byte+index);
}

PROGRAM * get_program(mem*X) {
	return (PROGRAM *)(X->byte+0);
}

Op0 * get_Op0(mem*X, idx index) {
	return (Op0 *)(X->byte+index);
}

Op0_u8 * get_Op0_u8(mem*X, idx index) {
	return (Op0_u8 *)(X->byte+index);
}

Op0_u32 * get_Op0_u32(mem*X, idx index) {
	return (Op0_u32 *)(X->byte+index);
}

GRAPH * get_graph(mem*X, idx index) {
	return (GRAPH *)(X->byte+index);
}

SHAPE * get_shape(mem*X, idx index) {
	return (SHAPE *)(X->byte+index);
}

ARRAY * get_array(mem*X, idx index) {
	return (ARRAY *)(X->byte+index);
}





idx Make_Program (mem*X) {
	ALIGN_MEM(X);
	CHECK_MEM(X, PROGRAM_size);


	idx index = X->end;
	X->lookup[X->count++] = index;


	PROGRAM*self = get_program(X);
	self->code.o = PROGRAM_O;
	self->code.k = U32_K;

	X->end += PROGRAM_size;
	return index;
}




idx Make_Op0_u8 (mem*X, u8 z) {
	ALIGN_MEM(X);
	CHECK_MEM(X, Op0_8_size);


	idx index = X->end;
	X->lookup[X->count++] = index;


	Op0_u8*op0 = get_Op0_u8(X, index);
	op0->code.o = VALUE_O;
	op0->code.k = U8_K;
	op0->z = z;

	X->end += Op0_8_size;
	return index;
}


idx Make_Op0_u32 (mem*X, u32 z) {
	ALIGN_MEM(X);
	CHECK_MEM(X, Op0_32_size);


	idx index = X->end;
	X->lookup[X->count++] = index;


	Op0_u32*op0 = get_Op0_u32(X, index);
	op0->code.o = VALUE_O;
	op0->code.k = U32_K;
	op0->z = z;

	X->end += Op0_32_size;
	return index;
}




idx Make_Graph (mem*X, uz rank) {
	ALIGN_MEM(X);

	uz size = GRAPH_size + idx_size*rank;
	CHECK_MEM(X, size);


	idx index = X->end;
	X->lookup[X->count++] = index;


	GRAPH*self = get_graph(X, index);
	self->code.o = VALUE_O;
	self->code.k = GRAPH_K;
	self->rank = rank;

	X->end += size;
	return index;
}

idx Make_Shape (mem*X, uz rank) {
	ALIGN_MEM(X);

	uz size = SHAPE_size + idx_size*rank;
	CHECK_MEM(X, size);


	idx index = X->end;
	X->lookup[X->count++] = index;


	SHAPE*self = get_shape(X, index);
	self->code.o = VALUE_O;
	self->code.k = SHAPE_K;
	self->rank = rank;

	X->end += size;
	return index;
}


idx Make_Array (mem*X, idx shape_idx, idx graph_idx, idx count, idx byte_size) {
	ALIGN_MEM(X);

	uz size = ARRAY_size + byte_size;
	CHECK_MEM(X, size);


	idx index = X->end;
	X->lookup[X->count++] = index;


	ARRAY*self = get_array(X, index);
	self->code.variable = get_code(X, graph_idx)->variable;
	self->code.o = VALUE_O;
	self->code.k = ARRAY_K;
	self->shape = shape_idx;
	self->graph = graph_idx;
	self->count = count;

	X->end += size;
	return index;
}








idx calc_size(mem*X, idx index) {
	Code*code = get_code(X, index);
	OP op = code->o;
	KIND kind = code->k;

	// printf("calc_size: %s\n", KIND_str[kind]);

	switch (kind) {
		case U8_K:  case I8_K:						return 1;
		case U16_K: case I16_K:						return 2;
		case U32_K: case I32_K: case F32_K:		return 4;
		case U64_K: case I64_K: case F64_K:		return 8;

		case IDX_K:										return 4;

		case ARRAY_K: {
			panic("mem/calc_size/op array");
			break;
		}

		case SHAPE_K: {
			panic("mem/calc_size/op shape");
			break;
		}

		case GRAPH_K: {
			GRAPH*graph = get_graph(X, index);
			uz size = 0;
			for (uz i = 0; i < graph->rank; i++) {
				size += calc_size(X, graph->data[i]);
			}
			return size;
			break;
		}

		default:
			panic("mem/calc_size/kind default");
			break;
	}

	
	panic("mem/calc_size returning 0");
	return 0;
}













void add_label (mem*X, label text, idx index) {
	CHECK_MEM(X, 0);

	X->key[X->key_count].index = index;
	X->key[X->key_count].shell = 0;
	memcpy(X->key[X->key_count].label, text, strlen(text));
	X->key_count++;
}

bool compare(label a, label b) {
	for (uz i = 0; i < 100; i++) {
		if (a[i] != b[i] || a[i] == 0 || b[i] == 0) {
			return false;
		}
	}
	return true;
}


// an idx of 0 means the key DNE
idx label_to_idx(mem*X, char label[100]) {
	for (uz i = 0; i < X->key_count; i++) {
		if (compare(X->key[i].label, label)) {
			return X->key[i].index;
		}
	}
	return 0;
}

key*get_key(mem*X, idx index) {
	for (uz i = 0; i < X->key_count; i++) {
		if (X->key[i].index == index) {
			return X->key+i;
		}
	}
	return X->key+0;

}


void inc_shells(mem*X, idx graph_idx) {
	GRAPH*graph = get_graph(X, graph_idx);
	for (uz i = 0; i < graph->rank; i++) {
		idx index = graph->data[i];
		Code*code = get_code(X, index);
		key*key   = get_key(X, index);

		if (key->index) {
			key->shell++;
			if (code->k == GRAPH_K) {
				inc_shells(X, index);
			}
		}
	}
}

