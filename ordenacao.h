//
// Created by Vanessa on 13/04/2026.
//

#include <stdlib.h>
#include <stdio.h>

#ifndef PROJ_ORDENACAO_H
#define PROJ_ORDENACAO_H

//Tipo de dado criado para encapsular as métricas de avaliação dos algoritmos de ordenação.
//Comparações são incrementadas sempre que duas posições do vetor têm seus valores comparados: if (vet[i] > vet[j]).
//Movimentações são incrementadas sempre que uma posição do vetor tem seu valor alterado: vet[i] = x.
typedef struct metricas met;
/*struct metricas
{
    unsigned long long comparacoes;
    unsigned long long movimentacoes;
};*/

//Tipo de dado criado para simular requisições de usuários
//As requisições são definidas pelo id do usuário e pelo tempo de chegada
typedef struct request r;
/*struct request {
    int user_id;   // chave de ordenação (prioridade implícita)
    int chegada;   // ordem de chegada (timestamp lógico)
};*/

//************ALGORITMOS DE ORDENAÇÃO **************//

// IMPORTANTE:
//Todos os métodos abaixo recebem o vetor de request e o tamanho do vetor.
// Alguns algoritmos retornam as métricas, enquanto outros recebem a estrutura de métricas por parâmetro.
// O vetor de entrada é sempre modificado in-place (não é criado um novo vetor).

//Ordenação pelo método bolha inteligente
//Retorna o vetor de métricas
met *bolhaInteligente(r *vet, int tam);

//Ordenação pelo método seleção
//Retorna o vetor de métricas
met *selecao(r *vet, int tam);

//Ordenação pelo método inserção
//Retorna o vetor de métricas
met *insercao(r *vet, int tam);

//Ordenação pelo método mergeSort
// O vetor é ordenado in-place.
// A estrutura de métricas deve ser previamente alocada e passada por parâmetro.
void mergeSort(r *vet, int inicio, int fim, met *m);
void merge(r *vet, int inicio, int meio, int fim, met *m);

//Ordenação pelo método quickSort
//O vetor de métricas deve ser passado para o método
//Implementação da versão clássica, onde o pivô é o primeiro elemento do vetor
// Utiliza particionamento do tipo Lomuto adaptado
void quickSort(r *vet, int inicio, int fim, met *m);
int particiona(r *vet, int inicio, int fim, met *m);

//Ordenação pelo método Shellsort
//Retorna o vetor de métricas
//Implementação da versão clássica, onde h inicia com h = tam/2 e é atualizado com h = h/2
met *shellSort(r *vet, int tam);

//Ordenação pelo método Heapsort
//Retorna o vetor de métricas
//Implementação da versão clássica, onde é construída uma heap de máximo
// depois é aplicado o processo de heapify
met *heapSort(r *vet, int tam);
void trocaMaior(r *vet, int pai, int tam, met *m) ; //auxiliar do heapsort

//Ordenação pelo método Bozosort
//Retorna o vetor de métricas
met *bozoSort(r *vet, int tam);

//*********IMPRESSÃO E MÉTRICAS DE AVALIAÇÃO**********************//

//Aloca a struct métricas e inicializa as variáveis
//comparacoes = 0
//movimentacoes = 0
met *alocaMetricas();

//Libera memória da struct de métricas
void liberaMetricas(met *m);

//Imprime o vetor na tela
//printf("%d - %d\n", vet[i].user_id, vet[i].chegada)
void imprimeVet(r *vet, int tam);

//Imprime as métricas na tela
//printf("Comparacoes: %llu\nMovimentacoes: %llu\n", m->comparacoes, m->movimentacoes);
void imprimeMetricas(met *m);

//Salva o vetor no arquivo nomeArquivo
//fprintf(arq, "%d\t%d\n", vet[i].user_id, vet[i].chegada)
void salvaVetor(r *vet, int tam, char *nomeArquivo);

//Salva as métricas no arquivo nomeArquivo
//fprintf(arq, "Comparacoes: %llu\nMovimentacoes: %llu\n", m->comparacoes, m->movimentacoes);
void salvaMetricas(met *m, char *nomeArquivo);

//***********GERAÇÃO DOS DATASETS*********************//

//IMPORTANTE:
// Todas as funções abaixo retornam um vetor de request alocado dinamicamente (deve ser liberado com free)

//Gera um vetor de request com valores aleatoriamente distribuidos
// Se seed != 0 → usa srand(seed)
// Se seed == 0 → usa srand(time(NULL))
r *geraAleatorios(int tam, unsigned int seed);

// Gera vetor quase ordenado
// porc: percentual de desordem (0 a 100)
// Processo:
// 1) gera vetor aleatório
// 2) ordena com mergeSort
// 3) embaralha porc% dos elementos
r *geraQuaseOrdenados(int tam, unsigned int seed, unsigned int porc);

// Gera vetor totalmente ordenado (crescente)
// Processo:
// 1) gera vetor aleatório
// 2) ordena com mergeSort
r *geraOrdenados(int tam, unsigned int seed);

// Gera vetor totalmente ordenado em ordem decrescente
// Processo:
// 1) gera vetor aleatório
// 2) ordena com mergeSort (ordem decrescente)
r *geraDecrescente(int tam, unsigned int seed);

// Libera a memória alocada para o vetor de Request (vet)
// Deve ser chamada para vetores criados pelas funções de geração (gera*)
// Após a chamada, o vetor não deve mais ser utilizado
void liberaVetor(r *vet);


#endif //PROJ_ORDENACAO_H
