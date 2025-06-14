#include <stdio.h>
#include <string.h>
#include "sensor_utils.h"

void ordenar(Registro *reg, int qtd) {
    for (int i = 0; i < qtd - 1; i++) {
        for (int j = 0; j < qtd - i - 1; j++) {
            if (reg[j].timestamp > reg[j + 1].timestamp) {
                Registro temp = reg[j];
                reg[j] = reg[j + 1];
                reg[j + 1] = temp;
            }
        }
    }
}
void gravarSeparado(Registro *reg, int qtd) {
    for (int i = 0; i < qtd; i++) {
        char nome_arquivo[30];
        snprintf(nome_arquivo, sizeof(nome_arquivo), "%s_dados.txt", reg[i].id);
        FILE *out = fopen(nome_arquivo, "a");
        if (out == NULL) {
            printf("Erro ao criar %s\n", reg[i].id);
            continue;
        }
        if (ftell(out) == 0)
            fprintf(out, "TIMESTAMP ID VALOR\n");

        fprintf(out, "%ld %s ", reg[i].timestamp, reg[i].id);
        switch (reg[i].tipo) {
            case TIPO_INT:
                fprintf(out, "int: %d\n", reg[i].dado.numero);
                break;
            case TIPO_BOOL:
                fprintf(out, "bool: %s\n", reg[i].dado.booleano ? "true" : "false");
                break;
            case TIPO_DOUBLE:
                fprintf(out, "double: %.2lf\n", reg[i].dado.real);
                break;
            case TIPO_STRING:
                fprintf(out, "str: %s\n", reg[i].dado.texto);
                break;
        }
        fclose(out);
    }
}
