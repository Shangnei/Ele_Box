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
 * @file spi_irq.c
 */

#include "device_registers.h"
#include "spi_shared_function.h"


/*!
 * @addtogroup spi_driver
 * @{
 */

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

#ifdef SPI0

/*!
 * @brief This function is the implementation of SPI0 handler named in startup code.
 *
 * It passes the instance to the shared SPI IRQ handler.
 */
void SPI0_IRQHandler(void);

void SPI0_IRQHandler(void)
{
    SPI_DRV_IRQHandler(0U);
}

#endif

#ifdef SPI1

/*!
 * @brief This function is the implementation of SPI0 handler named in startup code.
 *
 * It passes the instance to the shared SPI IRQ handler.
 */
void SPI1_IRQHandler(void);

void SPI1_IRQHandler(void)
{
    SPI_DRV_IRQHandler(1U);
}

#endif

#ifdef SPI2

/*!
 * @brief This function is the implementation of SPI0 handler named in startup code.
 *
 * It passes the instance to the shared SPI IRQ handler.
 */
void SPI2_IRQHandler(void);

void SPI2_IRQHandler(void)
{
    SPI_DRV_IRQHandler(2U);
}

#endif

#ifdef SPI3

/*!
 * @brief This function is the implementation of SPI0 handler named in startup code.
 *
 * It passes the instance to the shared SPI IRQ handler.
 */
void SPI3_IRQHandler(void);

void SPI3_IRQHandler(void)
{
    SPI_DRV_IRQHandler(3U);
}

#endif

#ifdef SPI4

/*!
 * @brief This function is the implementation of SPI0 handler named in startup code.
 *
 * It passes the instance to the shared SPI IRQ handler.
 */
void SPI4_IRQHandler(void);

void SPI4_IRQHandler(void)
{
    SPI_DRV_IRQHandler(4U);
}

#endif

#ifdef SPI5

/*!
 * @brief This function is the implementation of SPI0 handler named in startup code.
 *
 * It passes the instance to the shared SPI IRQ handler.
 */
void SPI5_IRQHandler(void);

void SPI5_IRQHandler(void)
{
    SPI_DRV_IRQHandler(5U);
}

#endif


/*! @} */

/*******************************************************************************
 * EOF
 ******************************************************************************/

