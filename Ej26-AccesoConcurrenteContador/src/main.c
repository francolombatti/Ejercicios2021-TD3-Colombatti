#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOSConfig.h"
#include "driver/gpio.h"
#include "../include/pulsador.h"



#define PERIODO_MS                    1000
#define PERIODO          pdMS_TO_TICKS(PERIODO_MS)

gpio_int_type_t led[2] = { GPIO_NUM_25, GPIO_NUM_26 };
extern int contador;
void tareaDestello( void* taskParmPtr ); //Prototipo de la función de la tarea

void app_main()
{
    // Crear tarea en freeRTOS
    // Devuelve pdPASS si la tarea fue creada y agregada a la lista ready
    // En caso contrario devuelve pdFAIL.
    inicializarPulsador();

    BaseType_t res = xTaskCreatePinnedToCore(
    	tareaDestello,                     	// Funcion de la tarea a ejecutar
        "tareaDestello",   	                // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE, 		// Cantidad de stack de la tarea
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
    gpio_pad_select_gpio(led[0]);
    gpio_set_direction(led[0], GPIO_MODE_OUTPUT);
    gpio_pad_select_gpio(led[1]);
    gpio_set_direction(led[1], GPIO_MODE_OUTPUT);

    
    TickType_t xPeriodicity = pdMS_TO_TICKS(contador/2); 
    //TickType_t xLastWakeTime = xTaskGetTickCount();
    // ---------- Bucle infinito --------------------------
    while( true )
    {
            xPeriodicity = pdMS_TO_TICKS(contador/2); 
            gpio_set_level(led[0], 1 );
            vTaskDelay( xPeriodicity);
            gpio_set_level(led[0], 0 );
            vTaskDelay( xPeriodicity);

           // vTaskDelayUntil( &xLastWakeTime, xPeriodicity );

    }
}