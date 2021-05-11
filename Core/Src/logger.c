/**
 * Outputs a string to the specified UART.
 *
 * "<NULL>" is transmitted if 'str' is equal to NULL.
 *
 * Nothing is done if 'nr' is invalid (equal or greater than 3).
 *
 * @param nr - number of the UART (between 0 and 2)
 * @param str - string to be sent to the UART, must be '\0' terminated.
 */

#include "logger.h"
/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */

extern UART_HandleTypeDef huart3;

void MX_USART3_UART_Init(void)
{

    /* USER CODE BEGIN USART3_Init 0 */

    /* USER CODE END USART3_Init 0 */

    /* USER CODE BEGIN USART3_Init 1 */

    /* USER CODE END USART3_Init 1 */
    huart3.Instance = USART3;
    huart3.Init.BaudRate = 9600;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
    huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart3) != HAL_OK)
    {
        vDirectPrintMsg("uart init failed");
    }
    if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        vDirectPrintMsg("uart init failed");
    }
    if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        vDirectPrintMsg("uart init failed");
    }
    if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
    {
        vDirectPrintMsg("uart init failed");
    }
    /* USER CODE BEGIN USART3_Init 2 */

    /* USER CODE END USART3_Init 2 */

}

void uart_print(UART_HandleTypeDef *uart, const char* str)
{
    /*
      if NULL is passed, avoid possible problems with dereferencing of NULL
      and print this string:
     */
    const char* null_str = "<NULL>\r\n";
    const char* cp;

    /* handle possible NULL value of str: */
    cp = ( NULL==str ? null_str : (char*) str );

    /*
     * Just print each character until a zero terminator is detected
     */
    for ( ; '\0' != *cp; ++cp )
    {
        //__printCh(nr, *cp);
        HAL_UART_Transmit_IT(uart, (uint8_t*)"*cp", 1);
    }
}

void vDirectPrintMsg(const char* msg)
{
    if ( NULL != msg )
    {
        //uart_print(&huart3, msg);
        //HAL_UART_Transmit_IT(&huart3, (uint8_t*)"simple hello", 12);
        for ( ; '\0' != *msg; ++msg )
        {
            //__printCh(nr, *cp);
            //HAL_UART_Transmit_IT(&huart3, (uint8_t*)(msg), 1);
            HAL_UART_Transmit(&huart3, (uint8_t*)(msg), 1,10);
            HAL_Delay(10);
        }
        HAL_UART_Transmit(&huart3, (uint8_t*)'\n', 1,10);
        HAL_Delay(10);

    }
}
