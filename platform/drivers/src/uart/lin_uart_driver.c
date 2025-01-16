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

/*!
 * @file lin_uart_driver.c
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "lin_uart_driver.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Table of base addresses for UART instances. */
UART_Type * const g_linUartBase[UART_INSTANCE_COUNT] = UART_BASE_PTRS;

/* Table to save UART IRQ enumeration numbers defined in the CMSIS header file */
const IRQn_Type g_linUartRxTxIrqId[UART_INSTANCE_COUNT] = UART_RX_TX_IRQS;

#ifdef UART_ERR_IRQS
/* Table to save UART_ERR IRQ enumeration numbers defined in the device header file */
const IRQn_Type g_linUartErrIrqId[UART_INSTANCE_COUNT] = UART_ERR_IRQS;
#endif

/* Table to save UART state structure pointers */
lin_state_t * g_linStatePtr[UART_INSTANCE_COUNT] = {NULL};

/* Table to save LIN user config structure pointers */
lin_user_config_t * g_linUserconfigPtr[UART_INSTANCE_COUNT] = {NULL};

/*******************************************************************************
 * Static variables
 ******************************************************************************/
static uint32_t s_previousTwoBitTimeLength[UART_INSTANCE_COUNT] = {0U};
static uint8_t s_wakeupSignal[UART_INSTANCE_COUNT] = {0U};
static uint8_t s_countMeasure[UART_INSTANCE_COUNT] = {0U};
static uint32_t s_timeMeasure[UART_INSTANCE_COUNT] = {0U};
static const clock_names_t s_linLpuartClkName[UART_INSTANCE_COUNT] = UART_CLOCK_NAMES;
static uint8_t s_txBuff[8] = {0U};

/*******************************************************************************
 * Static function prototypes
 ******************************************************************************/

static status_t LIN_UART_DRV_WaitComplete(uint32_t instance,
                                            uint32_t timeoutMSec);

static void LIN_UART_DRV_ProcessBreakDetect(uint32_t instance);

static void LIN_UART_DRV_CheckWakeupSignal(uint32_t instance);

static void LIN_UART_DRV_ProcessFrame(uint32_t instance,
                                        uint8_t tmpByte);

static void LIN_UART_DRV_ProcessFrameHeader(uint32_t instance,
                                              uint8_t tmpByte);

static void LIN_UART_DRV_ProcessReceiveFrameData(uint32_t instance,
                                                   uint8_t tmpByte);

static void LIN_UART_DRV_ProcessSendFrameData(uint32_t instance,
                                                uint8_t tmpByte);

static void LIN_UART_DRV_AutobaudTimerValEval(uint32_t instance,
                                                uint32_t twoBitTimeLength);

static void LIN_UART_DRV_EvalTwoBitTimeLength(uint32_t instance,
                                                uint32_t twoBitTimeLength);

static uint8_t LIN_UART_DRV_MakeChecksumByte(uint32_t instance,
                                               const uint8_t * buffer,
                                               uint8_t sizeBuffer,
                                               uint8_t PID);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_Init
 * Description   : This function initializes a UART instance for operation.
 * This function will initialize the run-time state structure to keep track of
 * the on-going transfers, initialize the module to user defined settings and
 * default settings, set break field length to be 13 bit times minimum, enable
 * the break detect interrupt, Rx complete interrupt, frame error detect interrupt,
 * and enable the UART module transmitter and receiver.
 *
 * Implements    : LIN_UART_DRV_Init_Activity
 *END**************************************************************************/
status_t LIN_UART_DRV_Init(uint32_t instance,
                             lin_user_config_t * linUserConfig,
                             lin_state_t * linCurrentState)
{
    /* Assert parameters. */
    DEV_ASSERT(linUserConfig && linCurrentState);
    DEV_ASSERT(instance < UART_INSTANCE_COUNT);

    uint32_t linSourceClockFreq = 0U;
    status_t retVal = STATUS_SUCCESS;

    /* Get base address of the UART instance. */
    UART_Type * base = g_linUartBase[instance];

    /* Get the UART clock as configured in the clock manager */
    (void)CLOCK_SYS_GetFreq(s_linLpuartClkName[instance], &linSourceClockFreq);

    /* Check if current instance is clock gated off. */
    DEV_ASSERT(linSourceClockFreq > 0U);

    /* Check if current instance is already initialized. */
    DEV_ASSERT(g_linStatePtr[instance] == NULL);

    /* Save runtime structure pointer. */
    g_linStatePtr[instance] = linCurrentState;

    /* Save LIN user config structure pointer. */
    g_linUserconfigPtr[instance] = linUserConfig;

    /* Clear linSourceClockFreq value */
    linCurrentState->linSourceClockFreq = linSourceClockFreq;

    /* Init UART */
    UART_Init(base);

    /* Create txCompleted and rxCompleted semaphores as counting, with init value is 0 */
    if (OSIF_SemaCreate(&linCurrentState->txCompleted, 0) != STATUS_SUCCESS)
    {
        retVal = STATUS_ERROR;
    }
    else if (OSIF_SemaCreate(&linCurrentState->rxCompleted, 0) != STATUS_SUCCESS)
    {
        retVal = STATUS_ERROR;
    }
    else
    {
        /* if autobaud is enabled */
        if ((linUserConfig->autobaudEnable) && (linUserConfig->nodeFunction == (bool)SLAVE))
        {
            /* Setting Slave's baudrate to 19200 will help Slave node */
            /* always detect LIN Break from Master */
            linUserConfig->baudRate = 19200U;
            linCurrentState->fallingEdgeInterruptCount = 0U;
            linCurrentState->baudrateEvalEnable = true;
            s_previousTwoBitTimeLength[instance] = 0U;
            s_countMeasure[instance] = 0U;
            s_timeMeasure[instance] = 0U;
        }

        /* Set baud rate to User's value */
        (void)UART_DRV_SetBaudRate(instance, linUserConfig->baudRate);

        /* Set 8 bit counts per char */
        UART_SetBitCountPerChar(base, UART_8_BITS_PER_CHAR, false);

        /* Set no parity mode */
        UART_SetParityMode(base, UART_PARITY_DISABLED);

        /* One stop bit */
        UART_SetStopBitCount(base, UART_ONE_STOP_BIT);

        /* Check if the current node is MASTER */
        if (linUserConfig->nodeFunction == (bool)MASTER)
        {
            /* Set Break char length as 13 bits minimum */
            UART_SetBreakCharTransmitLength(base, UART_BREAK_CHAR_13_BIT_MINIMUM);
        }

        /* Set Break char detect length as 13 bits minimum */
        UART_SetBreakCharDetectLength(base, UART_BREAK_CHAR_13_BIT_MINIMUM);

        /* Enable RX complete interrupt */
        UART_SetIntMode(base, UART_INT_RX_DATA_REG_FULL, true);

        /* Enable frame error interrupt */
        UART_SetIntMode(base, UART_INT_FRAME_ERR_FLAG, true);

        /* Enable LIN break detect interrupt */
        UART_SetIntMode(base, UART_INT_LIN_BREAK_DETECT, true);

        /* Install LIN_RxTx ISR for UART instance */
        INT_SYS_InstallHandler(g_linUartRxTxIrqId[instance], g_linUartIsrs[instance], (isr_t *)0);
        /* Enable UART interrupts. */
        INT_SYS_EnableIRQ(g_linUartRxTxIrqId[instance]);
#ifdef UART_ERR_IRQS
        /* Install LIN_ERR ISR for UART instance */
        INT_SYS_InstallHandler(g_linUartErrIrqId[instance], g_linUartErrIsrs[instance], (isr_t *)0);
        /* Enable UART error interrupts */
        INT_SYS_EnableIRQ(g_linUartErrIrqId[instance]);
#endif
        /* Change node's current state to IDLE */
        linCurrentState->currentNodeState = LIN_NODE_STATE_IDLE;

        /* Clear flags in current LIN state structure */
        linCurrentState->isTxBusy = false;
        linCurrentState->isRxBusy = false;
        linCurrentState->isBusBusy = false;
        linCurrentState->isRxBlocking = false;
        linCurrentState->isTxBlocking = false;
        linCurrentState->timeoutCounterFlag = false;
        linCurrentState->timeoutCounter = 0U;

        /* Assign wakeup signal to satisfy LIN Specifications specifies that
         * wakeup signal shall be in range from 250us to 5 ms.
         */
        if (linUserConfig->baudRate > 10000U)
        {
            /* Wakeup signal will be range from 400us to 800us depend on baudrate */
            s_wakeupSignal[instance] = 0x80U;
        }
        else
        {
            /* Wakeup signal will be range from 400us to 4ms depend on baudrate */
            s_wakeupSignal[instance] = 0xF8U;
        }

        if (!((linUserConfig->autobaudEnable) && (linUserConfig->nodeFunction == (bool)SLAVE)))
        {
            /* Enable the UART transmitter and receiver */
            UART_SetTransmitterCmd(base, true);
            UART_SetReceiverCmd(base, true);
        }
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_Deinit
 * Description   : This function shuts down the UART by disabling interrupts and
 *                 transmitter/receiver.
 *
 * Implements    : LIN_UART_DRV_Deinit_Activity
 *END**************************************************************************/
void LIN_UART_DRV_Deinit(uint32_t instance)
{
    /* Assert parameters. */
    DEV_ASSERT(instance < UART_INSTANCE_COUNT);

    /* Get base address of the UART instance. */
    UART_Type * base = g_linUartBase[instance];

    /* Get the current LIN state of this UART instance. */
    lin_state_t * linCurrentState = g_linStatePtr[instance];

    /* Check if current instance is already de-initialized or is gated.*/
    DEV_ASSERT(linCurrentState != NULL);
    DEV_ASSERT(linCurrentState->linSourceClockFreq > 0U);

    /* Wait until the data is completely shifted out of shift register */
    while (!UART_GetStatusFlag(base, UART_TX_COMPLETE))
    {
        /* Do nothing */
    }

    /* Disable the UART transmitter and receiver */
    UART_SetTransmitterCmd(base, false);
    UART_SetReceiverCmd(base, false);

    /* Destroy TX and RX semaphores. */
    (void)OSIF_SemaDestroy(&linCurrentState->txCompleted);
    (void)OSIF_SemaDestroy(&linCurrentState->rxCompleted);

    /* Disable UART interrupts. */
    INT_SYS_DisableIRQ(g_linUartRxTxIrqId[instance]);
#ifdef UART_ERR_IRQS
    INT_SYS_DisableIRQ(g_linUartErrIrqId[instance]);
#endif

    /* Change node's current state to UNINIT */
    linCurrentState->currentNodeState = LIN_NODE_STATE_UNINIT;

    /* Clear our saved pointer to the LIN state structure */
    g_linStatePtr[instance] = NULL;

}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_GetDefaultConfig
 * Description   : Initializes a configuration structure received
 * from the application with default values.
 *
 * Implements    : LIN_UART_DRV_GetDefaultConfig_Activity
 *END**************************************************************************/
void LIN_UART_DRV_GetDefaultConfig(bool isMaster,
                                     lin_user_config_t * linUserConfig)
{
    /* Assert parameters. */
    DEV_ASSERT(linUserConfig != NULL);

    linUserConfig->baudRate = 19200U;
    linUserConfig->autobaudEnable = false;
    linUserConfig->timerGetTimeIntervalCallback = NULL;
    if (isMaster == true)
    {
        linUserConfig->nodeFunction = (bool)MASTER;
    }
    else
    {
        linUserConfig->nodeFunction = (bool)SLAVE;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_InstallCallback
 * Description   : This function installs the callback function that is used for LIN_UART_DRV_IRQHandler.
 * Pass in Null pointer as callback will uninstall.
 *
 * Implements    : LIN_UART_DRV_InstallCallback_Activity
 *END**************************************************************************/
lin_callback_t LIN_UART_DRV_InstallCallback(uint32_t instance,
                                              lin_callback_t function)
{
    /* Assert parameters. */
    DEV_ASSERT(instance < UART_INSTANCE_COUNT);

    /* Get the current LIN state of this UART instance. */
    lin_state_t * linCurrentState = g_linStatePtr[instance];

    /* Get the current callback function. */
    lin_callback_t currentCallback = linCurrentState->Callback;

    /* Install new callback function. */
    linCurrentState->Callback = function;

    return currentCallback;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_MakeChecksumByte
 * Description   : This function calculate checksum for a frame. This function
 * will return classic or enhanced checksum base on data in
 * g_linUserconfigPtr[instance] and input parameter.
 *
 * Implements    : LIN_UART_DRV_SendFrameDataBlocking_Activity
 *END**************************************************************************/
static uint8_t LIN_UART_DRV_MakeChecksumByte(uint32_t instance,
                                               const uint8_t * buffer,
                                               uint8_t sizeBuffer,
                                               uint8_t PID)
{
    /* Get list of PIDs use classic checksum. */
    const uint8_t *classicPID = g_linUserconfigPtr[instance]->classicPID;
    const uint8_t numOfClassicPID = g_linUserconfigPtr[instance]->numOfClassicPID;
    uint8_t checkSum = PID;
    uint8_t retVal = 0U;

    if(numOfClassicPID == 255U)
    {
        /*all frame use enhanced checksum */
        checkSum = 0U;
    }
    else
    {
        if(classicPID != NULL)
        {
            for (retVal = 0U; retVal < numOfClassicPID; retVal++)
            {
                if(checkSum == classicPID[retVal])
                {
                    checkSum = 0U;
                    break;
                }
            }
        }
    }
    retVal = LIN_DRV_MakeChecksumByte(buffer, sizeBuffer, checkSum);
    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_SendFrameDataBlocking
 * Description   : This function sends data out through the UART module using
 * blocking method. This function checks if txSize is in range from 1 to 8.
 * If not, it will return STATUS_ERROR. This function also returns STATUS_ERROR
 * if node's current state is in SLEEP mode. This function checks if the
 * isBusBusy is false, if not it will return STATUS_BUSY. The function does not
 * return until the transmission is complete. If the transmission is successful,
 * it will return STATUS_SUCCESS. If not, it will return STATUS_TIMEOUT.
 *
 * Implements    : LIN_UART_DRV_SendFrameDataBlocking_Activity
 *END**************************************************************************/
status_t LIN_UART_DRV_SendFrameDataBlocking(uint32_t instance,
                                              const uint8_t * txBuff,
                                              uint8_t txSize,
                                              uint32_t timeoutMSec)
{
    uint8_t txCount;
    /* Assert parameters. */
    DEV_ASSERT(txBuff != NULL);
    DEV_ASSERT(instance < UART_INSTANCE_COUNT);

    /* Get base address of the UART instance. */
    UART_Type * base = g_linUartBase[instance];

    /* Get the current LIN state of this UART instance. */
    lin_state_t * linCurrentState = g_linStatePtr[instance];

    status_t retVal = STATUS_SUCCESS;

    /* Check whether current mode is sleep mode */
    bool checkSleepMode = (LIN_NODE_STATE_SLEEP_MODE == linCurrentState->currentNodeState);

    /* Check if txSize > 8 or equal to 0 or node's current state
     * is in SLEEP mode then return STATUS_ERROR */
    if ((8U < txSize) || (0U == txSize) || checkSleepMode)
    {
        retVal = STATUS_ERROR;
    }
    else
    {
        /* Check if the LIN Bus is busy */
        if (linCurrentState->isBusBusy)
        {
            retVal = STATUS_BUSY;
        }
        else
        {
            /* Make the checksum byte. */
            linCurrentState->checkSum = LIN_UART_DRV_MakeChecksumByte(instance, txBuff, txSize, linCurrentState->currentPid);

            /* Update the LIN state structure. */
            linCurrentState->txBuff = txBuff;
            /* Add a place for checksum byte */
            linCurrentState->txSize = (uint8_t)(txSize + 1U);
            linCurrentState->cntByte = 0U;
            linCurrentState->currentEventId = LIN_NO_EVENT;
            linCurrentState->isBusBusy = true;
            linCurrentState->isTxBusy = true;
            linCurrentState->isTxBlocking = true;

            /* Set Break char detect length as 10 bits minimum */
            UART_SetBreakCharDetectLength(base, UART_BREAK_CHAR_10_BIT_MINIMUM);

            /* Set node's current state to SEND_DATA */
            linCurrentState->currentNodeState = LIN_NODE_STATE_SEND_DATA;

            for(txCount=0; txCount<txSize; txCount++){
            /* Wait until the TXFIFO is under watermark */
            while (!UART_GetStatusFlag(base, (uart_status_flag_t)UART_INTF_TXIF_MASK)) {}

            /* Start sending data */
            UART_Putchar(base, *linCurrentState->txBuff);
  
            linCurrentState->txBuff++;
            }

            /* Wait until the TXFIFO is under watermark */
            while (!UART_GetStatusFlag(base, (uart_status_flag_t)UART_INTF_TXIF_MASK)) {}

            /* Start sending checksum */
            UART_Putchar(base, linCurrentState->checkSum);

            /* Wait until the transmission is complete. */
            retVal = LIN_UART_DRV_WaitComplete(instance, timeoutMSec);

            /* Clear Tx Blocking flag */
            linCurrentState->isTxBlocking = false;

            /* Clear Tx Busy flag. */
            linCurrentState->isTxBusy = false;

            /* Change node's current state to IDLE */
            (void)LIN_UART_DRV_GotoIdleState(instance);
        }
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_SendFrameData
 * Description   : This function sends data out through the UART module using
 * non-blocking method. This function will calculate the checksum byte and send
 * it with the frame data. The function will return immediately after calling
 * this function. If txSize is equal to 0 or greater than 8  or node's current
 * state is in SLEEP mode then the function will return STATUS_ERROR. If
 * isBusBusy is currently true then the function will return STATUS_BUSY.
 *
 * Implements    : LIN_UART_DRV_SendFrameData_Activity
 *END**************************************************************************/
status_t LIN_UART_DRV_SendFrameData(uint32_t instance,
                                      const uint8_t * txBuff,
                                      uint8_t txSize)
{
    /* Assert parameters. */
    DEV_ASSERT(txBuff != NULL);
    DEV_ASSERT(instance < UART_INSTANCE_COUNT);

    status_t retVal = STATUS_SUCCESS;

    /* Get base address of the UART instance. */
    UART_Type * base = g_linUartBase[instance];

    /* Get the current LIN state of this UART instance. */
    lin_state_t * linCurrentState = g_linStatePtr[instance];

    /* Check whether current mode is sleep mode */
    bool checkSleepMode = (LIN_NODE_STATE_SLEEP_MODE == linCurrentState->currentNodeState);

    /* Check if txSize > 8 or equal to 0 or node's current state
     * is in SLEEP mode then return STATUS_ERROR */
    if ((8U < txSize) || (0U == txSize) || checkSleepMode)
    {
        retVal = STATUS_ERROR;
    }
    else
    {
        /* Check if the LIN Bus is busy */
        if (linCurrentState->isBusBusy)
        {
            retVal = STATUS_BUSY;
        }
        else
        {
            /* Make the checksum byte. */
            linCurrentState->checkSum = LIN_UART_DRV_MakeChecksumByte(instance, txBuff, txSize, linCurrentState->currentPid);

            /* Update the LIN state structure. */
            for(uint8_t i=0; i<txSize; i++)
            {
                s_txBuff[i] = txBuff[i];
            }
            linCurrentState->txBuff = s_txBuff;
            /* Add a place for checksum byte */
            linCurrentState->txSize = (uint8_t)(txSize + 1U);
            linCurrentState->cntByte = 0U;
            linCurrentState->currentNodeState = LIN_NODE_STATE_SEND_DATA;
            linCurrentState->currentEventId = LIN_NO_EVENT;
            linCurrentState->isBusBusy = true;
            linCurrentState->isTxBusy = true;

            /* Set Break char detect length as 10 bits minimum */
            UART_SetBreakCharDetectLength(base, UART_BREAK_CHAR_10_BIT_MINIMUM);

            /* Start sending data */
            UART_Putchar(base, *linCurrentState->txBuff);
        }
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_GetTransmitStatus
 * Description   : This function returns whether the previous UART transmit has
 * finished. When performing non-blocking transmit, the user can call this
 * function to ascertain the state of the current transmission:
 * in progress (or busy that is STATUS_BUSY) or timeout (if timeout has occurred that is
 * STATUS_TIMEOUT) or complete (success that is STATUS_SUCCESS).
 * In addition, if the transmission is still in progress, the user can obtain the number
 * of bytes that still needed to transmit.
 *
 * Implements    : LIN_UART_DRV_GetTransmitStatus_Activity
 *END**************************************************************************/
status_t LIN_UART_DRV_GetTransmitStatus(uint32_t instance,
                                          uint8_t * bytesRemaining)
{
    /* Assert parameters. */
    DEV_ASSERT(instance < UART_INSTANCE_COUNT);

    status_t retVal = STATUS_SUCCESS;

    /* Get the current LIN state of this UART instance. */
    const lin_state_t * linCurrentState = g_linStatePtr[instance];

    /* Get the number of bytes that is still needed to transmit */
    *bytesRemaining = (uint8_t)(linCurrentState->txSize - linCurrentState->cntByte);

    /* Return status of the on-going transmission */
    if ((linCurrentState->currentEventId == LIN_NO_EVENT) && (*bytesRemaining != 0U))
    {
        if (linCurrentState->timeoutCounterFlag == false)
        {
            retVal = STATUS_BUSY;
        }
        else
        {
            retVal = STATUS_TIMEOUT;
        }
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_RecvFrmDataBlocking
 * Description   : This function receives data from UART module using blocking
 * method, the function does not return until the receive is complete. The interrupt
 * handler LIN_UART_DRV_IRQHandler will check the checksum byte. If the checksum
 * is correct, it will receive the frame data. If the checksum is incorrect, this
 * function will return STATUS_TIMEOUT and data in rxBuff might be wrong. This function
 * also check if rxSize is in range from 1 to 8. If not, it will return STATUS_ERROR.
 * This function also returns STATUS_ERROR if node's current state is in SLEEP mode.
 * This function checks if the isBusBusy is false, if not it will return
 * STATUS_BUSY.
 *
 * Implements    : LIN_UART_DRV_RecvFrmDataBlocking_Activity
 *END**************************************************************************/
status_t LIN_UART_DRV_RecvFrmDataBlocking(uint32_t instance,
                                            uint8_t * rxBuff,
                                            uint8_t rxSize,
                                            uint32_t timeoutMSec)
{
    /* Assert parameters. */
    DEV_ASSERT(rxBuff != NULL);
    DEV_ASSERT(instance < UART_INSTANCE_COUNT);

    /* Get the current LIN state of this UART instance. */
    lin_state_t * linCurrentState = g_linStatePtr[instance];
    status_t retVal = STATUS_SUCCESS;

    /* Check whether current mode is sleep mode */
    bool checkSleepMode = (LIN_NODE_STATE_SLEEP_MODE == linCurrentState->currentNodeState);

    /* Check if rxSize > 8 or equal to 0 or node's current state
     * is in SLEEP mode then return STATUS_ERROR */
    if ((8U < rxSize) || (0U == rxSize) || checkSleepMode)
    {
        retVal = STATUS_ERROR;
    }
    else
    {
        /* Check if the LIN Rx is busy */
        if (linCurrentState->isRxBusy)
        {
            retVal = STATUS_BUSY;
        }
        else
        {
            /* Update the LIN state structure. */
            linCurrentState->rxBuff = rxBuff;
            /* Add a place for checksum byte */
            linCurrentState->rxSize = (uint8_t)(rxSize + 1U);
            linCurrentState->cntByte = 0U;

            /* Setup receiving data blocking */
            linCurrentState->currentEventId = LIN_NO_EVENT;
            linCurrentState->isRxBlocking = true;

            /* Wait until the reception is complete. */
            retVal = LIN_UART_DRV_WaitComplete(instance, timeoutMSec);

            /* In case of receiving a go to sleep request, after callback, node is in SLEEP MODE */
            /* In this case, node is in SLEEP MODE state */
            if (linCurrentState->currentNodeState != LIN_NODE_STATE_SLEEP_MODE)
            {
                /* Update node's current state to IDLE. */
                linCurrentState->currentNodeState = LIN_NODE_STATE_IDLE;
            }

            /* Clear Rx Blocking flag. */
            linCurrentState->isRxBlocking = false;

            /* Update Bus Busy flag. */
            linCurrentState->isBusBusy = false;
            linCurrentState->isRxBusy = false;
        }
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_RecvFrmData
 * Description   : This function receives data from UART module using
 * non-blocking method. This function returns immediately after initiating the
 * receive function. The application has to get the receive status to see when
 * the receive is complete. In other words, after calling non-blocking get
 * function, the application must get the receive status to check if receive
 * is completed or not. The interrupt handler LIN_UART_DRV_IRQHandler will check
 * the checksum byte. If the checksum is correct, it will receive the frame data.
 * If the checksum is incorrect, this function will return STATUS_TIMEOUT and data in
 * rxBuff might be wrong. This function also check if rxSize is in range from 1 to 8.
 * If not, it will return STATUS_ERROR. This function also returns STATUS_ERROR if
 * node's current state is in SLEEP mode. This function checks if the
 * isBusBusy is false, if not it will return STATUS_BUSY.
 *
 * Implements    : LIN_UART_DRV_RecvFrmData_Activity
 *END**************************************************************************/
status_t LIN_UART_DRV_RecvFrmData(uint32_t instance,
                                    uint8_t * rxBuff,
                                    uint8_t rxSize)
{
    /* Assert parameters. */
    DEV_ASSERT(rxBuff != NULL);
    DEV_ASSERT(instance < UART_INSTANCE_COUNT);

    status_t retVal = STATUS_SUCCESS;

    /* Get base address of the UART instance. */
    UART_Type * base = g_linUartBase[instance];

    /* Get the current LIN state of this UART instance. */
    lin_state_t * linCurrentState = g_linStatePtr[instance];

    /* Check whether current mode is sleep mode */
    bool checkSleepMode = (LIN_NODE_STATE_SLEEP_MODE == linCurrentState->currentNodeState);

    /* Check if rxSize > 8 or equal to 0 or node's current state
     * is in SLEEP mode then return STATUS_ERROR */
    if ((8U < rxSize) || (0U == rxSize) || checkSleepMode)
    {
        retVal = STATUS_ERROR;
    }
    else
    {
        /* Check if the LIN Bus is busy */
        if (linCurrentState->isBusBusy)
        {
            retVal = STATUS_BUSY;
        }
        else
        {
            /* Update the LIN state structure. */
            linCurrentState->rxBuff = rxBuff;
            /* Add a place for checksum byte */
            linCurrentState->rxSize = (uint8_t)(rxSize + 1U);
            linCurrentState->cntByte = 0U;

            /* Start receiving data */
            linCurrentState->currentNodeState = LIN_NODE_STATE_RECV_DATA;
            linCurrentState->currentEventId = LIN_NO_EVENT;
            linCurrentState->isBusBusy = true;
            linCurrentState->isRxBusy = true;
            linCurrentState->isRxBlocking = false;

            /* Set Break char detect length as 10 bits minimum */
            UART_SetBreakCharDetectLength(base, UART_BREAK_CHAR_10_BIT_MINIMUM);
        }
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_AbortTransferData
 * Description   : Aborts an on-going non-blocking transmission/reception.
 * While performing a non-blocking transferring data, users can call this
 * function to terminate immediately the transferring.
 *
 * Implements    : LIN_UART_DRV_AbortTransferData_Activity
 *END**************************************************************************/
status_t LIN_UART_DRV_AbortTransferData(uint32_t instance)
{
    /* Assert parameters. */
    DEV_ASSERT(instance < UART_INSTANCE_COUNT);

    status_t retVal = STATUS_SUCCESS;

    /* Get the current LIN state of this UART instance. */
    lin_state_t * linCurrentState = g_linStatePtr[instance];

    /* Change node's current state to IDLE */
    (void)LIN_UART_DRV_GotoIdleState(instance);

    /* Clear LIN Tx and Rx Busy flag */
    linCurrentState->isTxBusy = false;
    linCurrentState->isRxBusy = false;

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_GetReceiveStatus
 * Description   : This function returns whether the previous UART reception is
 * complete. When performing a non-blocking receive, the user can call this
 * function to ascertain the state of the current receive progress: in progress
 * or complete. In addition, if the reception is still in progress, the user can
 * obtain the number of words that is still needed to receive.
 *
 * Implements    : LIN_UART_DRV_GetReceiveStatus_Activity
 *END**************************************************************************/
status_t LIN_UART_DRV_GetReceiveStatus(uint32_t instance,
                                         uint8_t * bytesRemaining)
{
    /* Assert parameters. */
    DEV_ASSERT(instance < UART_INSTANCE_COUNT);

    status_t retVal = STATUS_SUCCESS;

    /* Get the current LIN state of this UART instance. */
    const lin_state_t * linCurrentState = g_linStatePtr[instance];

    /* Get the number of bytes that is still needed to receive */
    *bytesRemaining = (uint8_t)(linCurrentState->rxSize - linCurrentState->cntByte);

    /* Return status of the on-going reception */
    if ((linCurrentState->currentEventId == LIN_NO_EVENT) && (*bytesRemaining != 0U))
    {
        if (linCurrentState->timeoutCounterFlag == false)
        {
            retVal = STATUS_BUSY;
        }
        else
        {
            retVal = STATUS_TIMEOUT;
        }
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_GoToSleepMode
 * Description   : This function puts current LIN node to sleep mode.
 * This function changes current node state to LIN_NODE_STATE_SLEEP_MODE.
 *
 * Implements    : LIN_UART_DRV_GoToSleepMode_Activity
 *END**************************************************************************/
status_t LIN_UART_DRV_GoToSleepMode(uint32_t instance)
{
    /* Assert parameters. */
    DEV_ASSERT(instance < UART_INSTANCE_COUNT);

    /* Get base address of the UART instance. */
    UART_Type * base = g_linUartBase[instance];

    /* Get the current LIN state of this UART instance. */
    lin_state_t * linCurrentState = g_linStatePtr[instance];

    /* Update node's current state to SLEEP_MODE. */
    linCurrentState->currentNodeState = LIN_NODE_STATE_SLEEP_MODE;

    /* Clear txBusy, rxBusy, busBusy flags */
    linCurrentState->isTxBusy  = false;
    linCurrentState->isRxBusy  = false;
    linCurrentState->isBusBusy = false;

    /* Clear UART_RX Pin Active Edge Interrupt Flag. */
    (void)UART_ClearStatusFlag(base, UART_RX_ACTIVE_EDGE_DETECT);

    /* Set Receive data not inverted */
    UART_SetRxDataPolarity(base, false);

    /* Disable RX complete interrupt */
    UART_SetIntMode(base, UART_INT_RX_DATA_REG_FULL, false);

    /* Enable RX Input Active Edge interrupt */
    UART_SetIntMode(base, UART_INT_RX_ACTIVE_EDGE, true);

    /* Disable frame error interrupt */
    UART_SetIntMode(base, UART_INT_FRAME_ERR_FLAG, false);

    /* Disable LIN break detect interrupt */
    UART_SetIntMode(base, UART_INT_LIN_BREAK_DETECT, false);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_SendWakeupSignal
 * Description   : This function sends a wakeup signal through the UART interface.
 *
 * Implements    : LIN_UART_DRV_SendWakeupSignal_Activity
 *END**************************************************************************/
status_t LIN_UART_DRV_SendWakeupSignal(uint32_t instance)
{
    /* DEV_ASSERT parameters. */
    DEV_ASSERT(instance < UART_INSTANCE_COUNT);

    /* Get base address of the UART instance. */
    UART_Type * base = g_linUartBase[instance];
    /* Get the current LIN state of this UART instance. */
    const lin_state_t * linCurrentState = g_linStatePtr[instance];
    status_t retVal = STATUS_SUCCESS;

    /* Check if bus is not busy */
    if (linCurrentState->isBusBusy == false)
    {
        /* Send a wakeup signal */
        UART_Putchar(base, s_wakeupSignal[instance]);
    }
    else
    {
        retVal = STATUS_BUSY;
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_GetCurrentNodeState
 * Description   : This function gets the current LIN node state.
 *
 * Implements    : LIN_UART_DRV_GetCurrentNodeState_Activity
 *END**************************************************************************/
lin_node_state_t LIN_UART_DRV_GetCurrentNodeState(uint32_t instance)
{
    /* Assert parameters. */
    DEV_ASSERT(instance < UART_INSTANCE_COUNT);

    lin_node_state_t retVal = LIN_NODE_STATE_UNINIT;
    /* Get the current LIN state of this UART instance. */
    const lin_state_t * linCurrentState = g_linStatePtr[instance];

    if (linCurrentState != NULL)
    {
        retVal = linCurrentState->currentNodeState;
    }

    /* Return LIN node's current state */
    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_TimeoutService
 * Description   : This is callback function for Timer Interrupt Handler.
 * Users shall initialize a timer (for example FTM) in Output compare mode
 * with period of about 500 micro seconds. In timer IRQ handler, call this function.
 *
 * Implements    : LIN_UART_DRV_TimeoutService_Activity
 *END**************************************************************************/
void LIN_UART_DRV_TimeoutService(uint32_t instance)
{
    /* Assert parameters. */
    DEV_ASSERT(instance < UART_INSTANCE_COUNT);

    /* Get the current LIN state of this UART instance. */
    lin_state_t * linCurrentState = g_linStatePtr[instance];

    /* Get LIN node's current state */
    lin_node_state_t state = linCurrentState->currentNodeState;

    switch (state)
    {
        /* If the node is SENDING DATA */
        case LIN_NODE_STATE_SEND_DATA:
            /* Check if timeout Counter is 0 */
            if (linCurrentState->timeoutCounter == 0U)
            {
                /* Set timeout Counter flag */
                linCurrentState->timeoutCounterFlag = true;

                if (linCurrentState->isTxBlocking == false)
                {
                    /* Callback to handle timeout Counter flag */
                    if (linCurrentState->Callback != NULL)
                    {
                        linCurrentState->Callback(instance, linCurrentState);
                    }

                    /* Clear Tx busy flag */
                    linCurrentState->isTxBusy = false;

                    /* Change the node's current state to IDLE */
                    (void)LIN_UART_DRV_GotoIdleState(instance);
                }
            }
            else /* If timeout Counter is not 0, then decrease timeout Counter by one */
            {
                linCurrentState->timeoutCounter--;
            }

            break;
        /* If the node is RECEIVING DATA */
        case LIN_NODE_STATE_RECV_DATA:
            /* Check if timeout Counter is 0 */
            if (linCurrentState->timeoutCounter == 0U)
            {
                /* Set timeout Counter flag */
                linCurrentState->timeoutCounterFlag = true;

                /* Check if the reception is non-blocking */
                if (linCurrentState->isRxBlocking == false)
                {
                    /* Callback to handle timeout Counter flag */
                    if (linCurrentState->Callback != NULL)
                    {
                        linCurrentState->Callback(instance, linCurrentState);
                    }

                    /* Clear Rx busy flag */
                    linCurrentState->isRxBusy = false;

                    /* Change the node's current state to IDLE */
                    (void)LIN_UART_DRV_GotoIdleState(instance);
                }
            }
            /* If timeout Counter is not 0, then decrease timeout Counter by one */
            else
            {
                linCurrentState->timeoutCounter--;
            }

            break;
        default:
            /* The node state is not SENDING nor RECEIVING data */
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_SetTimeoutCounter
 * Description   : This function sets value for timeout counter that is used in
 * LIN_UART_DRV_TimeoutService
 *
 * Implements    : LIN_UART_DRV_SetTimeoutCounter_Activity
 *END**************************************************************************/
void LIN_UART_DRV_SetTimeoutCounter(uint32_t instance,
                                      uint32_t timeoutValue)
{
    /* Assert parameters. */
    DEV_ASSERT(instance < UART_INSTANCE_COUNT);

    /* Get the current LIN state of this UART instance. */
    lin_state_t * linCurrentState = g_linStatePtr[instance];

    /* Clear Timeout Counter Flag */
    linCurrentState->timeoutCounterFlag = false;

    /* Set new value for Timeout Counter */
    linCurrentState->timeoutCounter = timeoutValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_MasterSendHeader
 * Description   : This function sends frame header out through the UART module
 * using a non-blocking method. Non-blocking  means that the function returns
 * immediately. This function sends LIN Break field, sync field then the ID with
 * correct parity. This function checks if the interface is Master, if not, it will
 * return STATUS_ERROR.This function checks if id is in range from 0 to 0x3F, if not
 * it will return STATUS_ERROR. This function also check node's current state is in
 * SLEEP mode then the function will return STATUS_ERROR. And check if isBusBusy is
 * currently true then the function will return STATUS_BUSY.
 *
 * Implements    : LIN_UART_DRV_MasterSendHeader_Activity
 *END**************************************************************************/
status_t LIN_UART_DRV_MasterSendHeader(uint32_t instance,
                                         uint8_t id)
{
    /* Assert parameters. */
    DEV_ASSERT(instance < UART_INSTANCE_COUNT);

    status_t retVal = STATUS_SUCCESS;

    /* Get the current LIN user config structure of this UART instance. */
    const lin_user_config_t * linUserConfig = g_linUserconfigPtr[instance];

    /* Get base address of the UART instance. */
    UART_Type * base = g_linUartBase[instance];

    /* Get the current LIN state of this UART instance. */
    lin_state_t * linCurrentState = g_linStatePtr[instance];

    /* Check whether current mode is sleep mode */
    bool checkSleepMode = (LIN_NODE_STATE_SLEEP_MODE == linCurrentState->currentNodeState);

    /* Check if the current node is slave or id is invalid or node's current
     * state is in SLEEP state */
    if ((linUserConfig->nodeFunction == (bool)SLAVE) || (0x3FU < id) || checkSleepMode)
    {
        retVal = STATUS_ERROR;
    }
    else
    {
        /* Check if the LIN bus is busy */
        if (linCurrentState->isBusBusy)
        {
            retVal = STATUS_BUSY;
        }
        else
        {
            linCurrentState->currentId = id;

            /* Make parity for the current ID */
            linCurrentState->currentPid = LIN_DRV_ProcessParity(id, MAKE_PARITY);

            /* Set LIN current state to sending Break field */
            linCurrentState->currentNodeState = LIN_NODE_STATE_SEND_BREAK_FIELD;
            linCurrentState->currentEventId = LIN_NO_EVENT;
            linCurrentState->isBusBusy = true;

            /* Set Break char detect length as 13 bits minimum */
            UART_SetBreakCharDetectLength(base, UART_BREAK_CHAR_13_BIT_MINIMUM);
            UART_SetIntMode(base, UART_INT_LIN_BREAK_DETECT, true);

            /* Send break char by using queue mode */
            UART_QueueBreakField(base);
        }
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_EnableIRQ
 * Description   : This function enables UART hardware interrupts.
 *
 * Implements    : LIN_UART_DRV_EnableIRQ_Activity
 *END**************************************************************************/
status_t LIN_UART_DRV_EnableIRQ(uint32_t instance)
{
    /* Assert parameters. */
    DEV_ASSERT(instance < UART_INSTANCE_COUNT);

    status_t retVal = STATUS_SUCCESS;

    /* Get base address of the UART instance. */
    UART_Type * base = g_linUartBase[instance];

    /* Get the current LIN state of this UART instance. */
    const lin_state_t * linCurrentState = g_linStatePtr[instance];

    if (linCurrentState->currentNodeState == LIN_NODE_STATE_SLEEP_MODE)
    {
        /* Enable RX Input Active Edge interrupt */
        UART_SetIntMode(base, UART_INT_RX_ACTIVE_EDGE, true);
    }
    else
    {
        /* Enable RX complete interrupt */
        UART_SetIntMode(base, UART_INT_RX_DATA_REG_FULL, true);

        /* Enable frame error interrupt */
        UART_SetIntMode(base, UART_INT_FRAME_ERR_FLAG, true);

        /* Enable LIN break detect interrupt */
        UART_SetIntMode(base, UART_INT_LIN_BREAK_DETECT, true);
    }

    /* Enable UART interrupts. */
    INT_SYS_EnableIRQ(g_linUartRxTxIrqId[instance]);
#ifdef UART_ERR_IRQS
    INT_SYS_EnableIRQ(g_linUartErrIrqId[instance]);
#endif

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_DisableIRQ
 * Description   : This function disables UART hardware interrupts.
 *
 * Implements    : LIN_UART_DRV_DisableIRQ_Activity
 *END**************************************************************************/
status_t LIN_UART_DRV_DisableIRQ(uint32_t instance)
{
    /* Assert parameters. */
    DEV_ASSERT(instance < UART_INSTANCE_COUNT);

    status_t retVal = STATUS_SUCCESS;

    /* Get base address of the UART instance. */
    UART_Type * base = g_linUartBase[instance];

    /* Get the current LIN state of this UART instance. */
    const lin_state_t * linCurrentState = g_linStatePtr[instance];

    if (linCurrentState->currentNodeState == LIN_NODE_STATE_SLEEP_MODE)
    {
        /* Disable RX Input Active Edge interrupt */
        UART_SetIntMode(base, UART_INT_RX_ACTIVE_EDGE, false);
    }
    else
    {
        /* Disable RX complete interrupt */
        UART_SetIntMode(base, UART_INT_RX_DATA_REG_FULL, false);

        /* Disable frame error interrupt */
        UART_SetIntMode(base, UART_INT_FRAME_ERR_FLAG, false);

        /* Disable frame error interrupt */
        UART_SetIntMode(base, UART_INT_LIN_BREAK_DETECT, false);
    }

    /* Disable UART interrupts. */
    INT_SYS_DisableIRQ(g_linUartRxTxIrqId[instance]);
#ifdef UART_ERR_IRQS
    INT_SYS_DisableIRQ(g_linUartErrIrqId[instance]);
#endif

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_GotoIdleState
 * Description   : This function puts current node to Idle state.
 *
 * Implements    : LIN_UART_DRV_GotoIdleState_Activity
 *END**************************************************************************/
status_t LIN_UART_DRV_GotoIdleState(uint32_t instance)
{
    /* Get base address of the UART instance. */
    UART_Type * base = g_linUartBase[instance];

    /* Get the current LIN state of this UART instance. */
    lin_state_t * linCurrentState = g_linStatePtr[instance];

    linCurrentState->currentEventId = LIN_NO_EVENT;

    /* Set Break char detect length as 13 bits minimum */
    UART_SetBreakCharDetectLength(base, UART_BREAK_CHAR_13_BIT_MINIMUM);

    /* Set Receive data not inverted */
    UART_SetRxDataPolarity(base, false);

    /* Enable RX complete interrupt */
    UART_SetIntMode(base, UART_INT_RX_DATA_REG_FULL, true);

    /* Disable RXEDG interrupt */
    UART_SetIntMode(base, UART_INT_RX_ACTIVE_EDGE, false);

    /* Enable frame error interrupt */
    UART_SetIntMode(base, UART_INT_FRAME_ERR_FLAG, true);

    /* Enable LIN break detect interrupt */
    UART_SetIntMode(base, UART_INT_LIN_BREAK_DETECT, true);

    /* Change node's current state to IDLE */
    linCurrentState->currentNodeState = LIN_NODE_STATE_IDLE;

    /* Clear Bus busy Flag */
    linCurrentState->isBusBusy = false;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_WaitComplete
 * Description   : This function waits until transmission/reception is complete and
 * returns status of the transaction.
 *
 * Implements    : LIN_UART_DRV_WaitComplete_Activity
 *END**************************************************************************/
static status_t LIN_UART_DRV_WaitComplete(uint32_t instance,
                                            uint32_t timeoutMSec)
{
    /* Get the current LIN state of this UART instance. */
    lin_state_t * linCurrentState = g_linStatePtr[instance];
    status_t retVal = STATUS_SUCCESS;

    if (linCurrentState->isTxBlocking == true)
    {
        if (OSIF_SemaWait(&linCurrentState->txCompleted, timeoutMSec) == STATUS_TIMEOUT)
        {
            retVal = STATUS_TIMEOUT;
        }
        /* Reset signal after transfer */
        (void)OSIF_SemaWait(&(linCurrentState->txCompleted), 0);
    }

    if (linCurrentState->isRxBlocking == true)
    {
        if (OSIF_SemaWait(&linCurrentState->rxCompleted, timeoutMSec) == STATUS_TIMEOUT)
        {
            retVal = STATUS_TIMEOUT;
        }
        /* Reset signal after transfer */
        (void)OSIF_SemaWait(&(linCurrentState->rxCompleted), 0);
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_IRQHandler
 * Description   : Interrupt handler for UART.
 * This handler uses the buffers stored in the lin_state_t struct to transfer
 * data. This is not a public API as it is called by IRQ whenever an interrupt
 * occurs.
 *
 * Implements    : LIN_UART_DRV_IRQHandler_Activity
 *END**************************************************************************/
void LIN_UART_DRV_IRQHandler(uint32_t instance)
{
    /* Assert parameters. */
    DEV_ASSERT(instance < UART_INSTANCE_COUNT);

    uint8_t tmpByte = 0U;

    /* Get base address of the UART instance. */
    UART_Type * base = g_linUartBase[instance];

    /* Get the current LIN state of this UART instance. */
    lin_state_t * linCurrentState = g_linStatePtr[instance];

    /* Check RX Input Active Edge interrupt enable */
    bool activeEdgeIntState = UART_GetIntMode(base, UART_INT_RX_ACTIVE_EDGE);

    /* If LIN break character has been detected. */
    if (UART_GetStatusFlag(base, UART_LIN_BREAK_DETECT))
    {
        LIN_UART_DRV_ProcessBreakDetect(instance);
    }
    else
    {
        /* If UART_RX Pin Active Edge has been detected. */
        if (UART_GetStatusFlag(base, UART_RX_ACTIVE_EDGE_DETECT) && activeEdgeIntState)
        {
            /* Clear UART_RX Pin Active Edge Interrupt Flag. */
            (void)UART_ClearStatusFlag(base, UART_RX_ACTIVE_EDGE_DETECT);

            /* Check if a wakeup signal has been received */
            LIN_UART_DRV_CheckWakeupSignal(instance);
        }
        else
        {
            /* If Framing Error has been detected */
            if (UART_GetStatusFlag(base, UART_FRAME_ERR))
            {
                /* Clear Framing Error Interrupt Flag */
                (void)UART_ClearStatusFlag(base, UART_FRAME_ERR);

                /* Read dummy to clear UART_RX_DATA_REG_FULL flag */
                UART_Getchar8(base, &tmpByte);

                /* Set current event id to LIN_FRAME_ERROR */
                linCurrentState->currentEventId = LIN_FRAME_ERROR;

                /* Check if LIN current node state is LIN_NODE_STATE_SEND_DATA */
                if (linCurrentState->currentNodeState == LIN_NODE_STATE_SEND_DATA)
                {
                    /* Callback function to handle Framing Error Event */
                    if (linCurrentState->Callback != NULL)
                    {
                        linCurrentState->Callback(instance, linCurrentState);
                    }
                }
                else
                {
                    /* Check if LIN current node state is LIN_NODE_STATE_RECV_DATA */
                    if (linCurrentState->currentNodeState == LIN_NODE_STATE_RECV_DATA)
                    {
                        /* Callback function to handle Framing Error Event */
                        if (linCurrentState->Callback != NULL)
                        {
                            linCurrentState->Callback(instance, linCurrentState);
                        }
                    }
                }

                /* Change node's state to IDLE */
                (void)LIN_UART_DRV_GotoIdleState(instance);
            }
            else
            {
                if (UART_GetStatusFlag(base, UART_RX_DATA_REG_FULL))
                {
                    /* Get data from Data Register & Clear UART_RX_DATA_REG_FULL flag */
                    UART_Getchar8(base, &tmpByte);

                    /* Process data in Data Register while receive, send data */
                    LIN_UART_DRV_ProcessFrame(instance, tmpByte);
                }
            } /* End else: if (UART_GetStatusFlag(base, UART_FRAME_ERR) == 0) */
        } /* End else: if (UART_GetStatusFlag(base, UART_RX_ACTIVE_EDGE_DETECT) == 0) */
    } /* End else: if (UART_GetStatusFlag(base, UART_LIN_BREAK_DETECT) == 0) */

    /* Get status RX overrun flag */
    if (UART_GetStatusFlag(base, UART_RX_OVERRUN))
    {
        /* Clear overrun flag */
        (void)UART_ClearStatusFlag(base, UART_RX_OVERRUN);

        /* Set current event id to LIN_RX_OVERRUN */
        linCurrentState->currentEventId = LIN_RX_OVERRUN;

        /* Callback function to handle RX Overrun Event */
        if (linCurrentState->Callback != NULL)
        {
            linCurrentState->Callback(instance, linCurrentState);
        }
    }
} /* End void LIN_UART_DRV_IRQHandler(uint32_t instance) */

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_ProcessBreakDetect
 * Description   : This function process break detect for LIN communication.
 *
 * Implements    : LIN_UART_DRV_ProcessBreakDetect_Activity
 *END**************************************************************************/
static void LIN_UART_DRV_ProcessBreakDetect(uint32_t instance)
{
    /* Get the current LIN user configure structure of this UART instance. */
    const lin_user_config_t * linUserConfig = g_linUserconfigPtr[instance];

    /* Get base address of the UART instance. */
    UART_Type * base = g_linUartBase[instance];

    /* Get the current LIN state of this UART instance. */
    lin_state_t * linCurrentState = g_linStatePtr[instance];

    /* Clear LIN Break Detect Interrupt Flag */
    (void)UART_ClearStatusFlag(base, UART_LIN_BREAK_DETECT);

    /* Set Break char detect length as 10 bits minimum */
    UART_SetBreakCharDetectLength(base, UART_BREAK_CHAR_10_BIT_MINIMUM);
    /* Disable LIN Break Detect Interrupt */
    UART_SetIntMode(base, UART_INT_LIN_BREAK_DETECT, false);

    /* Check if the current node is MASTER */
    if (linUserConfig->nodeFunction == (bool)MASTER)
    {
        /* Check if LIN current node state is LIN_NODE_STATE_SEND_BREAK_FIELD */
        if (linCurrentState->currentNodeState == LIN_NODE_STATE_SEND_BREAK_FIELD)
        {
            /* Set flag LIN bus busy */
            linCurrentState->isBusBusy = true;
            /* Change the node's current state to SENDING PID to send PID after send SYNC */
            linCurrentState->currentNodeState = LIN_NODE_STATE_SEND_PID;
            /* Send Sync Field 0x55 */
            UART_Putchar(base, 0x55);
        }
    }
    /* If the current node is SLAVE */
    else
    {
        /* Set flag LIN bus busy */
        linCurrentState->isBusBusy = true;
        /* Change the node's current state to RECEIVED BREAK FIELD */
        linCurrentState->currentEventId = LIN_RECV_BREAK_FIELD_OK;
        /* Callback function */
        if (linCurrentState->Callback != NULL)
        {
            linCurrentState->Callback(instance, linCurrentState);
        }
        /* Change the node's current state to RECEIVING SYNC FIELD */
        linCurrentState->currentNodeState = LIN_NODE_STATE_RECV_SYNC;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_CheckWakeupSignal
 * Description   : This function check if a dominant signal received is a wakeup
 *                signal.
 *
 * Implements    : LIN_UART_DRV_CheckWakeupSignal_Activity
 *END**************************************************************************/
static void LIN_UART_DRV_CheckWakeupSignal(uint32_t instance)
{
    uint32_t wakeupSignalLength = 0U;

    /* Get the current LIN user config structure of this UART instance. */
    const lin_user_config_t * linUserConfig = g_linUserconfigPtr[instance];

    /* Get base address of the UART instance. */
    UART_Type * base = g_linUartBase[instance];

    /* Get the current LIN state of this UART instance. */
    lin_state_t * linCurrentState = g_linStatePtr[instance];

    /* if UART_GetRxDataPolarity is 0: Receive Data is not inverted */
    if (UART_GetRxDataPolarity(base) == false)
    {
        /* Start measure time */
        (void)linUserConfig->timerGetTimeIntervalCallback(&wakeupSignalLength);

        /* Set Receive Data Inverted */
        UART_SetRxDataPolarity(base, true);
    }
    else
    {
        /* Set Receive Data is Not Inverted */
        UART_SetRxDataPolarity(base, false);

        /* Calculate time interval between the falling and rising edge */
        (void)linUserConfig->timerGetTimeIntervalCallback(&wakeupSignalLength);

        /* If length of the dominant signal is longer than 150us, it is a wakeup signal */
        if (wakeupSignalLength >= 150000U)
        {
            linCurrentState->currentEventId = LIN_WAKEUP_SIGNAL;

            /* Callback to handle event: Received a wakeup signal */
            if (linCurrentState->Callback != NULL)
            {
                linCurrentState->Callback(instance, linCurrentState);
            }

            /* Change node's state to IDLE */
            (void)LIN_UART_DRV_GotoIdleState(instance);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_ProcessFrame
 * Description   : Part of Interrupt handler for receiving and sending data.
 * Receive Header, Data and Send Data.
 *
 * Implements    : LIN_UART_DRV_ProcessFrame_Activity
 *END**************************************************************************/
static void LIN_UART_DRV_ProcessFrame(uint32_t instance,
                                        uint8_t tmpByte)
{
    /* Get the current LIN state of this UART instance. */
    const lin_state_t * linCurrentState = g_linStatePtr[instance];

    /* Check node's current state */
    switch (linCurrentState->currentNodeState)
    {
        /* if current state is RECEIVE SYNC FIELD */
        case LIN_NODE_STATE_RECV_SYNC:

        /* if current state is MASTER SENDING PID */
        case LIN_NODE_STATE_SEND_PID:

        /* if current state is RECEIVE PID */
        case LIN_NODE_STATE_RECV_PID:
            LIN_UART_DRV_ProcessFrameHeader(instance, tmpByte);
            break;
        /* if current state is RECEIVE DATA */
        case LIN_NODE_STATE_RECV_DATA:
            LIN_UART_DRV_ProcessReceiveFrameData(instance, tmpByte);
            break;
        /* if current state is SENDING DATA */
        case LIN_NODE_STATE_SEND_DATA:
            LIN_UART_DRV_ProcessSendFrameData(instance, tmpByte);
            break;

        default:
            /* Other node state */
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_ProcessFrameHeader
 * Description   : Part of Interrupt handler for receiving and sending data.
 * Receive Sync byte, PID and Send PID.
 *
 * Implements    : LIN_UART_DRV_ProcessFrameHeader_Activity
 *END**************************************************************************/
static void LIN_UART_DRV_ProcessFrameHeader(uint32_t instance,
                                              uint8_t tmpByte)
{
    /* Get base address of the UART instance. */
    UART_Type * base = g_linUartBase[instance];

    /* Get the current LIN user config structure of this UART instance. */
    const lin_user_config_t * linUserConfig = g_linUserconfigPtr[instance];

    /* Get the current LIN state of this UART instance. */
    lin_state_t * linCurrentState = g_linStatePtr[instance];

    /* Check node's current state */
    switch (linCurrentState->currentNodeState)
    {
        /* If current state is RECEIVE SYNC FIELD */
        case LIN_NODE_STATE_RECV_SYNC:
            if (tmpByte == 0x55U)
            {
                /* Set current event ID to Sync byte is correct */
                linCurrentState->currentEventId = LIN_SYNC_OK;
                /* Change node's current state to RECEIVE PID */
                linCurrentState->currentNodeState = LIN_NODE_STATE_RECV_PID;
            }
            else
            {
                /* Set current event ID to Sync byte is incorrect */
                linCurrentState->currentEventId = LIN_SYNC_ERROR;
                /* Callback function to handle event RECEIVED SYNC FIELD ERROR */
                if (linCurrentState->Callback != NULL)
                {
                    linCurrentState->Callback(instance, linCurrentState);
                }

                /* Change node's current state to IDLE */
                (void)LIN_UART_DRV_GotoIdleState(instance);
            }

            break;
        /* If current state is MASTER SENDING PID */
        case LIN_NODE_STATE_SEND_PID:
            /* Check if master node sent SYNC byte correctly before send PID */
            if (tmpByte == 0x55U)
            {
                /* Change node's current state to RECEIVING PID */
                linCurrentState->currentNodeState = LIN_NODE_STATE_RECV_PID;
                /* Send the current PID byte */
                UART_Putchar(base, linCurrentState->currentPid);
            }
            /* In case of errors during header transmission, it is up to the implementer
             * how to handle these errors (stop/continue transmission) and to decide if the
             * corresponding response is valid or not.
             * By default, LIN Driver set isBusBusy to false, and change node's state to IDLE.
             */
            else
            {
                /* Set current event ID to Sync byte is incorrect */
                linCurrentState->currentEventId = LIN_SYNC_ERROR;
                /* Clear Bus bus flag */
                linCurrentState->isBusBusy = false;
                /* Change node's current state to IDLE */
                linCurrentState->currentNodeState = LIN_NODE_STATE_IDLE;

                /* Callback function to handle event SENT SYNC BYTE ERROR */
                if (linCurrentState->Callback != NULL)
                {
                    linCurrentState->Callback(instance, linCurrentState);
                }
            }

            break;
        /* If current state is RECEIVE PID */
        case LIN_NODE_STATE_RECV_PID:
            /* If the node is MASTER */
            if (linUserConfig->nodeFunction == (bool)MASTER)
            {
                /* Check if master node sent PID correctly */
                if (tmpByte == linCurrentState->currentPid)
                {
                    /* Set current event ID to PID correct */
                    linCurrentState->currentEventId = LIN_PID_OK;

                    /* Check receiving data is blocking */
                    if(tmpByte == linUserConfig->numOfClassicPID)
                    {
                        /* Clear Bus bus flag */
                        linCurrentState->isBusBusy = false;
                        LIN_UART_DRV_SendFrameDataBlocking(instance, linCurrentState->rxBuff, linCurrentState->txSize, 10);
                    }
                    else if (linCurrentState->isRxBlocking == true)
                    {
                        /* Starting receive data blocking */
                        linCurrentState->currentNodeState = LIN_NODE_STATE_RECV_DATA;
                        linCurrentState->isBusBusy = true;
                        linCurrentState->isRxBusy = true;

                        /* Set Break char detect length as 10 bits minimum */
                        UART_SetBreakCharDetectLength(base, UART_BREAK_CHAR_10_BIT_MINIMUM);
                    }
                    else
                    {
                        /* Clear Bus bus flag */
                        linCurrentState->isBusBusy = false;

                        /* Callback function to handle correct PID */
                        if (linCurrentState->Callback != NULL)
                        {
                            linCurrentState->Callback(instance, linCurrentState);
                        }
                    }
                }
                /* In case of errors during header transmission, it is up to the implementer
                 * how to handle these errors (stop/continue transmission) and to decide if the
                 * corresponding response is valid or not.
                 * By default, LIN Driver set isBusBusy to false, and change node's state to IDLE.
                 */
                else
                {
                    /* Set current event ID to PID incorrect */
                    linCurrentState->currentEventId = LIN_PID_ERROR;
                    /* Clear bus busy flag */
                    linCurrentState->isBusBusy = false;
                    /* Change node's current state to IDLE */
                    linCurrentState->currentNodeState = LIN_NODE_STATE_IDLE;
                    /* Callback function to handle event MASTER SENT PID ERROR */
                    if (linCurrentState->Callback != NULL)
                    {
                        linCurrentState->Callback(instance, linCurrentState);
                    }
                }
            }
            /* If the node is SLAVE */
            else
            {
                /* Check the received PID */
                linCurrentState->currentId = LIN_DRV_ProcessParity(tmpByte, CHECK_PARITY);
                linCurrentState->currentPid = tmpByte;
                if (linCurrentState->currentId != 0xFFU)
                {
                    /* Set current event ID to PID correct */
                    linCurrentState->currentEventId = LIN_PID_OK;

                    /* Check receiving data is blocking */
                    if (linCurrentState->isRxBlocking == true)
                    {
                        /* Starting receive data blocking */
                        linCurrentState->currentNodeState = LIN_NODE_STATE_RECV_DATA;
                        linCurrentState->isBusBusy = true;
                        linCurrentState->isRxBusy = true;

                        /* Set Break char detect length as 10 bits minimum */
                        UART_SetBreakCharDetectLength(base, UART_BREAK_CHAR_10_BIT_MINIMUM);
                    }
                    else
                    {
                        /* Clear Bus bus flag */
                        linCurrentState->isBusBusy = false;

                        /* Callback function to handle event PID correct */
                        if (linCurrentState->Callback != NULL)
                        {
                            linCurrentState->Callback(instance, linCurrentState);
                        }
                    }
                }
                else
                {
                    /* Set current event ID to PID ERROR */
                    linCurrentState->currentEventId = LIN_PID_ERROR;

                    /* Callback function to handle event PID incorrect */
                    if (linCurrentState->Callback != NULL)
                    {
                        linCurrentState->Callback(instance, linCurrentState);
                    }

                    /* Change node's current state to IDLE */
                    (void)LIN_UART_DRV_GotoIdleState(instance);
                }
            }

            break;
        default:
            /* Other node state */
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_ProcessReceiveFrameData
 * Description   : Part of Interrupt handler for receiving.
 *
 * Implements    : LIN_UART_DRV_ProcessReceiveFrameData_Activity
 *END**************************************************************************/
static void LIN_UART_DRV_ProcessReceiveFrameData(uint32_t instance,
                                                   uint8_t tmpByte)
{
    /* Get the current LIN state of this UART instance. */
    lin_state_t * linCurrentState = g_linStatePtr[instance];

    if (linCurrentState->rxSize > (linCurrentState->cntByte + 1U))
    {
        *(linCurrentState->rxBuff) = tmpByte;
        linCurrentState->rxBuff++;
    }
    else
    {
        if ((linCurrentState->rxSize - linCurrentState->cntByte) == 1U)
        {
            linCurrentState->checkSum = tmpByte;
        }
    }

    linCurrentState->cntByte++;
    if (linCurrentState->cntByte == linCurrentState->rxSize)
    {
        /* Restore rxBuffer pointer */
        linCurrentState->rxBuff -= linCurrentState->rxSize - 1U;
        if (LIN_UART_DRV_MakeChecksumByte(instance, linCurrentState->rxBuff, linCurrentState->rxSize - 1U, linCurrentState->currentPid) == linCurrentState->checkSum)
        {
            linCurrentState->currentEventId = LIN_RX_COMPLETED;
            linCurrentState->currentNodeState = LIN_NODE_STATE_RECV_DATA_COMPLETED;

            /* callback function to handle RX COMPLETED */
            if (linCurrentState->Callback != NULL)
            {
                linCurrentState->Callback(instance, linCurrentState);
            }

            /* Check if the reception is non-blocking */
            if (linCurrentState->isRxBlocking == false)
            {
                /* Clear Bus busy flag */
                linCurrentState->isBusBusy = false;

                /* Clear Rx busy flag */
                linCurrentState->isRxBusy = false;

                /* In case of receiving a go to sleep request, after callback, node is in SLEEP MODE */
                /* In this case, node is in SLEEP MODE state */
                if (linCurrentState->currentNodeState != LIN_NODE_STATE_SLEEP_MODE)
                {
                    (void)LIN_UART_DRV_GotoIdleState(instance);
                }
            }
            else
            {
                /* Post Semaphore to signal Rx Completed*/
                (void)OSIF_SemaPost(&linCurrentState->rxCompleted);
            }
        }
        else
        {
            linCurrentState->currentEventId = LIN_CHECKSUM_ERROR;
            /* callback function to handle checksum error */
            if (linCurrentState->Callback != NULL)
            {
                linCurrentState->Callback(instance, linCurrentState);
            }

            /* Clear Rx busy flag */
            linCurrentState->isRxBusy = false;

            /* Change node's current state to IDLE */
            (void)LIN_UART_DRV_GotoIdleState(instance);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_ProcessSendFrameData
 * Description   : Part of Interrupt handler for sending data.
 *
 * Implements    : LIN_UART_DRV_ProcessSendFrameData_Activity
 *END**************************************************************************/
static void LIN_UART_DRV_ProcessSendFrameData(uint32_t instance,
                                                uint8_t tmpByte)
{
    bool sendFlag = true;
    uint8_t tmpSize;
    bool tmpCheckSumAndSize;
    bool tmpBuffAndSize;

    /* Get base address of the UART instance. */
    UART_Type * base = g_linUartBase[instance];

    /* Get the current LIN state of this UART instance. */
    lin_state_t * linCurrentState = g_linStatePtr[instance];

    /* Check if Tx data register empty flag is false */
    if (UART_GetStatusFlag(base, UART_TX_DATA_REG_EMPTY) == false)
    {
        linCurrentState->currentEventId = LIN_READBACK_ERROR;
        /* callback function to handle Readback error */
        if (linCurrentState->Callback != NULL)
        {
            linCurrentState->Callback(instance, linCurrentState);
        }

        /* Check if the transmission is non-blocking */
        if (linCurrentState->isTxBlocking == false)
        {
            /* Clear Tx busy flag */
            linCurrentState->isTxBusy = false;

            /* Change node's current state to IDLE */
            (void)LIN_UART_DRV_GotoIdleState(instance);
        }

        sendFlag = false;
    }
    else
    {
        tmpSize = (uint8_t)(linCurrentState->txSize - linCurrentState->cntByte);
        tmpCheckSumAndSize = (tmpSize == 1U) && (linCurrentState->checkSum != tmpByte);
        tmpBuffAndSize = (*linCurrentState->txBuff != tmpByte) && (tmpSize != 1U);
        if (tmpBuffAndSize || tmpCheckSumAndSize)
        {
            linCurrentState->currentEventId = LIN_READBACK_ERROR;

            /* callback function to handle Readback error */
            if (linCurrentState->Callback != NULL)
            {
                linCurrentState->Callback(instance, linCurrentState);
            }

            /* Check if the transmission is non-blocking */
            if (linCurrentState->isTxBlocking == false)
            {
                /* Clear Tx busy flag */
                linCurrentState->isTxBusy = false;

                /* Change node's current state to IDLE */
                (void)LIN_UART_DRV_GotoIdleState(instance);
            }

            sendFlag = false;
        }
        else
        {
            linCurrentState->txBuff++;
            linCurrentState->cntByte++;
        }
    }

    if (sendFlag)
    {
        if (linCurrentState->cntByte < linCurrentState->txSize)
        {
            /* Send checksum byte */
            if ((linCurrentState->txSize - linCurrentState->cntByte) == 1U)
            {
                UART_Putchar(base, linCurrentState->checkSum);
            }
            /* Send data bytes */
            else
            {
                UART_Putchar(base, *linCurrentState->txBuff);
            }
        }
        else
        {
            linCurrentState->currentEventId = LIN_TX_COMPLETED;
            linCurrentState->currentNodeState = LIN_NODE_STATE_SEND_DATA_COMPLETED;

            UART_SetIntMode(base, UART_INT_RX_DATA_REG_FULL, false);
            /* callback function to handle event TX COMPLETED */
            if (linCurrentState->Callback != NULL)
            {
                linCurrentState->Callback(instance, linCurrentState);
            }

            /* Check if the transmission is non-blocking */
            if (linCurrentState->isTxBlocking == false)
            {
                /* Clear Tx busy flag */
                linCurrentState->isTxBusy = false;

                /* In this case, node is in SLEEP MODE state */
                if (linCurrentState->currentNodeState != LIN_NODE_STATE_SLEEP_MODE)
                {
                    /* Change node's current state to IDLE */
                    (void)LIN_UART_DRV_GotoIdleState(instance);
                }
            }
            else
            {
                /* Post Semaphore to signal Tx Completed*/
                (void)OSIF_SemaPost(&linCurrentState->txCompleted);
            }
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_AutobaudTimerValEval
 * Description   : This function calculate LIN bus baudrate and set slave's baudrate accordingly.
 * Autobaud process runs only once after reset. After setting slave's baudrate to LIN bus baudrate,
 * slave does not evaluate LIN bus baudrate anymore.
 * This is not a public API as it is called from other driver functions.
 *
 * Implements    : LIN_UART_DRV_AutobaudTimerValEval_Activity
 *END**************************************************************************/
static void LIN_UART_DRV_AutobaudTimerValEval(uint32_t instance,
                                                uint32_t twoBitTimeLength)
{
    /* Assert parameters. */
    DEV_ASSERT(instance < UART_INSTANCE_COUNT);

    uint32_t MasterBaudRate = 0U;

    /* Get the current LIN user config structure of this UART instance. */
    lin_user_config_t * linUserConfig = g_linUserconfigPtr[instance];

    /* Get the current LIN state of this UART instance. */
    lin_state_t * linCurrentState = g_linStatePtr[instance];

    /* Check whether current node state is receive sync */
    bool checkNodeState = (linCurrentState->currentNodeState == LIN_NODE_STATE_RECV_SYNC);

    /* Evaluate average value against baudrate */
    LIN_UART_DRV_EvalTwoBitTimeLength(instance, twoBitTimeLength);

    if ((linCurrentState->fallingEdgeInterruptCount > 4U) && checkNodeState)
    {
        if ((twoBitTimeLength >= TWO_BIT_DURATION_MIN_19200) && (twoBitTimeLength <= TWO_BIT_DURATION_MAX_19200))
        {
            MasterBaudRate = 19200U;
        }
        else if ((twoBitTimeLength >= TWO_BIT_DURATION_MIN_14400) && (twoBitTimeLength <= TWO_BIT_DURATION_MAX_14400))
        {
            MasterBaudRate = 14400U;
        }
        else if ((twoBitTimeLength >= TWO_BIT_DURATION_MIN_9600) && (twoBitTimeLength <= TWO_BIT_DURATION_MAX_9600))
        {
            MasterBaudRate = 9600U;
        }
        else if ((twoBitTimeLength >= TWO_BIT_DURATION_MIN_4800) && (twoBitTimeLength <= TWO_BIT_DURATION_MAX_4800))
        {
            MasterBaudRate = 4800U;
        }
        else if ((twoBitTimeLength >= TWO_BIT_DURATION_MIN_2400) && (twoBitTimeLength <= TWO_BIT_DURATION_MAX_2400))
        {
            MasterBaudRate = 2400U;
        }
        else
        {
            /* Complete if-elseif-else block to avoid violating MISRA 2012 Rule 15.7 */
        }

        /* Check Master Baudrate against node's current baudrate */
        if ((MasterBaudRate != 0U) && (linUserConfig->baudRate != MasterBaudRate))
        {
            linUserConfig->baudRate = MasterBaudRate;

            /* Set new baud rate */
            (void)UART_DRV_SetBaudRate(instance, linUserConfig->baudRate);

            /* Assign wakeup signal to satisfy LIN Specifications specifies that
             * wakeup signal shall be in range from 250us to 5 ms.
             */
            if (linUserConfig->baudRate > 10000U)
            {
                /* Wakeup signal will be range from 400us to 800us depend on baudrate */
                s_wakeupSignal[instance] = 0x80U;
            }
            else
            {
                /* Wakeup signal will be range from 400us to 4ms depend on baudrate */
                s_wakeupSignal[instance] = 0xF8U;
            }
        }

        linCurrentState->currentEventId = LIN_BAUDRATE_ADJUSTED;
        /* Disable baudrate evaluation process */
        linCurrentState->baudrateEvalEnable = false;
        /* Callback function to handle this event */
        if (linCurrentState->Callback != NULL)
        {
            linCurrentState->Callback(instance, linCurrentState);
        }

        /* Update current state and current event */
        linCurrentState->currentNodeState = LIN_NODE_STATE_RECV_PID;
        linCurrentState->currentEventId = LIN_SYNC_OK;

    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_EvalTwoBitTimeLength
 * Description   : This function check time of double bit in sync byte for support autobaud.
 * This is not a public API as it is called from other driver functions.
 *
 * Implements    : LIN_UART_DRV_EvalTwoBitTimeLength_Activity
 *END**************************************************************************/
static void LIN_UART_DRV_EvalTwoBitTimeLength(uint32_t instance,
                                                uint32_t twoBitTimeLength)
{
    /* Assert parameters. */
    DEV_ASSERT(instance < UART_INSTANCE_COUNT);

    /* Get the current LIN state of this UART instance. */
    lin_state_t * linCurrentState = g_linStatePtr[instance];

    if (linCurrentState->fallingEdgeInterruptCount < 5U)
    {
        if (linCurrentState->fallingEdgeInterruptCount > 0U)
        {
            if ((twoBitTimeLength < TWO_BIT_DURATION_MIN_19200) ||
                ((twoBitTimeLength > TWO_BIT_DURATION_MAX_19200) && (twoBitTimeLength < TWO_BIT_DURATION_MIN_14400)) ||
                ((twoBitTimeLength > TWO_BIT_DURATION_MAX_14400) && (twoBitTimeLength < TWO_BIT_DURATION_MIN_9600)) ||
                ((twoBitTimeLength > TWO_BIT_DURATION_MAX_9600) && (twoBitTimeLength < TWO_BIT_DURATION_MIN_4800)) ||
                ((twoBitTimeLength > TWO_BIT_DURATION_MAX_4800) && (twoBitTimeLength < TWO_BIT_DURATION_MIN_2400)) ||
                (twoBitTimeLength > TWO_BIT_DURATION_MAX_2400))
            {
                /* Change node's current state to IDLE */
                (void)LIN_UART_DRV_GotoIdleState(instance);
                linCurrentState->fallingEdgeInterruptCount = 0U;
            }
            else
            {
                if (linCurrentState->fallingEdgeInterruptCount > 1U)
                {
                    if ((twoBitTimeLength < ((100U - BIT_RATE_TOLERANCE_UNSYNC) * s_previousTwoBitTimeLength[instance] / 100U)) ||
                        (twoBitTimeLength > ((100U + BIT_RATE_TOLERANCE_UNSYNC) * s_previousTwoBitTimeLength[instance] / 100U)))
                    {
                        /* Change node's current state to IDLE */
                        (void)LIN_UART_DRV_GotoIdleState(instance);
                        linCurrentState->fallingEdgeInterruptCount = 0U;
                    }
                }
            }

            s_previousTwoBitTimeLength[instance] = twoBitTimeLength;
        }
    }

    linCurrentState->fallingEdgeInterruptCount += 1U;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LIN_UART_DRV_AutoBaudCapture
 * Description   : This function capture bits time to detect break char, calculate
 * baudrate from sync bits and enable transceiver if autobaud successful.
 * This function should only be used in Slave.
 *
 * Implements    : LIN_UART_DRV_AutoBaudCapture_Activity
 *END**************************************************************************/
status_t LIN_UART_DRV_AutoBaudCapture(uint32_t instance)
{
    /* Assert parameters. */
    DEV_ASSERT(instance < UART_INSTANCE_COUNT);

    /* Get base address of the UART instance. */
    UART_Type * base = g_linUartBase[instance];
    /* Get the current LIN user config structure of this UART instance. */
    const lin_user_config_t * linUserConfig = g_linUserconfigPtr[instance];
    /* Get the current LIN state of this UART instance. */
    lin_state_t * linCurrentState = g_linStatePtr[instance];
    status_t retVal = STATUS_BUSY;
    uint32_t tmpTime = 0U;

    if (linCurrentState->baudrateEvalEnable)
    {
        /* Calculate time between two bit (for service autobaud) */
        (void)linUserConfig->timerGetTimeIntervalCallback(&tmpTime);

        /* Get two bits time length */
        s_timeMeasure[instance] += tmpTime;
        s_countMeasure[instance]++;
        if ((s_countMeasure[instance] > 1U))
        {
            switch (linCurrentState->currentNodeState)
            {
                /* If current state is SLEEP MODE */
                case LIN_NODE_STATE_SLEEP_MODE:
                    /* If length of the dominant signal is longer than 150us, it is a wakeup signal */
                    if (tmpTime >= 150000U)
                    {
                        linCurrentState->currentEventId = LIN_WAKEUP_SIGNAL;

                         /* Callback to handle event: Received a wakeup signal */
                        if (linCurrentState->Callback != NULL)
                        {
                            linCurrentState->Callback(instance, linCurrentState);
                        }

                        /* Change node's state to IDLE */
                        (void)LIN_UART_DRV_GotoIdleState(instance);
                    }
                    else
                    {
                        retVal = STATUS_ERROR;
                    }
                    s_countMeasure[instance] = 0U;

                    break;
                /* If current state is IDLE */
                case LIN_NODE_STATE_IDLE:
                    /* Check break time minimum */
                    if (tmpTime >= AUTOBAUD_BREAK_TIME_MIN)
                    {
                        /* Set Break char detect length as 10 bits minimum */
                        UART_SetBreakCharDetectLength(base, UART_BREAK_CHAR_10_BIT_MINIMUM);

                        /* Disable LIN Break Detect Interrupt */
                        UART_SetIntMode(base, UART_INT_LIN_BREAK_DETECT, false);

                        /* Set flag LIN bus busy */
                        linCurrentState->isBusBusy = true;

                        /* Change the node's current state to RECEIVED BREAK FIELD */
                        linCurrentState->currentEventId = LIN_RECV_BREAK_FIELD_OK;

                        /* Callback function */
                        if (linCurrentState->Callback != NULL)
                        {
                            linCurrentState->Callback(instance, linCurrentState);
                        }

                        /* Change the node's current state to RECEIVING SYNC FIELD */
                        linCurrentState->currentNodeState = LIN_NODE_STATE_RECV_SYNC;

                        /* Start Autobaud Count(initialize number of measurements in sync byte) */
                        linCurrentState->fallingEdgeInterruptCount = 0U;

                        s_countMeasure[instance] = 1U;
                    }
                    else
                    {
                        s_countMeasure[instance] = 0U;
                        retVal = STATUS_ERROR;
                    }

                    break;
                /* If current state is RECEIVE SYNC */
                default:
                    /* Calculate baudrate */
                    LIN_UART_DRV_AutobaudTimerValEval(instance, s_timeMeasure[instance]);

                    /* Reset to measure in next times */
                    s_countMeasure[instance] = 0U;
                    s_timeMeasure[instance] = 0U;

                    if (linCurrentState->currentNodeState == LIN_NODE_STATE_IDLE)
                    {
                        retVal = STATUS_ERROR;
                    }

                    break;
            }
        }
    }
    else
    {
        if (linCurrentState->fallingEdgeInterruptCount > 4U)
        {
            /* Enable the UART transmitter and receiver */
            UART_SetTransmitterCmd(base, true);
            UART_SetReceiverCmd(base, true);

            linCurrentState->fallingEdgeInterruptCount = 0U;
        }

        retVal = STATUS_SUCCESS;
    }

    return retVal;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
