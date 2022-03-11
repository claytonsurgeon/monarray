#pragma once
#include "common.h"
#include "meta.h"
#include "mem.h"

#define NUMARGS(...)  (sizeof((int[]){__VA_ARGS__})/sizeof(int))
#define SUM(...)  (sum(NUMARGS(__VA_ARGS__), __VA_ARGS__))


idx Variable(idx i) {
	get_code(i)->variable = true;
	return i;
}

idx U32(u32 x) {
	idx index = MEM_Push_Code(U32_OP);
	MEM_Push_Data(&x, OP_size[U32_OP]);
	return index;
}

idx I32(i32 x) {
	idx index = MEM_Push_Code(I32_OP);
	MEM_Push_Data(&x, OP_size[I32_OP]);
	return index;
}

idx I8(i8 x) {
	idx index = MEM_Push_Code(I8_OP);
	MEM_Push_Data(&x, OP_size[I8_OP]);
	return index;
}

idx U8(u8 x) {
	idx index = MEM_Push_Code(U8_OP);
	MEM_Push_Data(&x, OP_size[U8_OP]);
	return index;
}



idx _Graph(int length, ...) {
	va_list ptr;
	va_start(ptr, length);

	idx index = MEM_Push_Code(GRAPH_OP);

	MEM_Push_Rank1(index, length);

	bool variable = false;
	for (uz i = 0; i < length; i++) {
		idx child_idx = va_arg(ptr, int);
		variable = variable || get_code(child_idx)->variable;
		get_code(child_idx)->parent = index;
		MEM_Push_Data(&child_idx, IdxSize);
	}
	get_code(index)->variable = variable;

	return index;
}
#define Graph(...)  (_Graph(NUMARGS(__VA_ARGS__), __VA_ARGS__))



idx _Space(int length, ...) {
	va_list ptr;
	va_start(ptr, length);
	idx index = MEM_Push_Code(SPACE_OP);

	MEM_Push_Rank1(index, length);
	
	for (uz i = 0; i < length; i++) {
		idx child_idx = va_arg(ptr, int);
		if (i == length-1) {
			get_code(child_idx)->parent = index;
		}
		MEM_Push_Data(&child_idx, IdxSize);
	}

	return index;
}
#define Space(...)  (_Space(NUMARGS(__VA_ARGS__), __VA_ARGS__))



idx Cast(idx space_idx, idx graph_idx) {
	if(get_code(space_idx)->code == GRAPH_OP
	&&	get_code(graph_idx)->code == SPACE_OP) {
		idx temp = space_idx;
		space_idx = graph_idx;
		graph_idx = temp;
	}

	if (get_code(space_idx)->code != SPACE_OP
	||	get_code(graph_idx)->code != GRAPH_OP) {
		// error condition
		idx index = MEM_Push_Code(ER_OP);
		return index;
	}
	idx index = MEM_Push_Code(ARRAY_OP);
	MEM_Push_Rankx(index);

	idx len = get_rank1(space_idx)->len;

	idx * space_data = rank1_data(space_idx);

	u32 array_size = 1;
	u32 total_points = 1;

	for (uz i = 0; i < len; i++) {		
		if (i == len-1) {
			MEM_Set_Rankx_Base(index, space_data[i]);
			
			Code * type = get_code(space_data[i]);
			printf("test: %s\n", OP_str[type->code]);
			array_size = total_points * OP_size[type->code];
		}
		else {
			total_points *= space_data[i];
			printf("len %d\n", space_data[i]);
			MEM_Push_Rankx_len(index, space_data[i]);
		}
	}

	idx base = get_rankx(index)->base;
	idx base_width = OP_size[0];
	printf("base width %u, point count %u, space size %u\n", base_width, total_points, array_size);

	
	idx graph_len = get_rank1(graph_idx)->len;
	idx*graph_data = rank1_data(graph_idx);


	get_code(index)->variable = get_code(graph_idx)->variable;
	get_code(graph_idx)->parent = index;


	// cast must negotiate both the size of the space base and the size of each point in the operand graph
	uz base_size = CalcSize(base);
	for (uz i = 0; i < total_points; i++) {
		if (i < graph_len) {
			idx Idx = graph_data[i];
			MEM_Push_Data_sized(rank0_data(Idx), MIN(CalcSize(Idx), base_size), base_size);
		}
		else {
			MEM_Push_Zero_Data(base_size);
		}
	}



	return index;
}



idx Add(idx a_idx, idx b_idx) {
	OP a_op = get_code(a_idx)->code;
	OP b_op = get_code(b_idx)->code;

	if (a_op != b_op) {
		char buffer[100] = {0};
		snprintf(buffer, 100, "Type conflict: Add %s %s", OP_str[a_op], OP_str[b_op]);
		panic(buffer);
	}

	idx index = 0;
	switch(a_op) {
		case U8_OP: {
			index = MEM_Push_Code(ADDU8_OP);
			u8 result = get_data(u8, a_idx)[0] + get_data(u8, b_idx)[0];
			MEM_Push_Data(&result, OP_size[a_op]);
			break;
		}

		default: {
			break;
		}
	}

	
	get_code(a_idx)->parent = index;
	get_code(b_idx)->parent = index;
	get_code(index)->variable =
		get_code(a_idx)->variable || get_code(b_idx)->variable;

	return index;
}



idx Key(char label[], idx value) {

	add_label(label, value);

	return value;
}



void runnere () {
	I32(0);	// head data goes in index 0

	// idx k = Graph(
	// 	I8(0xcc),
	// 	I32(12345678),
	// 	U8(0xFF)
	// );

	// printf("%u\n", CalcSize(k));

	// Key("myarray", Cast(
	// 	Space(4, 10, I32(0)),
	// 	Graph(
	// 		I32(-10000), Variable(U8(7)), U8(7)
	// 		// U8(6), U8(1), U8(6),
	// 		// U8(8), U8(8), U8(1),
	// 		// U8(8), U8(8)
	// 	)
	// ));

	// Add(U8(10), Variable(U8(20)));

	Key("add", Graph(
		Key("a", U8(10)),
		Key("b", U8(20))
		// Ret(Add(
		// 	Ref("a"), Ref("b")
		// ))
	));
	// DUMP_MEM(120, 150);


	print_points();
	print_graph();
}