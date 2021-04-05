#include "../driver/include/driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define N_LED 		3
#define N_PULSADOR 	2
#define T_ON		500/portTICK_PERIOD_MS

int led [N_LED] = {GPIO_NUM_32, GPIO_NUM_33, GPIO_NUM_25};
int pulsador [N_PULSADOR] = {GPIO_NUM_26, GPIO_NUM_27};

void app_main()
{
	//Configuración
	for(int i = 0; i < N_LED; i++){
		gpio_pad_select_gpio(led[i]);
		gpio_set_direction(led[i], GPIO_MODE_OUTPUT);
	}
	
	for(int i = 0; i < N_PULSADOR; i++){
		gpio_pad_select_gpio(pulsador[i]);
		gpio_set_direction(pulsador[i], GPIO_MODE_INPUT);
		gpio_set_pull_mode(pulsador[i], GPIO_PULLDOWN_ONLY);
	}

   int i = 0, sentido = 0;
   //Bucle infinito
   while( true )
    {
		if (gpio_get_level(pulsador[0]) == 1)
		{
			sentido = 1;
		}
	   	else if (gpio_get_level(pulsador[1]) == 1)
		{
			sentido = 0;
		}	

	   	if (sentido == 0)
		{
			gpio_set_level(led[i], 0);
			i++;
			if ( i >= N_LED ){
				i = 0;
			}
			gpio_set_level(led[i], 1);
		}
	   	else 
		{
			gpio_set_level(led[i], 0);
			i--;
			if ( i < 0 ){
				i = N_LED;
			}
			gpio_set_level(led[i], 1);
		}
		vTaskDelay(T_ON);	
	}
   return 0;
}