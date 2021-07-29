#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "fvec.h"

#define start_test(test_name) start = clock() / (CLOCKS_PER_SEC / 1000); printf(test_name); printf("..."); fflush(stdout)

#define show_test() \
	stop = clock() / (CLOCKS_PER_SEC / 1000);\
	printf("%li", (stop - start));\
	printf("ms\n")

#define CHUNK_SIZE 1024
#define PREALOCATED_SIZE 0
#define VECTOR_SIZE 100000
#define VECTOR_COPY_SIZE 100000

typedef struct{
	int n;
	char c;
} Test;

fvec_define(Test)

int8_t cmp_Test(Test* x, Test* y){
	if (x->n > y->n) return 1;
	return 0;
}

int8_t cmp_equal_Test(Test* x, Test* y){
	if (x->n == y->n && x->c == y->c) return 1;
	return 0;
}

int main(){
	clock_t start, stop;

	start_test("Iniciando serviço de testes sobre performance para fvec");

	srand(time(NULL));
	FVecTest* vec = malloc(sizeof(FVecTest));
	fvec_newTest(vec, &cmp_Test, &cmp_equal_Test, PREALOCATED_SIZE, CHUNK_SIZE);
	show_test();

	printf("Dados pré-definidos: \n\
	Tamanho dos blocos de alocação: %d\n\
	Elementos pré-alocados: %d\n\
	Tamanho do vetor: %d\n\
	Tamanho do vetor copiado: %d\n\n",
		CHUNK_SIZE,
		PREALOCATED_SIZE,
		VECTOR_SIZE,
		VECTOR_COPY_SIZE);

	start_test("Incrementando elementos ao vetor");
	for (size_t i = 0; i < VECTOR_SIZE; i++){
		fvec_appendTest(vec, ((Test){.n = 0, .c = ' '}));
	}
	show_test();

	start_test("Definindo valor dos elementos");
	for (size_t i = 0; i < VECTOR_SIZE; i++){
		fvec_set(vec, ((Test){.n = rand(), .c = ' '}), i);
	}
	show_test();

	start_test("Percorrendo dados inseridos");
	for (size_t i = 0; i < fvec_size(vec); i++){
		fvec_get(vec, i);
	}
	show_test();

	start_test("Ordenando dados de pós-alocação");
	fvec_sortTest(vec);
	show_test();

	start_test("Procurando dados aleatórios com pesquisa binária");
	for (size_t i = 0; i < fvec_size(vec); i++){
		fvec_get(vec, fvec_bfindTest(vec, fvec_get(vec, i)));
	}
	show_test();

	start_test("Copiando elementos do vetor principal");
	FVecTest* copied_vec = fvec_copyTest(vec, 0, VECTOR_COPY_SIZE);
	show_test();

	start_test("Verificando valores do vetor cópia com pesquisa linear");
	for (size_t i = 0; i < fvec_size(copied_vec); i++){
		fvec_lfindTest(copied_vec, fvec_get(copied_vec, i));
	}
	show_test();

	start_test("Liberando memória do vetor cópia");
	fvec_free(copied_vec);
	show_test();

	start_test("Liberando memória do vetor");
	fvec_free(vec);
	show_test();

	printf("Teste finalizado com sucesso!\nEncerrando sessão de benchmarking...\n");
	return 0;
}
