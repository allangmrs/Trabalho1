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
    double tempo;
    met *metricas;
    r *vetorOrdenado;
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
        if (vet[i].user_id == vet[i-1].user_id) {
            if (vet[i].chegada < vet[i-1].chegada) {
                return 0; // instável
            }
        }
    }
    return 1; // estável
}

int particiona(r *vet, int inicio, int fim, met *m) {
    int idx_rand = inicio + rand() % (fim - inicio + 1);

    r tmp = vet[inicio];
    vet[inicio] = vet[idx_rand];
    vet[idx_rand] = tmp;
    m->movimentacoes += 3;

    r pivo = vet[inicio];
    int pos = inicio;

    for (int i = inicio + 1; i <= fim; i++) {
        m->comparacoes++;
        if (vet[i].chegada < pivo.chegada ||
   (vet[i].chegada == pivo.chegada && vet[i].user_id < pivo.user_id)) {
            pos++;
            if (i != pos) {
                r aux = vet[i];
                vet[i] = vet[pos];
                vet[pos] = aux;
                m->movimentacoes += 3;
            }
        }
    }

    r aux = vet[inicio];
    vet[inicio] = vet[pos];
    vet[pos] = aux;
    m->movimentacoes += 3;

    return pos;
}


void quickSortGrupo(r *vet, int inicio, int fim, met *m) {
    while (inicio < fim) {
        int pivo = particiona(vet, inicio, fim, m);

        // sempre recursiona no menor lado
        if (pivo - inicio < fim - pivo) {
            quickSortGrupo(vet, inicio, pivo - 1, m);
            inicio = pivo + 1; // continua no maior lado (loop)
        } else {
            quickSortGrupo(vet, pivo + 1, fim, m);
            fim = pivo - 1;
        }
    }
}

resultados rodaAlgoritmo(int alg, r *base, int tam, int retornaVetor) {
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
        case 7:
            m = alocaMetricas();
            quickSortGrupo(vet, 0, tam-1, m);
            break;
        default:
            m = NULL;
            break;
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    result.metricas = m;
    result.tempo = calculaTempo(inicio, fim);
    result.vetorOrdenado = NULL;

    if (retornaVetor)
    {
        result.vetorOrdenado = malloc(sizeof(r) * tam);
        for (int i = 0; i < tam; i++)
        {
            result.vetorOrdenado[i] = vet[i];
        }
    }
    free(vet);

    return result;
}

resultados mediaAlg(int alg, int tam, int tipoDataset) {
    double soma = 0;
    double repeticoes = 1;
    resultados result = { 0, NULL, NULL};

    met *metricas;
    metricas = alocaMetricas();
    if (!metricas)  return result;

    // Para os vetores ordenados de forma aleatória ou quase ordenado,
    // executamos 30 repetições e fazemos uma média do tempo
    if (tipoDataset == 0 || tipoDataset == 3)
        repeticoes = 30;

    for (int i = 0; i < repeticoes; i++) {
        int seed = rand();
        r *vet;

        //if ((alg == 4 || alg == 7) && (tipoDataset == 0 || tipoDataset == 2))
          //  continue;
        if (alg <= 2 && tipoDataset != 2)
            continue;

        if (tipoDataset == 0)
            vet = geraAleatorios(tam, seed);
        else if (tipoDataset == 1)
            vet = geraOrdenados(tam, seed);
        else if (tipoDataset == 2)
            vet = geraDecrescente(tam, seed);
        else
            vet = geraQuaseOrdenados(tam, seed, 10);

        resultados exec = rodaAlgoritmo(alg, vet, tam, 0);
        metricas->comparacoes += exec.metricas->comparacoes;
        metricas->movimentacoes += exec.metricas->movimentacoes;
        soma += exec.tempo;

        liberaMetricas(exec.metricas);
        liberaVetor(vet);
    }
    // O compilador me diz esse erro
    // Clang-Tidy: Narrowing conversion from 'double' to 'unsigned long long'
    metricas->comparacoes /= repeticoes;
    metricas->movimentacoes /= repeticoes;

    result.tempo = soma / repeticoes;
    result.metricas = metricas;
    return result;
}

void menuOpcoes(int *algEscolhido, int *modo, int *usarTodosDatasets)
{
    printf("===== CONFIGURACAO =====\n");
    printf("1 - Rodar TODOS algoritmos\n");
    printf("2 - Escolher algoritmo\n");
    printf("Opcao: ");
    scanf("%d", modo);

    if (*modo == 2) {
        printf("\nEscolha o algoritmo:\n");
        printf("0 - Bolha\n");
        printf("1 - Selecao\n");
        printf("2 - Insercao\n");
        printf("3 - MergeSort\n");
        printf("4 - QuickSort\n");
        printf("5 - ShellSort\n");
        printf("6 - HeapSort\n");
        printf("7 - QuickSortGrupo\n");
        printf("Opcao: ");
        scanf("%d", algEscolhido);
    }

}

int main() {
    int tamanhos[] = {1000, 10000, 100000, 1000000, 10000000};
    //int tamanhos[] = {100};
    int numTam = 5;

    char *nomesAlg[] = {
        "Bolha", "Selecao", "Insercao",
        "MergeSort", "QuickSort", "ShellSort", "HeapSort", "QuickSortGrupo"
    };

    srand(time(NULL));

    int modo = 1;
    int algEscolhido = -1;
    int usarTodosDatasets = 1;

    menuOpcoes(&algEscolhido, &modo, &usarTodosDatasets);

    FILE *csv_tempo = fopen("tabela_tempo.csv", "w");
    FILE *csv_movimentacoes = fopen("tabela_movimentacoes.csv", "w");
    FILE *csv_comparacoes = fopen("tabela_comparacoes.csv", "w");
    FILE *csv_estabilidade = fopen("tabela_estabilidade.csv", "w");

    if (!csv_tempo || !csv_movimentacoes || ! csv_comparacoes) {
        printf("Erro ao criar csv\n");
        return 1;
    }
    for (int alg = 0; alg < 8; alg++) {
        if (modo == 2 && alg != algEscolhido)
            continue;

#pragma region nometabelas
        fprintf(csv_tempo, "Algoritmo: %s\n", nomesAlg[alg]);
        fprintf(csv_movimentacoes, "Algoritmo: %s\n", nomesAlg[alg]);
        fprintf(csv_comparacoes, "Algoritmo: %s\n", nomesAlg[alg]);

        fprintf(csv_tempo, "Teste,Tamanho,Aleatorio(ms),Crescente(ms),Decrescente(ms),Quase(ms)\n");
        fprintf(csv_movimentacoes, "Teste,Tamanho,Aleatorio,Crescente,Decrescente,Quase\n");
        fprintf(csv_comparacoes, "Teste,Tamanho,Aleatorio,Crescente,Decrescente,Quase\n");
#pragma endregion

        for (int i = 0; i < numTam; i++) {
            int tam = tamanhos[i];

            // Pulei os mais lentos para 1 milhão e o quicksort está dando stack overflow
            if ((alg <= 2 && tam > 100000))
                continue;

            resultados aleatorio = mediaAlg(alg, tam, 0);
            resultados crescente = mediaAlg(alg, tam, 1);
            resultados decrescente = mediaAlg(alg, tam, 2);
            resultados quase = mediaAlg(alg, tam, 3);

#pragma region csv
            // csv de tempo
            fprintf(csv_tempo, "%d,%d,%.6f,%.6f,%.6f,%.6f\n",
                    i + 1, tam,
                    aleatorio.tempo, crescente.tempo, decrescente.tempo, quase.tempo);

            // csv de movimentações
            fprintf(csv_movimentacoes, "%d,%d,%.6llu,%.6llu,%.6llu,%.6llu\n",
                    i + 1, tam,
                    aleatorio.metricas->movimentacoes, crescente.metricas->movimentacoes, decrescente.metricas->movimentacoes, quase.metricas->movimentacoes);

            // csv de comparações
            fprintf(csv_comparacoes, "%d,%d,%.6llu,%.6llu,%.6llu,%.6llu\n",
                    i + 1, tam,
                    aleatorio.metricas->comparacoes, crescente.metricas->comparacoes, decrescente.metricas->comparacoes, quase.metricas->comparacoes);
#pragma endregion

            liberaMetricas(aleatorio.metricas);
            liberaMetricas(crescente.metricas);
            liberaMetricas(decrescente.metricas);
            liberaMetricas(quase.metricas);
        }
        fprintf(csv_tempo, "\n");
        fprintf(csv_movimentacoes, "\n");
        fprintf(csv_comparacoes, "\n");
    }
    // Verificando estabilidade
    r vet[] = {
        {1, 1},
        {2, 1},
        {1, 2},
        {2, 2},
        {1, 3},
        {2, 3},
        {2, 4}
    };
    r *teste;
    int tamTeste = 7;
    for (int j = 0; j < 8; j++)
    {
        teste = rodaAlgoritmo(j, vet, tamTeste, 1).vetorOrdenado;
        int estavel = verificaEstabilidade(teste, tamTeste);

        fprintf(csv_estabilidade, "Algoritmo: %s\n", nomesAlg[j]);
        fprintf(csv_estabilidade, "Estável: %d\n", estavel);

        for (int i = 0; i < tamTeste; i++) {
            printf("(%d,%d) ", teste[i].user_id, teste[i].chegada);
        }
        printf("\n");
        free(teste);
    }
    fclose(csv_tempo);
    fclose(csv_movimentacoes);
    fclose(csv_comparacoes);
    fclose(csv_estabilidade);

    printf("Arquivos gerados\n");
    return 0;
}

// met *bozoSort(r *vet, int tam) {
//     met *m = alocaMetricas();
//     if (!m) return NULL;
//
//     int ordenado = 0;
//
//     while (!ordenado) {
//         ordenado = 1;
//
//         // verifica se está ordenado
//         for (int i = 1; i < tam; i++) {
//             m->comparacoes++;
//             if (vet[i].chegada < vet[i - 1].chegada) {
//                 ordenado = 0;
//                 break;
//             }
//         }
//
//         if (!ordenado) {
//             int i = rand() % tam;
//             int j = rand() % tam;
//
//             // troca
//             r temp = vet[i];
//             vet[i] = vet[j];
//             vet[j] = temp;
//
//             m->movimentacoes += 3; // swap padrão
//         }
//     }
//
//     return m;
// }