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


typedef f32 c64[2];


#define x8_size  (1)
#define x16_size (2)
#define x32_size (4)
#define x64_size (8)

#define idx_size sizeof(idx)



// should have 1:1 mapping with fundemental CPU ops so as to minimize branching in executor

typedef enum {
	n_0,							// undecided
	b_1,							// for simplicity, 32bit width
	u_8,  i_8,  f_8,
	u_16, i_16, f_16, c_16,
	u_32, i_32, f_32,	c_32,	// only 32 bit supported at the moment
	u_64, i_64, f_64, c_64,

	t_ref
} KIND;

static char Kind_str[256][10] = {
	"none",
	"bool",
	"u8",  "i8",  "f8",
	"u16", "i16", "f16", "c16",
	"u32", "i32", "f32", "c32",
	"u64", "i64", "f64", "c64",
	"ref"
};


typedef enum {
	val_,
	add_, sub_, mul_, div_,

	cast_,
} OP;

static char Op_str[256][10] = {
	"val",
	"add", "sub", "mul", "div",
	"cast"
};



typedef enum {
	apply_, each_, fold_, scan_
} HOP;

static char Hop_str[256][10] = {
	"apply",
	"each",
	"fold",
	"scan"
};


typedef enum {
	PROGRAM_,

	GRAPH_,
	ARRAY_,		// scalars are rank'0 arrays

	KEY_,
	REF_,
	OP1_,
	OP2_,

	PUSH_
} Type;



static char Type_str[256][10] = {
	"program",

	"graph",
	"array",

	"key",
	"ref",
	"op1",
	"op2",

	"push"
};



typedef struct _Code {
	idx  parent;		// index of parent
	Type type;
} Code;



typedef struct _PROGRAM {
	idx  parent;		// index of parent
	Type type;
	
	u32  size;
	u32  hash;
} PROGRAM;




typedef struct _ARRAY {
	idx  parent;		// index of parent
	Type type;

	KIND kind;

	idx  size;
	u8   data[];
} ARRAY;


typedef struct _ARRAY_u32 {
	idx  parent;		// index of parent
	Type type;

	KIND kind;

	idx  size;
	u32  data[];
} ARRAY_u32;

typedef struct _ARRAY_i32 {
	idx  parent;		// index of parent
	Type type;

	KIND kind;

	idx  size;
	i32  data[];
} ARRAY_i32;

typedef struct _ARRAY_f32 {
	idx  parent;		// index of parent
	Type type;

	KIND kind;

	idx  size;
	f32  data[];
} ARRAY_f32;

typedef struct _ARRAY_c64 {
	idx  parent;		// index of parent
	Type type;

	KIND kind;

	idx  size;
	c64  data[];
} ARRAY_c64;






// graphs are arrays with 
typedef struct _GRAPH {
	idx  parent;		// index of parent
	Type type;

	idx  size;			// number of points in graph
	idx  data[];		// indexes to child points
} GRAPH;


#define PROGRAM_size (sizeof(PROGRAM))
#define GRAPH_size (sizeof(GRAPH))
#define ARRAY_size (sizeof(ARRAY))


typedef struct _KEY {
	idx  parent;		// index of parent
	Type type;

	idx  index;
	u16  label;
	u16  shell;
} KEY;

typedef struct _REF {
	idx  parent;		// index of parent
	Type type;

	idx  index;
	u16  label;
	u16  shell;
} REF;

#define REF_size (sizeof(REF))
#define KEY_size (sizeof(KEY))


// push is jimmied inbetween graph and point to allow for multi-push
typedef struct _PUSH {
	idx  parent;		// index of parent
	Type type;

	idx  count;			// logical length
	idx  push[];		// indexes of refs that rely on this key
} PUSH;

#define PUSH_size (sizeof(PUSH))




typedef struct _OP1 {
	idx  parent;		// index of parent
	Type type;

	KIND kind;
	OP   op;
	HOP  hop;

	idx  a;
	idx  z;
} OP1;



typedef struct _OP2 {
	idx  parent;		// index of parent
	Type type;

	KIND kind;
	OP   op;
	HOP  hop;

	idx  a;
	idx  b;
	idx  z;
} OP2;

#define OP1_size (sizeof(OP1))
#define OP2_size (sizeof(OP2))










idx inline MIN(idx a, idx b) {
	return a < b ? a : b;
}
idx inline MAX(idx a, idx b) {
	return a < b ? a : b;
}

idx UPPER_ROUND(uz num, uz base) {
	return (num + base - 1) / base * base;
}





#define label_size (100)
typedef char label[label_size];



typedef struct _mem {
	u8*byte;
	u32 end;
	u32 max;

	idx*lookup;
	u32 lookup_end;
	u32 lookup_max;
} mem;

typedef struct _global {
	mem A;
	mem B;

	label*label;
	u32 label_end;
	u32 label_max;
} global;
















void panic(char message[]) {
	printf("ERROR: ");
	puts(message);
	printf("\n");
	abort();
}
