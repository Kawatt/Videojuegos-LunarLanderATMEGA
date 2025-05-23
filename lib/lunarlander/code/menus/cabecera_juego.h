#ifndef CABECERA_JUEGO_H
#define CABECERA_JUEGO_H


/**
 * @brief Inicializa todos los elementos de la cabecera del juego.
 * 
 * Crea las palabras visuales estáticas y dinámicas (etiquetas y valores) para:
 * puntuación, tiempo, combustible, altitud, velocidad horizontal y velocidad
 * vertical.
 * También aplica un escalado inicial a todos los elementos.
 */
void inicializar_cabecera(void);

/**
 * @brief Aplica un escalado uniforme a todos los elementos de la cabecera.
 * 
 * @param factor El factor de escalado a aplicar en ambos ejes.
 */
void escalar_cabecera(float factor);

/**
 * @brief Dibuja en pantalla todos los elementos de la cabecera.
 * 
 * Llama a las funciones de actualización y luego dibuja todas las palabras
 * en el contexto de dispositivo (HDC) provisto.
 * 
 * @param hdc Contexto gráfico donde se dibujarán los elementos.
 */
void dibujar_cabecera();

/**
 * @brief Actualiza el contador de tiempo en la cabecera.
 * 
 * Incrementa los segundos y actualiza el texto del reloj en formato "MM:SS".
 * También recrea la palabra time_valor con el nuevo tiempo.
 */
void actualizar_segundos_cabecera(void);

/**
 * @brief Actualiza el valor de la puntuación en la cabecera.
 * 
 * Recrea el valor score_valor a partir de la variable global puntuacion_partida
 * y aplica escalado.
 */
void actualizar_puntuacion_cabecera(void);


#endif // CABECERA_JUEGO_H