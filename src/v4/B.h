
/** 'B', STAGE 2, second pass, reduction, etc
 * - retrace stage1 byte code
 * - Resolve remaining references 
 * - Determine remaining types, sizes, dimensions
 * - Allocate arrays
 */
#include "common.h"

static idx ref() {
	return 0;
}

void B(){

	ref();

}