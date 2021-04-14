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
#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>

#include "app_config.h"
#include "print.h"
#include "receive.h"
#include "elf.h"
#include "applications.h"
#include "task_manager.h"

#define EMBEDDED_TASK
/*
 * This diagnostic pragma will suppress the -Wmain warning,
 * raised when main() does not return an int
 * (which is perfectly OK in bare metal programming!).
 *
 * More details about the GCC diagnostic pragmas:
 * https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html
 */
#pragma GCC diagnostic ignored "-Wmain"


/* Struct with settings for each task */
typedef struct _paramStruct
{
    portCHAR* text;                  /* text to be printed by the task */
    UBaseType_t  delay;              /* delay in milliseconds */
} paramStruct;

#ifdef EMBEDDED_TASK
/* Parameters for two tasks */
static const paramStruct tParam[2] =
{
    (paramStruct) { .text="Task1\r\n", .delay=2000 },
   (paramStruct) { .text="Periodic task\r\n", .delay=3000 }
};

/* Note: QEMU model of PL011 serial port ignores the transmit
FIFO capabilities. When writing on a real SOC, the
"Transmit FIFO Full" flag must be checked in UARTFR register
before writing on the UART register*/

volatile unsigned int* const UART0 = (unsigned int*)0x0101F1000;

static void uart_print(const char *s)
{
	while(*s != '\n')
	{
		*UART0 = (unsigned int)(*s); /* send to UART */
		s++;
	}
}

/* Task function - may be instantiated in multiple tasks */
void vTaskFunction( void *pvParameters )
{
    const portCHAR* taskName;
    paramStruct* params = (paramStruct*) pvParameters;

    taskName = ( NULL==params || NULL==params->text ? "Hello world" : params->text );
    if (taskName == NULL)
    	vDirectPrintMsg("taskname is NULL\n");
    for( ; ; )
    {
    	/* Print out the name of this task. */
        vDirectPrintMsg("I am static task1\n");
        vTaskDelay( 1000 / portTICK_RATE_MS );
    }
    vTaskDelete(NULL);
}
#endif

/*
 * A convenience function that is called when a FreeRTOS API call fails
 * and a program cannot continue. It prints a message (if provided) and
 * ends in an infinite loop.
 */
static void FreeRTOS_Error(const portCHAR* msg)
{
    if ( NULL != msg )
    {
        vDirectPrintMsg(msg);
    }

    for ( ; ; );
}

/* Startup function that creates and runs two FreeRTOS tasks */
void main()
{
//#ifdef EMBEDDED_TASK
	entry_ptr_t entry_point = NULL;

    /* register the tasks */
    entry_point = vTaskFunction;
    uart_print("print without uart initialization \n");
    /* And finally create two tasks: */
    if ( pdPASS != xTaskCreate((pdTASK_CODE)entry_point, "task1", 128, (void*) &tParam[0],
                                 PRIOR_PERIODIC, NULL) )
    {
    	FreeRTOS_Error("Could not create task1\r\n");
    }

//#else
    //Elf32_Ehdr *simple_elfh = APPLICATION_ELF(binary_obj_app_image);
    Elf32_Ehdr *simple_elfh = APPLICATION_ELF(simple);
    /* Init of print related tasks: */
    if ( pdFAIL == printInit(PRINT_UART_NR) )
    {
        FreeRTOS_Error("Initialization of print failed\r\n");
    }

    task_register_cons * simplec = task_register("simple", simple_elfh);

    if (!task_alloc(simplec))
    {
        vDirectPrintMsg("Failed to allocate task");
    }

    if (!task_link(simplec))
    {
        vDirectPrintMsg("Failed to link task ");
    }

    if (!task_start(simplec))
    {
        vDirectPrintMsg("Failed to start task \n");
    }

    if (!migrator_stask_tart())
    {
    	vDirectPrintMsg("Failed to start the migrator");
    }
//#endif

    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();

    /*
     * If all goes well, vTaskStartScheduler should never return.
     * If it does return, typically not enough heap memory is reserved.
     */
    FreeRTOS_Error("Could not start the scheduler!!!\r\n");

    /* just in case if an infinite loop is somehow omitted in FreeRTOS_Error */
    for ( ; ; );
}
