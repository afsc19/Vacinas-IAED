/**
 * @brief Declarações relacionadas com o ficheiro project.c.
 * @file project.h
 * @author ist1114254 (Andre Cadete)
 */

/* Para ter a certeza de que o ficheiro apenas define e declara tudo uma única
 * vez, ao compilar múltiplos ficheiros. */
#ifndef PROJECT_H
#define PROJECT_H

#include "erros.h"
#include "estruturas.h"
#include "inoc.h"
#include "lote.h"

// Os tamanhos das cadeias de caracteres são incrementados em 1 para incluir o
// caracter null-terminator '\0'.
#define CMD_MAX_CHARS 65535 + 1

int processa(char *cmd, Sys *sys);
void imprimeErro(int codigo, int pt);

#endif
