#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "fvec.h"

#define start_test(test_name) start = clock() / (CLOCKS_PER_SEC / 1000); printf(test_name); printf("...")
#define show_test() \
	stop = clock() / (CLOCKS_PER_SEC / 1000);\
	printf("%d", (stop - start));\
	printf("ms\n")

#define CHUNK_SIZE 1024
#define PREALOCATED_SIZE 0
#define VECTOR_SIZE 1000000 / 4

fvec_define(int)

int8_t cmp_int(int* x, int* y){
	return (*x > *y ? 1 : 0);
}

int main(size_t argc, char** argv){
	clock_t start, stop;

	start_test("Iniciando serviço de testes sobre performance para fvec");

	srand(time(NULL));
	FVecint* vec = malloc(sizeof(FVecint));
	fvec_newint(vec, PREALOCATED_SIZE, CHUNK_SIZE);
	vec->cmp_func = &cmp_int;
	show_test();

	printf("Dados pré-definidos: \n\
	Tamanho dos blocos de alocação: %d\n\
	Elementos pré-alocados: %d\n\
	Tamanho do vetor: %d\n\n",
		CHUNK_SIZE,
		PREALOCATED_SIZE,
		VECTOR_SIZE);

	start_test("Incrementando elementos ao vetor");
	for (size_t i = 0; i < VECTOR_SIZE; i++){
		fvec_appendint(vec, 0);
	}
	show_test();

	start_test("Definindo valor dos elementos");
	for (size_t i = 0; i < VECTOR_SIZE; i++){
		fvec_set(vec, rand(), i);
	}
	show_test();

	start_test("Percorrendo dados inseridos");
	for (size_t i = 0; i < fvec_size(vec); i++){
		*fvec_get(vec, i);
	}
	show_test();

	start_test("Ordenando dados de pós-alocação");
	fvec_sortint(vec);
	show_test();

	start_test("Procurando dados aleatórios com pesquisa binária");
	for (size_t i = 0; i < fvec_size(vec); i++){
		*fvec_get(vec, fvec_bfindint(vec, *fvec_get(vec, i)));
	}
	show_test();

	start_test("Procurando dados aleatórios com pesquisa linear");
	for (size_t i = 0; i < fvec_size(vec); i++){
		*fvec_get(vec, fvec_lfindint(vec, *fvec_get(vec, i)));
	}
	show_test();

	start_test("Liberando memória do vetor");
	fvec_free(vec);
	show_test();

	printf("Teste finalizado com sucesso!\nEncerrando sessão de benchmarking...\n");
	return 0;
}
