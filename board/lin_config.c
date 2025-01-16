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
 * @file lin_config.c
 * @brief 
 * 
 */



#include "lin_config.h"


#define CLASSIC_PID_NUM 2
static uint8_t classicPID[CLASSIC_PID_NUM]={ 0x3C,0x3D };



/*lin_config0*/


lin_state_t lin_config0_State;
lin_user_config_t lin_config0 = {
    .baudRate=19200U,
    .nodeFunction=true,
    .autobaudEnable=false,
    .timerGetTimeIntervalCallback=NULL,
    .classicPID=classicPID,
    .numOfClassicPID=CLASSIC_PID_NUM,
};
