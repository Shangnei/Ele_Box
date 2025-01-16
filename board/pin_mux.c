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
 * @file pin_mux.c
 * @brief 
 * 
 */



#include "pin_mux.h"

const pin_settings_config_t g_pin_mux_InitConfigArr0[NUM_OF_CONFIGURED_PINS0] = {
    /*PTA_6-38-GPIO-DC*/
    {
        .base=PCTRLA,
        .pinPortIdx = 6U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PCTRL_LOW_DRIVE_STRENGTH,
        .passiveFilter = false,
        .mux = PCTRL_MUX_AS_GPIO,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
                        .width = 0
                        },
        .gpioBase = GPIOA,
        .direction=GPIO_OUTPUT_DIRECTION,
        .initValue=0,
    },
    /*PTB_0-34-GPIO-RES*/
    {
        .base=PCTRLB,
        .pinPortIdx = 0U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PCTRL_LOW_DRIVE_STRENGTH,
        .passiveFilter = false,
        .mux = PCTRL_MUX_AS_GPIO,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
                        .width = 0
                        },
        .gpioBase = GPIOB,
        .direction=GPIO_OUTPUT_DIRECTION,
        .initValue=0,
    },
    /*PTC_8-36-UART1_RX-LIN_RX*/
    {
        .base=PCTRLC,
        .pinPortIdx = 8U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PCTRL_LOW_DRIVE_STRENGTH,
        .passiveFilter = false,
        .mux = PCTRL_MUX_ALT2,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
                        .width = 0
                        },
        .gpioBase = GPIOC,
        .direction=GPIO_INPUT_DIRECTION,
        .initValue=0,
    },
    /*PTC_9-35-UART1_TX-LIIN_TX*/
    {
        .base=PCTRLC,
        .pinPortIdx = 9U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PCTRL_LOW_DRIVE_STRENGTH,
        .passiveFilter = false,
        .mux = PCTRL_MUX_ALT2,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
                        .width = 0
                        },
        .gpioBase = GPIOC,
        .direction=GPIO_INPUT_DIRECTION,
        .initValue=0,
    },
    /*PTC_14-30-SPI2_PCS0-*/
    {
        .base=PCTRLC,
        .pinPortIdx = 14U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PCTRL_LOW_DRIVE_STRENGTH,
        .passiveFilter = false,
        .mux = PCTRL_MUX_ALT4,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
                        .width = 0
                        },
        .gpioBase = GPIOC,
        .direction=GPIO_INPUT_DIRECTION,
        .initValue=0,
    },
    /*PTC_15-29-SPI2_SOUT-*/
    {
        .base=PCTRLC,
        .pinPortIdx = 15U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PCTRL_LOW_DRIVE_STRENGTH,
        .passiveFilter = false,
        .mux = PCTRL_MUX_ALT4,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
                        .width = 0
                        },
        .gpioBase = GPIOC,
        .direction=GPIO_INPUT_DIRECTION,
        .initValue=0,
    },
    /*PTC_16-28-SPI2_SCK-*/
    {
        .base=PCTRLC,
        .pinPortIdx = 16U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PCTRL_LOW_DRIVE_STRENGTH,
        .passiveFilter = false,
        .mux = PCTRL_MUX_ALT4,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
                        .width = 0
                        },
        .gpioBase = GPIOC,
        .direction=GPIO_INPUT_DIRECTION,
        .initValue=0,
    },
    /*PTC_1-25-SPI2_SIN-*/
    {
        .base=PCTRLC,
        .pinPortIdx = 1U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PCTRL_LOW_DRIVE_STRENGTH,
        .passiveFilter = false,
        .mux = PCTRL_MUX_ALT4,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
                        .width = 0
                        },
        .gpioBase = GPIOC,
        .direction=GPIO_INPUT_DIRECTION,
        .initValue=0,
    },
    /*PTA_2-48-UART0_RX-*/
    {
        .base=PCTRLA,
        .pinPortIdx = 2U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PCTRL_LOW_DRIVE_STRENGTH,
        .passiveFilter = false,
        .mux = PCTRL_MUX_ALT6,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
                        .width = 0
                        },
        .gpioBase = GPIOA,
        .direction=GPIO_INPUT_DIRECTION,
        .initValue=0,
    },
    /*PTA_3-47-UART0_TX-*/
    {
        .base=PCTRLA,
        .pinPortIdx = 3U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PCTRL_LOW_DRIVE_STRENGTH,
        .passiveFilter = false,
        .mux = PCTRL_MUX_ALT6,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
                        .width = 0
                        },
        .gpioBase = GPIOA,
        .direction=GPIO_INPUT_DIRECTION,
        .initValue=0,
    },
    /*PTD_2-46-GPIO-LIN_SLP*/
    {
        .base=PCTRLD,
        .pinPortIdx = 2U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PCTRL_LOW_DRIVE_STRENGTH,
        .passiveFilter = false,
        .mux = PCTRL_MUX_AS_GPIO,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
                        .width = 0
                        },
        .gpioBase = GPIOD,
        .direction=GPIO_OUTPUT_DIRECTION,
        .initValue=0,
    },
    /*PTB_1-33-ADC0_SE5-AINX*/
    {
        .base=PCTRLB,
        .pinPortIdx = 1U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PCTRL_LOW_DRIVE_STRENGTH,
        .passiveFilter = false,
        .mux = PCTRL_PIN_DISABLED,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
                        .width = 0
                        },
        .gpioBase = GPIOB,
        .direction=GPIO_INPUT_DIRECTION,
        .initValue=0,
    },
    /*PTB_2-32-ADC0_SE6-AINY*/
    {
        .base=PCTRLB,
        .pinPortIdx = 2U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PCTRL_LOW_DRIVE_STRENGTH,
        .passiveFilter = false,
        .mux = PCTRL_PIN_DISABLED,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
                        .width = 0
                        },
        .gpioBase = GPIOB,
        .direction=GPIO_INPUT_DIRECTION,
        .initValue=0,
    },
    /*PTB_3-31-GPIO-KEY*/
    {
        .base=PCTRLB,
        .pinPortIdx = 3U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PCTRL_LOW_DRIVE_STRENGTH,
        .passiveFilter = false,
        .mux = PCTRL_MUX_AS_GPIO,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
                        .width = 0
                        },
        .gpioBase = GPIOB,
        .direction=GPIO_INPUT_DIRECTION,
        .initValue=0,
    },
    /*PTD_1-1-I2C1_SCL-*/
    {
        .base=PCTRLD,
        .pinPortIdx = 1U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PCTRL_LOW_DRIVE_STRENGTH,
        .passiveFilter = false,
        .mux = PCTRL_MUX_ALT5,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
                        .width = 0
                        },
        .gpioBase = GPIOD,
        .direction=GPIO_INPUT_DIRECTION,
        .initValue=0,
    },
    /*PTD_0-2-I2C1_SDA-*/
    {
        .base=PCTRLD,
        .pinPortIdx = 0U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PCTRL_LOW_DRIVE_STRENGTH,
        .passiveFilter = false,
        .mux = PCTRL_MUX_ALT5,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
                        .width = 0
                        },
        .gpioBase = GPIOD,
        .direction=GPIO_INPUT_DIRECTION,
        .initValue=0,
    },
    /*PTD_6-23-GPIO-LED*/
    {
        .base=PCTRLD,
        .pinPortIdx = 6U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PCTRL_HIGH_DRIVE_STRENGTH,
        .passiveFilter = false,
        .mux = PCTRL_MUX_AS_GPIO,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
                        .width = 0
                        },
        .gpioBase = GPIOD,
        .direction=GPIO_OUTPUT_DIRECTION,
        .initValue=1,
    },
    /*PTD_4-44-GPIO-BUZZ*/
    {
        .base=PCTRLD,
        .pinPortIdx = 4U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PCTRL_HIGH_DRIVE_STRENGTH,
        .passiveFilter = false,
        .mux = PCTRL_MUX_AS_GPIO,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
                        .width = 0
                        },
        .gpioBase = GPIOD,
        .direction=GPIO_OUTPUT_DIRECTION,
        .initValue=0,
    },
    /*PTE_11-3-GPIO-WP*/
    {
        .base=PCTRLE,
        .pinPortIdx = 11U,
        .pullConfig = PCTRL_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect = PCTRL_LOW_DRIVE_STRENGTH,
        .passiveFilter = false,
        .mux = PCTRL_MUX_AS_GPIO,
        .intConfig = PCTRL_DMA_INT_DISABLED,
        .rateSelect = PCTRL_FAST_SLEW_RATE,
        .clearIntFlag = false,
        .digitalFilter = false,
        .filterConfig ={
                        .width = 0
                        },
        .gpioBase = GPIOE,
        .direction=GPIO_OUTPUT_DIRECTION,
        .initValue=0,
    },
};


/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/