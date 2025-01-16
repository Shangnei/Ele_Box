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
 * @file lin_irq.c
 *
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "lin_driver.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
#if (UART_INSTANCE_COUNT > 0U)
static void LIN_UART0_RxTx_IRQHandler(void)
{
    LIN_DRV_IRQHandler(0U);
}

#ifdef UART_ERR_IRQS
static void LIN_UART0_ERR_IRQHandler(void)
{
    LIN_DRV_IRQHandler(0U);
}
#endif
#endif /* if (UART_INSTANCE_COUNT > 0U) */

#if (UART_INSTANCE_COUNT > 1U)
static void LIN_UART1_RxTx_IRQHandler(void)
{
    LIN_DRV_IRQHandler(1U);
}

#ifdef UART_ERR_IRQS
static void LIN_UART1_ERR_IRQHandler(void)
{
    LIN_DRV_IRQHandler(1U);
}
#endif
#endif /* if (UART_INSTANCE_COUNT > 1U) */

#if (UART_INSTANCE_COUNT > 2U)
static void LIN_UART2_RxTx_IRQHandler(void)
{
    LIN_DRV_IRQHandler(2U);
}

#ifdef UART_ERR_IRQS
static void LIN_UART2_ERR_IRQHandler(void)
{
    LIN_DRV_IRQHandler(2U);
}
#endif
#endif /* if (UART_INSTANCE_COUNT > 2U) */

#if (UART_INSTANCE_COUNT > 3U)
static void LIN_UART3_RxTx_IRQHandler(void)
{
    LIN_DRV_IRQHandler(3U);
}

#ifdef UART_ERR_IRQS
static void LIN_UART3_ERR_IRQHandler(void)
{
    LIN_DRV_IRQHandler(3U);
}
#endif
#endif /* if (UART_INSTANCE_COUNT > 3U) */

#if (UART_INSTANCE_COUNT > 4U)
static void LIN_UART4_RxTx_IRQHandler(void)
{
    LIN_DRV_IRQHandler(4U);
}

#ifdef UART_ERR_IRQS
static void LIN_UART4_ERR_IRQHandler(void)
{
    LIN_DRV_IRQHandler(4U);
}
#endif
#endif /* if (UART_INSTANCE_COUNT > 4U) */

#if (UART_INSTANCE_COUNT > 5U)
static void LIN_UART5_RxTx_IRQHandler(void)
{
    LIN_DRV_IRQHandler(5U);
}

#ifdef UART_ERR_IRQS
static void LIN_UART5_ERR_IRQHandler(void)
{
    LIN_DRV_IRQHandler(5U);
}
#endif
#endif /* if (UART_INSTANCE_COUNT > 2U) */

isr_t g_linUartIsrs[UART_INSTANCE_COUNT] =
    {
#if (UART_INSTANCE_COUNT > 0U)
        LIN_UART0_RxTx_IRQHandler,
#endif
#if (UART_INSTANCE_COUNT > 1U)
        LIN_UART1_RxTx_IRQHandler,
#endif
#if (UART_INSTANCE_COUNT > 2U)
        LIN_UART2_RxTx_IRQHandler,
#endif
#if (UART_INSTANCE_COUNT > 3U)
        LIN_UART3_RxTx_IRQHandler,
#endif
#if (UART_INSTANCE_COUNT > 4U)
        LIN_UART4_RxTx_IRQHandler,
#endif
#if (UART_INSTANCE_COUNT > 5U)
        LIN_UART5_RxTx_IRQHandler,
#endif
    };

#ifdef UART_ERR_IRQS
isr_t g_linUartErrIsrs[UART_INSTANCE_COUNT] =
{
#if (UART_INSTANCE_COUNT > 0U)
    LIN_UART0_ERR_IRQHandler,
#endif
#if (UART_INSTANCE_COUNT > 1U)
    LIN_UART1_ERR_IRQHandler,
#endif
#if (UART_INSTANCE_COUNT > 2U)
    LIN_UART2_ERR_IRQHandler,
#endif
#if (UART_INSTANCE_COUNT > 3U)
    LIN_UART3_ERR_IRQHandler,
#endif
#if (UART_INSTANCE_COUNT > 4U)
    LIN_UART4_ERR_IRQHandler,
#endif
#if (UART_INSTANCE_COUNT > 5U)
    LIN_UART5_ERR_IRQHandler,
#endif
};
#endif
/******************************************************************************/
/* EOF */
/******************************************************************************/
