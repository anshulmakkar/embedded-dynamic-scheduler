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
//#include "uart.h"
//#include "receive.h"

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
    uart_print("= = = S I M P LE T E S T   S T A R T E D = = =\n");
    if (param != NULL)
    	uart_print("param to simple not null");
       /* Create a print gate keeper task: */

    /* just in case if an infinite loop is somehow omitted in FreeRTOS_Error */
    while (1)
    {
        if (rtu_requested)
        {
            rtu_requested = 0;
            vTaskSuspend();
        }

    	vTaskDelay(1000);
    	state = state + 50;
    }
}
