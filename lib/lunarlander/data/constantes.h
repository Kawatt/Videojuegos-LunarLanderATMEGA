#ifndef TABLAS_TRIGONOMETRICAS_H
#define TABLAS_TRIGONOMETRICAS_H

#define FRICCION_ATMOSFERA 0.001

// Angulo de la rotacion de la nave por cada pulsacion de las teclas
#define ANGULO_ROTACION (uint8_t)10 

// Declaración de las tablas de seno y coseno para las rotaciones de angulos
extern const float SIN_TABLA[64];
extern const float COS_TABLA[64];

extern const float valor_inicial_nave_x;

#define tamano_inicial_pantalla_X 255
#define tamano_inicial_pantalla_Y 255
#define anchura_minima_ventana 512
#define altura_minima_ventana 384
#define combustible_motor 1

#endif // TABLAS_TRIGONOMETRICAS_H
