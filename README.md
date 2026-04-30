# Sistema de Benchmarking de Algoritmos de Ordenação em C

Projeto desenvolvido para análise comparativa de algoritmos de ordenação utilizando métricas de desempenho para a matéria Algoritmos e Estruturas de Dados 2.

## Objetivo

Implementar diferentes algoritmos de ordenação e comparar:

- Tempo de execução
- Quantidade de comparações
- Quantidade de movimentações
- Estabilidade dos algoritmos

---

# Algoritmos Implementados

- Bubble Sort
- Selection Sort
- Insertion Sort
- Merge Sort
- Quick Sort
- Shell Sort
- Heap Sort
- Bozo Sort

---

# Estruturas Utilizadas

## request

```c
struct request {
    int user_id;
    int chegada;
};
```

Tipo de dado criado para simular requisições de usuários
As requisições são definidas pelo id do usuário e pelo tempo de chegada

---

## metricas

```c
struct metricas {
    unsigned long long comparacoes;
    unsigned long long movimentacoes;
};
```

Tipo de dado criado para armazenar as métricas de avaliação dos algoritmos de ordenação.

Comparações são incrementadas sempre que duas posições do vetor têm seus valores comparados: if (vet[i] > vet[j]). 

Movimentações são incrementadas sempre que uma posição do vetor tem seu valor alterado: vet[i] = x.

---

# Tipos de Dataset

Os algoritmos foram testados utilizando:

- Vetores aleatórios
- Vetores ordenados crescentemente
- Vetores ordenados decrescentemente
- Vetores quase ordenados

---

# Métricas Coletadas

- Quantidade de comparações realizadas.

- Movimentações realizadas.

- Tempo de execução em milissegundos.

---

# Estabilidade

O projeto também verifica se os algoritmos são estáveis.

Algoritmos estáveis mantêm a ordem relativa de elementos com chaves iguais.

---

# Arquivos Gerados

O programa gera automaticamente:

- `tabela_tempo.csv`
- `tabela_movimentacoes.csv`
- `tabela_comparacoes.csv`
- `tabela_estabilidade.csv`

---

# Observações

- Algoritmos O(n²) possuem limitação para entradas muito grandes.
- Foi adicionado outro QuickSort otimizado para evitar stack overflow.
- O BozoSort possui limitação de tamanho devido à sua complexidade extremamente alta.

---

# Autores
- Allan Pernes Guimarães
- Gustavo Dominguito Miranda Cruz
- João Otávio Zampieri Fornazeiro
- Lucas de Oliveira Paulino

Projeto desenvolvido para fins acadêmicos.
