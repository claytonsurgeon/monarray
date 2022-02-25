#pragma once
#include "common.h"
#include "array.h"
#include "mem.h"



void DUMP_MEM(uz a, uz z) {
	for (uz i = a; i < z; i++) {
		printf("@%.4lx %.2x\n", i, MEM[i]);
	}
}








void Print_Array_Point(idx array, idx i) {
	idx base = get_rankx(array)->base;
	OP  code = get_code(base)->code;

	switch (code) {
		case U32_OP: {
			u32 d = (u32) ((u32 *) rankx_data(array))[i];
			printf(" %u ", d);
			break;
		}
		case I32_OP: {
			i32 d = (i32) ((i32 *) rankx_data(array))[i];
			printf(" %d ", d);
			break;
		}
		case U8_OP: {
			u8 d = (u8) ((u8 *) rankx_data(array))[i];
			printf(" %u ", d);
			break;
		}
		case I8_OP: {
			i8 d = (i8) ((i8 *) rankx_data(array))[i];
			printf(" %d ", d);
			break;
		}
		default:
			printf("?");
			break;
	}
}





void print_point(idx i) {
	OP code = get_code(i)->code;
	printf(
		"@%.4x: %8s @%.4x %s:"//length:%u\n\tvolume:%u\n\tparent:%u\n\t"
		,	i
		,	OP_str[code]
		,	get_code(i)->parent
		,	get_code(i)->variable ? "variable" : "constant"
	);
	switch (code) {
		case F32_OP: {
			printf(" %f", get_data(f32, i)[0]);
			break;
		}
		case U8_OP: {
			printf(" %u", get_data(u8, i)[0]);
			break;
		}
		case I8_OP: {
			printf(" %d", get_data(i8, i)[0]);
			break;
		}

		case U32_OP: {
			printf(" %d", get_data(u32, i)[0] );
			break;
		}
		case I32_OP: {
			printf(" %d", get_data(i32, i)[0] );
			break;
		}




		case GRAPH_OP: {
			Rank1 * shape = get_rank1(i);
			idx len = shape->len;

			printf(" {");
				for (uz k = 0; k < len && k < 11; k++) {
					if (k == 10) {
						printf("... ");
					}
					else {
						printf(" @%.4x ", (idx) ((idx *) rank1_data(i))[k]);
					}
				}
			printf("}");
			break;
		}


		case SPACE_OP: {
			Rank1 * shape = get_rank1(i);
			idx len = shape->len;

			printf(" [");
				for (uz k = 0; k < len; k++) {
					if (k == len-1) {
						printf("@%.4x", (idx) ((idx *) rank1_data(i))[k]);
					}
					else {
						printf("%u, ", (idx) ((idx *) rank1_data(i))[k]);
					}
				}
			printf("]");
			break;
		}

		case ARRAY_OP: {
			// this currently only prints 4 byte data points
			// needs to be generalized
			
			Rankx * shape = get_rankx(i);
			idx base = shape->base;
			idx rank = shape->rank;
			idx*lens = shape->lens;


			idx total_points = 1;
			for (uz j = 0; j < rank; j++) {
				total_points *= lens[j];
			}
		

			printf(" base: @%.4x, rank: %u, shape:", base, rank);
			for (uz j = 0; j < rank; j++) {
				printf(" %u", lens[j]);
			}
			printf("\n\t    {");
			for (uz j = 0; j < total_points && j < 21; j++) {
					if (j == 20) {
						printf("... ");
					}
					else {
						Print_Array_Point(i, j);
					}
				// printf(" %d ", get_array(i32, i)[j]);
			}
			printf("}");
			break;
		}

		case ADDU8_OP: {
			printf(" %u", get_data(u8, i)[0]);
			break;
		}

		default: {
			printf("Unrecognized OP code: %u", code);
			break;
		}
	}
}

// print_data()

void print_points() {
	for (uz i = 0; i < LOOKUP_COUNT; i++) {
		print_point(LOOKUP[i]);
		printf("\n");
	}
}



void print_graph() {
	for (uz i = 0; i < key_count; i++) {
		printf("%s @%.4x %u\n", KEYS[i].label, KEYS[i].index, KEYS[i].shell);
	}
}