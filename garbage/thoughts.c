
#include "common.h"



/*
[1000, 10, i32]! {10,20} => { 10, 20, 0, 0, ... }

[1000, 10, i32] $ { 10, 20 } => { 10, 20, 10, 20, ... }

![1000, 10, f32] => { 0.0, 0.0, ... }
[1000, 10, f32]! 1


add: { a: b: ![i32] -> a + b }

ID3x3: [3,3,f64]! {{ 1,0,0,0 }}
	{	1 0 0
		0 1 0
		0 0 1  }

*/





// array has two code points head and body


// when you specify the indiviual elements of an array, you are specifying a pattern. This pattern is repeated for each array in the lowest dimension



typedef struct {
	bool	variable;	// variable or constant?
	u8		code;		// joining operation
	union {
		struct point {
			u8		word;		// 1,2,4,8,16 bytes = 8,16,32,64,128 bits
			u32	volume;	// physical capacity
			u32 	length;	// logical length
			idx 	parent;	// index of parent
		};
		struct array {
			u32	word;		// any size, for example elements may be structs of 387 byte size
			u32	volume;	// length * word * fold(*, dimensions)
			u32 	length;	// logical length
			idx 	parent;	// index of parent
			u8		dimens[];	// object dimensions	
		} array;
	};
} Code;



typedef struct {
    union {
        struct {
            uint16_t firstfour  : 4;
            uint16_t secondfour : 4;
            uint16_t thirdfour  : 4;
            uint16_t lastfour   : 4;
        };
        /* EDIT - Duplicate structure with different member names
           added, in response to a comment below. */
        struct {
            uint16_t nibble1    : 4;
            uint16_t nibble2    : 4;
            uint16_t nibble3    : 4;
            uint16_t nibble4    : 4;
        };
        struct {
            uint16_t bit0  : 1;
            uint16_t bit1  : 1;
            uint16_t bit2  : 1;
            uint16_t bit3  : 1;
            uint16_t bit4  : 1;
            uint16_t bit5  : 1;
            uint16_t bit6  : 1;
            uint16_t bit7  : 1;
            uint16_t bit8  : 1;
            uint16_t bit9  : 1;
            uint16_t bit10 : 1;
            uint16_t bit11 : 1;
            uint16_t bit12 : 1;
            uint16_t bit13 : 1;
            uint16_t bit14 : 1;
            uint16_t bit15 : 1;
        };
        uint16_t value;
    };
} foo;








typedef struct codex {
	bool variable;	// variable or constant?
	u8		code;		// joining operation
	u8		word;		// 1,2,4,8,16 bytes = 8,16,32,64,128 bits
	u32	size;		// physical capacity
	u32 	length;	// logical length
	idx 	parent;	// index of parent
	// u8		data[];	// what are the length of each dimension?
} Coded;
	//  data field always follows the code point


// a matrix is defined as follows

Matrix_3x3 {
	code: Array_Op,
	word: 1,				// dimension width, 1 byte
	size: 2,				// 2 * 1 = 2 bytes
	length: 2			// logical length
	data: { 3, 3 }		// 3x3 matrix
}

Matrix_3x3 {
	code: Array_Op,
	word: 1,				// dimension width, 1 byte
	size: 2,				// 2 * 1 = 2 bytes
	length: 2			// logical length
	data: {
		{	code

		}
	}		// 3x3 matrix
}


Matrix_10000x10000 {
	code: Array_Op,
	word: 2,				// dimension width, 2 bytes
	size: 4,				// 2 * 2 = 4 bytes
	length: 2			// logical length
	data: { 10000, 10000 }		// 3x3 matrix
}


