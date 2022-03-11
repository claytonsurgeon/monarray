#pragma once
#include "./common.h"

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

	memset(MEM.A.byte, 0, MEM.A.max);
	memset(MEM.A.lookup, 0, MEM.A.lookup_max*idx_size);

	

	// MEM.B.max = 100*KB;
	MEM.B.max = 1;
	MEM.B.byte = malloc(MEM.B.max);

	MEM.B.lookup_max = 1;
	MEM.B.lookup = malloc(MEM.B.lookup_max*idx_size);

	memset(MEM.B.byte, 0, MEM.B.max);
	memset(MEM.B.lookup, 0, MEM.B.lookup_max*idx_size);

}

void MEM_exit() {
	free(MEM.label);

	free(MEM.A.byte);
	free(MEM.A.lookup);

	free(MEM.B.byte);
	free(MEM.B.lookup);
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


GRAPH * get_graph(mem*X, idx index) {
	return (GRAPH *)(X->byte+index);
}

ARRAY * get_array(mem*X, idx index) {
	return (ARRAY *)(X->byte+index);
}

ARRAY_u32 * get_array_u32(mem*X, idx index) {
	return (ARRAY_u32 *)(X->byte+index);
}




REF * get_ref(mem*X, idx index) {
	return (REF *)(X->byte+index);
}

KEY * get_key(mem*X, idx index) {
	return (KEY *)(X->byte+index);
}

OP1 * get_op1(mem*X, idx index) {
	return (OP1 *)(X->byte+index);
}

OP2 * get_op2(mem*X, idx index) {
	return (OP2 *)(X->byte+index);
}

PUSH * get_push(mem*X, idx index) {
	return (PUSH *)(X->byte+index);
}




idx Make_Program (mem*X) {
	ALIGN_MEM(X);
	CHECK_MEM(X, PROGRAM_size);


	idx index = X->end;
	X->lookup[X->lookup_end++] = index;


	PROGRAM*self = get_program(X);
	self->parent = 0;
	self->type = PROGRAM_;
	self->size = 0;
	self->hash = 0;

	X->end += PROGRAM_size;
	return index;
}


idx Make_u32 (mem*X, u32 z) {
	ALIGN_MEM(X);
	uz size = ARRAY_size + x32_size;
	CHECK_MEM(X, size);

	idx index = X->end;
	X->lookup[X->lookup_end++] = index;


	ARRAY_u32*self = get_array_u32(X, index);
	self->parent = 0;
	self->type = ARRAY_;
	self->kind = u_32;

	self->size = 1;
	self->data[0] = z;	

	X->end += size;
	return index;
}

idx Make_Array_u32 (mem*X, uz count) {
	ALIGN_MEM(X);
	uz size = ARRAY_size + x32_size * count;
	CHECK_MEM(X, size);

	idx index = X->end;
	X->lookup[X->lookup_end++] = index;


	ARRAY_u32*self = get_array_u32(X, index);
	self->parent = 0;
	self->type = ARRAY_;
	self->kind = u_32;

	self->size = count;	

	X->end += size;
	return index;
}

idx Make_i32 (mem*X, i32 z) {
	ALIGN_MEM(X);
	uz size = ARRAY_size + x32_size;
	CHECK_MEM(X, size);

	idx index = X->end;
	X->lookup[X->lookup_end++] = index;


	ARRAY_i32*self = (ARRAY_i32*)get_array(X, index);
	self->parent = 0;
	self->type = ARRAY_;
	self->kind = i_32;

	self->size = 1;
	self->data[0] = z;	

	X->end += size;
	return index;
}

idx Make_f32 (mem*X, f32 z) {
	ALIGN_MEM(X);
	uz size = ARRAY_size + x32_size;
	CHECK_MEM(X, size);

	idx index = X->end;
	X->lookup[X->lookup_end++] = index;


	ARRAY_f32*self = (ARRAY_f32*)get_array(X, index);
	self->parent = 0;
	self->type = ARRAY_;
	self->kind = f_32;

	self->size = 1;
	self->data[0] = z;	

	X->end += size;
	return index;
}


idx Make_Graph (mem*X, uz count) {
	ALIGN_MEM(X);

	uz size = GRAPH_size + idx_size*count;
	CHECK_MEM(X, size);


	idx index = X->end;
	X->lookup[X->lookup_end++] = index;


	GRAPH*self = get_graph(X, index);
	self->parent = 0;
	self->type = GRAPH_;
	self->size = count;

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
	self->parent = 0;
	self->type = REF_;
	self->index = 0;	// solved in stage2
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
	self->parent = 0;
	self->type = KEY_;
	self->index = point;
	self->shell = 0;
	self->label = label_index;


	X->end += KEY_size;
	return index;
}




idx Make_Op2(mem*X, idx a, idx b) {
	ALIGN_MEM(X);
	CHECK_MEM(X, OP2_size);
	CHECK_LABEL();

	idx index = X->end;
	X->lookup[X->lookup_end++] = index;

	OP2*self = get_op2(X, index);
	self->parent = 0;
	self->type = OP2_;
	self->kind = 0;
	self->op = 0;
	self->hop = 0;

	self->a = a;
	self->b = b;
	self->z = 0;

	X->end += OP2_size;
	return index;
}









// void inc_shells(mem*X, idx graph_idx) {
// 	GRAPH*graph = get_graph(X, graph_idx);

// 	if (graph->type != GRAPH_) panic("inc_shells should only be called on graphs");

// 	for (uz i = 0; i < graph->size; i++) {
// 		Code*code   = get_code(X, graph->data[i]);

// 		if (code->type == KEY_) {
// 			KEY*key = get_key(X, graph->data[i]);
// 			key->shell++;

// 			Code*point = get_code(X, key->index);
// 			if (point->type == GRAPH_) {
// 				inc_shells(X, key->index);
// 			}
			
// 		}
// 		else if (code->type == REF_) {
// 			get_ref(X, graph->data[i])->shell++;
// 		}
// 		else if (code->type == GRAPH_) {
// 			inc_shells(X, graph->data[i]);
// 		}
// 	}
// }



void inc_shells(mem*X, idx index) {
	Code*code = get_code(X, index);

	switch (code->type) {
		case ARRAY_: {
			break;
		}

		case GRAPH_: {
			GRAPH*graph = get_graph(X, index);
			for (uz i = 0; i < graph->size; i++) {
				inc_shells(X, graph->data[i]);
			}
			break;
		}

		case KEY_: {
			KEY*key = get_key(X, index);
			key->shell++;
			inc_shells(X, key->index);
			break;
		}

		case REF_: {
			get_ref(X, index)->shell++;
			break;
		}

		case OP1_: {
			OP1*op1 = get_op1(X, index);
			inc_shells(X, op1->a);
			break;
		}

		case OP2_: {
			OP2*op2 = get_op2(X, index);
			inc_shells(X, op2->a);
			inc_shells(X, op2->b);
			break;
		}

		case PROGRAM_:
		case PUSH_: {
			break;
		}
	}
}
