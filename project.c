/* iaed25 - ist1114254 - project */
/**
 * @brief Ficheiro principal do projeto.
 * @file project.c
 * @author ist1114254 (Andre Cadete)
 */

#include "project.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"

/**
 * Função principal do projeto.
 * @returns Sempre 0.
 */
int main(int argc, char *argv[]) {
    int resultado, i, pt = (argc > 1 && !strcmp(argv[1], "pt"));
    char cmd[CMD_MAX_CHARS];
    Sys sys;

    // Inicialização das informações de estado
    sys.cntLotes = 0;
    sys.cntInocs = 0;
    sys.pt = pt;
    sys.dataAtual = (Data){1, 1, 2025};
    sys.inocs = malloc(sizeof(Inoc));
    sys.lotes = malloc(sizeof(Lote));
    if (sys.inocs == NULL || sys.lotes == NULL) {
        if (sys.inocs != NULL)
            free(sys.inocs);
        imprimeErro(ERR_SEM_MEM, pt);
        return 0;
    }

    do {
        if (fgets(cmd, CMD_MAX_CHARS, stdin) == NULL)
            break;
        // Caso ocorra algum erro, imprime-o
        if ((resultado = processa(cmd, &sys)) > 0)
            imprimeErro(resultado, pt);
    } while (cmd[0] != 'q' || resultado == ERR_SEM_MEM);

    // Libertações de memória
    free(sys.lotes);
    for (i = 0; i < sys.cntInocs; i++) {
        if (sys.inocs[i].utente != NULL)
            free(sys.inocs[i].utente);
    }
    free(sys.inocs);
    return 0;
}

/**
 * Imprime uma mensagem de erro.
 * @param codigo Código correspondente ao erro
 * @param pt Linguagem do programa (0 - ENG, 1 - PT)
 */
void imprimeErro(int codigo, int pt) {
    printf("%s\n", erros[codigo - 1][pt]);
}

/**
 * Processa o input.
 * @param cmd Linha de input
 * @param sys informações de estado
 * @returns Código de erro. (0 - Sucesso)
 */
int processa(char *cmd, Sys *sys) {
    switch (cmd[0]) {
        case 'c':
            return criaLote(cmd, sys);
        case 'l':
            return listaLotes(cmd, sys);
        case 'a':
            return aplicaDose(cmd, sys);
        case 'r':
            return retiraLote(cmd, sys);
        case 'd':
            return apagaInoc(cmd, sys);
        case 'u':
            return listaInocs(cmd, sys);
        case 't':
            return atualizaData(cmd, sys);
    }
    return 0;
}
