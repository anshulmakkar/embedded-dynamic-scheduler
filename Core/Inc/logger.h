#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "stm32h7xx_hal.h"

#define ERROR_TASK_LINK FAIL -1
#define ERROR_TASK_ALLOCATION -2
#define ERROR_TASK_START -3
#defien ERROR_ELF_MAGIC -4

void uart_print(UART_HandleTypeDef *uart, const char* str);

void vDirectPrintMsg(const char* msg);

#endif
