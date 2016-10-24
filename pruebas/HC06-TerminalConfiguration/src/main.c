
/*==================[inclusions]=============================================*/

#include "main.h"         /* <= own header */

#include "sAPI.h"         /* <= sAPI header */
#include "hc06_driver.h"         /* <= sAPI header */

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/



/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();

   /* Inicializar el conteo de Ticks con resolución de 1ms, sin tickHook */
   tickConfig( 1, 0 );

   /* Inicializar DigitalIO */
   digitalConfig( 0, ENABLE_DIGITAL_IO );

   /* Configuración de pines de entrada para Teclas de la CIAA-NXP */
   digitalConfig( TEC1, INPUT );
   digitalConfig( TEC2, INPUT );
   digitalConfig( TEC3, INPUT );
   digitalConfig( TEC4, INPUT );

   /* Configuración de pines de salida para Leds de la CIAA-NXP */
   digitalConfig( LEDR, OUTPUT );
   digitalConfig( LEDG, OUTPUT );
   digitalConfig( LEDB, OUTPUT );
   digitalConfig( LED1, OUTPUT );
   digitalConfig( LED2, OUTPUT );
   digitalConfig( LED3, OUTPUT );

   /* Inicializar UART_USB a 115200 baudios */

   uartConfig( UART_USB, 115200);
   HC06_init(115200);
   uint8_t dato  = 0;
   uint8_t dato2 =0;

   /* Buffer */
   static uint8_t uartBuff[10]; /*Se utiliza para encolar los datos a enviar hasta recibir el caracter '-' */
   uint8_t cant = 0;

   uartWriteString(UART_USB,"Escriba comando AT\r\n");
   uartWriteString(UART_USB,"Apretar tecla '-' para enviar comando\r\n");

   while(1) {
      dato  = HC06_ReadByte();
      dato2 = uartReadByte( UART_USB );
      if(dato2){
        if(dato2 == '-'){  /*Se envìa la cadena de caracteres al HC-06 */
          uartBuff[cant] = '\0';
          uartWriteString(UART_USB,"\r\n");
          HC06_WriteString(uartBuff);
          cant=0;
        }else{
          uartWriteByte(UART_USB,dato2);
          uartBuff[cant] = dato2;
          cant++;
        }
      }
      if( dato ){
         uartWriteByte(UART_USB,dato);
      }
   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}

/*==================[end of file]============================================*/
