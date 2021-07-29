#ifndef FVEC_HEADER
#define FVEC_HEADER

#include <stdlib.h>
#include <stdio.h>

#define fvec_get(fvec, index) (fvec->start[index])

#define fvec_set(fvec, val, index) (fvec->start[index] = val)

#define fvec_size(fvec) ((size_t)(fvec->end - fvec->start))

#define fvec_allocated_size(fvec) ((size_t)(fvec->alloc_adr - fvec->start))

#define fvec_is_full(fvec) (fvec->end >= fvec->alloc_adr)

#define fvec_is_empty(fvec) (fvec->end == fvec->start)

#define fvec_slice(from, to, type, srt_idx, end_idx) (FVec##type*)malloc(sizeof(FVec##type));\
 to->start = from->start + srt_idx;\
to->alloc_adr = from->alloc_adr;\
to->greater_func = from->greater_func;\
to->end = from->start + end_idx

#define fvec_free(fvec) \
	free(fvec->start);\
	fvec->start = fvec->alloc_adr = fvec->end = NULL;

#define fvec_define(type)\
typedef struct {\
	type* start;\
	type* alloc_adr;\
	type* end;\
	int8_t (*greater_func)(type* x, type* y);\
	int8_t (*equal_func)(type* x, type* y);\
	size_t chunk_size;\
} FVec##type;\
\
void fvec_new##type(FVec##type* fvec, int8_t (*greater_func)(type* x, type* y), int8_t (*equal_func)(type* x, type* y), const size_t size, const size_t chunk_size){\
	fvec->start = malloc(sizeof(type) * size);\
	fvec->end = fvec->start;\
	fvec->alloc_adr = fvec->start + size - 1;\
	fvec->chunk_size = chunk_size;\
	fvec->greater_func = greater_func;\
	fvec->equal_func = equal_func;\
}\
\
void fvec_resize##type(FVec##type* fvec, const size_t new_size){\
	const size_t old_size = fvec_size(fvec);\
	fvec->start = realloc(fvec->start, sizeof(type) * new_size);\
	fvec->alloc_adr = fvec->start + new_size - 1;\
	fvec->end = fvec->start + old_size;\
}\
\
FVec##type* fvec_append##type(FVec##type* fvec, const type val){\
	if (fvec_is_full(fvec)){\
		fvec_resize##type(fvec, fvec_size(fvec) + fvec->chunk_size);\
	}\
	*fvec->end = val;\
	fvec->end++;\
	return fvec;\
}\
\
void fvec_swap##type(type* elm1, type* elm2){\
	const type buff = *elm1;\
	*elm1 = *elm2;\
	*elm2 = buff;\
}\
\
void fvec_sort##type(FVec##type* fvec){\
	type piv;\
	int beg[1000], end[1000], i = 0, L, R ;\
\
	beg[0] = 0;\
	end[0] = fvec_size(fvec) - 1;\
	while (i >= 0) {\
		L = beg[i];\
		R = end[i] - 1;\
		if (L < R) {\
      			piv = fvec->start[L];\
			while (L < R) {\
		        	while ((fvec->greater_func(fvec->start + R, &piv) || fvec->equal_func(fvec->start + R, &piv)) == 1 && L < R)\
					R--;\
					if (L < R)\
					fvec->start[L++] = fvec->start[R];\
	        		while ((!fvec->greater_func(fvec->start + L, &piv) || fvec->equal_func(fvec->start +  L, &piv)) == 1 && L < R)\
					L++;\
					if (L < R)\
						fvec->start[R--] = fvec->start[L];\
			}\
			fvec->start[L] = piv;\
			beg[i + 1] = L + 1;\
			end[i + 1] = end[i];\
			end[i++] = L;\
		}\
		else {\
		    i--;\
		}\
	}\
}\
\
size_t fvec_bfind##type(const FVec##type* fvec, type val){\
	size_t srt = 0,\
		end = fvec_size(fvec) - 1,\
		tmp = end / 2;\
	while (fvec->greater_func(fvec->start + tmp, &val) == 1 && srt <= end){\
		if (fvec->greater_func(fvec->start + tmp, &val) == 1){\
			end = tmp - 1;\
		}\
		else {\
			srt = tmp + 1;\
		}\
		tmp = (srt + end) / 2;\
	}\
	return tmp;\
}\
\
size_t fvec_lfind##type(const FVec##type* fvec, const type val){\
	for (size_t i = 0; i < fvec_size(fvec); i++){\
		if (fvec->equal_func(fvec->start + i, &val) == 1){\
			return i;\
		}\
	}\
	return fvec_size(fvec);\
}\
\
FVec##type* fvec_copy##type(const FVec##type* from,\
		const size_t srt,\
		const size_t end){\
	FVec##type* result = malloc(sizeof(FVec##type));\
	FVec##type* to_copy = fvec_slice(from, to_copy, type, srt, end);\
	fvec_new##type(result, to_copy->greater_func, to_copy->equal_func, fvec_size(to_copy), to_copy->chunk_size);\
	for (size_t i = 0; i < fvec_size(to_copy); i++){\
		fvec_get(result, i) = fvec_get(to_copy, i);\
	}\
	return result;\
}

#endif
