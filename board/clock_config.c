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
 * @file clock_config.c
 * @brief 
 * 
 */

#include "stddef.h"
#include "clock_config.h"

/*! @brief peripheral clock PeripheralClockConfig */

peripheral_clock_config_t clock_config0PeripheralClockConfig[7] = {
    {
        .clkName = GPIO_CLK,
        .clkGate = true,
        .divider = DIV_BY_1,
        .clkSrc = CLK_SRC_FIRC,
    },
    {
        .clkName = SPI2_CLK,
        .clkGate = true,
        .divider = DIV_BY_2,
        .clkSrc = CLK_SRC_FIRC,
    },
    {
        .clkName = pTMR0_CLK,
        .clkGate = true,
        .divider = DIV_BY_1,
        .clkSrc = CLK_SRC_DISABLED,
    },
    {
        .clkName = ADC0_CLK,
        .clkGate = true,
        .divider = DIV_BY_4,
        .clkSrc = CLK_SRC_FXOSC,
    },
    {
        .clkName = I2C1_CLK,
        .clkGate = true,
        .divider = DIV_BY_3,
        .clkSrc = CLK_SRC_FIRC,
    },
    {
        .clkName = UART0_CLK,
        .clkGate = true,
        .divider = DIV_BY_2,
        .clkSrc = CLK_SRC_FIRC,
    },
    {
        .clkName = UART1_CLK,
        .clkGate = true,
        .divider = DIV_BY_1,
        .clkSrc = CLK_SRC_FXOSC,
    },
};

const scu_config_t clock_config0ScuConfig = {
    .fircEnable = true,
    .fircDeepSleepEnable = false,
    .sircDeepSleepEnable = false,
    .sircStandbyEnable = false,
    .sysClkSrc = SCU_SYSTEM_CLOCK_SRC_FIRC,
    .fxoscConfig =
        {
            .enable = true,
            .bypassMode = false,
            .gainSelection = 6,
            .frequency = 24000000U,
        },
    .sxoscConfig = 
        {
            .enable = false,
            .gainSelection = 0,
            .bypassMode = false,
            .deepsleepEnable = false,
            .standbyEnable = false,
            .frequency = 32768U,
        },
    .sysDiv = SCU_SYS_CLK_DIV_BY_1,
    .fastBusDiv = SCU_SYS_CLK_DIV_BY_1,
    .slowBusDiv = SCU_SYS_CLK_DIV_BY_2,
    .flashDiv = SCU_SYS_CLK_DIV_BY_2,
    .clockOutConfig =
        {
            .enable = false,
            .source = SCU_CLKOUT_SEL_FIRC_CLK,
            .divider = 1
        },
};

const cmu_config_t clock_config0CmuConfig = {
    .fircClockMonitor={
        .enable = true,                 
        .resetEnable = true,           
        .compareHigh = (60 * 128 / 2),        
        .compareLow = (36 * 128 / 2),  
    },
    .fxoscClockMonitor={
        .enable = true,                 
        .resetEnable = true,             
        .compareHigh = (30 * 128 / 2),        
        .compareLow = (18 * 128 / 2),         
    },
};

/*! @brief User Configuration structure clock_config0ClockManager */
clock_manager_user_config_t clock_config0ClockManager = {
    .scuConfigPtr = &clock_config0ScuConfig,
    .cmuConfigPtr = &clock_config0CmuConfig,
    .ipcConfig =
        {
            .peripheralClocks = clock_config0PeripheralClockConfig,
            .count = 7,
        },
};

/*! @brief Array of pointers to User configuration structures */
clock_manager_user_config_t const *g_clockManConfigsArr[] = {
    &clock_config0ClockManager,
};

/*! @brief Array of pointers to User defined Callbacks configuration structures */
/* The tool do not support generate Callbacks configuration. It's always empty. */
clock_manager_callback_user_config_t *g_clockManCallbacksArr[] = {(void *)0};
