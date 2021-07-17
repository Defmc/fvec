#ifndef FVEC_HEADER
#define FVEC_HEADER

#include <stdlib.h>
#include <stdio.h>

#define fvec_get(fvec, index) (fvec->start + index)

#define fvec_set(fvec, val, index) (*(fvec->start + index) = val)

#define fvec_size(fvec) (fvec->end - fvec->start)

#define fvec_slice(from, to, type, srt_idx, end_idx) (FVec##type*)malloc(sizeof(FVec##type));\
 to->start = from->start + srt_idx;\
to->alloc_adr = from->alloc_adr;\
to->end = from->start + end_idx

#define fvec_free(fvec) \
	free(fvec->start);\
	fvec->start = fvec->alloc_adr = fvec->end = NULL;

#define fvec_define(type)\
typedef struct {\
	type* start;\
	type* alloc_adr;\
	type* end;\
	int8_t (*cmp_func)(type* x, type* y);\
	size_t chunk_size;\
} FVec##type;\
void fvec_new##type(FVec##type* fvec, const size_t size, const size_t chunk_size){\
	fvec->start = malloc(sizeof(type) * size);\
	fvec->end = fvec->start;\
	fvec->alloc_adr = fvec->start + size - 1;\
	fvec->chunk_size = chunk_size;\
}\
void fvec_resize##type(FVec##type* fvec, const size_t new_size){\
	const size_t old_size = fvec_size(fvec);\
	fvec->start = realloc(fvec->start, sizeof(type) * new_size);\
	fvec->alloc_adr = fvec->start + new_size - 1;\
	fvec->end = fvec->start + old_size;\
}\
\
FVec##type* fvec_append##type(FVec##type* fvec, const type val){\
	if (fvec->end >= fvec->alloc_adr){\
		fvec_resize##type(fvec, fvec_size(fvec) + fvec->chunk_size);\
	}\
	*(fvec->end) = val;\
	fvec->end++;\
	return fvec;\
}\
void fvec_swap##type(type* elm1, type* elm2){\
	const type buff = *elm1;\
	*elm1 = *elm2;\
	*elm2 = buff;\
}\
void fvec_sort##type(FVec##type* fvec){\
	size_t min_index = 0;\
	for (size_t range_init = 0; range_init < fvec_size(fvec); range_init++){\
		min_index = range_init;\
		for (size_t i = range_init; i < fvec_size(fvec); i++){\
			if (fvec->cmp_func(fvec->start + min_index, fvec->start + i)){\
				min_index = i;\
			}\
		}\
		fvec_swap##type(fvec->start + range_init, fvec->start + min_index);\
	}\
}\
size_t fvec_bfind##type(FVec##type* fvec, type val){\
	size_t srt = 0,\
		end = fvec_size(fvec) - 1,\
		tmp = end / 2;\
	while ((*fvec_get(fvec, tmp) != val && srt <= end)){\
		if (fvec->cmp_func(fvec_get(fvec, tmp), &val) == 1){\
			end = tmp - 1;\
		}\
		else {\
			srt = tmp + 1;\
		}\
		tmp = (srt + end) / 2;\
	}\
	return tmp;\
}\
size_t fvec_lfind##type(const FVec##type* fvec, const type val){\
	for (size_t i = 0; i < fvec_size(fvec); i++){\
		if (*(fvec->start + i) == val){\
			return i;\
		}\
	}\
	return fvec_size(fvec);\
}\
const FVec##type* fvec_copy##type(const FVec##type* fvec){\
	FVec##type* result = NULL;\
	fvec_new##type(result, fvec_size(fvec), fvec->chunk_size);\
	for (size_t i = 0; i < fvec_size(fvec); i++){\
		*(result->start + i) = *(fvec->start + i);\
	}\
	return result;\
}

#endif
