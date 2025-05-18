/**
 * @brief Declarações relacionadas com o ficheiro mergesort.c.
 * @file mergesort.h
 * @author ist1114254 (Andre Cadete)
 */

#ifndef MERGESORT_H
#define MERGESORT_H

#include "estruturas.h"

// Lotes
void merge_sort_Lotes(Lote a[], int left, int right);
void merge_Lotes(Lote a[], int left, int m, int right, Lote *aux);
int less_Inoc(Inoc A, Inoc B);

// Inoculações
void merge_sort_Inocs(Inoc a[], int left, int right);
void merge_Inocs(Inoc a[], int left, int m, int right, Inoc *aux);
int less_Lote(Lote A, Lote B);

#endif