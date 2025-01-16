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

/**
 * @file uart_irq.h
 */

#ifndef UART_IRQ_H__
#define UART_IRQ_H__

#include "device_registers.h"
#include "interrupt_manager.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void UART_DRV_IRQHandler(uint32_t instance);

/*******************************************************************************
 *  Default interrupt handlers signatures
 ******************************************************************************/

#if (UART_INSTANCE_COUNT > 0U)
/*! @brief UART0 interrupt handler. */
void UART0_IrqHandler(void);
#endif

#if (UART_INSTANCE_COUNT > 1U)
/*! @brief UART1 interrupt handler. */
void UART1_IrqHandler(void);
#endif

#if (UART_INSTANCE_COUNT > 2U)
/*! @brief UART2 interrupt handler. */
void UART2_IrqHandler(void);
#endif

#if (UART_INSTANCE_COUNT > 3U)
/*! @brief UART3 interrupt handler. */
void UART3_IrqHandler(void);
#endif

#if (UART_INSTANCE_COUNT > 4U)
/*! @brief UART4 interrupt handler. */
void UART4_IrqHandler(void);
#endif

#if (UART_INSTANCE_COUNT > 5U)
/*! @brief UART5 interrupt handler. */
void UART5_IrqHandler(void);
#endif
/*! Array storing references to UART irq handlers */
extern isr_t g_uartIsr[UART_INSTANCE_COUNT];

#endif /* UART_IRQ_H__ */
/*******************************************************************************
 * EOF
 ******************************************************************************/
