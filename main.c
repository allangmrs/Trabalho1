#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ordenacao.h"

struct metricas {
    unsigned long long comparacoes;
    unsigned long long movimentacoes;
};

struct request {
    int user_id;
    int chegada;
};

r *copiaVetor(r *orig, int tam) {
    r *novo = (r *) malloc(sizeof(r) * tam);
    memcpy(novo, orig, sizeof(r) * tam);
    return novo;
}

double calculaTempo(struct timespec inicio, struct timespec fim) {
    return (fim.tv_sec - inicio.tv_sec) +
           (fim.tv_nsec - inicio.tv_nsec) / 1e9;
}

void escreveCSV(FILE *f, char *alg, char *dataset, int tam, met *m, double tempo) {
    fprintf(f, "%s,%s,%d,%llu,%llu,%.9f\n",
            alg,
            dataset,
            tam,
            m->comparacoes,
            m->movimentacoes,
            tempo);
}

void testaAlgoritmos(r *base, int tam, char *tipo, FILE *csv) {
    r *vet;
    met *m;
    struct timespec inicio, fim;
    double tempo;
    // Merge
    vet = copiaVetor(base, tam);
    m = alocaMetricas();

    clock_gettime(CLOCK_MONOTONIC, &inicio);
    mergeSort(vet, 0, tam - 1, m);
    clock_gettime(CLOCK_MONOTONIC, &fim);

    tempo = calculaTempo(inicio, fim);
    escreveCSV(csv, "MergeSort", tipo, tam, m, tempo);

    liberaMetricas(m);
    free(vet);
    // Provavelmente o quicksort é o algoritmo bugado, ele causa stackoverflow com o tamanho muito grande de vetores
/*
    // Quick
    vet = copiaVetor(base, tam);
    m = alocaMetricas();

    clock_gettime(CLOCK_MONOTONIC, &inicio);
    quickSort(vet, 0, tam - 1, m);
    clock_gettime(CLOCK_MONOTONIC, &fim);

    tempo = calculaTempo(inicio, fim);
    escreveCSV(csv, "QuickSort", tipo, tam, m, tempo);

    liberaMetricas(m);
    free(vet);
    */

    // Shell
    vet = copiaVetor(base, tam);
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    m = shellSort(vet, tam);
    clock_gettime(CLOCK_MONOTONIC, &fim);

    tempo = calculaTempo(inicio, fim);
    escreveCSV(csv, "ShellSort", tipo, tam, m, tempo);

    liberaMetricas(m);
    free(vet);

    // Heap
    vet = copiaVetor(base, tam);
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    m = heapSort(vet, tam);
    clock_gettime(CLOCK_MONOTONIC, &fim);

    tempo = calculaTempo(inicio, fim);
    escreveCSV(csv, "HeapSort", tipo, tam, m, tempo);

    liberaMetricas(m);
    free(vet);
    if (tam <= 100000)
    {
        // Bolha
        vet = copiaVetor(base, tam);
        clock_gettime(CLOCK_MONOTONIC, &inicio);
        m = bolhaInteligente(vet, tam);
        clock_gettime(CLOCK_MONOTONIC, &fim);

        tempo = calculaTempo(inicio, fim);
        escreveCSV(csv, "Bolha", tipo, tam, m, tempo);

        liberaMetricas(m);
        free(vet);

        // Selecao
        vet = copiaVetor(base, tam);
        clock_gettime(CLOCK_MONOTONIC, &inicio);
        m = selecao(vet, tam);
        clock_gettime(CLOCK_MONOTONIC, &fim);

        tempo = calculaTempo(inicio, fim);
        escreveCSV(csv, "Selecao", tipo, tam, m, tempo);

        liberaMetricas(m);
        free(vet);

        // Insercao
        vet = copiaVetor(base, tam);
        clock_gettime(CLOCK_MONOTONIC, &inicio);
        m = insercao(vet, tam);
        clock_gettime(CLOCK_MONOTONIC, &fim);

        tempo = calculaTempo(inicio, fim);
        escreveCSV(csv, "Insercao", tipo, tam, m, tempo);

        liberaMetricas(m);
        free(vet);

    }
}

int main() {
    int tamanhos[] = {1000, 10000, 100000, 1000000, 10000000};
    srand(time(NULL));
    FILE *csv = fopen("resultados.csv", "w");
    if (!csv) {
        printf("Erro ao criar CSV\n");
        return 1;
    }

    // cabeçalho
    fprintf(csv, "algoritmo,dataset,tamanho,comparacoes,movimentacoes,tempo\n");
    for (int i = 0; i < 5; i++)
    {
        int tam = tamanhos[i];
        int seed = rand();
        r *ordenado = geraOrdenados(tam, seed);
        r *quase = geraQuaseOrdenados(tam, seed, 10);
        r *decrescente = geraDecrescente(tam, seed);

        if (tam >= 1000000) {
            testaAlgoritmos(ordenado, tam, "Ordenado", csv);
        } else {
            testaAlgoritmos(ordenado, tam, "Ordenado", csv);
            testaAlgoritmos(quase, tam, "Quase", csv);
            testaAlgoritmos(decrescente, tam, "Decrescente", csv);
        }
        liberaVetor(ordenado);
        liberaVetor(quase);
        liberaVetor(decrescente);
    }
    fclose(csv);
    printf("\ncsv gerado: resultados.csv\n");

    return 0;
}