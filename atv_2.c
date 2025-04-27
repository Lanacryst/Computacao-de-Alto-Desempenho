/*Implemente três laços em C para investigar os efeitos do paralelismo ao nível de instrução (ILP): 
1) inicialize um vetor com um cálculo simples; 2) some seus elementos de forma acumulativa, 
criando dependência entre as iterações;e 3) quebre essa dependência utilizando múltiplas variáveis.
Compare o tempo de execução das versões compiladas com diferentes níveis de otimização (O1, O2, O3)
e analise com o estilo do código e as dependências influenciam o desempenho.*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void nivelUm(int n, int vetor[n], int *soma) {
    for (int i = 0; i < n; i++) {
        *soma += vetor[i];
    }
}

void tempoNivelUm(int n, int vetor[n]) {
    struct timespec start, stop;
    int soma = 0;
    clock_gettime(CLOCK_MONOTONIC, &start);
    nivelUm(n, vetor, &soma);
    clock_gettime(CLOCK_MONOTONIC, &stop);

    printf("Nivel 1 - Soma acumulativa: %d\n", soma);
    printf("Nivel 1 - Tempo: %f milissegundos\n", (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / 1e6);
}

void nivelDois(int n, int vetor[n], int *soma_total) {
    int soma0 = 0, soma1 = 0;
    for (int i = 0; i < n; i += 2) {
        soma0 += vetor[i];
        if (i + 1 < n) {
            soma1 += vetor[i+1];
        }
    }
    *soma_total = soma0 + soma1;
}

void tempoNivelDois(int n, int vetor[n]) {
    struct timespec start, stop;
    int soma_total = 0;
    clock_gettime(CLOCK_MONOTONIC, &start);
    nivelDois(n, vetor, &soma_total);
    clock_gettime(CLOCK_MONOTONIC, &stop);

    printf("Nivel 2 - Soma quebrando dependencia: %d\n", soma_total);
    printf("Nivel 2 - Tempo: %f milissegundos\n", (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / 1e6);
}

int main() {
    int n = 50000;
    int vetor[n];

    for (int i = 0; i < n; i++) {
        vetor[i] = i + 1;
    }

    tempoNivelUm(n, vetor);
    tempoNivelDois(n, vetor);

    return 0;
}
