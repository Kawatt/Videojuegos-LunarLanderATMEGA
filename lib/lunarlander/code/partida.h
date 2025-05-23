/**
 * @file partida.h
 * @brief Contiene las funciones necesarias para manejar la partida y su estado
 */

#ifndef PARTIDA_H
#define PARTIDA_H

#include "lunar_lander.h"
#include "fisicas.h"
#include "transformaciones.h"
#include "../resources/nave.h"
#include "../resources/caracteres.h"
#include "../data/variables_globales.h"

extern int combustible;

void dibujar_escena();

void rotar_nave(uint8_t direccion);
void rotar_nave_ang(int angulo);

void manejar_instante_partida();

void inicializar_partida();

void insertar_monedas(int monedas);

void comenzarPartida();

void finalizarPartida();

void escalar_escena_partida(float factor_x, float factor_y);

void continuar_tras_aterrizaje_partida();

#endif // PARTIDA_H