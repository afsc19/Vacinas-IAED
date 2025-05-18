/**
 * @brief Algortimo de merge sort, adaptado do exemplificado em aula teórica.
 * @file mergesort.c
 * @authors ist1114254 (Andre Cadete), Prof. Vasco Manquinho (exemplo em aula).
 */

#include "mergesort.h"

#include <stdlib.h>
#include <string.h>

#include "data.h"

#define key(A) (A)
#define exch_Lote(A, B) \
    {                   \
        Lote t = A;     \
        A = B;          \
        B = t;          \
    }
#define exch_Inoc(A, B) \
    {                   \
        Inoc t = A;     \
        A = B;          \
        B = t;          \
    }
#define cmpexch(A, B)    \
    if (less_Lote(B, A)) \
    exch_Lote(A, B)

/**
 * Função de merge sort para um vetor de lotes.
 * @param a Vetor de lotes
 * @param left O índice do primeiro lote a ordenar
 * @param right O índice do último lote a ordenar
 */
void merge_sort_Lotes(Lote a[], int left, int right) {
    int m = (right + left) / 2;
    if (right <= left) {
        return;
    }
    Lote *aux = malloc((right + 1) * sizeof(Lote));
    merge_sort_Lotes(a, left, m);
    merge_sort_Lotes(a, m + 1, right);
    merge_Lotes(a, left, m, right, aux);

    free(aux);
}

/**
 * Função de merge para um vetor de lotes.
 * @param a Subvetor de lotes a ser ordenado
 * @param left Índice do primeiro lote do subvetor
 * @param m Índice do lote do meio do subvetor
 * @param right Índice do último lote do subvetor
 * @param aux Vetor auxiliar com os elementos do merge
 */
void merge_Lotes(Lote a[], int left, int m, int right, Lote *aux) {
    int i, j, k;
    for (i = m + 1; i > left; i--)
        aux[i - 1] = a[i - 1];
    for (j = m; j < right; j++)
        aux[right + m - j] = a[j + 1];

    for (k = left; k <= right; k++) {
        if (less_Lote(aux[j], aux[i]) || i == m + 1)
            a[k] = aux[j--];
        else
            a[k] = aux[i++];
    }
}

/**
 * Compara dois lotes e indica qual o menor.
 * @param A O primeiro lote
 * @param B O segundo lote
 * @returns 1 se o lote A for menor que o lote B, 0 caso contrário.
 */
int less_Lote(Lote A, Lote B) {
    return (comparaDatas(A.validade, B.validade) < 0 ||
            (comparaDatas(A.validade, B.validade) == 0 &&
             strcmp(A.lote, B.lote) < 0));
}

/**
 * Função de merge sort para um vetor de inoculações.
 * @param a Vetor de inoculações
 * @param left O índice da primeira inoculação a ordenar
 * @param right O índice da útlima inoculação a ordenar
 */
void merge_sort_Inocs(Inoc a[], int left, int right) {
    int m = (right + left) / 2;
    if (right <= left) {
        return;
    }
    Inoc *aux = malloc((right + 1) * sizeof(Inoc));
    merge_sort_Inocs(a, left, m);
    merge_sort_Inocs(a, m + 1, right);
    merge_Inocs(a, left, m, right, aux);
    free(aux);
}

/**
 * Função de merge para um vetor de inoculações.
 * @param a Subvetor de inoculações a ser ordenado
 * @param left Índice da primeira inoculação do subvetor
 * @param m Índice da inoculação do meio do subvetor
 * @param right Índice da última inoculação do subvetor
 * @param aux Vetor auxiliar com os elementos do merge
 */
void merge_Inocs(Inoc a[], int left, int m, int right, Inoc *aux) {
    int i, j, k;
    for (i = m + 1; i > left; i--)
        aux[i - 1] = a[i - 1];
    for (j = m; j < right; j++)
        aux[right + m - j] = a[j + 1];

    for (k = left; k <= right; k++) {
        if (less_Inoc(aux[j], aux[i]) || i == m + 1)
            a[k] = aux[j--];
        else
            a[k] = aux[i++];
    }
}

/**
 * Compara duas inoculações e indica qual a menor.
 * @param A A primeira inoculação
 * @param B A segunda inoculação
 * @returns 1 se a inoculação A for menor que a inoculação B, 0 caso contrário.
 */
int less_Inoc(Inoc A, Inoc B) {
    return comparaDatas(A.dataAplicacao, B.dataAplicacao) < 0;
}