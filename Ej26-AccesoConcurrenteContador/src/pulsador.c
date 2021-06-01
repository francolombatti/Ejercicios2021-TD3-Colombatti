/*==================[ Inclusiones ]============================================*/
#include "pulsador.h"


/*==================[ Definiciones ]===================================*/


#define T_REBOTE_MS   40
#define T_REBOTE pdMS_TO_TICKS(T_REBOTE_MS)

/*==================[Prototipos de funciones]======================*/

static void errorPulsadorA( void );
static void errorPulsadorB( void );
static void botonPresionadoA( void );
static void botonPresionadoB( void );



void tareaPulsador( void* taskParmPtr );

/*==================[Variables]==============================*/
gpio_int_type_t pulsador[2] = { GPIO_NUM_27, GPIO_NUM_14 };
pulsadorInfo pulsadorA;
pulsadorInfo pulsadorB;
int contador = 100;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
/*==================[Implementaciones]=================================*/


void inicializarPulsador( void )
{
    pulsadorA.tecla             = pulsador[0];
    pulsadorA.estado            = ALTO;                     //Estado inicial
    pulsadorB.tecla             = pulsador[1];
    pulsadorB.estado            = ALTO;

    gpio_pad_select_gpio(pulsadorA.tecla);
    gpio_set_direction(pulsadorA.tecla , GPIO_MODE_INPUT);
    gpio_set_pull_mode(pulsadorA.tecla, GPIO_PULLDOWN_ONLY); //Habilita resistencia de PULLDOWN interna
    gpio_pad_select_gpio(pulsadorB.tecla);
    gpio_set_direction(pulsadorB.tecla , GPIO_MODE_INPUT);
    gpio_set_pull_mode(pulsadorB.tecla, GPIO_PULLDOWN_ONLY); //Habilita resistencia de PULLDOWN interna

 
    // Crear tareas en freeRTOS
    BaseType_t res = xTaskCreatePinnedToCore(
    	tareaPulsador,                     	// Funcion de la tarea a ejecutar
        "tareaPulsador",   	                // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*2, 		// Cantidad de stack de la tarea
        NULL,                          	// Parametros de tarea
        tskIDLE_PRIORITY+1,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
        NULL,                          		// Puntero a la tarea creada en el sistema
        PROCESADORB
    );

    // Gestion de errores
	if(res == pdFAIL)
	{
		printf( "Error al crear la tarea.\r\n" );
		while(true);					// si no pudo crear la tarea queda en un bucle infinito
	}
}

static void errorPulsadorA( void )
{
    portENTER_CRITICAL(&mux);
    pulsadorA.estado = ALTO;
    portEXIT_CRITICAL(&mux);
}
static void errorPulsadorB( void )
{
    portENTER_CRITICAL(&mux);
    pulsadorB.estado = ALTO;
    portEXIT_CRITICAL(&mux);
}

// pulsador_ Update State Function
void actualizarPulsadorA()
{
    switch( pulsadorA.estado )
    {
        case BAJO:
            if( gpio_get_level( pulsadorA.tecla ) ){
                pulsadorA.estado = ASCENDENTE;
            }
            break;

        case ASCENDENTE:
            if( gpio_get_level( pulsadorA.tecla ) ){
                pulsadorA.estado = ALTO;
                botonPresionadoA();
            }
            else{
                pulsadorA.estado = BAJO;
            }
            break;

        case ALTO:
            if( !gpio_get_level( pulsadorA.tecla ) ){
                pulsadorA.estado = DESCENDENTE;
            }
            break;

        case DESCENDENTE:
            if( !gpio_get_level( pulsadorA.tecla ) ){
                pulsadorA.estado = BAJO;
            }
            else{
                pulsadorA.estado = ALTO;
            }
            break;

        default:
            errorPulsadorA();
            break;
    }
    
}
void actualizarPulsadorB()
{
    switch( pulsadorB.estado )
    {
        case BAJO:
            if( gpio_get_level( pulsadorB.tecla ) ){
                pulsadorB.estado = ASCENDENTE;
            }
            break;

        case ASCENDENTE:
            if( gpio_get_level( pulsadorB.tecla ) ){
                pulsadorB.estado = ALTO;
                botonPresionadoB();
            }
            else{
                pulsadorB.estado = BAJO;
            }
            break;

        case ALTO:
            if( !gpio_get_level( pulsadorB.tecla ) ){
                pulsadorB.estado = DESCENDENTE;
            }
            break;

        case DESCENDENTE:
            if( !gpio_get_level( pulsadorB.tecla ) ){
                pulsadorB.estado = BAJO;
            }
            else{
                pulsadorB.estado = ALTO;
            }
            break;

        default:
            errorPulsadorB();
            break;
    }
    
}

/* accion de el evento de tecla pulsada */
static void botonPresionadoA()
{
    portENTER_CRITICAL(&mux);
 if(contador==900){}
    else{
       contador = contador + 100;
      }
    portEXIT_CRITICAL(&mux);
}              
static void botonPresionadoB()
{
    portENTER_CRITICAL(&mux);
 if(contador==100){}
    else{
       contador = contador - 100;
      }
    portEXIT_CRITICAL(&mux);
} 


void tareaPulsador( void* taskParmPtr )
{
    while( true )
    {
        actualizarPulsadorA();
        actualizarPulsadorB();
        vTaskDelay( T_REBOTE );
    }
}