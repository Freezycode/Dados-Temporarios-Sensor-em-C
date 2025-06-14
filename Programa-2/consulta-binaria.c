#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sensor_utils.h"
#define MAX 10000

int converterDataHora(const char *texto, time_t *saida) {
    struct tm temp = {0};
    if (sscanf(texto, "%d/%d/%d %d:%d:%d", &temp.tm_mday, &temp.tm_mon, &temp.tm_year,
               &temp.tm_hour, &temp.tm_min, &temp.tm_sec) != 6) {
        return -1;
    }
    temp.tm_mon -= 1;
    temp.tm_year -= 1900;
    *saida = mktime(&temp);
    return 0;
}

int buscaBinariaMaisProxima(Registro *dados, int tam, time_t alvo) {
    int ini = 0, fim = tam - 1, meio;
    int maisProx = 0;

    while (ini <= fim) {
        meio = (ini + fim) / 2;

        if (llabs(dados[meio].timestamp - alvo) < llabs(dados[maisProx].timestamp - alvo)) {
            maisProx = meio;
        }

        if (dados[meio].timestamp == alvo) return meio;
        else if (dados[meio].timestamp < alvo) ini = meio + 1;
        else fim = meio - 1;
    }
    return maisProx;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: ./consulta <ID_SENSOR> <dd/mm/yyyy hh:mm:ss>\n");
        return 1;
    }

    char nomeArquivo[30];
    snprintf(nomeArquivo, sizeof(nomeArquivo), "%s_dados.txt", argv[1]);

    FILE *arq = fopen(nomeArquivo, "r");
    if (!arq) {
        printf("Arquivo %s não encontrado.\n", nomeArquivo);
        return 1;
    }

    Registro dados[MAX];
    int qtd = 0;
    char linha[150];
    fgets(linha, sizeof(linha), arq);
    while (fgets(linha, sizeof(linha), arq) && qtd < MAX) {
        char tipo[10], valor[40];
        sscanf(linha, "%ld %s %s %s", &dados[qtd].timestamp, dados[qtd].id, tipo, valor);
        if (strcmp(tipo, "int:") == 0) dados[qtd].tipo = TIPO_INT, dados[qtd].dado.numero = atoi(valor);
        else if (strcmp(tipo, "bool:") == 0) dados[qtd].tipo = TIPO_BOOL, dados[qtd].dado.booleano = strcmp(valor, "true") == 0;
        else if (strcmp(tipo, "double:") == 0) dados[qtd].tipo = TIPO_DOUBLE, dados[qtd].dado.real = atof(valor);
        else if (strcmp(tipo, "str:") == 0) dados[qtd].tipo = TIPO_STRING, strncpy(dados[qtd].dado.texto, valor, sizeof(dados[qtd].dado.texto));
        qtd++;
    }
    fclose(arq);
    ordenar(dados, qtd);
    time_t alvo;
    if (converterDataHora(argv[2], &alvo) != 0) {
        printf("Formato de data/hora inválido.\n");
        return 1;
    }
    int pos = buscaBinariaMaisProxima(dados, qtd, alvo);
    printf("Registro encontrado:\nTIMESTAMP: %ld | ID: %s | VALOR: ", dados[pos].timestamp, dados[pos].id);
    switch (dados[pos].tipo) {
        case TIPO_INT:
            printf("%d\n", dados[pos].dado.numero); break;
        case TIPO_BOOL:
            printf("%s\n", dados[pos].dado.booleano ? "true" : "false"); break;
        case TIPO_DOUBLE:
            printf("%.2lf\n", dados[pos].dado.real); break;
        case TIPO_STRING:
            printf("%s\n", dados[pos].dado.texto); break;
    }
    return 0;
}
