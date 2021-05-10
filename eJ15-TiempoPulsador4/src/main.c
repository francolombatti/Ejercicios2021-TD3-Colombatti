#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOSConfig.h"
#include "driver/gpio.h"
#include "pulsador.h"

#define SALIDA1     GPIO_NUM_25
#define T_ESPERA_MS  40
#define T_ESPERA     pdMS_TO_TICKS(T_ESPERA_MS)
#define PROCESADORA 0
#define PROCESADORB 1

void tareaDestello( void* taskParmPtr ); //Prototipo de la función de la tarea
void crearTareaDestello(void);

TaskHandle_t xHandle1 = NULL; 

void app_main()
{
    inicializarPulsador();
}

// Implementacion de funcion de la tarea
void tareaDestello( void* taskParmPtr )
{   
    printf( "Se ejecuto la tarea destello\n" );
    // ---------- Congiguraciones ------------------------------
    gpio_pad_select_gpio(SALIDA1);
    gpio_set_direction(SALIDA1, GPIO_MODE_OUTPUT);

    TickType_t dif;

    // ---------- Bucle infinito --------------------------
    while( true )
    {
        dif = obtenerDiferencia();

        if( dif > TIEMPO_NO_VALIDO )
        {
            gpio_set_level( SALIDA1, 1 );
            vTaskDelay( dif );
            gpio_set_level( SALIDA1, 0 );
            borrarDiferencia();
            printf("tarea eliminada\n");
            vTaskDelete( xHandle1 );
        }
        else
        {
            vTaskDelay( T_ESPERA );
        }

    }
}

void crearTareaDestello(void){
     printf( "Se creo la tarea destello\n" );
 BaseType_t res = xTaskCreatePinnedToCore(
    	tareaDestello,                     	// Funcion de la tarea a ejecutar
        "tareaDestello",   	                // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*2, 		// Cantidad de stack de la tarea
        NULL,                          	// Parametros de tarea
        tskIDLE_PRIORITY+1,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
        xHandle1,                          		// Puntero a la tarea creada en el sistema
        PROCESADORA
    );
   
    // Gestion de errores
	if(res == pdFAIL)
	{
		printf( "Error al crear la tarea.\r\n" );
		while(true);					// si no pudo crear la tarea queda en un bucle infinito
	}

}