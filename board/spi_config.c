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
 * @file spi_config.c
 * @brief 
 * 
 */


#include <stddef.h>
#include "spi_config.h"

/* SPI master */
/*spi_MasterConfig0*/



spi_state_t spi_MasterConfig0_State;
const spi_master_config_t spi_MasterConfig0 = {
    .bitsPerSec=5000000U,
    .whichPcs=SPI_PCS0,
    .pcsPolarity=SPI_ACTIVE_LOW,
    .isPcsContinuous=false,
    .bitcount=8,
    .clkPhase=SPI_CLOCK_PHASE_1ST_EDGE,
    .clkPolarity=SPI_SCK_ACTIVE_HIGH,
    .lsbFirst=false,
    .transferType=SPI_USING_INTERRUPTS,
    .rxDMAChannel=0,
    .txDMAChannel=0,
    .callback=NULL,
    .callbackParam=NULL,
    .width=SPI_SINGLE_BIT_XFER,
};
/* SPI slave */
