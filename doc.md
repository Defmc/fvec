# Documentação

Afim de ajudá-lo a entender esta implementação de vetores, foi criada esta documentação. Qalquer tópico que não ficou muito claro, pode ser atualizado por meio de um [Pull Request](https://github.com/Defmc/fvec/pulls) ou sendo informado via [Issues](https://github.com/Defmc/fvec/issues). Caso queira apenas entender a utilização, também pode me contatar pelo Discord sobre o usuário `Defmc#2687`.

## fvec\_define(type)

Define uma struct-base em modelo de `FVec<type`, onde `type` é o tipo indicado na chamada a macro `fvec_define`.
Informações adicionais:
* Para cada objeto de compilação (cada arquivo `.c` ou `.cc` incluído nas flags do compilador) deve haver apenas um `fvec_define` para cada tipo afim de evitar múltiplas declarações de um mesmo escopo.

## fvec\_get(fvec, index)

Retorna um ponteiro para o elemento presente no índice (representado por `index`) indicado dentro de `fvec`. Esta macro é um alias para `fvec->start[index]` ou `*(fvec->start + index)`.

## fvec\_set(fvec, val, index)

Define o valor presente em `index` dentro de `fvec` como `val`, a mesma é um alias para `fvec->start[index] = val` ou `*(fvec->start + index) = val`.
Informações adicionais:
* Por questões de performance, esta macro não irá gerenciar as gravações de valores de modo a se assegurar que os mesmos já tenham sido alocados, em casos onde `Segmentation Fault` for retornado, há uma grande chance do motivo por trás disso ter sido a escrita a um endereço não reservado pela instância da struct.

## fvec\_size(fvec)

Retorna o tamanho atual de `fvec` tendo como valor a diferença entre o endereço de `fvec->end` e `fvec->start`. Em todo caso, o resultado será um valor do tipo `size_t`.
Informações adicionais:
* O tamanho de um vetor nesta implementação é seu espaço em uso, não seu devido valor alocado via `malloc`/`realloc`. Para esses casos, utilizar a macro `fvec_allocated_size(fvec)` seria mais indicado.

## fvec\_allocated\_size(fvec)

Semelhante a `fvec_size`, mas ao invés de retornar a quantidade de elementos do vetor, retorna o tamanho alocado dele. Tendo como base a diferença entre `fvec->alloc_adr` e `fvec->start`

## fvec\_is\_full(fvec)

Retorna se `fvec` está cheio, tendo como base a equivalência ou diferença de `fvec->end` e `fvec->alloc_adr`, visto que um vetor completamente em uso teria ambos os ponteiros indicado o mesmo endereço.

## fvec\_is\_empty(fvec)

Verifica se `fvec` está vazio, tendo como base a equivalência entre `fvec->end` e `fvec->start`.

## fvec\_slice(from, to, type, srt\_idx, end\_idx)

Define `to` como um slice (um fragmento do vetor principal) de `from`, tendo seu início o equivalente ao índice `srt_idx` de `from` e seu final o índice `end_idx` de `from`.
Informações adicionais:
* Esta macro compartilhará o `alloc_adr` e `grater_func`de `from` para `to`;
* Não haverá nenhuma alteração em `from`, ela deve-se igual durante o processo de slice para `to`;
* O slice não oferecerá suporte a chamadas de `fvec_resize<type>` já que ela não terá informações no ponteiro `to->start` a respeito dos metadados gerados por `from`;
* O compartilhamento dos membros da struct será instantâneo no entanto não será constante. Após a execução da macro, os valor podem muito bem não estarem mais sincronizados.

## fvec\_free(fvec)

Libera o espaço da memória utilizado por `fvec` e define todos seus membros sobre os elementos como `NULL`.
* Esta macro não pode ser chamada em um slice, visto que o mesmo não contém metadados sobre a alocação;
* A mesma não alterará membros que não dizem respeito aos elementos antes alocados, como `chunk_size` e `greater_func`.

# Observação antes da leitura dos métodos metaprogramados

Em vista de otimizar métodos que farão uso do tipo dos ponteiros de elementos da instância de fvec, as mesmas são geradas a partir de um template definido por `fvec_define`. Devido a isso, a leitura posterior conterá `##type` no final de cada método, este tipo de utilização do operador `##` em macros é chamado de _token pasting_ ou colagem de tokens, Onde o mesmo faz a concactenação de dois tokens distintos. Os tokens que são unidos neste caso será `FVec` e o tipo passado para `fvec_define`.

## void fvec\_new##type(FVec##type\* fvec, int8\_t (\*greater\_func)(type\* x, type\* y), const size\_t size, const size\_t chunk\_size)

Automatiza o processo de atribuição para uma instância de `FVec##type`, definindo seu delegate `FVec##type->greater_func`, o tamanho inicial do vetor e `FVec##type->chunk_size` do mesmo. Além de inicializar os ponteiros.
Informações adicionais:
* Caso queira alocar um vetor vazio, basta passar `size` como `0`.

## void fvec\_resize##type(FVec##type\* fvec, const size\_t new\_size)

Redimensiona o vetor de modo a atualizar os ponteiros do mesmo
Informações adicionais:
* Em casos onde a realocação falhe, você acabará perdendo todo seu vetor, por isso é recomendado utilizar um vetor temporário e fazer a realocação do mesmo em seguida passar o temporário para a instância desejada.

## FVec##type\* fvec\_append##type(FVec##type\* fvec, const type val)

Adiciona o elemento solicitado a `fvec` e atualiza o ponteiro `fvec->end` para `fvec->end + 1`.
Informações adicionais:
* Caso o vetor esteja cheio, o mesmo chamará `fvec_resize##type` e adicionará a quantidade de elementos indicada por `chunk_size`.

## void fvec\_swap##type(type\* elm1, type\* elm2)

Realiza a troca de dois elementos de modo a substituir `elm1` por `elm2` e `elm2` por `elm1`.

## void fvec\_sort##type(FVec##type\* fvec)

Realizará a ordenação do vetor utilizando uma versão otimizada do algoritmo _Quick Sort_, o qual possui complexidade `O(n log n)`.
Informações adicionais:
* Está função requer o membro `fvec->greater_func` ter sido inicializado. Caso contrário, retornará `Segmentation Fault` interrompendo a thread atual.

## size\_t fvec\_bfind##type(const FVec##type\* fvec, type val)

Retorna o índice de um elemento equivalente a `val` utilizando um algoritmo de complexidade `O(log n)` conhecido como _Binary Search_ ou Pesquisa Binária.
Informações adicionais:
* Este método requer um vetor já ordenado, seja manualmente ou utilizando `fvec_sort##type`. Caso contrário, o mesmo poderá retornar um valor aleatório ou até mesmo um erro `Segmentation Fault`.

## size\_t fvec\_lfind##type(const FVec##type\* fvec, const type val)

Retorna o índice do primeiro elemento equivalente a `val` utilizando um algoritmo de complexidade `O(n)` conhecido como _Linear Search_ ou Pesquisa Linear.

## const FVec##type\* fvec\_copy##type(const FVec##type\* fvec)

Irá criar uma instância do mesmo tipo de `fvec` e copiará todos seus valores, não apenas referenciando-os como em `fvec_slice`.
Informações adicionais:
* Diferente de `fvec_slice`, este método pode sofrer realocações e liberações sem nenhum problema.
