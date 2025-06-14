#ifndef SENSOR_UTILS_H
#define SENSOR_UTILS_H
#include <time.h>
#define TAM_ID 10
#define TAM_TEXTO 20

typedef enum {
    TIPO_INT,
    TIPO_BOOL,
    TIPO_DOUBLE,
    TIPO_STRING
} TipoDado;
typedef struct {
    time_t timestamp;
    char id[TAM_ID];
    TipoDado tipo;
    union {
        int numero;
        int booleano;
        double real;
        char texto[TAM_TEXTO];
    } dado;
} Registro;

void ordenar(Registro *reg, int qtd);
void gravarSeparado(Registro *reg, int qtd);

#endif
