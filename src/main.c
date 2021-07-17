#include <stdio.h>
#include <time.h>
#include "fvec.h"

fvec_define(int)

int8_t cmp_int(int* x, int* y){
	return (*x > *y ? 1 : 0);
}

int main(){
	srand(time(NULL));
	FVecint* vec = malloc(sizeof(FVecint));
	fvec_newint(vec, 0, 32);

	for (size_t i = 0; i < 50; i++){
		fvec_appendint(vec, i);
	}

	for (size_t i = 0; i < fvec_size(vec); i++){
		printf("Index: %ld | Value: %d | Adress: %p\n",
				i,
				*fvec_get(vec, i),
				vec->start + i);
	}

	return 0;
	fvec_free(vec);
}
