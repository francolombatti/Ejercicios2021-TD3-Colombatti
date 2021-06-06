#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOSConfig.h"
#include "driver/gpio.h"
#include "../include/pulsador.h"



gpio_int_type_t led[2] = { GPIO_NUM_25, GPIO_NUM_26 };
extern int contador;
extern portMUX_TYPE mux;
void tareaDestello( void* taskParmPtr ); //Prototipo de la función de la tarea
void tareaC( void* taskParmPtr ); //Prototipo de la función de la tarea c
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
        tskIDLE_PRIORITY+2,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
        NULL,                          		// Puntero a la tarea creada en el sistema
        PROCESADORA
    );

    // Gestion de errores
	if(res == pdFAIL)
	{
		printf( "Error al crear la tarea.\r\n" );
		while(true);					// si no pudo crear la tarea queda en un bucle infinito
	}

    BaseType_t resa = xTaskCreatePinnedToCore(
    	tareaC,                     	// Funcion de la tarea a ejecutar
        "tareaC",   	                // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE, 		// Cantidad de stack de la tarea
        NULL,                          	// Parametros de tarea
        tskIDLE_PRIORITY+1,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
        NULL,                          		// Puntero a la tarea creada en el sistema
        PROCESADORA
    );

    // Gestion de errores
	if(resa == pdFAIL)
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
//Tarea C: Destella otro led conun periodo de 2 s y un tiempo de encendido de 2xcontador ms.
//En cada ciclo deberá decrementar el contador en 100.
void tareaC( void* taskParmPtr )
{
    // ---------- Configuraciones ------------------------------
    gpio_pad_select_gpio(led[1]);
    gpio_set_direction(led[1], GPIO_MODE_OUTPUT);

    portENTER_CRITICAL(&mux);
    TickType_t xPeriodicity1 = pdMS_TO_TICKS(contador*2); 
    portEXIT_CRITICAL(&mux);
    TickType_t xPeriodicityC = pdMS_TO_TICKS(2000); 
    TickType_t xLastWakeTimeC = xTaskGetTickCount();
    // ---------- Bucle infinito --------------------------
    while( true )
    {
            portENTER_CRITICAL(&mux);
            xPeriodicity1 = pdMS_TO_TICKS(contador*2); 
            portEXIT_CRITICAL(&mux);
            gpio_set_level(led[1], 1 );
            vTaskDelay( xPeriodicity1);
            gpio_set_level(led[1], 0 );
            portENTER_CRITICAL(&mux);
                if(contador==100){}
                    else{
                    contador = contador - 100;
                    }
            portEXIT_CRITICAL(&mux);

           vTaskDelayUntil( &xLastWakeTimeC, xPeriodicityC );

    }
}