#include <Arduino.h>
#include "code/lunar_lander.h"
#include "code/gestor_plataformas.h"
#include "code/palabra.h"
#include "code/fragmentacion_nave.h"

#include "code/menus/cabecera_juego.h"
#include "code/menus/menu_aterrizaje.h"
#include "code/menus/menu_final_partida.h"

#include "resources/superficie_lunar.h"

#include "data/variables_globales.h"
#include "data/constantes.h"
#include "data/variables_juego.h"

#define timer_TICK_juego 1
#define timer_IA 2
#define timer_segundo 3
#define timer_mostrar_mensajes 4
#define timer_IA_2 5
#define timer_fin_partida 6

unsigned long tiempoAnterior = 0;
unsigned long tickAnterior = 0;
const unsigned long intervalo = 1000;

unsigned int ADoutTemp;

// VARIABLES MAQUINA ESTADOS JUEGO
uint8_t monedas_introducidas = 0;
int timestamp_pintar_mensaje = 0;

void iniciar_nueva_partida() {
    monedas_introducidas = 0;
    estado_actual = ESTADO_PIDIENDO_MONEDA;
    iniciar_partida(monedas_introducidas);
}

int main() {
    //Outputs:
    DDRB = 255;
    DDRD = 255;
    PORTB = 0;
    PORTD = 0; // Salidas para dibujar

    //Inputs:
    DDRC = 0; // Entradas para leer botones
    PORTC = 0; // Pull-ups off.  We have our own external pull-ups anyway.

    // ADC Setup
    PRR &=  ~(_BV(ICF1));  //Allow ADC to be powered up

    //ADC 3-5
    ADMUX = 0;	 
    ADCSRA = 197;  // Enable ADC & start, prescale at 32

    for(;;) {
        
        inicializar_aleatoriedad();
        iniciar_nueva_partida();

        unsigned long tiempoActual = millis();
        if (tiempoActual - tiempoAnterior >= intervalo) {
            tiempoAnterior = tiempoActual;
            actualizar_segundos_cabecera();
        }
        if (tiempoActual - tickAnterior >= intervalo) {
            tickAnterior = tiempoActual;
            switch(estado_actual) {
                case ESTADO_JUEGO: {
                    manejar_instante();
                    manejar_teclas();
                    break;
                }
                case ESTADO_ATERRIZAJE:{
                    if(tipo_aterrizaje == COLISION){
                        fisicas_fragmentos();
                    }
                    break;
                }
                default: break;
            }
        }

        switch(estado_actual) {
            case ESTADO_PIDIENDO_MONEDA: {
            }
            case ESTADO_JUEGO: {
                pintar_pantalla();
                break;
            }
            case ESTADO_ATERRIZAJE: {
                pintar_pantalla();
                dibujar_mensajes_aterrizaje();
                break;
            }
            case ESTADO_FIN_PARTIDA: {
                dibujar_mensajes_final_partida();
                break;
            }
            default: break;
        }
        if ((PINC & 2U) == 0) { //Pulsado botón
            switch(estado_actual) {
                case ESTADO_PIDIENDO_MONEDA: {                     
                    pulsar_tecla(ESPACIO);
                    iniciar_partida(1);
                    estado_actual = ESTADO_JUEGO;
                    break;
                }
                case ESTADO_JUEGO: {
                    if(combustible >= combustible_motor){
                        pulsar_tecla(ARRIBA);
                    }
                    break;
                }
                case ESTADO_ATERRIZAJE: {
                    if(combustible < combustible_motor) {
                        generar_mensaje_final_partida(puntuacion_partida);
                        estado_actual = ESTADO_FIN_PARTIDA;
                    }
                    else {
                        continuar_tras_aterrizaje();
                        estado_actual = ESTADO_JUEGO;
                    }
                    break;
                }
                default: break;
            }
        } else {
            if (estado_actual == ESTADO_JUEGO) {
                if(combustible >= combustible_motor){
                    levantar_tecla(ARRIBA);
                }
            }
        }
        if (estado_actual == ESTADO_JUEGO) {
            if ((ADCSRA & _BV(ADSC)) == 0) {	// If ADC conversion has finished
                ADoutTemp = ADCW;			// Read out ADC value	
                
                //Langle = ADoutTemp >> 4; //ADoutTemp >> 2;

                // 64 angles allowed
                
                ADCSRA |= _BV(ADSC);	// Start new ADC conversion 
            }
            /*if (GetAsyncKeyState(TECLA_ROTAR_IZDA) & 0x8000) pulsar_tecla(IZQUIERDA);
            if (GetAsyncKeyState(TECLA_ROTAR_DCHA) & 0x8000) pulsar_tecla(DERECHA);*/
        }
        
    }

    
}