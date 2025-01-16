/*
 *  Copyright 2020-2024 Yuntu Microelectronics co.,ltd
 *  All rights reserved.
 * 
 *  YUNTU Confidential. This software is owned or controlled by YUNTU and may only be
 *  used strictly in accordance with the applicable license terms. By expressly
 *  accepting such terms or by downloading, installing, activating and/or otherwise
 *  using the software, you are agreeing that you have read, and that you agree to
 *  comply with and are bound by, such license terms. If you do not agree to be
 *  bound by the applicable license terms, then you may not retain, install,
 *  activate or otherwise use the software. The production use license in
 *  Section 2.3 is expressly granted for this software.
 * 
 * @file i2c_config.c
 * @brief 
 * 
 */


#include <stddef.h>
#include "i2c_config.h"

/* I2C master */
/*I2C_MasterConfig0*/



i2c_master_state_t I2C_MasterConfig0_State;
const i2c_master_user_config_t I2C_MasterConfig0 = {
    .slaveAddress=0x40U,
    .is10bitAddr=false,
    .operatingMode=I2C_FAST_MODE,
    .transferType=I2C_USING_INTERRUPTS,
    .baudRate=400000,
    .dmaChannel=0,
    .masterCallback=NULL,
    .callbackParam=NULL,
};
/*I2C_MasterConfig1*/



i2c_master_state_t I2C_MasterConfig1_State;
const i2c_master_user_config_t I2C_MasterConfig1 = {
    .slaveAddress=0x54U,
    .is10bitAddr=false,
    .operatingMode=I2C_FAST_MODE,
    .transferType=I2C_USING_INTERRUPTS,
    .baudRate=400000,
    .dmaChannel=0,
    .masterCallback=NULL,
    .callbackParam=NULL,
};
/* I2C slave */
