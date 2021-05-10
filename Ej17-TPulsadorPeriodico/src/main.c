#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOSConfig.h"
#include "driver/gpio.h"
#include "pulsador.h"

#define SALIDA1     GPIO_NUM_25
#define PERIODO_MS  1000
#define PERIODO     pdMS_TO_TICKS(PERIODO_MS)
#define PROCESADORA 0
#define PROCESADORB 1

void tareaDestello( void* taskParmPtr ); //Prototipo de la función de la tarea

void app_main()
{
    inicializarPulsador();
    // Crear tarea en freeRTOS
    // Devuelve pdPASS si la tarea fue creada y agregada a la lista ready
    // En caso contrario devuelve pdFAIL.
    BaseType_t res = xTaskCreatePinnedToCore(
    	tareaDestello,                     	// Funcion de la tarea a ejecutar
        "tareaDestello",   	                // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*2, 		// Cantidad de stack de la tarea
        NULL,                          	// Parametros de tarea
        tskIDLE_PRIORITY+1,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
        NULL,                          		// Puntero a la tarea creada en el sistema
        PROCESADORA
    );

    // Gestion de errores
	if(res == pdFAIL)
	{
		printf( "Error al crear la tarea.\r\n" );
		while(true);					// si no pudo crear la tarea queda en un bucle infinito
	}
}

// Implementacion de funcion de la tarea
void tareaDestello( void* taskParmPtr )
{
    // ---------- Configuraciones ------------------------------
    gpio_pad_select_gpio(SALIDA1);
    gpio_set_direction(SALIDA1, GPIO_MODE_OUTPUT);

    TickType_t xPeriodicity =  PERIODO;			// Tarea periodica cada 1000 ms

    TickType_t xLastWakeTime = xTaskGetTickCount();
    TickType_t duty = 0;

    // ---------- Bucle infinito --------------------------
    while( true )
    {
        duty = obtenerDiferencia();

       printf("El ancho de pulso es de %i \n", duty);

        if ( duty > PERIODO_MS )
        {
            duty = PERIODO_MS;
        }

        gpio_set_level(SALIDA1, 1);
        vTaskDelay ( duty );
        gpio_set_level(SALIDA1, 0);

        // Envia la tarea al estado bloqueado durante xPeriodicity (delay periodico)
        vTaskDelayUntil( &xLastWakeTime , xPeriodicity );
    }
}