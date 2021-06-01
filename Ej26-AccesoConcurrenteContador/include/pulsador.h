#ifndef PULSADOR_H_
#define PULSADOR_H_
/*==================[ Inclusiones ]============================================*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
/*==================[ Definiciones ]===================================*/
#define TIEMPO_NO_VALIDO   0
#define PROCESADORA 0
#define PROCESADORB 1

typedef enum
{
    ALTO,
    BAJO,
    DESCENDENTE,
    ASCENDENTE
} estadoPulsador;

typedef struct
{
    gpio_int_type_t tecla;
    estadoPulsador estado;   //variables
} pulsadorInfo;

/*==================[Prototipos de funciones]======================*/
void inicializarPulsador( void );
void actualizarPulsadorA();
void actualizarPulsadorB();

#endif