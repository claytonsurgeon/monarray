#pragma once
#include "./common.h"

#define KB (1<<10)
#define MB (1<<20)

#define get_code(i) ((Code *) (MEM+i))
#define get_data(T, i) ((T *) (MEM+i+CodeSize))

#define get_rankx(i) ((Rankx *) (get_code(i)->shape))
#define get_rank1(i) ((Rank1 *) (get_code(i)->shape))

#define rankx_data(i) (get_rankx(i)->lens + (get_rankx(i)->rank))
#define rank1_data(i) (get_code(i)->shape + 1)
#define rank0_data(i) (get_code(i)->shape)

#define T(t, x) ((t *) x)


u8	MEM[500*KB];
idx END = 0;


idx LOOKUP[500] = {0};
u32 LOOKUP_COUNT = 0;



typedef struct _key {
	idx index;
	idx shell;
	char label[100];
} key;

key KEYS[1000] = {0};
idx key_count = 0;




void ALIGN_MEM(){
	END = UPPER_ROUND(END, IdxSize);
}

idx MEM_Push_Code(OP op) {
	#ifdef WORD_ALIGN
	ALIGN_MEM();
	#endif

	idx index = END;
	LOOKUP[LOOKUP_COUNT++] = index;

	Code code = {
		.code = op,
		.variable = false,
		.parent = 0,
	};

	memcpy(MEM+END, &code, CodeSize);
	END += CodeSize;
	
	return index;
}

void MEM_Push_Data(void * data, uz byte_size) {
	memcpy(MEM+END, data, byte_size);
	END += byte_size;
}

void MEM_Push_Data_sized(void * data, uz byte_size, uz size) {
	memcpy(MEM+END, data, byte_size);
	END += size;
}

void MEM_Push_Zero_Data(uz byte_size) {
	memset(MEM+END, 0, byte_size);
	END += byte_size;
}



void MEM_Push_Rank1(idx index, idx len) {
	get_rank1(index)->len = len;
	END += Rank1_Size;
}

void MEM_Push_Rankx(idx index) {
	// get_rankx(index)->rank = rank;
	END += Rankx_Size;
}

void MEM_Push_Rankx_len(idx index, idx len) {
	get_rankx(index)->lens[get_rankx(index)->rank++] = len;
	END += IdxSize;
}

void MEM_Set_Rankx_Base(idx index, idx base) {
	get_rankx(index)->base = base;
}





idx CalcSize(idx base) {
	OP code = get_code(base)->code;

	switch(code) {
		case U8_OP:  case I8_OP:
		case U32_OP: case I32_OP: case F32_OP:
		case U64_OP: case I64_OP: case F64_OP: {
			return OP_size[code];
			break;
		}

		case GRAPH_OP: {
			idx len = get_rank1(base)->len;
			idx*idxs = rank1_data(base);
			idx acc = 0;
			for (uz i = 0; i < len; i++) {
				acc += CalcSize(idxs[i]);
			}
			return acc;
			break;
		} 

		default: {
			return 0;
			break;
		}
	}

}



void add_label (char label[], idx value) {
	KEYS[key_count].index = value;
	KEYS[key_count].shell = 0;
	memcpy(KEYS[key_count].label, label, strlen(label));
	key_count++;
}

idx key_to_idx(char label[]) { return 0;}
void idx_to_key(idx index, char label[]) {}


void inc_shells(){}



void panic(char message[]) {
	printf("ERROR: ");
	puts(message);
	printf("\n");
	abort();
}
