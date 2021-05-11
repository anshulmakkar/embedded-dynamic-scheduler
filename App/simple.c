/*
 * Copyright 2013, 2017, Jernej Kovacic
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software. If you wish to use our Amazon
 * FreeRTOS name, please do so in a fair use way that does not cause confusion.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


/**
 * @file
 * A simple demo application.
 *
 * @author Jernej Kovacic
 */


#include <stddef.h>

#include <FreeRTOS.h>
#include <task.h>
//#include "app_config.h"
#include "rtu.h"
//#include "logger.h"
//#include "uart.h"
//#include "receive.h"

typedef struct
{
   uint32_t CR1;    /*!< USART Control register 1,                 Address offset: 0x00 */
   uint32_t CR2;    /*!< USART Control register 2,                 Address offset: 0x04 */
   uint32_t CR3;    /*!< USART Control register 3,                 Address offset: 0x08 */
   uint32_t BRR;    /*!< USART Baud rate register,                 Address offset: 0x0C */
   uint32_t GTPR;   /*!< USART Guard time and prescaler register,  Address offset: 0x10 */
   uint32_t RTOR;   /*!< USART Receiver Time Out register,         Address offset: 0x14 */
   uint32_t RQR;    /*!< USART Request register,                   Address offset: 0x18 */
   uint32_t ISR;    /*!< USART Interrupt and status register,      Address offset: 0x1C */
   uint32_t ICR;    /*!< USART Interrupt flag Clear register,      Address offset: 0x20 */
   uint32_t RDR;    /*!< USART Receive Data register,              Address offset: 0x24 */
   uint32_t TDR;    /*!< USART Transmit Data register,             Address offset: 0x28 */
   uint32_t PRESC;  /*!< USART clock Prescaler register,           Address offset: 0x2C */
} USART_TypeDef;

#define PERIPH_BASE               (0x40000000UL) /*!< Base address of : AHB/APB Peripherals                                                   */
#define D2_APB1PERIPH_BASE        PERIPH_BASE
#define D2_AHB2PERIPH_BASE       (PERIPH_BASE + 0x08020000UL)
#define USART3_BASE           (D2_APB1PERIPH_BASE + 0x4800UL)
#define USART3              ((USART_TypeDef *) USART3_BASE)
int rtu_requested = 0;

int _RTU_DATA_ state = 10;

void cpRequestHook(int type)
{
	rtu_requested = 1;
}


/*
 * This diagnostic pragma will suppress the -Wmain warning,
 * raised when main() does not return an int
 * (which is perfectly OK in bare metal programming!).
 *
 * More details about the GCC diagnostic pragmas:
 * https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html
 */

/* Startup function that creates and runs two FreeRTOS tasks */
void simple_entry(void *param)
{
    /*
     * I M P O R T A N T :
     * Make sure (in startup.s) that main is entered in Supervisor mode.
     * When vTaskStartScheduler launches the first task, it will switch
     * to System mode and enable interrupt exceptions.
     */
	//uart_print(0, "test test");
    //vDirectPrintMsg("= = = S I M P LE T E S T   S T A R T E D = = =");
    int i = 0;
    //uint8_t * str = "hello";
    //for (i =0 ; i < 500000; i++);

    //while (*str != '\0')
    //{
     //   USART3->TDR = (uint8_t)(*str & 0xFFU);
     //   str++;
    //}
    USART3->TDR = (uint8_t)'s';

    if (param != NULL)
        USART3->TDR = (uint8_t)'d';
    	//vDirectPrintMsg("param to simple not null");
       /* Create a print gate keeper task: */

    /* just in case if an infinite loop is somehow omitted in FreeRTOS_Error */
    while (1)
    {
        //USART3->TDR = (uint8_t)('d' & 0xFFU);
        //USART3->TDR = (uint8_t)'d';
        if (rtu_requested)
        {
            rtu_requested = 0;
            //vTaskSuspend(1000);
        }

    	//for ( i = 0; i < 5000; i++);
    	state = 20;
    }
}
