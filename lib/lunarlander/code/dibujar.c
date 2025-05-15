#include "dibujar.h"

/**
 * @brief Dibuja una posición.
 *
 * @param x1    Coordenada X del punto a dibujar.
 * @param y1    Coordenada Y del punto a dibujar.
 *
 */
void SetPixel(uint8_t xp, uint8_t yp) {
    PORTB = xp;
    PORTD = yp;
}

void DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    uint8_t i = 0;
    while (i < repeticiones) {
        SetPixel(x1, y1);
        SetPixel(x2, y2);
        i++;
    }
}