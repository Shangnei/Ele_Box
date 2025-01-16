/*
 * Copyright 2020-2022 Yuntu Microelectronics co.,ltd
 * All rights reserved.
 *
 * YUNTU Confidential. This software is owned or controlled by YUNTU and may
 * only be used strictly in accordance with the applicable license terms. By expressly
 * accepting such terms or by downloading, installing, activating and/or otherwise
 * using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms. If you do not agree to be
 * bound by the applicable license terms, then you may not retain, install,
 * activate or otherwise use the software. The production use license in
 * Section 2.3 is expressly granted for this software.
 */

/*!
 * @file uart_irq.c
 */

#include "uart_irq.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

#if (UART_INSTANCE_COUNT > 0U)
/* Implementation of UART0 handler named in startup code. */
void UART0_IrqHandler(void)
{
    UART_DRV_IRQHandler(0);
}
#endif

#if (UART_INSTANCE_COUNT > 1U)
/* Implementation of UART1 handler named in startup code. */
void UART1_IrqHandler(void)
{
    UART_DRV_IRQHandler(1);
}
#endif

#if (UART_INSTANCE_COUNT > 2U)
/* Implementation of UART2 handler named in startup code. */
void UART2_IrqHandler(void)
{
    UART_DRV_IRQHandler(2);
}
#endif

#if (UART_INSTANCE_COUNT > 3U)
/* Implementation of UART3 handler named in startup code. */
void UART3_IrqHandler(void)
{
    UART_DRV_IRQHandler(3);
}
#endif

#if (UART_INSTANCE_COUNT > 4U)
/* Implementation of UART4 handler named in startup code. */
void UART4_IrqHandler(void)
{
    UART_DRV_IRQHandler(4);
}
#endif

#if (UART_INSTANCE_COUNT > 5U)
/* Implementation of UART5 handler named in startup code. */
void UART5_IrqHandler(void)
{
    UART_DRV_IRQHandler(5);
}
#endif

/* Array storing references to UART irq handlers */
isr_t g_uartIsr[UART_INSTANCE_COUNT] =
{
#if (UART_INSTANCE_COUNT > 0U)
    UART0_IrqHandler,
#endif
#if (UART_INSTANCE_COUNT > 1U)
    UART1_IrqHandler,
#endif
#if (UART_INSTANCE_COUNT > 2U)
    UART2_IrqHandler,
#endif
#if (UART_INSTANCE_COUNT > 3U)
    UART3_IrqHandler,
#endif
#if (UART_INSTANCE_COUNT > 4U)
    UART4_IrqHandler,
#endif
#if (UART_INSTANCE_COUNT > 5U)
    UART5_IrqHandler,
#endif
};

/*******************************************************************************
 * EOF
 ******************************************************************************/
