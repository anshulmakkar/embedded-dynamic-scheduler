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
#include "rtu.h"
#include "logger.h"
#include "jumptbl.h"


int rtu_requested = 0;

//uint8_t _RTU_DATA_ tst = 'b';

//uint8_t state __attribute__ ((section (RTU_DATA_SECTION_NAME))) = 'd';
_RTU_DATA_ uint32_t * state = NULL;

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
typedef void (*p_jumptbl_logmsg)();
typedef void (*p_jumptbl_taskdelay)(uint32_t);
/* Startup function that creates and runs two FreeRTOS tasks */
void simple_entry(void *param)
{
    //p = (uint8_t*)&__RTU_DATA_START;
    //if (state == NULL)
    //{
    //    state = app_malloc()
    //}
    //p_jumptbl_logmsg jumptbl_logmsg = (p_jumptbl_logmsg)(0x20008609|1); //address of jumptbl_logmsg
    //jumptbl_logmsg();
    //p_jumptbl_taskdelay jumptbl_taskdelay = (p_jumptbl_taskdelay)(0x08007935|1); // address of vTaskDelay
    //jumptbl_taskdelay(2000);
    //state = 'z';
    /* just in case if an infinite loop is somehow omitted in FreeRTOS_Error */
    while (1)
    {

       //jumptbl_logmsg();
       //jumptbl_taskdelay(2000);

        if (rtu_requested)
        {
            rtu_requested = 0;
            //vTaskSuspend();
        }

        //vTaskDelay(1000);

    }
}
