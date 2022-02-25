#pragma once
#include "./common.h"

#define KB (1<<10)
#define MB (1<<20)


u8 MEM[500*KB] = {0};
idx END = 0;


void ALIGN_MEM(){
	END = UPPER_ROUND(END, idx_size);
}

idx LOOKUP[500] = {0};
idx LOOKUP_COUNT = 0;


typedef char LABEL[100];
typedef struct _key {
	idx index;
	idx shell;
	char label[100];
} KEY;

KEY KEYS[1000] = {0};
idx key_count = 1;	// reserve first key for DNE signaling









Code * get_code(idx index) {
	return (Code *)(MEM+index);
}

Op0 * get_Op0(idx index) {
	return (Op0 *)(MEM+index);
}

Op0_u8 * get_Op0_u8(idx index) {
	return (Op0_u8 *)(MEM+index);
}

Op0_u32 * get_Op0_u32(idx index) {
	return (Op0_u32 *)(MEM+index);
}

PROGRAM * get_program(idx index) {
	return (PROGRAM *)(MEM+index);
}

GRAPH * get_graph(idx index) {
	return (GRAPH *)(MEM+index);
}

SHAPE * get_shape(idx index) {
	return (SHAPE *)(MEM+index);
}

ARRAY * get_array(idx index) {
	return (ARRAY *)(MEM+index);
}







idx Make_Op0_u8 (u8 z) {
	ALIGN_MEM();
	idx index = END;
	LOOKUP[LOOKUP_COUNT++] = index;

	Op0_u8*op0 = get_Op0_u8(index);
	op0->code.o = VALUE_O;
	op0->code.k = U8_K;
	op0->z = z;

	END += Op0_8_size;
	return index;
}



idx Make_Op0_u32 (u32 z) {
	ALIGN_MEM();
	idx index = END;
	LOOKUP[LOOKUP_COUNT++] = index;

	Op0_u32*op0 = get_Op0_u32(index);
	op0->code.o = VALUE_O;
	op0->code.k = U32_K;
	op0->z = z;

	END += Op0_32_size;
	return index;
}



idx Make_Program () {
	ALIGN_MEM();
	idx index = END;
	LOOKUP[LOOKUP_COUNT++] = index;

	PROGRAM*self = get_program(index);
	self->code.o = PROGRAM_O;
	self->code.k = U32_K;

	END += PROGRAM_size;
	return index;
}



idx Make_Graph (uz rank) {
	ALIGN_MEM();
	idx index = END;
	LOOKUP[LOOKUP_COUNT++] = index;

	GRAPH*self = get_graph(index);
	self->code.o = VALUE_O;
	self->code.k = GRAPH_K;
	self->rank = rank;

	END += GRAPH_size + idx_size*rank;
	return index;
}



idx Make_Shape (uz rank) {
	ALIGN_MEM();
	idx index = END;
	LOOKUP[LOOKUP_COUNT++] = index;

	SHAPE*self = get_shape(index);
	self->code.o = VALUE_O;
	self->code.k = SHAPE_K;
	self->rank = rank;

	END += SHAPE_size + idx_size*rank;
	return index;
}



idx Make_Array (idx shape_idx, idx graph_idx, idx count, idx byte_size) {
	ALIGN_MEM();
	idx index = END;
	LOOKUP[LOOKUP_COUNT++] = index;

	ARRAY*self = get_array(index);
	self->code.variable = get_code(graph_idx)->variable;
	self->code.o = VALUE_O;
	self->code.k = ARRAY_K;
	self->shape = shape_idx;
	self->graph = graph_idx;
	self->count = count;

	END += ARRAY_size + byte_size;
	return index;
}







idx calc_size(idx index) {
	Code*code = get_code(index);
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
			GRAPH*graph = get_graph(index);
			uz size = 0;
			for (uz i = 0; i < graph->rank; i++) {
				size += calc_size(graph->data[i]);
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







void add_label (LABEL label, idx index) {
	KEYS[key_count].index = index;
	KEYS[key_count].shell = 0;
	memcpy(KEYS[key_count].label, label, strlen(label));
	key_count++;
}

bool compare(LABEL a, LABEL b) {
	for (uz i = 0; i < 100; i++) {
		if (a[i] != b[i] || a[i] == 0 || b[i] == 0) {
			return false;
		}
	}
	return true;
}


// an idx of 0 means the key DNE
idx label_to_idx(char label[100]) {
	for (uz i = 0; i < key_count; i++) {
		if (compare(KEYS[i].label, label)) {
			return KEYS[i].index;
		}
	}
	return 0;
}
KEY* get_key(idx index) {
	for (uz i = 0; i < key_count; i++) {
		if (KEYS[i].index == index) {
			return KEYS+i;
		}
	}
	return KEYS+0;

}


void inc_shells(idx graph_idx) {
	GRAPH*graph = get_graph(graph_idx);
	for (uz i = 0; i < graph->rank; i++) {
		idx index = graph->data[i];
		Code*code = get_code(index);
		KEY*key   = get_key(index);

		if (key->index) {
			key->shell++;
			if (code->k == GRAPH_K) {
				inc_shells(index);
			}
		}
	}
}













void DUMP_MEM(uz a, uz z) {
	for (uz i = a; i < z; i++) {
		printf("@%.4lx %.2x\n", i, MEM[i]);
	}
}

