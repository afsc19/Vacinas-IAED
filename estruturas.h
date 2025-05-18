/**
 * @brief Declarações das estruturas utilizadas no projeto.
 * @file estruturas.h
 * @author ist1114254 (Andre Cadete)
 */

#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

// Os tamanhos das cadeias de caracteres são incrementados em 1 para incluir o
// caracter null-terminator '\0'.
#define LOTE_MAX_BYTES 20
#define LOTE_MAX_CHARS LOTE_MAX_BYTES + 1
#define L_NOME_MAX_BYTES 50
#define L_NOME_MAX_CHARS L_NOME_MAX_BYTES + 1

typedef struct {
    int dia, mes, ano;
} Data;

typedef struct {
    char nome[L_NOME_MAX_CHARS], lote[LOTE_MAX_CHARS];
    Data validade;
    int doses, aplicacoes;
} Lote;

typedef unsigned long Hash;

typedef struct {
    char *utente, lote[LOTE_MAX_CHARS];
    Data dataAplicacao;
    Hash hash;
} Inoc;

typedef struct {
    int pt, cntLotes, cntInocs;
    Data dataAtual;
    Lote *lotes;
    Inoc *inocs;
} Sys;


#endif