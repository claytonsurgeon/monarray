#pragma once

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

#define IdxSize sizeof(idx)

typedef enum {
	ER_OP,
	U8_OP,  I8_OP,
	U32_OP, I32_OP, F32_OP,
	U64_OP, I64_OP, F64_OP,

	SPACE_OP,
	GRAPH_OP,
	ARRAY_OP,

	ADDU8_OP, ADDI8_OP,
	ADDU32_OP, ADDI32_OP, ADDF32_OP
} __attribute__((packed)) OP_;
typedef OP_ OP;


static char OP_str[256][10] = {
	"ER",
	"u8", "i8",
	"u32", "i32", "f32",

	"space",
	"graph",
	"array",

	"addu8",  "addi8",
	"addu32", "addi32", "addf32",

	"subu8",  "subi8",
	"subu32", "subi32", "subf32",
};

// size only includes space occupied by data, NOT code
// zero size implies complex size, e.g. an array has a variadic length
static u8 OP_size[256] = {
	0,
	1, 1,
	4,	4, 4,
	8,	8, 8,

	0,
	0,
	0,

	1, 1,
	4,	4, 4,
};


#define MIN(_a, _b)						\
({								\
	typeof(_a) __a = (_a);					\
	typeof(_b) __b = (_b);					\
	__a <= __b ? __a : __b;					\
})
#define MAX(_a, _b)						\
({								\
	typeof(_a) __a = (_a);					\
	typeof(_b) __b = (_b);					\
	__a >= __b ? __a : __b;					\
})

idx UPPER_ROUND(uz num, uz base) {
	return (num + base - 1) / base * base;
}


// TODO
/** Get ride of weakly nested structures
 *  give every data type a uniquie structure that is perfectly sized
 *  ensure that they all start with op, var, and par
 * !!!!!!!!!! spacers already contain the information about an arrays' dimensions
 * just use that and don't store dimensions with arrays, just give an array two idxs
 * one to the operand graph and one to the dimension space
 */
















// typedef struct _Op1 {
// 	idx a;
// 	u8 r;
// } Op1;

// typedef struct _Op2 {
// 	idx a;
// 	idx b;
// 	u8 r;
// } Op2;


// typedef struct _Rankx {
// 	idx	base;		// index to point that is being used as the base element
// 	idx	rank;
// 	idx	lens[];	
// } Rankx;

// typedef struct _Rank1 {
// 	idx len;
// } Rank1;

// // #define Rank0_Size (sizeof(Rank0))
// #define Rank1_Size (sizeof(Rank1))
// #define Rankx_Size (sizeof(Rankx))
// #define Op1_Size (sizeof(Op1))
// #define Op2_Size (sizeof(Op2))


// typedef struct _Code {
// 	OP		code;			// joining operation
// 	bool	variable;	// variable or constant?
// 	idx 	parent;		// index of parent
// 	idx	shape[];		// array rank dimension lengths, non arrays have an implied 0 rank
// } Code;



// u32 CodeSize = sizeof(Code);