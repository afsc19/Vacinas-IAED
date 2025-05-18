/**
 * @brief Funções relacionadas com inoculações.
 * @file inoc.c
 * @author ist1114254 (Andre Cadete)
 */

#include "inoc.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "erros.h"
#include "lote.h"
#include "mergesort.h"

/**
 * Aplica uma dose de vacina a um utente
 * @param cmd Linha de input
 * @param sys informações de estado
 * @returns Código de erro. (0 - Sucesso)
 */
int aplicaDose(char *cmd, Sys *sys) {
    char vacina[L_NOME_MAX_CHARS] = "\0";
    int vac = 0;
    Inoc novaInoc;
    Lote *resultados = malloc(sizeof(Lote));
    // Irá ter pelo menos um caracter e um '\0'
    novaInoc.utente = malloc(2 * sizeof(char));
    if (resultados == NULL || novaInoc.utente == NULL) {
        return ERR_SEM_MEM;
    }
    recebeUtenteVacina(cmd, &novaInoc.utente, vacina);
    // Verifica se é possível realizar a inoculação
    if (!pesquisaLotesPorVacina(vacina, 1, &resultados, sys) ||
        (vac = vacinado(vacina, novaInoc.utente, sys))) {
        free(novaInoc.utente);
        free(resultados);
        if (!vac)
            return ERR_ESGOTADO;
        return ERR_VACINADO;
    }
    retiraDose(resultados[0].lote, sys);
    strncpy(novaInoc.lote, resultados[0].lote, LOTE_MAX_CHARS);
    novaInoc.hash = hash(novaInoc.utente);
    novaInoc.dataAplicacao = sys->dataAtual;

    sys->inocs = realloc(sys->inocs, sizeof(Inoc) * ++sys->cntInocs);
    if (sys->inocs == NULL)
        return ERR_SEM_MEM;
    sys->inocs[sys->cntInocs - 1] = novaInoc;

    printf("%s\n", resultados[0].lote);
    free(resultados);
    return 0;
}

/**
 * Lista todos as inoculações de acordo com os nomes dos utentes introduzidos no
 * input. Caso ocorra algum erro, imprime-o.
 * @param cmd Linha de input
 * @param sys informações de estado.
 * @returns Código de erro. (0 - Sucesso)
 */
int listaInocs(char *cmd, Sys *sys) {
    int nlotes = -1;
    char *utente = malloc(2 * sizeof(char));
    if (utente == NULL)
        return ERR_SEM_MEM;
    Inoc *inocs = malloc(sizeof(Inoc));
    if (inocs == NULL)
        return ERR_SEM_MEM;

    if ((int)strlen(cmd) > 2) {
        if (recebeUtente(cmd, &utente)==-1)
            return ERR_SEM_MEM;
        // Verifica se o utente existe e se existem inoculações associadas
        if (!existeUtente(utente, sys))
            printf("%s: %s\n", utente, erros[ERR_UT_INEX - 1][sys->pt]);
        else if ((nlotes = pesquisaInocsPorUtente(utente, &inocs, sys)) > 0)
            imprimeInocs(inocs, nlotes);
        else if (nlotes == -1)
            return ERR_SEM_MEM;
        else
            inocs = NULL;
    } else {
        // Verifica se existem inoculações no sistema
        if ((nlotes = pesquisaInocsPorUtente("", &inocs, sys)) > 0)
            imprimeInocs(inocs, nlotes);
        if (nlotes == -1)
            return ERR_SEM_MEM;
    }
    if (inocs != NULL)
        free(inocs);
    free(utente);
    return 0;
}

/**
 * Apaga uma inoculação do sistema.
 * @param cmd Linha de input
 * @param sys Informações de estado
 * @returns Código de erro. (0 - Sucesso)
 */
int apagaInoc(char *cmd, Sys *sys) {
    int scans, dels;
    char *utente = malloc(2 * sizeof(char)), lote[LOTE_MAX_CHARS] = "\0";
    if (utente == NULL)
        return ERR_SEM_MEM;
    Data *data = malloc(sizeof(Data));
    scans = recebeUtenteDataLote(cmd, &utente, data, lote);
    // Valida os valores recebidos
    if (scans == -2)
        return ERR_SEM_MEM;
    if (!existeUtente(utente, sys)) {
        printf("%s: %s\n", utente, erros[ERR_UT_INEX - 1][sys->pt]);
        free(utente);
        free(data);
        return 0;
    }
    if (scans >= 3 && (!validaFormatoData(*data) ||
                       comparaDatas(*data, sys->dataAtual) > 0)) {
        free(utente);
        free(data);
        return ERR_DATA_INV;
    }
    if (scans == 4 && !existeLote(lote, sys)) {
        printf("%s: %s\n", lote, erros[ERR_LOTE_INEX - 1][sys->pt]);
        free(utente);
        free(data);
        return 0;
    }
    // Pesquisa e apaga as inoculações pretendidas do sistema
    if ((dels = apagaInocSys(utente, *data, lote, scans, sys)) == -1)
        return ERR_SEM_MEM;
    printf("%d\n", dels);
    free(utente);
    free(data);
    return 0;
}

/**
 * Imprime as inoculações recebidas.
 * @param inocs Vetor com as inoculações a serem impressas
 * @param tamanho Tamanho do vetor recebido
 */
void imprimeInocs(Inoc *inocs, int tamanho) {
    int i;
    for (i = 0; i < tamanho; i++)
        printf("%s %s %02d-%02d-%04d\n", inocs[i].utente, inocs[i].lote,
               inocs[i].dataAplicacao.dia, inocs[i].dataAplicacao.mes,
               inocs[i].dataAplicacao.ano);
}

/**
 * Apaga inoculações de acordo com o utente e,
 * caso pretendido, com a data e com o lote.
 * @param utente Nome do utente
 * @param data Data da/s inoculação/ões
 * @param scans Número de variáveis inseridas pelo utilizador
 * @param sys Informações de estado
 * @returns O número de inoculações apagadas caso suceda,
 * -1 caso tenha erros de memória.
 */
int apagaInocSys(char *utente, Data data, char *lote, int scans, Sys *sys) {
    int i, j, dels = 0;
    // Encontra as inoculações com o nome de utente e, caso pretendido, com a
    // data de aplicação e lote especificados
    for (i = 0; i < sys->cntInocs; i++) {
        if (!strcmp(sys->inocs[i].utente, utente) &&
            (scans < 3 ||
             comparaDatas(sys->inocs[i].dataAplicacao, data) == 0) &&
            (scans < 4 || !strcmp(sys->inocs[i].lote, lote))) {
            dels++;
            free(sys->inocs[i].utente);
            // Realiza um shift left das inoculações
            for (j = i + 1; j < sys->cntInocs; j++) {
                sys->inocs[j - 1] = sys->inocs[j];
            }
            // Mantém o vetor inocs apenas com a memória necessária alocada
            if (--sys->cntInocs) {
                sys->inocs = realloc(sys->inocs, sys->cntInocs * sizeof(Inoc));
                if (sys->inocs == NULL)
                    return -1;
            }
            i--;
        }
    }
    return dels;
}

/**
 * Verifica se o utente existe nalguma das inoculações do sistema.
 * @param nome O nome do utente
 * @param sys Informações de estado
 * @returns 1 se o utente existir, 0 caso contrário.
 */
int existeUtente(char *utente, Sys *sys) {
    int i;
    Hash uhash = hash(utente);
    for (i = 0; i < sys->cntInocs; i++)
        if (sys->inocs[i].hash == uhash)
            return 1;
    return 0;
}

/**
 * Conta o número de inoculações associadas com um lote.
 * @param lote O lote pretendido
 * @param sys Informações de estado
 */
int inocsPorLote(char *lote, Sys *sys) {
    int i, nInocs = 0, tamanho = sys->cntInocs;
    for (i = 0; i < tamanho; i++)
        if (!strcmp(sys->inocs[i].lote, lote))
            nInocs++;
    return nInocs;
}

/**
 * Pesquisa as inoculações associadas ao utente especificado.
 * @param utente Nome do utente, vazio caso não pretendido
 * @param resultados Vetor com as inoculações filtradas
 * @param sys Informações de estado
 * @returns O tamanho do vetor com as inoculações filtradas
 */
int pesquisaInocsPorUtente(char *utente, Inoc **resultados, Sys *sys) {
    int i, tamanho = 0;
    Hash uhash = hash(utente);
    // Encontra as inoculações com o mesmo nome de utente, caso pretendido
    for (i = 0; i < sys->cntInocs; i++) {
        if (sys->inocs[i].hash == uhash || !strcmp(utente, "")) {
            // Mantém o vetor resultados apenas com a memória necessária alocada
            if (++tamanho > 1) {
                *resultados = realloc(*resultados, tamanho * sizeof(Inoc));
                if (*resultados == NULL)
                    return -1;
            }
            (*resultados)[tamanho - 1] = sys->inocs[i];
        }
    }
    inocSort(*resultados, tamanho);
    // Liberta a memória do vetor resultados caso não venha a ser utilizada
    if (!tamanho)
        free(*resultados);
    return tamanho;
}

/**
 * Guarda o nome do utente e da vacina nos vetores recebidos.
 * @param cmd Linha de input
 * @param utente Ponteiro para o nome do utente
 * @param vacina Ponteiro para a vacina
 * @returns 0 se executar com sucesso, -1 caso tenha falhas de memória.
 */
int recebeUtenteVacina(char *cmd, char **utente,
                       char vacina[L_NOME_MAX_CHARS]) {
    // Recebe o utente
    int i = recebeUtente(cmd, utente), j, offset=1;
    if (cmd[2] == '"')
        offset = 2;
    // Recebe a vacina
    for (j = i + offset;
         (j < L_NOME_MAX_BYTES + offset + i && !isspace(cmd[j])); j++) {
        vacina[j - offset - i] = cmd[j];
    }
    vacina[j - offset - i] = '\0';
    return 0;
}

/**
 * Guarda o nome do utente recebido.
 * @param cmd Linha de input
 * @param utente Ponteiro para o nome do utente
 * @param data Ponteiro para a data
 * @returns O número de valores lidos depois do nome do utente se executar com
 * sucesso, -1 caso não tenha lido nenhum, -2 caso tenha falhas de memória.
 */
int recebeUtenteDataLote(char *cmd, char **utente, Data *data,
                         char lote[LOTE_MAX_CHARS]) {
    int i = recebeUtente(cmd, utente), j;
    if (i==-1)
        return -2;
    // Caso não existam mais argumentos
    if (cmd[i + 1] == '\0')
        return 0;
    // Shift left do restante na linha de input
    for (j = 0; cmd[++i] != '\n'; j++) {
        cmd[j] = cmd[i];
    }
    cmd[j] = '\0';
    return sscanf(cmd, "%d-%d-%d %s", &data->dia, &data->mes, &data->ano, lote);
}

/**
 * Guarda o nome do utente recebido.
 * @param cmd Linha de input
 * @param utente Ponteiro para o nome do utente
 * @param data Ponteiro para a data
 * @returns O índice do comando onde acaba o utente caso leia com sucesso,
 * -1 caso tenha falhas de memória.
 */
int recebeUtente(char *cmd, char **utente) {
    int i, offset = 2;
    char next = ' ';
    if (cmd[2] == '"') {
        offset = 3;
        next = '"';
    }
    for (i = offset; cmd[i] != next && (next != ' ' || !isspace(cmd[i])); i++) {
        *utente = realloc(*utente, (i - offset + 2) * sizeof(char));
        if (*utente == NULL)
            return -1;
        (*utente)[i - offset] = cmd[i];
    }
    (*utente)[i - offset] = '\0';
    return i;
}

/**
 * Ordena o vetor das inoculações utilizando o algoritmo Merge Sort,
 * implementado em merge_sort.c.
 * @param inocs Vetor das inoculações a serem ordenadas
 * @param tamanho Tamanho do vetor
 */
void inocSort(Inoc *inocs, int tamanho) {
    merge_sort_Inocs(inocs, 0, tamanho - 1);
}

/**
 * Adaptação da função de hash djb2.
 * http://www.cse.yorku.ca/~oz/hash.html
 *
 * @param str String para fazer hash
 * @returns Um hash como unsigned long.
 */
unsigned long hash(char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}
