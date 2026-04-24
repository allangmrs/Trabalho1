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
    r *novo = malloc(sizeof(r) * tam);
    memcpy(novo, orig, sizeof(r) * tam);
    return novo;
}

double calculaTempo(struct timespec inicio, struct timespec fim) {
    return (fim.tv_sec - inicio.tv_sec) +
           (fim.tv_nsec - inicio.tv_nsec) / 1e9;
}

double rodaAlgoritmo(int alg, r *base, int tam) {
    r *vet = copiaVetor(base, tam);
    met *m;
    struct timespec inicio, fim;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    switch (alg) {
        case 0:
            m = bolhaInteligente(vet, tam);
            break;
        case 1:
            m = selecao(vet, tam);
            break;
        case 2:
            m = insercao(vet, tam);
            break;
        case 3:
            m = alocaMetricas();
            mergeSort(vet, 0, tam - 1, m);
            break;
        case 4:
            m = alocaMetricas();
            quickSort(vet, 0, tam - 1, m);
            break;
        case 5:
            m = shellSort(vet, tam);
            break;
        case 6:
            m = heapSort(vet, tam);
            break;
        default:
            return 0;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo = calculaTempo(inicio, fim);

    liberaMetricas(m);
    free(vet);

    return tempo;
}

double mediaAlg(int alg, int tam, int tipoDataset) {
    double soma = 0;
    double repeticoes = 1;

    if (tipoDataset == 0 || tipoDataset == 3)
        repeticoes = 30;

    for (int i = 0; i < repeticoes; i++) {
        int seed = rand();
        r *vet;

        if (tipoDataset == 0)
            vet = geraAleatorios(tam, seed);
        else if (tipoDataset == 1)
            vet = geraOrdenados(tam, seed);
        else if (tipoDataset == 2)
            vet = geraDecrescente(tam, seed);
        else
            vet = geraQuaseOrdenados(tam, seed, 10);

        soma += rodaAlgoritmo(alg, vet, tam);
        liberaVetor(vet);
    }

    return soma / repeticoes;
}

int main() {
    int tamanhos[] = {1000, 10000, 100000, 1000000};
    int numTam = 4;

    char *nomesAlg[] = {
        "Bolha", "Selecao", "Insercao",
        "MergeSort", "QuickSort", "ShellSort", "HeapSort"
    };

    srand(time(NULL));

    FILE *csv = fopen("tabelas.csv", "w");

    if (!csv) {
        printf("Erro ao criar csv\n");
        return 1;
    }

    for (int alg = 0; alg < 7; alg++) {

        fprintf(csv, "Algoritmo: %s\n", nomesAlg[alg]);
        fprintf(csv, "Teste,Tamanho,Aleatorio,Crescente,Decrescente,Quase\n");

        for (int i = 0; i < numTam; i++) {
            int tam = tamanhos[i];

            // Pulei os mais lentos para 1 milhão e o quicksort está dando stack overflow
            if ((alg <= 2 && tam > 100000) || alg == 4)
                continue;

            double aleatorio = mediaAlg(alg, tam, 0);
            double crescente = mediaAlg(alg, tam, 1);
            double decrescente = mediaAlg(alg, tam, 2);
            double quase = mediaAlg(alg, tam, 3);

            fprintf(csv, "%d,%d,%.6f,%.6f,%.6f,%.6f\n",
                    i + 1, tam,
                    aleatorio, crescente, decrescente, quase);
        }

        fprintf(csv, "\n\n");
    }

    fclose(csv);

    printf("Arquivo tabelas.csv gerado\n");
    return 0;
}