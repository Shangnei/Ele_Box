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

#ifndef CFMU_HW_ACCESS_H
#define CFMU_HW_ACCESS_H

#include "device_registers.h"
#include <stdbool.h>
#include <stddef.h>

/*!
 * @file cfmu_hw_access.h
 */

/*!
 * @ingroup cfmu_hw_access
 * @defgroup cfmu_hw_access
 * @{
 */

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief Enable CFMU clock out enable or not.
 *
 * This function enable/disable to-be-measured clock out.
 *
 * @param[in] base  Base address for current CFMU instance.
 * @param[in] enable    CFMU clock out enable or disable
 *            true - CFMU clock out enabled
 *            false - CFMU clock out disabled
 */
static inline void CFMU_SetClkOutEnable(CFMU_Type *base, const bool enable)
{
    uint32_t regValue = base->CTRL;
    regValue &= (uint32_t)(~(0x01 << CFMU_CTRL_CLK_OUTEN_SHIFT));
    base->CTRL = regValue | (enable << CFMU_CTRL_CLK_OUTEN_SHIFT);
}

/*!
 * @brief Select clock out source.
 *
 * This function select the to-be-measured clock out source.
 *
 * @param[in] base  Base address for current CFMU instance.
 * @param[in] clkoutSrc  clockout source.
 *            0 : Disable clock output
 *            1 : FIRC
 *            2 : SIRC
 *            3 : FXOSC
 *            4 : SXOSC
 *            5 : LPO
 */
static inline void CFMU_SetClkOutSrc(CFMU_Type* const base, const uint8_t src)
{
    uint32_t regValue = base->CTRL;
    regValue &= ~(CFMU_CTRL_CLK_SEL_MASK);
    regValue |= CFMU_CTRL_CLK_SEL(src);
    base->CTRL = regValue;
}

/*!
 * @brief Set SCU clock output divider
 *
 * This function set to-be-measured clock output divider.
 *
 * @param[in] base  Base address for current CFMU instance.
 * @param[in] clkSrc Clock output divider.
 */
static inline void CFMU_SetClkOutDiv(CFMU_Type *base, const uint8_t clkDiv)
{
    uint32_t regValue = base->CTRL;
    regValue &= (uint32_t)(~CFMU_CTRL_CLK_DIV_MASK);
    regValue |= (uint32_t)CFMU_CTRL_CLK_DIV(clkDiv);
    base->CTRL = regValue;
}

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*! @}*/

#endif /* CFMU_HW_ACCESS_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
