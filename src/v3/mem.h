#pragma once
#include <stdlib.h>
#include "./common.h"


// store all labels in a symbol table and dole indexes to table






global MEM = {0};

void MEM_init() {
	MEM.label_end = 1;	// 0 reserved for DNE
	MEM.label_max = 2;
	MEM.label = malloc(MEM.label_max*label_size);

	memset(MEM.label, 0, MEM.label_max * label_size);


	// MEM.A.max = 100*KB;
	MEM.A.max = 1;
	MEM.A.byte = malloc(MEM.A.max);

	MEM.A.lookup_max = 1;
	MEM.A.lookup = malloc(MEM.A.lookup_max*idx_size);

	// MEM.A.key_max = 2;
	// // skip first key slot
	// MEM.A.key_count = 1;
	// MEM.A.key = malloc(MEM.A.key_max * key_size);
	
	memset(MEM.A.byte, 0, MEM.A.max);
	memset(MEM.A.lookup, 0, MEM.A.lookup_max*idx_size);
	// memset(MEM.A.key, 0, MEM.A.key_max*key_size);

	

	// MEM.B.max = 100*KB;
	MEM.B.max = 1;
	MEM.B.byte = malloc(MEM.B.max);

	MEM.B.lookup_max = 1;
	MEM.B.lookup = malloc(MEM.B.lookup_max*idx_size);

	// MEM.B.key_max = 2;
	// // skip first key slot
	// MEM.B.key_count = 1;
	// MEM.B.key = malloc(MEM.B.key_max * key_size);

	memset(MEM.B.byte, 0, MEM.B.max);
	memset(MEM.B.lookup, 0, MEM.B.lookup_max*idx_size);
	// memset(MEM.B.key, 0, MEM.B.key_max*key_size);

}

void MEM_exit() {
	free(MEM.label);

	free(MEM.A.byte);
	free(MEM.A.lookup);
	// free(MEM.A.key);

	free(MEM.B.byte);
	free(MEM.B.lookup);
	// free(MEM.B.key);
}



void CHECK_LABEL() {
	if (MEM.label_end + label_size >= MEM.label_max) {
		MEM.label_max *= 2;

		printf("label max: %u\n", MEM.label_max);
		label*next_label = malloc(MEM.label_max * label_size);
		if (next_label == NULL) panic("CHECK_LABEL next_label malloc failed");

		memset(next_label, 0, MEM.label_max * label_size);

		memcpy(next_label, MEM.label, MEM.label_end * label_size);
		free(MEM.label);
		MEM.label = next_label;
	}
}


// check if mem bytes needs resizing
void CHECK_MEM(mem*X, uz next_alloc) {
	// printf("%lu %lu %lu %lu %d\n", X->end, X->max, next_alloc, X->end + next_alloc, X->end + next_alloc >= X->max);

	if (X->end + next_alloc >= X->max) {

		do {
			X->max *= 2;
		} while (X->end + next_alloc >= X->max);


		printf("byte max: %u\n", X->max);
		u8 * next_byte = malloc(X->max);
		if (next_byte == NULL) panic("CHECK_MEM next_byte malloc failed");

		memset(next_byte, 0, X->max);

		memcpy(next_byte, X->byte, X->end);
		free(X->byte);
		X->byte = next_byte;

	}

	if (X->lookup_end + 1 >= X->lookup_max) {
		X->lookup_max *= 2;

		printf("lookup max: %u\n", X->lookup_max);
		idx * next_lookup = malloc(X->lookup_max * idx_size);
		if (next_lookup == NULL) panic("CHECK_MEM next_lookup malloc failed");

		memset(next_lookup, 0, X->lookup_max);

		memcpy(next_lookup, X->lookup, X->lookup_end * idx_size);
		free(X->lookup);
		X->lookup = next_lookup;
	}
	
	// if (X->key_count + 1 >= X->key_max) {
	// 	X->key_max *= 2;

	// 	printf("key max: %u\n", X->key_max);
	// 	key * next_key = malloc(X->key_max * key_size);
	// 	if (next_key == NULL) panic("CHECK_MEM next_key malloc failed");

	// 	memset(next_key, 0, X->key_max * key_size);

	// 	memcpy(next_key, X->key, X->key_count * key_size);
	// 	free(X->key);
	// 	X->key = next_key;
	// }

}

// align beginning of points
void ALIGN_MEM(mem*X){
	uz temp = UPPER_ROUND(X->end, idx_size);
	CHECK_MEM(X, temp - X->end);
	X->end = temp;
}



bool compare(label a, label b) {
	for (uz i = 0; i < label_size; i++) {
		if (a[i] != b[i]) {
			return false;
		}
		if (a[i] == 0) {
			return b[i] == 0;
		}
	}
	return true;
}

u16 GET_LABEL(label label) {
	for (uz i = 0; i < MEM.label_end; i++) {
		if (compare(label, MEM.label[i])) {
			return i;
		}
	}

	return 0;
}

u16 ADD_LABEL(label label) {
	CHECK_LABEL();

	u16 index = GET_LABEL(label);
	if (index) return index;

	memcpy(MEM.label[MEM.label_end], label, label_size);
	index = MEM.label_end;
	MEM.label_end++;

	return index;
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

REF * get_ref(mem*X, idx index) {
	return (REF *)(X->byte+index);
}

KEY * get_key(mem*X, idx index) {
	return (KEY *)(X->byte+index);
}





idx Make_Program (mem*X) {
	ALIGN_MEM(X);
	CHECK_MEM(X, PROGRAM_size);


	idx index = X->end;
	X->lookup[X->lookup_end++] = index;


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
	X->lookup[X->lookup_end++] = index;


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
	X->lookup[X->lookup_end++] = index;


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
	X->lookup[X->lookup_end++] = index;


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
	X->lookup[X->lookup_end++] = index;


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
	X->lookup[X->lookup_end++] = index;


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


idx Make_Ref(mem*X, label label) {
	ALIGN_MEM(X);
	CHECK_MEM(X, REF_size);

	idx index = X->end;
	X->lookup[X->lookup_end++] = index;

	u16 label_index = ADD_LABEL(label);

	REF*self = get_ref(X, index);
	self->code.o = VALUE_O;
	self->code.k = REF_K;

	self->index = 0;	// solved in stage 2
	self->shell = 0;
	self->label = label_index;

	X->end += REF_size;
	return index;
}

idx Make_Key(mem*X, idx point, label label) {
	ALIGN_MEM(X);
	CHECK_MEM(X, KEY_size);
	CHECK_LABEL();

	idx index = X->end;
	X->lookup[X->lookup_end++] = index;

	
	u16 label_index = ADD_LABEL(label);


	KEY*self = get_key(X, index);
	self->code.o = VALUE_O;
	self->code.k = KEY_K;
	self->point = point;

	self->shell = 0;
	self->label = label_index;

	self->count = 0;

	X->end += KEY_size;
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

		case IDX_K:										return idx_size;

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













// void add_label (mem*X, label text, idx index) {
// 	CHECK_MEM(X, 0);

// 	X->key[X->key_count].index = index;
// 	X->key[X->key_count].shell = 0;
// 	memcpy(X->key[X->key_count].label, text, strlen(text));
// 	X->key_count++;
// }

// bool compare(label a, label b) {
// 	for (uz i = 0; i < 100; i++) {
// 		if (a[i] != b[i] || a[i] == 0 || b[i] == 0) {
// 			return false;
// 		}
// 	}
// 	return true;
// }


// // an idx of 0 means the key DNE
// idx label_to_idx(mem*X, char label[100]) {
// 	for (uz i = 0; i < X->key_count; i++) {
// 		if (compare(X->key[i].label, label)) {
// 			return X->key[i].index;
// 		}
// 	}
// 	return 0;
// }

// key*get_key(mem*X, idx index) {
// 	for (uz i = 0; i < X->key_count; i++) {
// 		if (X->key[i].index == index) {
// 			return X->key+i;
// 		}
// 	}
// 	return X->key+0;

// }


// void inc_shells(mem*X, idx graph_idx) {
// 	GRAPH*graph = get_graph(X, graph_idx);
// 	for (uz i = 0; i < graph->rank; i++) {
// 		idx index = graph->data[i];
// 		Code*code = get_code(X, index);
// 		key*key   = get_key(X, index);

// 		if (key->index) {
// 			key->shell++;
// 			if (code->k == GRAPH_K) {
// 				inc_shells(X, index);
// 			}
// 		}
// 	}
// }


void inc_shells(mem*X, idx graph_idx) {
	GRAPH*graph = get_graph(X, graph_idx);

	if (graph->code.k != GRAPH_K) panic("inc_shells should only be called on graphs");

	for (uz i = 0; i < graph->rank; i++) {
		Code*code   = get_code(X, graph->data[i]);

		if (code->k == KEY_K) {
			KEY*key = get_key(X, graph->data[i]);
			key->shell++;

			Code*point = get_code(X, key->point);
			if (point->k == GRAPH_K) {
				inc_shells(X, key->point);
			}
			
		}
		else if (code->k == REF_K) {
			get_ref(X, graph->data[i])->shell++;
		}
		else if (code->k == GRAPH_K) {
			inc_shells(X, graph->data[i]);
		}
	}
}
