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

#ifndef DEVICE_REGISTERS_H
#define DEVICE_REGISTERS_H

#define STD_ON  (1)
#define STD_OFF (0)

/*
 * Include the cpu specific register header files.
 *
 * The CPU macro should be declared in the project or makefile.
 */

#if defined(CPU_YTM32B1LE0)
    #define YTM32B1L_SERIES
    /* Register definitions */
    #include "YTM32B1LE0/include/YTM32B1LE0.h"
    /* CPU specific feature definitions */
    #include "YTM32B1LE0/include/YTM32B1LE0_features.h"
    #include "YTM32B1LE0/startup/system_YTM32B1LE0.h"
#elif defined(CPU_YTM32B1LE1)
    #define YTM32B1L_SERIES
    /* Register definitions */
    #include "YTM32B1LE1/include/YTM32B1LE1.h"
    /* CPU specific feature definitions */
    #include "YTM32B1LE1/include/YTM32B1LE1_features.h"
    #include "YTM32B1LE1/startup/system_YTM32B1LE1.h"
#elif defined(CPU_YTM32B1ME0)
    #define YTM32B1M_SERIES
    /* Register definitions */
    #include "YTM32B1ME0/include/YTM32B1ME0.h"
    /* CPU specific feature definitions */
    #include "YTM32B1ME0/include/YTM32B1ME0_features.h"
    #include "YTM32B1ME0/startup/system_YTM32B1ME0.h"
#elif defined(CPU_YTM32B1MD1)
    #define YTM32B1M_SERIES
    /* Register definitions */
    #include "YTM32B1MD1/include/YTM32B1MD1.h"
    /* CPU specific feature definitions */
    #include "YTM32B1MD1/include/YTM32B1MD1_features.h"
    #include "YTM32B1MD1/startup/system_YTM32B1MD1.h"
#elif defined(CPU_YTM32B1MD2)
    #define YTM32B1M_SERIES
    /* Register definitions */
    #include "YTM32B1MD2/include/YTM32B1MD2.h"
    /* CPU specific feature definitions */
    #include "YTM32B1MD2/include/YTM32B1MD2_features.h"
    #include "YTM32B1MD2/startup/system_YTM32B1MD2.h"
#elif defined(CPU_YTM32B1MC0)
    #define YTM32B1M_SERIES
    /* Register definitions */
    #include "YTM32B1MC0/include/YTM32B1MC0.h"
    /* CPU specific feature definitions */
    #include "YTM32B1MC0/include/YTM32B1MC0_features.h"
    #include "YTM32B1MC0/startup/system_YTM32B1MC0.h"
#elif defined(CPU_YTM32B1ME1)
    #define YTM32B1M_SERIES
    /* Register definitions */
    #include "YTM32B1ME1/include/YTM32B1ME1.h"
    /* CPU specific feature definitions */
    #include "YTM32B1ME1/include/YTM32B1ME1_features.h"
    #include "YTM32B1ME1/startup/system_YTM32B1ME1.h"
#elif defined(CPU_YTM32B1MF0)
    #define YTM32B1M_SERIES
    /* Register definitions */
    #include "YTM32B1MF0/include/YTM32B1MF0.h"
    /* CPU specific feature definitions */
    #include "YTM32B1MF0/include/YTM32B1MF0_features.h"
    #include "YTM32B1MF0/startup/system_YTM32B1MF0.h"
#elif defined(CPU_YTM32B1HA0)
    #define YTM32B1H_SERIES
    /* Register definitions */
    #include "YTM32B1HA0/include/YTM32B1HA0.h"
    /* CPU specific feature definitions */
    #include "YTM32B1HA0/include/YTM32B1HA0_features.h"
    #include "YTM32B1HA0/startup/system_YTM32B1HA0.h"
#elif defined(CPU_YTM32B1HA1)
    #define YTM32B1H_SERIES
    /* Register definitions */
    #include "YTM32B1HA1/include/YTM32B1HA1.h"
    /* CPU specific feature definitions */
    #include "YTM32B1HA1/include/YTM32B1HA1_features.h"
    #include "YTM32B1HA1/startup/system_YTM32B1HA1.h"
#elif defined(CPU_YTM32B1HB0)
#define YTM32B1H_SERIES
/* Register definitions */
#include "YTM32B1HB0/include/YTM32B1HB0.h"
/* CPU specific feature definitions */
#include "YTM32B1HB0/include/YTM32B1HB0_features.h"
#include "YTM32B1HB0/startup/system_YTM32B1HB0.h"
#elif defined(CPU_YTM32Z1MC0)
    #define YTM32Z1M_SERIES
    /* Register definitions */
    #include "YTM32Z1MC0/include/YTM32Z1MC0.h"
    /* CPU specific feature definitions */
    #include "YTM32Z1MC0/include/YTM32Z1MC0_features.h"
    #include "YTM32Z1MC0/startup/system_YTM32Z1MC0.h"
#elif defined(CPU_YTM32Z1MD0)
    #define YTM32Z1M_SERIES
    /* Register definitions */
    #include "YTM32Z1MD0/include/YTM32Z1MD0.h"
    /* CPU specific feature definitions */
    #include "YTM32Z1MD0/include/YTM32Z1MD0_features.h"
    #include "YTM32Z1MD0/startup/system_YTM32Z1MD0.h"
#elif defined(CPU_YTM32Z1ME0)
#define YTM32Z1M_SERIES
/* Register definitions */
#include "YTM32Z1ME0/include/YTM32Z1ME0.h"
/* CPU specific feature definitions */
#include "YTM32Z1ME0/include/YTM32Z1ME0_features.h"
#include "YTM32Z1ME0/startup/system_YTM32Z1ME0.h"
#elif defined(CPU_YTM32Z1DS0)
    #define YTM32Z1DS_SERIES
    /* Register definitions */
    #include "YTM32Z1DS0/include/YTM32Z1DS0.h"
    /* CPU specific feature definitions */
    #include "YTM32Z1DS0/include/YTM32Z1DS0_features.h"
    #include "YTM32Z1DS0/startup/system_YTM32Z1DS0.h"
#elif defined(CPU_YTM32Z1LS0)
    #define YTM32B1L_SERIES
    /* Register definitions */
    #include "YTM32Z1LS0/include/YTM32Z1LS0.h"
    /* CPU specific feature definitions */
    #include "YTM32Z1LS0/include/YTM32Z1LS0_features.h"
    #include "YTM32Z1LS0/startup/system_YTM32Z1LS0.h"
#else
    #error "No valid CPU defined!"
#endif

#include "devassert.h"

#endif /* DEVICE_REGISTERS_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/
