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
 * @file adc_config.c
 * @brief 
 * 
 */


#include "adc_config.h"



/* adc_config0 */
const adc_converter_config_t adc_config0={
    .clockDivider=0,
    .startTime=12,
    .sampleTime=2,
    .overrunMode=false,
    .autoOffEnable=false,
    .waitEnable=false,
    .trigger=ADC_TRIGGER_SOFTWARE,
    .align=ADC_ALIGN_RIGHT,
    .resolution=ADC_RESOLUTION_12BIT,
    .dmaWaterMark=0,
    .dmaEnable=false,
    .sequenceConfig={
        .sequenceMode=ADC_CONV_STEP,
        .sequenceIntEnable=true,
        .convIntEnable=false,
        .readyIntEnable=false,
        .ovrunIntEnable=false,
        .sampIntEnable=false,
        .channels={
            ADC_INPUTCHAN_EXT5,
            ADC_INPUTCHAN_EXT6,
        },
        .totalChannels=2,
    },
    .compareConfig={
        .compareEnable=false,
        .compareAllChannelEnable=false,

        .compHigh=4095,
        .compLow=0,
        .compIntEnable=false,
    },
};


