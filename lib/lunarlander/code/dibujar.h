#ifndef DIBUJAR_H
#define DIBUJAR_H

#include <stdio.h>
#include <Arduino.h>

#define repeticiones 20

/**
 * @brief Dibuja una línea.
 * 
 * Esta función dibuja una línea entre dos puntos (x1, y1) y (x2, y2).
 *
 * @param x1    Coordenada X del punto inicial de la línea.
 * @param y1    Coordenada Y del punto inicial de la línea.
 * @param x2    Coordenada X del punto final de la línea.
 * @param y2    Coordenada Y del punto final de la línea.
 *
 * @note Utiliza la función SetPixel para dibujar cada punto
 */
void DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);


#endif // DIBUJAR_H