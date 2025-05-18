/**
 * @brief Funções relacionadas com datas.
 * @file data.c
 * @author ist1114254 (Andre Cadete)
 */

#include "data.h"

#include <stdio.h>

#include "erros.h"

/**
 * Verifica se a data é válida.
 * @param data A data a ser verificada
 * @returns 1 se a data for válida, 0 caso contrário
 */
int validaFormatoData(Data data) {
    return (1 <= data.mes && data.mes <= 12 && 1 <= data.dia &&
            data.dia <= maxDias(data.mes));
}

/**
 * Verifica se a data está no formato correto e é válida, de acordo com a data
 * atual.
 * @param data A data a ser verificada
 * @param sys informações de estado
 * @returns 1 se a data for válida, 0 caso contrário
 */
int validaData(Data data, Sys *sys) {
    return (validaFormatoData(data) && verificaValidade(data, sys));
}

/**
 * Verifica se a data é válida, de acordo com a data atual.
 * @param data A data a ser verificada
 * @param sys informações de estado
 * @returns 1 se a data for válida, 0 caso contrário
 */
int verificaValidade(Data data, Sys *sys) {
    return comparaDatas(data, sys->dataAtual) >= 0;
}

/**
 * Compara duas datas.
 * @param d1 A primeira data
 * @param d2 A segunda data
 * @returns 1 se a primeira data for posterior à segunda,
 * -1 se for anterior e 0 se forem iguais.
 */
int comparaDatas(Data d1, Data d2) {
    if (d1.ano != d2.ano)
        return (d1.ano > d2.ano) ? 1 : -1;
    if (d1.mes != d2.mes)
        return (d1.mes > d2.mes) ? 1 : -1;
    if (d1.dia != d2.dia)
        return (d1.dia > d2.dia) ? 1 : -1;
    return 0;
}

/**
 * Obtém o número máximo de dias consoante o mês.
 * @param mes O mês em causa
 * @returns O número máximo de dias, representado por um inteiro.
 */
int maxDias(int mes) {
    const int meses30[4] = {4, 6, 9, 11};
    // Assumindo que fevereiro tem sempre 28 dias
    if (mes == 2)
        return 28;
    for (int i = 0; i < 4; i++)
        if (mes == meses30[i])
            return 30;
    return 31;
}

/**
 * Atualiza a data atual.
 * @param cmd Linha de input
 * @param sys informações de estado
 * @returns Código de erro. (0 - Sucesso)
 */
int atualizaData(char *cmd, Sys *sys) {
    Data novaData;
    if (sscanf(cmd, "t %d-%d-%d", &novaData.dia, &novaData.mes,
               &novaData.ano) != 3 ||
        !validaData(novaData, sys))
        return ERR_DATA_INV;
    sys->dataAtual = novaData;
    printf("%02d-%02d-%04d\n", novaData.dia, novaData.mes, novaData.ano);
    return 0;
}