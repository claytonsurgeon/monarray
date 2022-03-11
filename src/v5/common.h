#pragma once
/**
 * Defines the format of data in the monarray
 * data creation/allocation functions are
 * defined in 'mem' 
 */



#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>


#define NUMARGS(...)  (sizeof((int[]){__VA_ARGS__})/sizeof(int))
#define SUM(...)  (sum(NUMARGS(__VA_ARGS__), __VA_ARGS__))

#define KB (1<<10)
#define MB (1<<20)

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;

typedef int8_t    i8;
typedef int16_t   i16;
typedef int32_t   i32;
typedef int64_t   i64;

// typedef __int128 i128;
// typedef unsigned __int128 u128;

typedef volatile u32 r32;
typedef volatile u32 reg32;

typedef volatile u64 r64;
typedef volatile u64 reg64;

typedef float f32;
typedef double f64;

typedef size_t uz;
typedef u32 idx;


// typedef f32 c64[2];
// typedef f64 c128[2];

typedef struct { f32 real; f32 imag; } c64;
typedef struct { f64 real; f64 imag; } c128;


#define bool_size (sizeof(bool))

#define x8_size  (1)
#define x16_size (2)
#define x32_size (4)
#define x64_size (8)

#define idx_size sizeof(idx)






inline idx MIN(idx a, idx b) {
	return a < b ? a : b;
}
inline idx MAX(idx a, idx b) {
	return a > b ? a : b;
}

inline idx UPPER_ROUND(uz num, uz base) {
	return (num + base - 1) / base * base;
}










typedef enum {
	n_0,							// undecided
	b_1,							// for simplicity, 32bit width
	u_8,  i_8,  f_8,
	u_16, i_16, f_16, c_16,
	u_32, i_32, f_32,	c_32,	// only 32 bit supported at the moment
	u_64, i_64, f_64, c_64,

	t_ref
} Kind;

static char Kind_str[256][10] = {
	"none",
	"bool",
	"u8",  "i8",  "f8",
	"u16", "i16", "f16", "c16",
	"u32", "i32", "f32", "c32",
	"u64", "i64", "f64", "c64",
	"ref"
};






// some of these objects will not apply to the Glider project, because
// the glider project has no compilation stage

typedef enum {
	PROGRAM_,	// program meta data

	VALUE_,		// any scalar
	GRAPH_,		// reference array
	ARRAY_,		// fixed and variable length arrays

	KEY_,	
	REF_,
	OP1_,
	OP2_,

	PUSH_
} Type;



static char Type_str[256][10] = {
	"program",

	"value",
	"graph",
	"array",

	"key",
	"ref",
	"op1",
	"op2",

	"push"
};






typedef struct _CODE {
	// idx  parent;		// index of parent
	Type	type;
	Kind	kind;
} CODE;







typedef struct _VALUE {
	// idx parent;
	Type	type;
	Kind	kind;
	u8		data[];
}	VALUE;

#define VALUE_size (sizeof(VALUE))


typedef struct _VALUE_f32 {
	// idx parent;
	Type	type;
	Kind	kind;
	f32	data;
}	VALUE_f32;

typedef struct _VALUE_c64 {
	// idx parent;
	Type	type;
	Kind	kind;
	c64	data;
}	VALUE_c64;


typedef struct _VALUE_bool {
	// idx parent;
	Type	type;
	Kind	kind;
	bool	data;
}	VALUE_bool;


#define VALUE_f32_size (sizeof(VALUE_f32))
#define VALUE_c64_size (sizeof(VALUE_c64))
#define VALUE_bool_size (sizeof(VALUE_bool))




typedef struct _ARRAY {
	// idx	parent;
	Type	type;
	Kind	kind;
	idx	rank;
	idx	len;
	idx	idx[16];
} ARRAY;

#define ARRAY_size (sizeof(ARRAY))

typedef f32 f32s_0x0[1<<0x0];
typedef f32 f32s_0x1[1<<0x1];
typedef f32 f32s_0x2[1<<0x2];
typedef f32 f32s_0x3[1<<0x3];
typedef f32 f32s_0x4[1<<0x4];
typedef f32 f32s_0x5[1<<0x5];
typedef f32 f32s_0x6[1<<0x6];
typedef f32 f32s_0x7[1<<0x7];
typedef f32 f32s_0x8[1<<0x8];
typedef f32 f32s_0x9[1<<0x9];
typedef f32 f32s_0xA[1<<0xA];
typedef f32 f32s_0xB[1<<0xB];
typedef f32 f32s_0xC[1<<0xC];
typedef f32 f32s_0xD[1<<0xD];
typedef f32 f32s_0xE[1<<0xE];
typedef f32 f32s_0xF[1<<0xF];



uz f32s_sizes[16] = {
	sizeof(f32s_0x0),
	sizeof(f32s_0x1),
	sizeof(f32s_0x2),
	sizeof(f32s_0x3),
	sizeof(f32s_0x4),
	sizeof(f32s_0x5),
	sizeof(f32s_0x6),
	sizeof(f32s_0x7),
	sizeof(f32s_0x8),
	sizeof(f32s_0x9),
	sizeof(f32s_0xA),
	sizeof(f32s_0xB),
	sizeof(f32s_0xC),
	sizeof(f32s_0xD),
	sizeof(f32s_0xE),
	sizeof(f32s_0xF)
};

typedef c64 c64s_0x0[1<<0x0];
typedef c64 c64s_0x1[1<<0x1];
typedef c64 c64s_0x2[1<<0x2];
typedef c64 c64s_0x3[1<<0x3];
typedef c64 c64s_0x4[1<<0x4];
typedef c64 c64s_0x5[1<<0x5];
typedef c64 c64s_0x6[1<<0x6];
typedef c64 c64s_0x7[1<<0x7];
typedef c64 c64s_0x8[1<<0x8];
typedef c64 c64s_0x9[1<<0x9];
typedef c64 c64s_0xA[1<<0xA];
typedef c64 c64s_0xB[1<<0xB];
typedef c64 c64s_0xC[1<<0xC];
typedef c64 c64s_0xD[1<<0xD];
typedef c64 c64s_0xE[1<<0xE];
typedef c64 c64s_0xF[1<<0xF];



uz c64s_sizes[16] = {
	sizeof(c64s_0x0),
	sizeof(c64s_0x1),
	sizeof(c64s_0x2),
	sizeof(c64s_0x3),
	sizeof(c64s_0x4),
	sizeof(c64s_0x5),
	sizeof(c64s_0x6),
	sizeof(c64s_0x7),
	sizeof(c64s_0x8),
	sizeof(c64s_0x9),
	sizeof(c64s_0xA),
	sizeof(c64s_0xB),
	sizeof(c64s_0xC),
	sizeof(c64s_0xD),
	sizeof(c64s_0xE),
	sizeof(c64s_0xF)
};


typedef bool bools_0x0[1<<0x0];
typedef bool bools_0x1[1<<0x1];
typedef bool bools_0x2[1<<0x2];
typedef bool bools_0x3[1<<0x3];
typedef bool bools_0x4[1<<0x4];
typedef bool bools_0x5[1<<0x5];
typedef bool bools_0x6[1<<0x6];
typedef bool bools_0x7[1<<0x7];
typedef bool bools_0x8[1<<0x8];
typedef bool bools_0x9[1<<0x9];
typedef bool bools_0xA[1<<0xA];
typedef bool bools_0xB[1<<0xB];
typedef bool bools_0xC[1<<0xC];
typedef bool bools_0xD[1<<0xD];
typedef bool bools_0xE[1<<0xE];
typedef bool bools_0xF[1<<0xF];



uz bools_sizes[16] = {
	sizeof(bools_0x0),
	sizeof(bools_0x1),
	sizeof(bools_0x2),
	sizeof(bools_0x3),
	sizeof(bools_0x4),
	sizeof(bools_0x5),
	sizeof(bools_0x6),
	sizeof(bools_0x7),
	sizeof(bools_0x8),
	sizeof(bools_0x9),
	sizeof(bools_0xA),
	sizeof(bools_0xB),
	sizeof(bools_0xC),
	sizeof(bools_0xD),
	sizeof(bools_0xE),
	sizeof(bools_0xF)
};









void panic(char message[]) {
	printf("ERROR: ");
	puts(message);
	printf("\n");
	abort();
}









