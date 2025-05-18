/**
 * @brief Declarações relacionadas com o ficheiro inoc.c.
 * @file inoc.h
 * @author ist1114254 (Andre Cadete)
 */

#ifndef INOC_H
#define INOC_H

#include "estruturas.h"

#define UTENTE_MAX_BYTES 200
#define UTENTE_MAX_CHARS UTENTE_MAX_BYTES + 1

// Funções de comandos
int aplicaDose(char *cmd, Sys *sys);
int listaInocs(char *cmd, Sys *sys);
int apagaInoc(char *cmd, Sys *sys);

// Funções auxiliares
void imprimeInocs(Inoc *inocs, int tamanho);
int apagaInocSys(char *utente, Data data, char *lote, int scans, Sys *sys);

// Pesquisas
int existeUtente(char *utente, Sys *sys);
int inocsPorLote(char *lote, Sys *sys);
int pesquisaInocsPorUtente(char *utente, Inoc **inocs, Sys *sys);

// Processamento da linha de input
int recebeUtenteVacina(char *cmd, char **utente, char vacina[L_NOME_MAX_CHARS]);
int recebeUtenteDataLote(char *cmd, char **utente, Data *data,
                         char lote[LOTE_MAX_CHARS]);
int recebeUtente(char *cmd, char **utente);

// Sorting/otimização
void inocSort(Inoc *inocs, int tamanho);
unsigned long hash(char *str);

#endif