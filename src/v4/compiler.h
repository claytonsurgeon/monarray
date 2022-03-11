#pragma once

#include "mem.h"
#include "common.h"
#include "A.h"
#include "B.h"
#include "meta.h"





void compile() {
	MEM_init();
	Program();



	// Graph(
	// 	U32(200),
	// 	U32(1),
	// 	U32(2),
	// 	U32(3),
	// 	I32(-4),
	// 	I32(-5),
	// 	F32(3.14)
	// );
	
	// Key("Monkey", Graph(
	// 	Key("Hamster", Graph(
	// 		Key("duck", Graph(
	// 			Key("a", U32(20)),
	// 			Ref("a"),
	// 			Key("b", U32(30))
	// 		)),
	// 		Key("penguin", Graph(
	// 			U32(1001), F32(2), F32(3),
	// 			F32(4), F32(255), F32(6),
	// 			U32(738945739), F32(8), F32(9)
	// 		))
	// 	))
	// ));



	Graph(
		Key("mydata", U32(10,20,30,40,50)),
		Add(Ref("mydata"), Ref("mydata"))
	);



	get_program(&MEM.A)->size = MEM.A.end;
	
	print_mem(&MEM.A);
	print_points(&MEM.A);


	B();

	// printf("op1 %lu\n", OP1_size);
	// printf("array %lu, arrayu32 %lu\n", ARRAY_size, sizeof(ARRAY_f32));
	// printf("ARRAY %lu, GRAPH %lu, KEY %lu\n", sizeof(ARRAY), sizeof(GRAPH), sizeof(KEY));
	// printf("REF %lu, PUSH %lu, KO/K_O %lu\n", sizeof(REF), sizeof(PUSH), sizeof(union {
	// 	KO   base;
	// 	K_O  k_o;
	// }));
	MEM_exit();
}