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

#ifndef CMU_HW_ACCESS_H
#define CMU_HW_ACCESS_H

#include "device_registers.h"
#include <stdbool.h>
#include <stddef.h>

/*!
 * @file cmu_hw_access.h
 */

/*!
 * @ingroup cmu_hw_access
 * @defgroup cmu_hw_access
 * @{
 */

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief Enable CMU channel monitor enable or not.
 *
 * This function enable/disable CMU channel monitor.
 *
 * @param[in] base  Base address for current CMU instance.
 * @param[in] ch    CMU channel monitor channel
 * @param[in] enable    CMU channel monitor enable or disable
 *            true - CMU channel monitor enabled
 *            false - CMU channel monitor disabled
 */
static inline void CMU_SetChEnable(CMU_Type *base, const uint8_t ch, const bool enable)
{
    uint32_t regValue = base->CTRL;
    regValue &= (uint32_t)(~(0x01 << (CMU_CTRL_EN_SHIFT + ch)));
    base->CTRL = regValue | (enable << (CMU_CTRL_EN_SHIFT + ch));
}

/*!
 * @brief Enable CMU channel monitor reset enable or not.
 *
 * This function enable/disable CMU channel monitor reset function.
 *
 * @param[in] base  Base address for current CMU instance.
 * @param[in] ch    CMU channel monitor channel
 * @param[in] enable    CMU channel monitor enable or disable
 *            true - CMU channel monitor will reset MCU
 *            false - CMU channel monitor won't reset MCU
 */
static inline void CMU_SetChResetEnable(CMU_Type *base, const uint8_t ch, const bool enable)
{
    uint32_t regValue = base->CTRL;
    regValue &= (uint32_t)(~(0x01 << (CMU_CTRL_RE_SHIFT + ch)));
    base->CTRL = regValue | (enable << (CMU_CTRL_RE_SHIFT + ch));
}

/*!
 * @brief Select CMU channel reference clock
 *
 * This function set CMU channel reference clock
 *
 * @param[in] base  Base address for current CMU instance.
 * @param[in] ch    CMU channel monitor channel
 * @param[in] refClock CMU channel monitor reference clock
 *            0 - SIRC as reference clock
 *            1 - FXOSC as reference clock
 */
static inline void CMU_SetChRefClock(CMU_Type *base, const uint8_t ch, const uint8_t refClock)
{
    uint32_t regValue = base->CTRL;
    regValue &= (uint32_t)(~(0x01 << (CMU_CTRL_REFS_SHIFT + ch)));
    base->CTRL = regValue | (refClock << (CMU_CTRL_REFS_SHIFT + ch));
}

/*!
 * @brief Set CMU channel compare high value
 *
 * This function set CMU channel compare high value.
 *
 * @param[in] base  Base address for current CMU instance.
 * @param[in] ch    CMU channel monitor channel
 * @param[in] value CMU channel compare high value
 */
static inline void CMU_SetChCmpHigh(CMU_Type *base, const uint8_t ch, const uint16_t value)
{
    base->CMP[ch].HIGH = value;
}
/*!
 * @brief Set CMU channel compare low value
 *
 * This function set CMU channel compare low value.
 *
 * @param[in] base  Base address for current CMU instance.
 * @param[in] ch    CMU channel monitor channel
 * @param[in] value CMU channel compare low value
 */
static inline void CMU_SetChCmpLow(CMU_Type *base, const uint8_t ch, const uint16_t value)
{
    base->CMP[ch].LOW = value;
}

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*! @}*/

#endif /* CMU_HW_ACCESS_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
