#ifndef SUPERFICIE_LUNAR_H
#define SUPERFICIE_LUNAR_H


#include "../code/dibujable.h"

#define ANCHURA_TERRENO 255
#define ALTURA_TERRENO 81

#define PLATAFORMA_X2 50
#define PLATAFORMA_X3 14
#define PLATAFORMA_X4 10
#define PLATAFORMA_X5 8

// Declaración anticipada de los structs que se referencian
struct DibujableConstante;
struct Palabra;

// Definición de la estructura Plataforma
struct Plataforma {
    struct Dibujable* linea;
    uint8_t bonificador;
    struct Palabra* palabra;
};

extern const struct DibujableConstante Terreno;

#endif // SUPERFICIE_LUNAR_H
