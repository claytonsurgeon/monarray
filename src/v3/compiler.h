#pragma once

#include <stdlib.h>
#include "mem.h"
#include "./common.h"
#include "./A.h"
#include "./B.h"
#include "./meta.h"


void compile() {
	MEM_init();

	// print_mem(&MEM.A);


	Program();


	// U32(200);
	// U32(1);
	// U32(2);
	// U32(3);
	// U32(4);
	// U32(5);

	// Graph(
	// 	U8(10),
	// 	U32(10000),
	// 	Graph(
	// 		Variable(U8(10)),
	// 		U32(10000),
	// 		U8(20)
	// 	),
	// 	U8(20)
	// );


	Graph(
		Cast(
			Shape(2,2500, U32(0)),
			Graph(
				U32(1001), U8(2), U8(3),
				U8(4), U8(255), U8(6),
				U32(738945739), U8(8), U8(9)
			)
		)
	);


	Key("cool-cat-9000", U32(20));



	Key("Monkey", Graph(
		Key("Hamster", Graph(
			Key("duck", Graph(
				Key("a", U32(20)),
				Ref("a"),
				Key("b", U32(30))
			)),
			Key("penguin", Cast(
				Shape(2,2500, U32(0)),
				Graph(
					U32(1001), U8(2), U8(3),
					U8(4), U8(255), U8(6),
					U32(738945739), U8(8), U8(9)
				)
			))
		))
	));


	get_program(&MEM.A)->size = MEM.A.end;


	// print_mem(&MEM.A);
	print_points(&MEM.A);

	// printf("%lu %lu\n", sizeof(Code), label_size);

	MEM_exit();
}