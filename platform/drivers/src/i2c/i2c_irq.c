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
 * @file i2c_irq.c
 */

#include "device_registers.h"
#include "i2c_driver.h"
#include "i2c_irq.h"



/*******************************************************************************
 * Define
 ******************************************************************************/
/* Pointer to runtime state structure.*/
extern i2c_master_state_t *g_i2cMasterStatePtr[];
extern i2c_slave_state_t *g_i2cSlaveStatePtr[];

/*******************************************************************************
 * Code
 ******************************************************************************/

#if defined (YTM32B1L_SERIES)
/* Implementation of I2C0 master and slave handler named in startup code. */
void I2C0_IRQHandler(void)
{
    I2C_DRV_ModuleIRQHandler(0);
}
void I2C1_IRQHandler(void)
{
    I2C_DRV_ModuleIRQHandler(1);
}

#elif defined(YTM32B1M_SERIES)
#if (I2C_INSTANCE_COUNT > 0u)

/* Implementation of I2C0 master handler named in startup code. */
void I2C0_Master_IRQHandler(void)
{
    I2C_DRV_MasterIRQHandler(0);
}

/* Implementation of I2C0 slave handler named in startup code. */
void I2C0_Slave_IRQHandler(void)
{
    I2C_DRV_SlaveIRQHandler(0);
}

#if(I2C_INSTANCE_COUNT > 1u)

/* Implementation of I2C1 master handler named in startup code. */
void I2C1_Master_IRQHandler(void)
{
    I2C_DRV_MasterIRQHandler(1);
}

/* Implementation of I2C1 slave handler named in startup code. */
void I2C1_Slave_IRQHandler(void)
{
    I2C_DRV_SlaveIRQHandler(1);
}

#endif

#if(I2C_INSTANCE_COUNT > 2u)

/* Implementation of I2C2 master handler named in startup code. */
void I2C2_Master_IRQHandler(void)
{
    I2C_DRV_MasterIRQHandler(2);
}

/* Implementation of I2C2 slave handler named in startup code. */
void I2C2_Slave_IRQHandler(void)
{
    I2C_DRV_SlaveIRQHandler(2);
}

#endif

#endif

#elif defined(YTM32B1H_SERIES)
#if (I2C_INSTANCE_COUNT > 0u)

/* Implementation of I2C0 handler named in startup code. */
void I2C0_Master_Slave_IRQHandler(void)
{
    if(g_i2cMasterStatePtr[0] != NULL)
    {
        I2C_DRV_MasterIRQHandler(0);
    }else if(g_i2cSlaveStatePtr[0] != NULL)
    {
        I2C_DRV_SlaveIRQHandler(0);
    }
}


#if(I2C_INSTANCE_COUNT > 1u)

/* Implementation of I2C1 handler named in startup code. */
void I2C1_Master_Slave_IRQHandler(void)
{
    if(g_i2cMasterStatePtr[1] != NULL)
    {
        I2C_DRV_MasterIRQHandler(1);
    }else if(g_i2cSlaveStatePtr[1] != NULL)
    {
        I2C_DRV_SlaveIRQHandler(1);
    }
}

#endif

#if(I2C_INSTANCE_COUNT > 2u)

/* Implementation of I2C2 handler named in startup code. */
void I2C2_Master_Slave_IRQHandler(void)
{
    if(g_i2cMasterStatePtr[2] != NULL)
    {
        I2C_DRV_MasterIRQHandler(2);
    }else if(g_i2cSlaveStatePtr[2] != NULL)
    {
        I2C_DRV_SlaveIRQHandler(2);
    }
}

#endif
#if(I2C_INSTANCE_COUNT > 3u)

/* Implementation of I2C3 handler named in startup code. */
void I2C3_Master_Slave_IRQHandler(void)
{
    if(g_i2cMasterStatePtr[3] != NULL)
    {
        I2C_DRV_MasterIRQHandler(3);
    }else if(g_i2cSlaveStatePtr[3] != NULL)
    {
        I2C_DRV_SlaveIRQHandler(3);
    }
}

#endif
#if(I2C_INSTANCE_COUNT > 4u)

/* Implementation of I2C4 handler named in startup code. */
void I2C4_Master_Slave_IRQHandler(void)
{
    if(g_i2cMasterStatePtr[4] != NULL)
    {
        I2C_DRV_MasterIRQHandler(4);
    }else if(g_i2cSlaveStatePtr[4] != NULL)
    {
        I2C_DRV_SlaveIRQHandler(4);
    }
}

#endif

#endif
#endif
/*******************************************************************************
 * EOF
 ******************************************************************************/
