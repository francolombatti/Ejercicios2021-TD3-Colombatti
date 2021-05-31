#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOSConfig.h"
#include "driver/gpio.h"
#include "pulsador.h"
#include "freertos/semphr.h" 

#define SALIDA1     GPIO_NUM_25
#define SALIDA2     GPIO_NUM_26
#define T_ESPERA_MS  40
#define T_ESPERA     pdMS_TO_TICKS(T_ESPERA_MS)
#define PROCESADORA 0
#define PROCESADORB 1
#define LED_ON_MS    500
#define LED_ON       pdMS_TO_TICKS(LED_ON_MS)

void tareaDestello( void* taskParmPtr ); 
TickType_t xLastWakeTime = 0;
TickType_t sinc = pdMS_TO_TICKS(500);
SemaphoreHandle_t semaforo = NULL; 

void app_main()
{
    
    semaforo = xSemaphoreCreateBinary();
    if(semaforo == NULL)
    {
        printf("No se pudo crear\n");
        while(true);
    }
    
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
    gpio_pad_select_gpio(SALIDA1);
    gpio_set_direction(SALIDA1, GPIO_MODE_OUTPUT);
    gpio_pad_select_gpio(SALIDA2);
    gpio_set_direction(SALIDA2, GPIO_MODE_OUTPUT);

    
    while( true )
    {
        if(xSemaphoreTake( semaforo , LED_ON)==pdTRUE)
        {
            vTaskDelayUntil(&xLastWakeTime , sinc);
            gpio_set_level( SALIDA1, 1 );
            vTaskDelay( LED_ON );
            gpio_set_level( SALIDA1, 0 );
        }
        else
        {
            gpio_set_level( SALIDA2, 1 );
            vTaskDelay( LED_ON );
            gpio_set_level( SALIDA2, 0 );
            xLastWakeTime = xTaskGetTickCount(); 

        }
    }
}