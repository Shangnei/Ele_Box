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
/**
 * @file clock_YTM32B1Lx.c
 */

#include "device_registers.h"
#include "ipc_hw_access.h"
#include "scu_hw_access.h"
#include "clock.h"
#include "interrupt_manager.h"
#include <stddef.h>   /* This header is included for bool type */
/*
 * README:
 * This file provides these APIs:
 * 1. APIs to get the frequency of output clocks in Reference Manual ->
 * Chapter Clock Distribution -> Figure Clocking diagram.
 * 2. APIs for IP modules listed in Reference Manual -> Chapter Clock Distribution
 * -> Module clocks.
 */

#if defined(FEATURE_SOC_HAS_SEPARATE_CMU_AND_CLKOUT) && (FEATURE_SOC_HAS_SEPARATE_CMU_AND_CLKOUT == 1)
#include "cfmu_hw_access.h"
#include "cmu_hw_access.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

static clock_manager_state_t g_clockState;

/* FXOSC clock frequency */
static uint32_t g_fxoscClkFreq;                         /* FXOSC clock */
#if defined(FEATURE_SCU_SUPPORT_SXOSC) && (FEATURE_SCU_SUPPORT_SXOSC == 1)
/* SXOSC clock frequency */
static uint32_t g_sxoscClkFreq;                         /* FXOSC clock */
#endif

/*! @brief Clock name mappings
 *         Constant array storing the mappings between clock names and peripheral clock control indexes.
 *         If there is no peripheral clock control index for a clock name, then the corresponding value is
 *         IPC_INVALID_INDEX.
 */
const uint16_t clockNameMappings[] = IPC_CLOCK_NAME_MAPPINGS;
/*!
 * @brief SCU CMU channel defination.
 */
typedef enum
{
    SCU_CMU_CHANNEL_FIRC_CLK = 0x00U,        /*!< CMU checks FIRC clock.   */
    SCU_CMU_CHANNEL_FXOSC_CLK = 0x01U        /*!< CMU checks FOXOSC clock. */
} scu_cmu_channel_t;
/*!
 * @brief SCU system clock type.
 * Implements scu_system_clock_type_t_Class
 */
typedef enum
{
    SCU_SYSTEM_CLOCK_SYSTEM,        /*!< System clock.      */
    SCU_SYSTEM_CLOCK_FAST_BUS,      /*!< Fast bus clock.    */
    SCU_SYSTEM_CLOCK_SLOW_BUS,      /*!< Slow bus clock.    */
    SCU_SYSTEM_CLOCK_MAX            /*!< Max value.         */
} scu_system_clock_type_t;


/*******************************************************************************
 * INTERNAL FUNCTIONS
 ******************************************************************************/
static status_t CLOCK_SYS_SetScuConfiguration(const scu_config_t * scuConfig,
                                              const cmu_config_t* cmuConfig);
static inline void CLOCK_SYS_SetCmuChannelConfiguration(const cmu_ch_config_t* cmuChConfig,
                                                        scu_cmu_channel_t channel);
static status_t CLOCK_SYS_ConfigureSystemClock(scu_system_clock_src_t clkSrc);
#if FEATURE_SCU_SUPPORT_PLL
static status_t CLOCK_SCU_WaitPllLock(void);
status_t CLOCK_DRV_GetPllFreq(uint32_t * frequency);
#endif /* FEATURE_SCU_SUPPORT_PLL */
static status_t CLOCK_SCU_WaitDividerStable(void);
static status_t CLOCK_SCU_WaitFIRCValid(void);
static void CLOCK_SYS_SetIpcConfiguration(const ipc_config_t * peripheralClockConfig);
static inline volatile uint32_t *CLOCK_GetIpcCtrlReg(clock_names_t clockName);
static void CLOCK_SYS_DisableCmu(void);
static void CLOCK_SYS_ConfigureClkOut(const scu_config_t * scuConfig);
static void CLOCK_SYS_DisableClkOut(void);
static inline void CLOCK_SYS_SetDefaultCmuChannelConfiguration(uint32_t channelFreq, scu_cmu_channel_t  channel);

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_DRV_Init
 * Description   : This function sets the system to target configuration, it
 * only sets the clock modules registers for clock mode change, but not send
 * notifications to drivers.
 *
 * Implements CLOCK_DRV_Init_Activity
 * END**************************************************************************/
status_t CLOCK_DRV_Init(clock_manager_user_config_t const * config)
{
    status_t result = STATUS_SUCCESS;

    DEV_ASSERT(config != NULL);

    /* Set IPC settings before clock update. */
    CLOCK_SYS_SetIpcConfiguration(&config->ipcConfig);

    if (config->scuConfigPtr != NULL)
    {
        /* Set SCU settings. */
        result = CLOCK_SYS_SetScuConfiguration(config->scuConfigPtr, config->cmuConfigPtr);
    }

    return result;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_DisableCmu
 * Description   : This function disables the CMU module
 *
 * END**************************************************************************/
static void CLOCK_SYS_DisableCmu(void)
{
#if defined(FEATURE_SOC_HAS_SEPARATE_CMU_AND_CLKOUT) && (FEATURE_SOC_HAS_SEPARATE_CMU_AND_CLKOUT == 0)
    /* Disable CMU. */
    SCU->CMU_CTRL = 0;
    SCU->CMUSTS = SCU->CMUSTS;
#else
    /* Disable CMU. */
    CMU0->CTRL = 0;
    CMU0->STS = CMU0->STS;
#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_ConfigureClkOut
 * Description   : This function configures the clock out
 *
 * END**************************************************************************/
static void CLOCK_SYS_ConfigureClkOut(const scu_config_t * scuConfig)
{
#if defined(FEATURE_SOC_HAS_SEPARATE_CMU_AND_CLKOUT) && (FEATURE_SOC_HAS_SEPARATE_CMU_AND_CLKOUT == 0)
    SCU_SetClkOutSrc(SCU, scuConfig->clockOutConfig.source);
    SCU_SetClkOutDiv(SCU, scuConfig->clockOutConfig.divider);
#else
    CFMU_SetClkOutEnable(CFMU0, scuConfig->clockOutConfig.enable);
    CFMU_SetClkOutSrc(CFMU0, scuConfig->clockOutConfig.source);
    CFMU_SetClkOutDiv(CFMU0, scuConfig->clockOutConfig.divider);
#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_DisableClkOut
 * Description   : This function disables the clock out
 *
 * END**************************************************************************/
static void CLOCK_SYS_DisableClkOut(void)
{
#if defined(FEATURE_SOC_HAS_SEPARATE_CMU_AND_CLKOUT) && (FEATURE_SOC_HAS_SEPARATE_CMU_AND_CLKOUT == 0)
    SCU_SetClkOutSrc(SCU, SCU_CLKOUT_SEL_DISABLE);
#else
    CFMU_SetClkOutEnable(CFMU0, false);
#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_SetScuConfiguration
 * Description   : This function configures the SCU blocks
 *
 * END**************************************************************************/
static status_t CLOCK_SYS_SetScuConfiguration(const scu_config_t * scuConfig,
                                              const cmu_config_t* cmuConfig)
{
    status_t retVal = STATUS_SUCCESS;
    uint32_t tempReg;
    if (NULL != scuConfig)
    {
        /* Only update when the SCU configuration is valid. */
        /* Disable CMU before updating the SCU registers. */
        CLOCK_SYS_DisableCmu();
        /* First switch to SIRC clock. */
        retVal = CLOCK_SYS_ConfigureSystemClock(SCU_SYSTEM_CLOCK_SRC_SIRC);
        if (STATUS_SUCCESS != retVal)
        {
            return retVal;
        }
        /* SIRC low power mode enable configuration */
        SCU_SircDeepSleepEnable(SCU, scuConfig->sircDeepSleepEnable);
        SCU_SircStandbyEnable(SCU, scuConfig->sircStandbyEnable);
        /* FIRC enable configuration */
        if (scuConfig->fircEnable)
        {
            SCU_EnableFirc(SCU, true);
            /* Enable FIRC CMU check. */
            if (cmuConfig != NULL)
            {
                /* Wait for FIRC valid. */
                retVal = CLOCK_SCU_WaitFIRCValid();
                if (STATUS_SUCCESS != retVal)
                {
                    return retVal;
                }
                /* Set CMU settings for FIRC. */
                CLOCK_SYS_SetCmuChannelConfiguration(&(cmuConfig->fircClockMonitor),SCU_CMU_CHANNEL_FIRC_CLK);
            }
            else
            {
                CLOCK_SYS_SetDefaultCmuChannelConfiguration(FEATURE_SCU_FIRC_FREQ, SCU_CMU_CHANNEL_FIRC_CLK);
            }
        }
        else
        {
            /* FIRC disable */
            SCU_EnableFirc(SCU, false);
        }
        SCU_FircDeepSleepEnable(SCU, scuConfig->fircDeepSleepEnable);
        if (scuConfig->fxoscConfig.enable){
            SCU_SetFxoscGain(SCU, scuConfig->fxoscConfig.gainSelection);
            SCU_SetFxoscBypassMode(SCU, scuConfig->fxoscConfig.bypassMode);
            SCU_SetFxoscOutDelay(SCU, scuConfig->fxoscConfig.bypassMode);
            SCU_EnableFxosc(SCU, scuConfig->fxoscConfig.enable);
            retVal = CLOCK_SYS_WaitFXOSCValid();
            if (STATUS_SUCCESS == retVal)
            {
                g_fxoscClkFreq = scuConfig->fxoscConfig.frequency;
            }
            else
            {
                SCU_EnableFxosc(SCU, false);
                g_fxoscClkFreq = 0;
                return retVal;
            }
            if (cmuConfig != NULL)
            {
                /* Set CMU settings for FXOSC. */
                CLOCK_SYS_SetCmuChannelConfiguration(&(cmuConfig->fxoscClockMonitor),SCU_CMU_CHANNEL_FXOSC_CLK);
            }
            else
            {
                CLOCK_SYS_SetDefaultCmuChannelConfiguration(g_fxoscClkFreq, SCU_CMU_CHANNEL_FXOSC_CLK);
            }
        }
        else
        {
            /* FXOSC configuration */
            SCU_EnableFxosc(SCU, false);
            g_fxoscClkFreq = 0;
        }
#if defined(FEATURE_SCU_SUPPORT_SXOSC) && (FEATURE_SCU_SUPPORT_SXOSC == 1)
        if (scuConfig->sxoscConfig.enable)
        {
            SCU_SetSxoscGain(SCU, scuConfig->sxoscConfig.gainSelection);
            SCU_SetSxoscBypassMode(SCU, scuConfig->sxoscConfig.bypassMode);
            SCU_SxoscDeepSleepEnable(SCU, scuConfig->sxoscConfig.deepsleepEnable);
            SCU_SxoscStandbyEnable(SCU, scuConfig->sxoscConfig.standbyEnable);
            SCU_EnableSxosc(SCU, scuConfig->sxoscConfig.enable);
            g_sxoscClkFreq = scuConfig->sxoscConfig.frequency;
        }
        else
        {
            /* SXOSC configuration */
            SCU_EnableSxosc(SCU, false);
            g_sxoscClkFreq = 0;
        }
#endif
#if FEATURE_SCU_SUPPORT_PLL
        /* PLL configuration */
        SCU_EnablePll(SCU, false);
        if (scuConfig->pllConfig.enable)
        {
            SCU_SetPllRefClk(SCU, scuConfig->pllConfig.pllRefClock);
            SCU_SetPllFeedBackDiv(SCU, scuConfig->pllConfig.pllFeedBackDiv);
            SCU_SetPllRefClkDiv(SCU, scuConfig->pllConfig.pllRefClkDiv);
            SCU_EnablePll(SCU, scuConfig->pllConfig.enable);
            /* Wait pll lock */
            retVal = CLOCK_SCU_WaitPllLock();
            if (STATUS_SUCCESS != retVal)
            {
                /* PLL lock timeout */
                SCU->PLL_CTRL = SCU_PLL_CTRL_FBDIV(1);
                return retVal;
            }
            if (cmuConfig != NULL)
            {
                /* Set CMU settings for PLL. */
                CLOCK_SYS_SetCmuChannelConfiguration(&(cmuConfig->pllClockMonitor), SCU_CMU_CHANNEL_PLL_CLK);
            }
            else
            {
                CLOCK_DRV_GetPllFreq(&tempFreq);
                CLOCK_SYS_SetDefaultCmuChannelConfiguration(tempFreq, SCU_CMU_CHANNEL_PLL_CLK);
        }
#endif /* FEATURE_SCU_SUPPORT_PLL */
        /* Update System clock divider */
        SCU_SetSysClkDiv(SCU, scuConfig->sysDiv);
        SCU_SetFastBusClkDiv(SCU, scuConfig->fastBusDiv);
        SCU_SetSlowBusClkDiv(SCU, scuConfig->slowBusDiv);
        /* Update EFM divider */
        tempReg = EFM->CTRL;
        tempReg &= ~(EFM_CTRL_RWS_MASK);
        EFM->CTRL = tempReg | EFM_CTRL_RWS(scuConfig->flashDiv);
#if defined(EFM_CTRL_DPD_EN_MASK)
        EFM->CTRL |= EFM_CTRL_DPD_EN_MASK;
#endif
#if defined(EFM_CTRL_LPEN_MASK) && defined(EFM_CTRL_FWUP_EN_MASK)
        EFM->CTRL |= EFM_CTRL_LPEN_MASK | EFM_CTRL_FWUP_EN_MASK;
#endif
        /* Wait divider stable */
        /* Switch to target clock */
        retVal = CLOCK_SCU_WaitDividerStable();
        if (STATUS_SUCCESS != retVal)
        {
            return retVal;
        }
        /* Switch to target clock */
        retVal = CLOCK_SYS_ConfigureSystemClock(scuConfig->sysClkSrc);
        if (STATUS_SUCCESS != retVal)
        {
            return retVal;
        }
        /* config clock output */
        if (scuConfig->clockOutConfig.enable)
        {
            CLOCK_SYS_ConfigureClkOut(scuConfig);
        }
        else
        {
            /* clock out disabled */
            CLOCK_SYS_DisableClkOut();
        }
    }
    /* Update system clock frequency */
    CLOCK_SYS_GetFreq(CORE_CLK, &SystemCoreClock);

    /* Configure flash prescaler */
    tempReg = EFM->CTRL;
    tempReg &= ~(EFM_CTRL_PRESCALER_MASK);
    EFM->CTRL = tempReg | EFM_CTRL_PRESCALER(SystemCoreClock/2000000);
    /* Update system clock frequency */
    CLOCK_SYS_GetFreq(CORE_CLK, &SystemCoreClock);

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_SetDefaultCmuChannelConfiguration
 * Description   : This function configures the default SCU CMU blocks
 *
 * END**************************************************************************/
static inline void CLOCK_SYS_SetDefaultCmuChannelConfiguration(uint32_t channelFreq, scu_cmu_channel_t channel)
{
#ifdef CMU_ALWAYS_ON
    /* 25% offset for cmu default configuration */
    uint16_t compareHigh = (channelFreq/1000000) * 128 * 5 / 4 / (FEATURE_SCU_SIRC_FREQ/1000000);
    uint16_t compareLow = (channelFreq/1000000) * 128 * 3 / 4 / (FEATURE_SCU_SIRC_FREQ/1000000);
    /* set compare value */
    SCU_SetCmuChCompHigh(SCU, channel, compareHigh);
    SCU_SetCmuChCompLow(SCU, channel, compareLow);
    /* set error reset enable */
    SCU_SetCmuChResetEnable(SCU, channel, true);
    /* set enable */
    SCU_SetCmuChEnable(SCU, channel, true);
#else
    (void)channelFreq;
    (void)channel;
#endif /* CMU_CONFIG_ALWAYS_ON */
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_SetCmuChannelConfiguration
 * Description   : This function configures the SCU CMU blocks
 *
 * END**************************************************************************/
static inline void CLOCK_SYS_SetCmuChannelConfiguration(const cmu_ch_config_t * cmuChConfig,
                                                        const scu_cmu_channel_t channel)
{
    if (NULL != cmuChConfig)
    {
#if defined(FEATURE_SOC_HAS_SEPARATE_CMU_AND_CLKOUT) && (FEATURE_SOC_HAS_SEPARATE_CMU_AND_CLKOUT == 0)
        /* set compare value */
        SCU_SetCmuChCompHigh(SCU, channel, cmuChConfig->compareHigh);
        SCU_SetCmuChCompLow (SCU, channel, cmuChConfig->compareLow);
        /* set reset enable */
        SCU_SetCmuChResetEnable (SCU, channel, cmuChConfig->resetEnable);
        /* set enable */
        SCU_SetCmuChEnable (SCU, channel, cmuChConfig->enable);
#else
        /* set compare value */
        CMU_SetChCmpHigh(CMU0, channel, cmuChConfig->compareHigh);
        CMU_SetChCmpLow(CMU0, channel, cmuChConfig->compareLow);
        /* set reset enable */
        CMU_SetChResetEnable(CMU0, channel, cmuChConfig->resetEnable);
        /* set enable */
        CMU_SetChEnable(CMU0, channel, cmuChConfig->enable);
#endif
    }
}

/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_ConfigureSystemClock
 * Description   : Configures and transitions to a temporary system clock source: FIRC
 * END**************************************************************************/
static status_t CLOCK_SYS_ConfigureSystemClock(scu_system_clock_src_t clkSrc)
{
    uint32_t timeoutLoopCnt;
    SCU_SetSysClkSrc(SCU, clkSrc);
    for(timeoutLoopCnt = 0; timeoutLoopCnt < FEATURE_CLOCK_TIMEOUT_LOOP_CNT; timeoutLoopCnt++){
        if (clkSrc == SCU_GetSysClkSrc(SCU))
        {
            return STATUS_SUCCESS;
        }
    }
    return STATUS_SCU_CLK_SWITCH_TIMEOUT;
}
#if FEATURE_SCU_SUPPORT_PLL
/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SCU_WaitPllLock
 * Description   : Wait PLL lock
 * END**************************************************************************/
static status_t CLOCK_SCU_WaitPllLock(void)
{
    uint32_t timeoutLoopCnt;
    for(timeoutLoopCnt = 0; timeoutLoopCnt < FEATURE_CLOCK_TIMEOUT_LOOP_CNT; timeoutLoopCnt++){
        if (SCU_GetPllLock(SCU))
        {
            return STATUS_SUCCESS;
        }
    }
    return STATUS_SCU_PLL_LOCK_TIMEOUT;
}
#endif /* FEATURE_SCU_SUPPORT_PLL */
/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SCU_WaitDividerStable
 * Description   : Wait system divider stable
 * END**************************************************************************/
static status_t CLOCK_SCU_WaitDividerStable(void)
{
    uint32_t timeoutLoopCnt;
    for(timeoutLoopCnt = 0; timeoutLoopCnt < FEATURE_CLOCK_TIMEOUT_LOOP_CNT; timeoutLoopCnt++){
        if (SCU_GetDividerStable(SCU))
        {
            return STATUS_SUCCESS;
        }
    }
    return STATUS_SCU_PLL_LOCK_TIMEOUT;
}

/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SCU_WaitFIRCValid
 * Description   : Wait FIRC clock valid
 * END**************************************************************************/
static status_t CLOCK_SCU_WaitFIRCValid(void)
{
    uint32_t timeoutLoopCnt;
    for(timeoutLoopCnt = 0; timeoutLoopCnt < FEATURE_CLOCK_TIMEOUT_LOOP_CNT; timeoutLoopCnt++){
        if (SCU_GetFIRCValid(SCU))
        {
            return STATUS_SUCCESS;
        }
    }
    return STATUS_SCU_FIRC_READY_TIMEOUT;
}

/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_WaitFXOSCValid
 * Description   : Wait FXOSC clock valid
 * END**************************************************************************/
status_t CLOCK_SYS_WaitFXOSCValid(void)
{
    uint32_t timeoutLoopCnt;
    uint32_t regValue;
    int8_t fxosc_gain = 1;
    int8_t fxosc_step = 1;
    status_t status = STATUS_SCU_FXOSC_READY_TIMEOUT;
    regValue = SCU->FXOSC_CTRL;
    for(timeoutLoopCnt = 0; timeoutLoopCnt < FEATURE_CLOCK_TIMEOUT_LOOP_CNT; timeoutLoopCnt++)
    {
        /* sweep FXOSC gain */
        SCU_SetFxoscGain(SCU, fxosc_gain);
        if (fxosc_gain == 1)
        {
            fxosc_step = 1;
        }
        else if (fxosc_gain == 7)
        {
            fxosc_step = -1;
        }
        fxosc_gain += fxosc_step;

        if (SCU_GetFXOSCValid(SCU))
        {
            status = STATUS_SUCCESS;
            break;
        }
    }
    /* Update user config gain */
    SCU->FXOSC_CTRL = regValue;
    return status;
}
/*FUNCTION**********************************************************************
 * Function Name : CLOCK_GetIpcCtrlReg
 * Description   : Get IPC base address of given clock name
 * END**************************************************************************/
static inline volatile uint32_t *CLOCK_GetIpcCtrlReg(clock_names_t clockName)
{
    uint32_t idx = clockNameMappings[clockName];
    return &(IPC->CTRL[idx]);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_SetIpcConfiguration
 * Description   : This function configures the IPC block
 *
 * END**************************************************************************/
static void CLOCK_SYS_SetIpcConfiguration(const ipc_config_t * peripheralClockConfig)
{
    DEV_ASSERT(peripheralClockConfig != NULL);
    uint32_t i;
    volatile uint32_t * ipcCtrlReg;

    if ((peripheralClockConfig != NULL) && (peripheralClockConfig->peripheralClocks != NULL))
    {
        for (i = 0U; i < peripheralClockConfig->count; i++)
        {
            ipcCtrlReg = CLOCK_GetIpcCtrlReg(peripheralClockConfig->peripheralClocks[i].clkName);
            /* Disable the peripheral clock before update new clock source */
            IPC_SetClockMode(ipcCtrlReg, false);

            /* Set peripheral clock control */
            IPC_SetPeripheralClockControl(ipcCtrlReg,
                                          peripheralClockConfig->peripheralClocks[i].clkGate,
                                          (uint32_t)peripheralClockConfig->peripheralClocks[i].clkSrc,
                                          (uint32_t)peripheralClockConfig->peripheralClocks[i].divider);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_DRV_ResetModule
 * Description   : This function reset module by clock names
 *
 * Implements CLOCK_DRV_ResetModule_Activity
 * END**************************************************************************/
void CLOCK_DRV_ResetModule(clock_names_t clockName)
{
    volatile uint32_t *ipcCtrlReg;
    ipcCtrlReg = CLOCK_GetIpcCtrlReg(clockName);
    /* reset module */
    IPC_ResetModule(ipcCtrlReg);
}
/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_DRV_SetModuleClock
 * Description   : This function reset module by clock names
 *
 * Implements CLOCK_DRV_SetModuleClock_Activity
 * END**************************************************************************/
void CLOCK_DRV_SetModuleClock(clock_names_t clockName, bool clockGate, uint32_t clkSrc, uint32_t div)
{
    volatile uint32_t *ipcCtrlReg;
    ipcCtrlReg = CLOCK_GetIpcCtrlReg(clockName);
    IPC_SetPeripheralClockControl(ipcCtrlReg, clockGate, clkSrc, div);
}

#if FEATURE_SCU_SUPPORT_PLL
/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_DRV_GetPllFreq
 * Description   : This function returns the frequency of a given clock
 *
 * Implements CLOCK_DRV_GetPllFreq_Activity
 * END**************************************************************************/
status_t CLOCK_DRV_GetPllFreq(uint32_t * frequency)
{
    uint32_t refClkFreq;
    /* Check if PLL is locked */
    if (0 == SCU_GetPllLock(SCU)){
        *frequency = 0;
        return STATUS_MCU_GATED_OFF;
    }
    if (SCU_PLL_REF_FIRC_CLK == SCU_GetPllRefClk(SCU))
    {
        refClkFreq = FEATURE_SCU_FIRC_FREQ;
    }
    else
    {
        /* FXOSC as reference clock */
        refClkFreq = g_fxoscClkFreq;
    }
    /* Fout = (Fref * Ndiv) / ( 2 * Npre) */
    *frequency = ((refClkFreq / SCU_GetPllRefClkDiv(SCU)) * SCU_GetPllFeedBackDiv(SCU)) >> 1;
    return STATUS_SUCCESS;
}
#endif /* FEATURE_SCU_SUPPORT_PLL */

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_DRV_GetFreq
 * Description   : This function returns the frequency of a given clock
 *
 * Implements CLOCK_DRV_GetFreq_Activity
 * END**************************************************************************/
status_t CLOCK_DRV_GetFreq(clock_names_t clockName, uint32_t * frequency)
{
    status_t status = STATUS_SUCCESS;
    volatile uint32_t *ipcCtrlReg;
    if (clockName < IPC_PERI_END_OF_CLOCK)
    {
        /* Note: API won't check if IP have function clock or not */
        ipcCtrlReg = CLOCK_GetIpcCtrlReg(clockName);
        if (0 == (*ipcCtrlReg & IPC_CTRL_CLKEN_MASK)){
            /* IP clock is disabled */
            *frequency = 0;
            return STATUS_MCU_GATED_OFF;
        }
        switch((*ipcCtrlReg & IPC_CTRL_SRCSEL_MASK) >> IPC_CTRL_SRCSEL_SHIFT)
        {
            case CLK_SRC_FIRC:
                *frequency = FEATURE_SCU_FIRC_FREQ;
                break;
            case CLK_SRC_SIRC:
                *frequency = FEATURE_SCU_SIRC_FREQ;
                break;
            case CLK_SRC_FXOSC:
                *frequency = g_fxoscClkFreq;
                break;
#if defined(FEATURE_SCU_SUPPORT_SXOSC) && (FEATURE_SCU_SUPPORT_SXOSC == 1)
                case CLK_SRC_SXOSC:
                *frequency = g_sxoscClkFreq;
                break;
#endif
            case CLK_SRC_LPO:
                *frequency = FEATURE_SCU_LPO_FREQ;
                break;
            default :
                *frequency = 0;
                break;
        }
        /* add divider */
        *frequency = *frequency / (((*ipcCtrlReg & IPC_CTRL_DIV_MASK) >> IPC_CTRL_DIV_SHIFT) + 1);
    }
    else if (clockName < IPC_END_OF_CLOCK)
    {
        switch (clockName)
        {
            case IPC_SIRC_CLK:
                *frequency = FEATURE_SCU_SIRC_FREQ;
                break;
            case IPC_FIRC_CLK:
                *frequency = FEATURE_SCU_FIRC_FREQ;
                break;
            case IPC_FXOSC_CLK:
                *frequency = g_fxoscClkFreq;
                break;
#if defined(FEATURE_SCU_SUPPORT_SXOSC) && (FEATURE_SCU_SUPPORT_SXOSC == 1)
                case IPC_SXOSC_CLK:
                *frequency = g_sxoscClkFreq;
                break;
#endif
            default :
                *frequency = 0;
                break;
        }
    }
    else if (clockName < CLOCK_NAME_COUNT)
    {
        switch (SCU_GetSysClkSrc(SCU))
        {
            case SCU_SYSTEM_CLOCK_SRC_FIRC:
                *frequency = FEATURE_SCU_FIRC_FREQ;
                break;
            case SCU_SYSTEM_CLOCK_SRC_FXOSC:
                *frequency = g_fxoscClkFreq;
                break;
            case SCU_SYSTEM_CLOCK_SRC_SIRC:
                *frequency = FEATURE_SCU_SIRC_FREQ;
                break;
            default:
                *frequency = 0;
                break;
        }
        /* get core clock frequency */
        *frequency = *frequency / (SCU_GetSysClkDiv(SCU) + 1);
        if (CORE_CLK != clockName)
        {
            /* Fast bus clock */
            *frequency = *frequency / (SCU_GetFastBusClkDiv(SCU) + 1);
        }
        if(SLOW_BUS_CLK == clockName)
        {
            /* Slow bus clock */
            *frequency = *frequency / (SCU_GetSlowBusClkDiv(SCU) + 1);
        }
    }
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_Init
 * Description   : Install pre-defined clock configurations.
 * This function installs the pre-defined clock configuration table to the
 * clock manager.
 *
 * Implements CLOCK_SYS_Init_Activity
 *END**************************************************************************/
status_t CLOCK_SYS_Init(clock_manager_user_config_t const **clockConfigsPtr,
                        uint8_t configsNumber,
                        clock_manager_callback_user_config_t **callbacksPtr,
                        uint8_t callbacksNumber)
{
    DEV_ASSERT(clockConfigsPtr != NULL);

    g_clockState.configTable     = clockConfigsPtr;
    g_clockState.clockConfigNum  = configsNumber;
    g_clockState.callbackConfig  = callbacksPtr;
    g_clockState.callbackNum     = callbacksNumber;

    /*
     * errorCallbackIndex is the index of the callback which returns error
     * during clock mode switch. If all callbacks return success, then the
     * errorCallbackIndex is callbacksNumber.
     */
    g_clockState.errorCallbackIndex = callbacksNumber;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_UpdateConfiguration
 * Description   : Send notification and change system clock configuration.
 * This function sends the notification to all callback functions, if all
 * callbacks return OK or forceful policy is used, this function will change
 * system clock configuration. The function should be called only on run mode.
 *
 * Implements CLOCK_SYS_UpdateConfiguration_Activity
 *END**************************************************************************/
status_t CLOCK_SYS_UpdateConfiguration(uint8_t targetConfigIndex,
                                       clock_manager_policy_t policy)
{
    uint8_t callbackIdx;
    bool successfulSetConfig;           /* Set Configuration status */
    status_t ret = STATUS_SUCCESS;
    const clock_manager_callback_user_config_t * callbackConfig;
    clock_notify_struct_t notifyStruct;

    DEV_ASSERT(targetConfigIndex < g_clockState.clockConfigNum);       /* Clock configuration index is out of range. */

    notifyStruct.targetClockConfigIndex = targetConfigIndex;
    notifyStruct.policy                 = policy;

    /* Disable interrupts */
    INT_SYS_DisableIRQGlobal();
    /* Set error callback index as callbackNum, which means no callback error now.*/
    g_clockState.errorCallbackIndex = g_clockState.callbackNum;

    /* First step: Send "BEFORE" notification. */
    notifyStruct.notifyType = CLOCK_MANAGER_NOTIFY_BEFORE;

    /* Send notification to all callback. */
    for (callbackIdx=0; callbackIdx<g_clockState.callbackNum; callbackIdx++)
    {
        callbackConfig = g_clockState.callbackConfig[callbackIdx];
        if ((callbackConfig) &&
            (callbackConfig->callbackType != CLOCK_MANAGER_CALLBACK_AFTER))
        {
            if (STATUS_SUCCESS !=
                (*callbackConfig->callback)(&notifyStruct,
                                            callbackConfig->callbackData))
            {
                g_clockState.errorCallbackIndex = callbackIdx;

                if (CLOCK_MANAGER_POLICY_AGREEMENT == policy)
                {
                    /* Save the error callback index. */
                    ret = STATUS_MCU_NOTIFY_BEFORE_ERROR;
                    break;
                }
            }
        }
    }

    /* If all callback success or forceful policy is used. */
    if ((STATUS_SUCCESS == ret) ||
        (policy == CLOCK_MANAGER_POLICY_FORCIBLE))
    {
        /* clock mode switch. */
        ret = CLOCK_SYS_SetConfiguration(g_clockState.configTable[targetConfigIndex]);
        successfulSetConfig = (STATUS_SUCCESS == ret);

        g_clockState.curConfigIndex = targetConfigIndex;
    }
    else
    {
        /* Unsuccessful setConfiguration */
        successfulSetConfig = false;
    }

    if(successfulSetConfig){
        notifyStruct.notifyType = CLOCK_MANAGER_NOTIFY_AFTER;

        for (callbackIdx=0; callbackIdx<g_clockState.callbackNum; callbackIdx++)
        {
            callbackConfig = g_clockState.callbackConfig[callbackIdx];
            if ((callbackConfig) &&
                (callbackConfig->callbackType != CLOCK_MANAGER_CALLBACK_BEFORE))
            {
                if (STATUS_SUCCESS !=
                    (*callbackConfig->callback)(&notifyStruct,
                                                callbackConfig->callbackData))
                {
                    g_clockState.errorCallbackIndex = callbackIdx;

                    if (CLOCK_MANAGER_POLICY_AGREEMENT == policy)
                    {
                        /* Save the error callback index. */
                        ret = STATUS_MCU_NOTIFY_AFTER_ERROR;
                        break;
                    }
                }
            }
        }
    }
    else /* Error occurs, need to send "RECOVER" notification. */
    {
        notifyStruct.notifyType = CLOCK_MANAGER_NOTIFY_RECOVER;
        for (callbackIdx=0; callbackIdx<g_clockState.callbackNum; callbackIdx++)
        {
            callbackConfig = g_clockState.callbackConfig[callbackIdx];
            if (callbackConfig != NULL)
            {
                (void)(*callbackConfig->callback)(&notifyStruct,
                                                  callbackConfig->callbackData);
            }
        }
    }

    /* Enable interrupts */
    INT_SYS_EnableIRQGlobal();

    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_GetCurrentConfiguration
 * Description   : Get current clock configuration index.
 *
 * Implements CLOCK_SYS_GetCurrentConfiguration_Activity
 *END**************************************************************************/
uint8_t CLOCK_SYS_GetCurrentConfiguration(void)
{
    return g_clockState.curConfigIndex;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_GetErrorCallback
 * Description   : Get the callback which returns error in last clock switch.
 *
 * Implements CLOCK_SYS_GetErrorCallback_Activity
 *END**************************************************************************/
clock_manager_callback_user_config_t* CLOCK_SYS_GetErrorCallback(void)
{
    clock_manager_callback_user_config_t *retValue;

    /* If all callbacks return success. */
    if (g_clockState.errorCallbackIndex >= g_clockState.clockConfigNum)
    {
        retValue = NULL;
    }
    else
    {
        retValue = g_clockState.callbackConfig[g_clockState.errorCallbackIndex];
    }
    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_GetFreq
 * Description   : Wrapper over CLOCK_DRV_GetFreq function. It's part of the old API.
 *
 * Implements CLOCK_SYS_GetFreq_Activity
 *END**************************************************************************/
status_t CLOCK_SYS_GetFreq(clock_names_t clockName, uint32_t *frequency)
{
    return CLOCK_DRV_GetFreq(clockName,frequency);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_SetConfiguration
 * Description   : Wrapper over CLOCK_DRV_Init function. It's part of the old API.
 *
 * Implements CLOCK_SYS_SetConfiguration_Activity
 *END**************************************************************************/
status_t CLOCK_SYS_SetConfiguration(clock_manager_user_config_t const * config)
{
    return CLOCK_DRV_Init(config);
}



/*******************************************************************************
 * EOF
 ******************************************************************************/
