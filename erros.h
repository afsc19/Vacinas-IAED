/**
 * @brief Declarações relacionadas com os erros utilizados no projeto.
 * @file erros.h
 * @author ist1114254 (Andre Cadete)
 */

#define N_ERROS 12
#define LANGS 2
#define ERROS_MAX_CHARS 100

#define ERR_DEM_VAC 1
#define ERR_LOTE_DUP 2
#define ERR_LOTE_INV 3
#define ERR_NOME_INV 4
#define ERR_DATA_INV 5
#define ERR_QUANT_INV 6
#define ERR_VAC_INEX 7
#define ERR_ESGOTADO 8
#define ERR_VACINADO 9
#define ERR_LOTE_INEX 10
#define ERR_UT_INEX 11
#define ERR_SEM_MEM 12

/**
 * Vetor com todas as mensagens de erro, separadas por códigos (índices) e por
 * linguagens.
 */
static const char erros[N_ERROS][LANGS][ERROS_MAX_CHARS] = {
    {"too many vaccines", "demasiadas vacinas"},             // CODIGO 1
    {"duplicate batch number", "número de lote duplicado"},  // CODIGO 2
    {"invalid batch", "lote inválido"},                      // CODIGO 3
    {"invalid name", "nome inválido"},                       // CODIGO 4
    {"invalid date", "data inválida"},                       // CODIGO 5
    {"invalid quantity", "quantidade inválida"},             // CODIGO 6
    {"no such vaccine", "vacina inexistente"},               // CODIGO 7
    {"no stock", "esgotado"},                                // CODIGO 8
    {"already vaccinated", "já vacinado"},                   // CODIGO 9
    {"no such batch", "lote inexistente"},                   // CODIGO 10
    {"no such user", "utente inexistente"},                  // CODIGO 11
    {"No memory.", "sem memória"}                            // CODIGO 12
};