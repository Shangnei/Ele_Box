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
 * @file i2c_config.h
 * @brief 
 * 
 */




#ifndef __I2C_CONFIG_H__
#define __I2C_CONFIG_H__




#include "i2c_driver.h"

/* I2C master */
/*I2C_MasterConfig0*/
extern i2c_master_state_t I2C_MasterConfig0_State;
extern const i2c_master_user_config_t I2C_MasterConfig0;
/*I2C_MasterConfig1*/
extern i2c_master_state_t I2C_MasterConfig1_State;
extern const i2c_master_user_config_t I2C_MasterConfig1;
/* I2C slave */




#endif