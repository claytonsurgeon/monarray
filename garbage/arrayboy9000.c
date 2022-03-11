
/**
 * Operations Implemented as stack machine with array registers
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 *
 */




#include "./common.h"
#include <stdarg.h>
#include <stdio.h>


typedef struct __Array__ {
	int shape[3];
	float * _;
} Array;

typedef struct __Context__ {
	int shape[3];
	float * _;
} Context;


typedef enum _Operator {
	Add, Sub,
	Mul, Div,

	And, Or, Not
} Operator;



/**
 * How to get a variable length array from the stack
 * POP operator
 * POP array info struct
 * POP array data into execution stack
**/





/**
 * How ADD works
 * POP operator
 * POP array info struct
 * POP array data into execution stack
 * POP operator
 * POP array info struct
 * Perform add on data stack top with data in operand stack
**/

void ADD(Context ctx) {
}




void DO(Context ctx, Operator op1, ...) {

}

#define COL 4
#define ROW 100

f32 BANK[ROW][COL] = {0};

int main(int argc, char const *argv[]) {
	
	f32 array_a[100] = {0};
	f32 array_b[100] = {0};
	f32 array_c[100] = {0};

	Context ctx = {0};

	Load(ctx, array_a);
	Load(ctx, array_b);

	DO(ctx,
		Add
	);

	Save(ctx, array_c);

	return 0;
}
