/**
 * @brief Declarações relacionadas com o ficheiro data.c.
 * @file data.h
 * @author ist1114254 (Andre Cadete)
 */

#ifndef DATA_H
#define DATA_H

#include "estruturas.h"

// Função do comando t
int atualizaData(char *cmd, Sys *sys);

// Validações/Comparações
int validaFormatoData(Data data);
int validaData(Data d, Sys *sys);
int verificaValidade(Data data, Sys *sys);
int comparaDatas(Data d1, Data d2);

// Auxiliar
int maxDias(int mes);

#endif