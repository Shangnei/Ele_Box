/* USER CODE BEGIN Header */
/* you can remove the copyright */

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
 * @file main.c
 * @brief 
 * 
 */

/* USER CODE END Header */
#include "sdk_project_config.h"
/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function declare --------------------------------------------------*/
/* USER CODE BEGIN PFDC */
AutoFox_INA226 Ina226;
const uint8_t INA226_IC2_ADDRESS = 0x40;
const double SHUNT_RESISTOR_OHMS = 0.01;
const double aMaxCurrent_AMPS = 5.0;
double Current_vlue;
unsigned char Currflag = 0;
/* USER CODE END PFDC */
static void Board_Init(void);

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void User_Init(void);
void pTMR0_IRQHandler(void);
void ADC0_IRQHandler(void);
 
/* USER CODE END 0 */


/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* USER CODE BEGIN 1 */
    
    /* USER CODE END 1 */ 
    Board_Init();
    /* USER CODE BEGIN 2 */
    User_Init();

    
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    
    while (1)
    {
        /* USER CODE END WHILE */
        /* USER CODE BEGIN 3 */
       
      
        Menu_Show();
        if(Currflag)
        {
            Current_vlue = AutoFox_INA226_GetCurrent_uA(&Ina226)/1000.0;
            Currflag = 0;
        }
        // Key_Work();
        // I2C_DRV_MasterReceiveDataBlocking(1, g_at24c02_rxData, 7, true, 1000);
    
    }
    /* USER CODE END 3 */
}

static void Board_Init(void)
{
    CLOCK_SYS_Init(g_clockManConfigsArr,CLOCK_MANAGER_CONFIG_CNT,g_clockManCallbacksArr,CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(CLOCK_MANAGER_ACTIVE_INDEX,CLOCK_MANAGER_POLICY_AGREEMENT);
    pTMR_DRV_Init(0,&PTMR_Config);
    pTMR_DRV_InitChannel(0,0,&ptmr_channel_0);
    pTMR_DRV_InitChannel(0,1,&ptmr_channel_1);
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0,g_pin_mux_InitConfigArr0);
    SPI_DRV_MasterInit(2,&spi_MasterConfig0_State,&spi_MasterConfig0);
    UTILITY_PRINT_Init();
    LIN_DRV_Init(0,&lin_config0,&lin_config0_State);
    ADC_DRV_ConfigConverter(0,&adc_config0);
    I2C_DRV_MasterInit(1,&I2C_MasterConfig0,&I2C_MasterConfig0_State);
    I2C_DRV_MasterInit(1,&I2C_MasterConfig1,&I2C_MasterConfig1_State);
}

/* USER CODE BEGIN 4 */
void User_Init(void)
{
    // unsigned char a = 1;
    Board_Init();
    INT_SYS_EnableIRQ(pTMR0_IRQn);
    INT_SYS_EnableIRQ(ADC0_IRQn);
    pTMR_DRV_StartTimerChannels(PTMR_INST, pTMR0_CH0);
    pTMR_DRV_StartTimerChannels(PTMR_INST, pTMR0_CH1);
    
    OLED_Init(); 
    u8g2_init();
    I2C_DRV_MasterSetSlaveAddr(1,I2C_MasterConfig0.slaveAddress,I2C_MasterConfig0.is10bitAddr);
    AutoFox_INA226_Init(&Ina226,INA226_IC2_ADDRESS,SHUNT_RESISTOR_OHMS,aMaxCurrent_AMPS);
//    I2C_DRV_MasterSendDataBlocking(1,&a,1,false,1000);  
}

void pTMR0_IRQHandler(void)
{
    if (pTMR_DRV_GetInterruptFlagTimerChannels(0, 0))
    {
        pTMR_DRV_ClearInterruptFlagTimerChannels(0, 0);
        /* Note: Debug output inserted into interrupt routine for demo clarity. Might introduce delay. */
        ADC_DRV_Start(ADC_INST);
        Botton_Scan();

        // PRINTF("channel value x = %d  y = %d\n", AdcData[0], AdcData[1]);
    }
    if (pTMR_DRV_GetInterruptFlagTimerChannels(0, 1))
    {
        pTMR_DRV_ClearInterruptFlagTimerChannels(0, 1);
        Potenmeter();
        Currflag = 1; 
        // PRINTF("%d \r\n",INA226_Read2Byte(Current_Reg)) ;
        /* Note: Debug output inserted into interrupt routine for demo clarity. Might introduce delay. */
        

    }

}

void ADC0_IRQHandler(void)
{
    ADC_DRV_ClearEoseqFlagCmd(0);

    AINX = ADC_DRV_ReadFIFO(ADC_INST);
    AINY = ADC_DRV_ReadFIFO(ADC_INST);
    // PRINTF("%d\r\n",AINX );
    // PRINTF("%d\r\n",AINY );

}
/* USER CODE END 4 */
