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


#define idx_size sizeof(idx)


typedef enum {
	ER_O,

	PROGRAM_O,
	// think of a value operation as a function that returns a literal value in the monarray
	VALUE_O,


	ADD_O, SUB_O,
	MUL_O, DIV_O
} O;
typedef O OP;

static char OP_str[256][10] = {
	"ER",
	"Program",
	"val",

	"add", "sub",
	"mul", "div"
};



typedef enum {
	NO_K,
	U8_K,  U16_K, U32_K, U64_K,
	I8_K,  I16_K, I32_K, I64_K,
	F32_K, F64_K,


	SHAPE_K,
	GRAPH_K,
	ARRAY_K,


	IDX_K,
	MIX_K
} K;
typedef K KIND;

static char KIND_str[256][10] = {
	"nothing",
	"u8", "u16", "u32", "u64",
	"i8", "i16", "i32", "i64",
	"f32", "f64",

	"shape",
	"graph",
	"array",

	"idx",
	"mix"
};



idx inline MIN(idx a, idx b) {
	return a < b ? a : b;
}
idx inline MAX(idx a, idx b) {
	return a < b ? a : b;
}

idx UPPER_ROUND(uz num, uz base) {
	return (num + base - 1) / base * base;
}



typedef struct _Code {
	OP		o;				// operation
	KIND	k;				// format kind
	bool	variable;	// variable or constant?
	idx 	parent;		// index of parent
} Code;


typedef struct _Op0  { Code code; u8 z[]; } Op0;

typedef struct _Op0_u8  { Code code; u8  z; } Op0_u8;
typedef struct _Op0_u16 { Code code; u16 z; } Op0_u16;
typedef struct _Op0_u32 { Code code; u32 z; } Op0_u32;
typedef struct _Op0_u64 { Code code; u64 z; } Op0_u64;

typedef struct _Op0_i8  { Code code; i8  z; } Op0_i8;
typedef struct _Op0_i16 { Code code; i16 z; } Op0_i16;
typedef struct _Op0_i32 { Code code; i32 z; } Op0_i32;
typedef struct _Op0_i64 { Code code; i64 z; } Op0_i64;

typedef struct _Op0_f32 { Code code; f32 z; } Op0_f32;
typedef struct _Op0_f64 { Code code; f64 z; } Op0_f64;

#define Op0_8_size  (sizeof(Op0_u8));
#define Op0_16_size (sizeof(Op0_u16));
#define Op0_32_size (sizeof(Op0_u32));
#define Op0_64_size (sizeof(Op0_u64));



// generic zero sized data accessor "u8 z[]"; requires cast
typedef struct _Op1     { Code code; idx a; u8 z[]; } Op1;

typedef struct _Op1_u8  { Code code; idx a; u8  z; } Op1_u8;
typedef struct _Op1_u16 { Code code; idx a; u16 z; } Op1_u16;
typedef struct _Op1_u32 { Code code; idx a; u32 z; } Op1_u32;
typedef struct _Op1_u64 { Code code; idx a; u64 z; } Op1_u64;

typedef struct _Op1_i8  { Code code; idx a; i8  z; } Op1_i8;
typedef struct _Op1_i16 { Code code; idx a; i16 z; } Op1_i16;
typedef struct _Op1_i32 { Code code; idx a; i32 z; } Op1_i32;
typedef struct _Op1_i64 { Code code; idx a; i64 z; } Op1_i64;

typedef struct _Op1_f32 { Code code; idx a; f32 z; } Op1_f32;
typedef struct _Op1_f64 { Code code; idx a; f64 z; } Op1_f64;

#define Op1_8_size  (sizeof(Op1_u8));
#define Op1_16_size (sizeof(Op1_u16));
#define Op1_32_size (sizeof(Op1_u32));
#define Op1_64_size (sizeof(Op1_u64));


typedef struct _Op2     { Code code; idx a; idx b; u8 z[]; } Op2;

typedef struct _Op2_u8  { Code code; idx a; idx b; u8  z; } Op2_u8;
typedef struct _Op2_u16 { Code code; idx a; idx b; u16 z; } Op2_u16;
typedef struct _Op2_u32 { Code code; idx a; idx b; u32 z; } Op2_u32;
typedef struct _Op2_u64 { Code code; idx a; idx b; u64 z; } Op2_u64;

typedef struct _Op2_i8  { Code code; idx a; idx b; i8  z; } Op2_i8;
typedef struct _Op2_i16 { Code code; idx a; idx b; i16 z; } Op2_i16;
typedef struct _Op2_i32 { Code code; idx a; idx b; i32 z; } Op2_i32;
typedef struct _Op2_i64 { Code code; idx a; idx b; i64 z; } Op2_i64;

typedef struct _Op2_f32 { Code code; idx a; idx b; f32 z; } Op2_f32;
typedef struct _Op2_f64 { Code code; idx a; idx b; f64 z; } Op2_f64;

#define Op2_8_size  (sizeof(Op2_u8));
#define Op2_16_size (sizeof(Op2_u16));
#define Op2_32_size (sizeof(Op2_u32));
#define Op2_64_size (sizeof(Op2_u64));

typedef struct _PROGRAM {
	Code code;
	u32 z;
} PROGRAM;

typedef struct _SHAPE {
	Code code;
	idx  base;
	idx  rank;			// number of multipliers in shape
	idx  data[];		// dimension multipliers used to define arrays
} SHAPE;

typedef struct _GRAPH {
	Code code;
	idx  rank;			// number of points in graph
	idx  data[];		// indexes to child points
} GRAPH;

typedef struct _ARRAY {
	Code code;			// parent is operand graph
	idx  shape;
	idx  graph;
	idx  count;			// logical length
	u8   data[];		// indexes to child points
} ARRAY;


#define PROGRAM_size (sizeof(PROGRAM))
#define SHAPE_size (sizeof(SHAPE))
#define GRAPH_size (sizeof(GRAPH))
#define ARRAY_size (sizeof(ARRAY))



void panic(char message[]) {
	printf("ERROR: ");
	puts(message);
	printf("\n");
	abort();
}
