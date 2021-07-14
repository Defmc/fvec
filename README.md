# Fast Vector

Uma simples e otimizada implementação de Arrays Dinâmicos (vetores) em C. Tem como foco ser otimizada para alocação/realocação/desalocação de valores e rápida para procura de dados.

Seu funcionamento é semelhante às classes genéricas presentes em diversas linguagens de programação moderna e busca otimizar funções simples (como acesso a um valor baseado em seu índice) por meio de expressões de metaprogramação geradas em compile-time. Não possuindo uma pilha de chamadas para a execução da macro.

## Utilização

Para implementar a `fvec` para um tipo específico, antes é necessário declará-lo por meio da macro `fvec_define(<tipo>)` de modo a gerar toda a programação referente a struct da mesma e funções auxiliares.

Após isso, basta utilizar todas as funções geradas de modo a acrescentar o nome do tipo ao final da mesma, como `fvec_new<tipo>` seria para o tipo `int` o equivalente a `fvec_newint`.

## Garantindo que a implementação para um tipo de fvec não ocorra duas vezes

~~(Ainda em produção)~~ Não é necessário nenhum tipo de interferência na declaração da mesma, a própria macro `fvec_define` se encarrega de gerenciar as chamadas duplicadas para definição de uma mesma implementação.

## Documentação

~~(Ainda em produção)~~ Toda a documentação pode ser conferida no arquivo de [documentação](../blob/master/doc.md) do repositório.

## Contribuições

Há várias formas de contribuir para o projeto, você pode por exemplo, abrir um [Pull Request](https://github.com/Defmc/fvec/pulls) para contribuições de código. Outras formas disponíveis é por meio de [issues](https://github.com/Defmc/fvec/issues) quanto a utilização da classe e/ou sugestões de implementação de novos recursos ou remodelagem dos já existentes.
