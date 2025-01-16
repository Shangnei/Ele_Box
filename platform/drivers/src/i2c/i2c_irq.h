/* 
 * Copyright 2020-2022 Yuntu Microelectronics co.,ltd
 * All rights reserved.                                                                  
 *                                                                                       
 * YUNTU Confidential. This software is owned or controlled by YUNTU and may only be
 * used strictly in accordance with the applicable license terms. By expressly
 * accepting such terms or by downloading, installing, activating and/or otherwise
 * using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms. If you do not agree to be
 * bound by the applicable license terms, then you may not retain, install,
 * activate or otherwise use the software. The production use license in
 * Section 2.3 is expressly granted for this software.                          
 */
/*!
 * @file i2c_irq.h
 *
 */

#ifndef I2C_IRQ_H__
#define I2C_IRQ_H__

#include "device_registers.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

#if defined (YTM32B1L_SERIES)

/* I2C0 master and slave handler named in startup code. */
void I2C0_Master_Slave_IRQHandler(void);

#else

#if (I2C_INSTANCE_COUNT > 0u)

/* I2C0 master handler named in startup code. */
void I2C0_Master_IRQHandler(void);

/* I2C0 slave handler named in startup code. */
void I2C0_Slave_IRQHandler(void);

#if(I2C_INSTANCE_COUNT > 1u)

/* I2C1 master handler named in startup code. */
void I2C1_Master_IRQHandler(void);

/* I2C1 slave handler named in startup code. */
void I2C1_Slave_IRQHandler(void);

#if(I2C_INSTANCE_COUNT > 2u)

/* I2C2 master handler named in startup code. */
void I2C2_Master_IRQHandler(void);

/* I2C2 slave handler named in startup code. */
void I2C2_Slave_IRQHandler(void);

#endif /* I2C_INSTANCE_COUNT == 3u */

#endif /* I2C_INSTANCE_COUNT == 2u */

#endif /* I2C_INSTANCE_COUNT > 0u */

#endif /* endif cpu defined */

#endif /* I2C_IRQ_H__ */
/*******************************************************************************
 * EOF
 ******************************************************************************/
