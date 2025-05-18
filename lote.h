/**
 * @brief Declarações relacionadas com o ficheiro lote.c.
 * @file lote.h
 * @author ist1114254 (Andre Cadete)
 */

#ifndef LOTE_H
#define LOTE_H

#include "estruturas.h"

#define MAX_LOTES 1000

// Funções de comandos
int criaLote(char *cmd, Sys *sys);
int listaLotes(char *cmd, Sys *sys);
int retiraLote(char *cmd, Sys *sys);

// Funções auxiliares
void loteSort(Lote *lotes, int tamanho);
void imprimeLotes(Lote *lotes, int tamanho);
void retiraDose(char *lote, Sys *sys);

// Validações
int validaLote(const char *lote);
int validaNome(const char *nome);

// Pesquisas
int existeLote(const char *lote, Sys *sys);
int existeVacina(const char *nome, Sys *sys);
int pesquisaLotesPorVacina(char *nome, int comDoses, Lote **resultado,
                           Sys *sys);
int vacinado(char *vacina, char *utente, Sys *sys);

#endif