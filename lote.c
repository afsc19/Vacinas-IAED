/**
 * @brief Funções relacionadas com lotes.
 * @file lote.c
 * @author ist1114254 (Andre Cadete)
 */

#include "lote.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "erros.h"
#include "inoc.h"
#include "mergesort.h"

/**
 * Cria um lote novo.
 * @param cmd Linha de input
 * @param sys informações de estado
 * @returns Código de erro. (0 - Sucesso)
 */
int criaLote(char *cmd, Sys *sys) {
    int scans, i, espacos = 0, len_nome;
    Lote novoLote;
    Data novaData;
    if (sys->cntLotes >= MAX_LOTES)
        return ERR_DEM_VAC;
    scans =
        sscanf(cmd, "c %s %d-%d-%d %d %s", novoLote.lote, &novaData.dia,
               &novaData.mes, &novaData.ano, &novoLote.doses, novoLote.nome);
    
    // Verifica se os caracteres do nome não são demasiados
    // tal que sobrepuseram o lote.
    for (i = 0; i < (int)strlen(cmd) && espacos < 4; i++) {
        if (cmd[i] == ' ')
            espacos++;
        if (espacos == 4)
            len_nome = strlen(cmd) - i;
        if (len_nome > L_NOME_MAX_BYTES)
            return ERR_NOME_INV;
    }
    // Verificações dos valores necessários
    if (((0 < scans) && (scans < 1)) || !validaLote(novoLote.lote))
        return ERR_LOTE_INV;
    if (existeLote(novoLote.lote, sys))
        return ERR_LOTE_DUP;
    if (scans < 4 || !validaData(novaData, sys))
        return ERR_DATA_INV;
    if (scans < 5 || novoLote.doses <= 0)
        return ERR_QUANT_INV;
    if (scans < 6 || !validaNome(novoLote.nome))
        return ERR_NOME_INV;

    novoLote.aplicacoes = 0;
    novoLote.validade = novaData;
    sys->lotes = realloc(sys->lotes, sizeof(Lote) * ++sys->cntLotes);
    if (sys->lotes == NULL)
        return ERR_SEM_MEM;
    sys->lotes[sys->cntLotes - 1] = novoLote;

    printf("%s\n", novoLote.lote);
    return 0;
}

/**
 * Lista todos os lotes de acordo com os nomes das vacinas introduzidos no
 * input. Caso ocorra algum erro, imprime-o.
 * @param cmd Linha de input
 * @param sys informações de estado.
 * @returns Código de erro. (0 - Sucesso)
 */
int listaLotes(char *cmd, Sys *sys) {
    int i, li = 0, len = strlen(cmd), nlotes = -1;
    char vac[L_NOME_MAX_CHARS];
    Lote *lotes = malloc(sizeof(Lote));
    if (lotes == NULL)
        return ERR_SEM_MEM;
    if (len > 2 && !isspace(cmd[2])) {
        // Recebe uma vacina de cada vez
        for (i = 2; i < len; i++) {
            if (!isspace(cmd[i])) {
                vac[li++] = cmd[i];
            } else {
                vac[li] = '\0';
                // Imprime os lotes caso existam,
                // caso contrário imprime a mensagem de erro.
                if (!existeVacina(vac, sys))
                    printf("%s: %s\n", vac, erros[ERR_VAC_INEX - 1][sys->pt]);
                else if ((nlotes = pesquisaLotesPorVacina(vac, 0, &lotes, sys)))
                    imprimeLotes(lotes, nlotes);
                li = 0;
            }
        }
    } else {
        if ((nlotes = pesquisaLotesPorVacina("", 0, &lotes, sys)))
            imprimeLotes(lotes, nlotes);
    }
    free(lotes);
    return 0;
}

/**
 * Apaga um lote do sistema.
 * @param cmd Linha de input
 * @param sys Informações de estado
 * @returns Código de erro. (0 - Sucesso)
 */
int retiraLote(char *cmd, Sys *sys) {
    int i, j, nInocs;
    char lote[LOTE_MAX_CHARS];
    sscanf(cmd, "r %s", lote);
    // Verifica se o lote existe
    if (!existeLote(lote, sys)) {
        printf("%s: %s\n", lote, erros[ERR_LOTE_INEX - 1][sys->pt]);
        return 0;
    }
    // Percorre os lotes
    for (i = 0; i < sys->cntLotes; i++) {
        if (!(strcmp(sys->lotes[i].lote, lote))) {
            // Caso não existam inoculações associadas
            if (!(nInocs = inocsPorLote(lote, sys))) {
                // Shift left dos lotes
                for (j = i + 1; j < sys->cntLotes; j++) {
                    sys->lotes[j - 1] = sys->lotes[j];
                }
                if (--(sys->cntLotes)) {
                    sys->lotes =
                        realloc(sys->lotes, sys->cntLotes * sizeof(Lote));
                    if (sys->lotes == NULL)
                        return ERR_SEM_MEM;
                }
            } else {
                sys->lotes[i].doses = 0;
            }
            printf("%d\n", nInocs);
            break;
        }
    }
    return 0;
}

/**
 * Ordena o vetor dos lotes utilizando o algoritmo Merge Sort, implementado em
 * merge_sort.c.
 * @param lotes Vetor dos lotes a serem ordenados
 * @param tamanho Tamanho do vetor
 */
void loteSort(Lote *lotes, int tamanho) {
    merge_sort_Lotes(lotes, 0, tamanho - 1);
}

/**
 * Imprime os lotes recebidos.
 * @param lotes Vetor com os lotes a serem impressos
 * @param tamanho Tamanho do vetor
 * @param sys Informações de estado
 */
void imprimeLotes(Lote *lotes, int tamanho) {
    int i;
    for (i = 0; i < tamanho; i++)
        printf("%s %s %02d-%02d-%04d %d %d\n", lotes[i].nome, lotes[i].lote,
               lotes[i].validade.dia, lotes[i].validade.mes,
               lotes[i].validade.ano, lotes[i].doses, lotes[i].aplicacoes);
}

/**
 * Retira uma dose de um certo lote e marca como aplicada.
 * @param lote O lote a ser removido
 * @param sys Informações de estado
 */
void retiraDose(char *lote, Sys *sys) {
    int i;
    for (i = 0; i < sys->cntLotes; i++) {
        if (!strcmp(sys->lotes[i].lote, lote)) {
            sys->lotes[i].doses--;
            sys->lotes[i].aplicacoes++;
            break;
        }
    }
}

/**
 * Verifica se o lote é válido.
 * @param lote O lote a ser verificado.
 * @param sys informações de estado.
 * @returns 1 se o lote for válido, 0 caso contrário.
 */
int validaLote(const char *lote) {
    int i, len = strlen(lote);
    if (len > LOTE_MAX_BYTES)
        return 0;
    // Verifica se é constitúido por dígitos hexadecimais maiúsculos
    for (i = 0; i < len; i++)
        if (!(('0' <= lote[i] && lote[i] <= '9') ||
              ('A' <= lote[i] && lote[i] <= 'F')))
            return 0;
    return 1;
}

/**
 * Verifica se o nome é válido.
 * @param nome O nome
 * @returns 1 se o nome for válido, 0 caso contrário.
 */
int validaNome(const char *nome) {
    if (strlen(nome) > L_NOME_MAX_BYTES) {
        return 0;
    }
    // Verifica se tem espaços
    for (int i = 0; nome[i] != '\0'; i++) {
        if (isspace(nome[i])) {
            return 0;
        }
    }
    return 1;
}

/**
 * Verifica se o lote já existe no sistema.
 * @param lote O lote a ser verificado.
 * @param sys informações de estado.
 * @returns 1 se o lote for válido, 0 caso contrário.
 */
int existeLote(const char *lote, Sys *sys) {
    int i;
    for (i = 0; i < sys->cntLotes; i++)
        if (!strcmp(sys->lotes[i].lote, lote))
            return 1;
    return 0;
}

/**
 * Verifica se a vacina já existe nalgum dos lotes do sistema.
 * @param nome O nome da vacina
 * @param sys Informações de estado
 * @returns 1 se a vacina existir, 0 caso contrário.
 */
int existeVacina(const char *nome, Sys *sys) {
    int i;
    for (i = 0; i < sys->cntLotes; i++)
        if (!strcmp(sys->lotes[i].nome, nome))
            return 1;
    return 0;
}

/**
 * Pesquisa todos os lotes com a vacina especificada dentro da validade.
 * @param vacina Nome da vacina, vazio caso não pretendido
 * @param comDoses 1 para filtrar por doses disponiveis, 0 caso contrario
 * @param resultados Vetor com os resultados
 * @param sys Informações de estado
 * @returns O tamanho do vetor com os resultados
 */
int pesquisaLotesPorVacina(char *vacina, int comDoses, Lote **resultados,
                           Sys *sys) {
    int i, tamanho = 0;
    // Encontra os lotes dentro da validade e verifica o nome da vacina e o
    // número de doses, caso pretendido
    for (i = 0; i < sys->cntLotes; i++) {
        if ((!strcmp(vacina, "") || !strcmp(sys->lotes[i].nome, vacina)) &&
            verificaValidade(sys->lotes[i].validade, sys) &&
            (!comDoses || sys->lotes[i].doses > 0)) {
            // Mantém o vetor resultados apenas com a memória necessária alocada
            if (++tamanho > 1)
                *resultados = realloc(*resultados, tamanho * sizeof(Lote));
            (*resultados)[tamanho - 1] = sys->lotes[i];
        }
    }
    loteSort(*resultados, tamanho);
    return tamanho;
}

/**
 * Verifica se um utente já foi vacinado no mesmo dia para a mesma vacina.
 * @param utente Nome da utente
 * @param vacina Nome do vacina
 * @param sys informações de estado
 * @returns 1 se já foi vacinado, 0 caso contrário.
 */
int vacinado(char *vacina, char *utente, Sys *sys) {
    int i, j, numLotes;
    Lote *lotes = malloc(sizeof(Lote));
    // Guarda todos os lotes com a vacina pretendida
    if (!(numLotes = pesquisaLotesPorVacina(vacina, 0, &lotes, sys))) {
        free(lotes);
        return 0;
    }
    unsigned long uhash = hash(utente);
    // Encontra as inoculações com o nome de utente e com a data atual
    for (i = 0; i < sys->cntInocs; i++)
        if (sys->inocs[i].hash == uhash &&
            comparaDatas(sys->inocs[i].dataAplicacao, sys->dataAtual) == 0)
            // Caso encontre um lote da vacina pretendida
            for (j = 0; j < numLotes; j++)
                if (!strcmp(lotes[j].lote, sys->inocs[i].lote)) {
                    free(lotes);
                    return 1;
                }
    free(lotes);
    return 0;
}