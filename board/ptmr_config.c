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
 * @file ptmr_config.c
 * @brief 
 * 
 */


#include "ptmr_config.h"

const ptmr_user_channel_config_t ptmr_channel_0={
    .periodUnits=pTMR_PERIOD_UNITS_MICROSECONDS,
    .period=10000,
    .chainChannel=false,
    .isInterruptEnabled=true,
};
const ptmr_user_channel_config_t ptmr_channel_1={
    .periodUnits=pTMR_PERIOD_UNITS_MICROSECONDS,
    .period=200000,
    .chainChannel=false,
    .isInterruptEnabled=true,
};

const ptmr_user_config_t PTMR_Config={
    .enableRunInDebug=false,
};
