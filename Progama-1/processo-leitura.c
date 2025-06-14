#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sensor_utils.h"

#define LIMITE 10000

int main() {
    FILE *arq = fopen("leituras_geral.txt", "r");
    if (arq == NULL) {
        printf("Erro ao abrir arquivo.\n");
        return 1;
    }

    Registro lista[LIMITE];
    int total = 0;
    char linha[150];
    fgets(linha, sizeof(linha), arq);

    while (fgets(linha, sizeof(linha), arq) != NULL && total < LIMITE) {
        char tipo[10], valor[40];
        sscanf(linha, "%ld %s %s %s", &lista[total].timestamp, lista[total].id, tipo, valor);

        if (strcmp(tipo, "int:") == 0) {
            lista[total].tipo = TIPO_INT;
            lista[total].dado.numero = atoi(valor);
        } else if (strcmp(tipo, "bool:") == 0) {
            lista[total].tipo = TIPO_BOOL;
            lista[total].dado.booleano = strcmp(valor, "true") == 0 ? 1 : 0;
        } else if (strcmp(tipo, "double:") == 0) {
            lista[total].tipo = TIPO_DOUBLE;
            lista[total].dado.real = atof(valor);
        } else if (strcmp(tipo, "str:") == 0) {
            lista[total].tipo = TIPO_STRING;
            strncpy(lista[total].dado.texto, valor, sizeof(lista[total].dado.texto));
        }
        total++;
    }
    fclose(arq);

    ordenar(lista, total);
    gravarSeparado(lista, total);

    printf("os registros foram organizados.\n");
    return 0;
}
