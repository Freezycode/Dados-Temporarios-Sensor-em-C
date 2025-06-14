#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sensor_utils.h"
#define QUANTIDADE 2000
void gerarValor(char *tipo, char *saida) {
    if (strcmp(tipo, "int") == 0) {
        sprintf(saida, "int: %d", rand() % 100);
    } else if (strcmp(tipo, "bool") == 0) {
        sprintf(saida, "bool: %s", (rand() % 2) ? "true" : "false");
    } else if (strcmp(tipo, "double") == 0) {
        double v = (double)rand() / RAND_MAX * 100.0;
        sprintf(saida, "double: %.2lf", v);
    } else if (strcmp(tipo, "str") == 0) {
        char texto[16] = "";
        for (int i = 0; i < 8; i++) texto[i] = 'A' + rand() % 26;
        texto[8] = '\0';
        sprintf(saida, "str: %s", texto);
    }
}
time_t toTimestamp(char *texto) {
    struct tm tempo = {0};
    sscanf(texto, "%d/%d/%d %d:%d:%d", &tempo.tm_mday, &tempo.tm_mon, &tempo.tm_year,
           &tempo.tm_hour, &tempo.tm_min, &tempo.tm_sec);
    tempo.tm_mon--;
    tempo.tm_year -= 1900;
    return mktime(&tempo);
}

int main(int argc, char *argv[]) {
    if (argc < 5 || (argc - 3) % 2 != 0) {
        printf("gerador ((iniciio)) ((fim)) ((id1)) ((tipo1)) <((id2)) ((tipo2))>\n");
        return 1;
    }
    srand(time(NULL));
    time_t inicio = toTimestamp(argv[1]);
    time_t fim = toTimestamp(argv[2]);
    int sensores = (argc - 3) / 2;

    FILE *saida = fopen("leituras_geral.txt", "w");
    if (!saida) {
        printf("Erro ao criar\n");
        return 1;
    }
    fprintf(saida, "TIMESTAMP ID VALOR\n");
    for (int i = 0; i < sensores; i++) {
        char *id = argv[3 + i * 2];
        char *tipo = argv[4 + i * 2];

        for (int j = 0; j < QUANTIDADE; j++) {
            time_t aleatorio = inicio + rand() % (fim - inicio + 1);
            char val[50];
            gerarValor(tipo, val);
            fprintf(saida, "%ld %s %s\n", aleatorio, id, val);
        }
    }
    fclose(saida);
    printf("Arquivo de teste gerado\n");
    return 0;
}
