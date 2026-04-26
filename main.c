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

typedef struct resultados
{
    met *metricas;
    double tempo;
} resultados;

r *copiaVetor(r *orig, int tam) {
    r *novo = malloc(sizeof(r) * tam);
    memcpy(novo, orig, sizeof(r) * tam);
    return novo;
}

double calculaTempo(struct timespec inicio, struct timespec fim) {
    return ((fim.tv_sec - inicio.tv_sec) * 1000.0) +
           ((fim.tv_nsec - inicio.tv_nsec) / 1e6);
}

int verificaEstabilidade(r *vet, int n) {
    for (int i = 1; i < n; i++) {
        if (vet[i].chegada == vet[i-1].chegada) {
            if (vet[i].user_id < vet[i-1].user_id) {
                return 0; // instável
            }
        }
    }
    return 1; // estável
}

resultados rodaAlgoritmo(int alg, r *base, int tam) {
    r *vet = copiaVetor(base, tam);
    met *m;
    struct timespec inicio, fim;
    resultados result;

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
            m = NULL;
            break;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    result.metricas = m;
    result.tempo = calculaTempo(inicio, fim);

    liberaMetricas(m);
    free(vet);

    return result;
}

resultados mediaAlg(int alg, int tam, int tipoDataset) {
    double soma = 0;
    double repeticoes = 1;
    resultados result;

    // Para os vetores ordenados de forma aleatória ou quase ordenado,
    // executamos 30 repetições e fazemos uma média do tempo
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

        resultados result = rodaAlgoritmo(alg, vet, tam);
        soma += result.tempo;

        liberaVetor(vet);
    }

    return soma / repeticoes;
}

int main() {
    int tamanhos[] = {1000, 10000, 100000, 1000000, 10000000};
    int numTam = 4;

    char *nomesAlg[] = {
        "Bolha", "Selecao", "Insercao",
        "MergeSort", "QuickSort", "ShellSort", "HeapSort"
    };

    srand(time(NULL));

    FILE *csv_tempo = fopen("tabelas.csv", "w");
    FILE *csv_movimentacoes = fopen("tabelas.csv", "w");
    FILE *csv_comparacoes = fopen("tabelas.csv", "w");

    if (!csv_tempo || !csv_movimentacoes || ! csv_comparacoes) {
        printf("Erro ao criar csv\n");
        return 1;
    }

    for (int alg = 0; alg < 7; alg++) {

        fprintf(csv_tempo, "Algoritmo: %s\n", nomesAlg[alg]);
        fprintf(csv_tempo, "Teste,Tamanho,Aleatorio(ms),Crescente(ms),Decrescente(ms),Quase(ms)\n");

        for (int i = 0; i < numTam; i++) {
            int tam = tamanhos[i];

            // Pulei os mais lentos para 1 milhão e o quicksort está dando stack overflow
            // todo: talvez temos que implementar os algoritmos novamente e comparar com os já implementados dela
            // todo limitar os algoritmos n²
            if ((alg <= 2 && tam > 100000) || alg == 4)
                continue;

            resultados aleatorio = mediaAlg(alg, tam, 0);
            resultados crescente = mediaAlg(alg, tam, 1);
            resultados decrescente = mediaAlg(alg, tam, 2);
            resultados quase = mediaAlg(alg, tam, 3);

            // csv de tempo
            fprintf(csv_tempo, "%d,%d,%.6f,%.6f,%.6f,%.6f\n",
                    i + 1, tam,
                    aleatorio.tempo, crescente.tempo, decrescente.tempo, quase.tempo);

            // todo: csv de movimentações

            // todo: csv de comparações

            // todo: Verificando estabilidade
        }

        fprintf(csv_tempo, "\n");
        fprintf(csv_movimentacoes, "\n");
        fprintf(csv_comparacoes, "\n");
    }

    fclose(csv_tempo);
    fclose(csv_movimentacoes);
    fclose(csv_comparacoes);

    printf("Arquivo tabelas.csv gerado\n");
    return 0;
}