# Description

The YTM32 Software Development Kit (YTM32 SDK) is an extensive suite of peripheral abstraction layers, peripheral drivers, RTOS and so on designed to simplify and accelerate application development on Yuntu Microcontrollers.

# Compiler and IDE Versions

SDK contains many demos base on Yuntu Config Tool(YCT), and the version of supported IDE as follows:

- GCC (10.3.1)
- IAR (8.40)
- MDK (5.35)
- YuntuIDE

# YTM32 SDK Release Change Log

## YTM32Z1LS0

### 1_3_1 2024-07-18
* [Opt] - [SDK_GUI] Freertos and secure boot demo are generated through YCT.
* [Opt] - Add FXOSC gain scan when enabling FXOSC or waking up from low power mode.
* [Opt] - Display WDG, EMU, pTMR instance index for ipc clock name and irq name.
* [Opt] - Add ipc module reset api, update clock initialize flow.
* [Opt] - Put DMA_DRV_ClearIntStatus() api in front of callback in the DMA_DRV_IRQHandler() function.
* [Opt] - Dummy wait to ensure transfer complete signal is 0 for uart and lin.
* [Opt] - Clear the count of capture when measurement complete.
* [Fix] - RTOS would cause LPACK reset after wakeup.
* [Fix] - The pcs continuous function of spi is only valid for the first transmission.
* [Fix] - I2C timeout flag is set abnormally.
* [Fix] - I2C bus busy condition process in the I2C_DRV_MasterInit api.
* [Fix] - UART Tx/Rx DMA enablement can not be clear.
* [Fix] - ECC error when viewing register value of flexcan in the debug mode.
* [Fix] - DEV_ASSERT error in can pal layer when using enhanced FIFO.
* [Fix] - [SDK_GUI] The sequence of pins which have been configured will change when modifying the feature of pinout.
* [Fix] - [SDK_GUI] There's no value in parameter pullConfig of pinmux.c when just enabling pull enable.
* [Fix] - [SDK_GUI] Interrupt vector table update.
* [Fix] - [SDK_GUI] Data section is not initialzied completely when power on reset.
* [Fix] - [SDK_GUI] Configuration issue of driveSelect in pinout configuration.
* [Fix] - [SDK-GUI] DMA callback function is not declared externally when generating dma configuration.
* [Fix] - [SDK-GUI] FlexCAN payload size label is incorrect.
* [Fix] - [SDK_GUI] eTMR measurement configuration in input capture mode is missing.
* [Fix] - [SDK_GUI] eTMR overflow and fault handler name both mismatch.
* [Fix] - [SDK_GUI] ADC trigger type does not support hardware trigger.
* [New] - [SDK_GUI] Add api highlight for pTMR_DRV_InitChannel api.
* [New] - Open debug disable bit of CTRL_OVRD register.
* [New] - Add EMU_DRV_GetChannelSBInterruptFlag api.

### 1_3_0 2024-04-19
First release.

## YTM32B1HA0

### 1_3_1 2024-07-18
* [Opt] - [SDK_GUI] Freertos and secure boot demo are generated through YCT.
* [Opt] - Add FXOSC gain scan when enabling FXOSC or waking up from low power mode.
* [Opt] - Display WDG, EMU, pTMR instance index for ipc clock name and irq name.
* [Opt] - Put DMA_DRV_ClearIntStatus api in front of callback in the DMA_DRV_IRQHandler function.
* [Opt] - Remove Timeout enable and disable operation in linflexd uart isr handler, and set to configurable in the linflexd yct demo by user.
* [Opt] - Dummy wait to ensure transfer complete signal is 0 for linflexd and sai module.
* [Opt] - Add ipc module reset api for module reset function.
* [Fix] - __vector_table not found occurred when debugging iar project.
* [Fix] - RWS is set as 2 in 100MHz fast bus in YCT GUI.
* [Fix] - RTOS would cause LPACK reset after wakeup.
* [Fix] - The pcs continuous function of spi is only valid for the first transmission.
* [Fix] - I2C timeout flag is set abnormally.
* [Fix] - I2C bus busy condition process in the I2C_DRV_MasterInit api.
* [Fix] - ECC error when viewing register value of flexcan in the debug mode.
* [Fix] - DEV_ASSERT error in can pal layer when using enhanced FIFO.
* [Fix] - The input size of flash api check issue.
* [Fix] - PLL clock frequency will overflow when selecting FIRC as reference for pll.
* [Fix] - MPWM disable counter api can not disable the counter of mpwm.
* [Fix] - The variable type of monitorIrqn in the intm_config_t will cause compile warning when using iar project.
* [Fix] - [SDK_GUI] The sequence of pins which have been configured will change when modifying the feature of pinout.
* [Fix] - [SDK_GUI] There's no value in parameter pullConfig of pinmux.c when just enabling pull enable.
* [Fix] - [SDK_GUI] Interrupt vector table update.
* [Fix] - [SDK_GUI] Data section is not initialzied completely when power on reset.
* [Fix] - [SDK_GUI] Configuration issue of driveSelect in pinout configuration.
* [Fix] - [SDK-GUI] DMA callback function is not declared externally when generating dma configuration.
* [Fix] - [SDK_GUI] WKU channel configuration issue.
* [Fix] - [SDK-GUI] FlexCAN payload size label is incorrect.
* [Fix] - [SDK_GUI] eTMR measurement configuration in input capture mode is missing.
* [Fix] - [SDK_GUI] ADC trigger type does not support hardware trigger.
* [Fix] - [SDK_GUI] INTM can not load IRQn of chip in the GUI.
* [Fix] - [SDK_GUI] INTM module initial api parameter issue.
* [New] - [SDK_GUI] Add pulse check in sent configuation ui.
* [New] - [SDK_GUI] Add mpwm module configuration ui.
* [New] - [SDK_GUI] Add api highlight for pTMR_DRV_InitChannel api.
* [New] - [YCT_Demo] Add flash disable debugger and flash ecc demos.
* [New] - [YCT_Demo] Add mpwm pwm demo.
* [New] - Open debug disable bit of CTRL_OVRD register.
* [New] - Add eTMR_DRV_SetCounterMid and eTMR_DRV_GetMid api.
* [New] - Add EMU_DRV_GetChannelSBInterruptFlag api.

### 1_3_0 2024-04-19
* [Fix] - [FLEXCAN] Add enhanceRxFifoState.state init when init flexcan.
* [Fix] - I2C won't detect line status when init function calls.
* [Fix] - I2C won't process SDA SCL low timeout event, user may need this callback for bus error process.
* [Fix] - LINFlexD deinit function disable IRQ flow error.

### 1_2_2 2024-04-12
The SDK code remains the same as the last version. Just YT Config Tool has been updated to fix the issue that would cause the loss of secure_boot_YTM32B1HA0.c/h file in the startup directory.

### 1_2_1 2024-03-28
* [Opt] - [FEE] Loosen the restriction that pflash cannot use fee
* [Fix] - [FEE] Fix sync mode donot launch command in RAM.
* [Fix] - [DMA] It issues BTCE error, if call DMA_CTSClearReg() when dma request enable.
* [Fix] - YCT Demos YT_LINK issue and the size of generated binary file is overlarge.

### 1_2_0 2024-03-12
* [Opt] - Enable/Disable EFM ECC Error interrupt should not change NVIC configuration
* [Opt] - Add FEATURE_EFM_OPERATION_CLOCK_SOURCE for YTM32B1Mx.
* [Opt] - Update mpwm driver
* [Opt] - Disable or enable each arithmetic by the HCU features of each device
* [Opt] - DMA_REQ_Disable change to DMA_REQ_DISABLED
* [Opt] - Optimize ACMP YCT demos
* [Opt] - Update all spi demos for new YT link.
* [Opt] - Wdg_Demo always reset, added abort condition.
* [Fix] - Defines of EFM_READ_COLLISION_IRQS are wrong in in YTM32B1HA0.h
* [Fix] - MPWM function redefinition
* [Fix] - Add global interrupt disable in the FLEXCAN_SetMsgBuffIntCmd() function
* [Fix] - The event type of error_callback in the FlexCANState should be flexcan_error_event_type_t 
* [Fix] - [TRNG] Cannot generate random data after TRNG_DRV_DeInit() and reinit.
* [Fix] - [Flash] Boot swap alway fail if fail once.
* [Fix] - Lin will stop transfer when bit error occurs.
* [Fix] - WDG reset value is not correct.
* [Fix] - CR[DSDIS] and CR[DBGDIS] bit logic error in the WDG driver.
* [Fix] - The value of typeOfUpdate is wrong when using eTMR_DRV_UpdatePwmPeriodAndDuty function
* [Fix] - VAL0 and VAL1 can't sync when using eTMR_DRV_InitPwm after calling eTMR_DRV_DeinitPwm function
* [Fix] - Flash async mode cannot get idle by FLASH_DRV_GetBusyStatus() if occur access error.
* [Fix] - It would hardfault, if callback is not NULL in flash sync mode.
* [Fix] - Data left in RxFIFO cannot be read during i2c transmission timeout
* [Fix] - i2c deinit function does not clear the flag register
* [Fix] - The value of clock out divider can not be the value+1
* [Fix] - loading source and opportunity will be changed when calling eTMR_DRV_SetChnOutMask API
* [Fix] - Remove loading source and opportunity configuration in eTMR_DRV_SetCounterInit API
* [Fix] - It will generate assert error when calling eTMR_DRV_QuadDecodeStop API
* [Fix] - VAL0 and VAL1 can't sync when using eTMR_DRV_InitOutputCompare after calling eTMR_DRV_DeinitOutputCompare API
* [Fix] - Etmr channel n(n>0) interrupt can not be set correctly
* [Fix] - Assert will be triggered when to capture just one channel
* [Fix] - When using the GPIO falling edge interrupt, an interrupt will be mistakenly responded to.
* [Fix] - Dead time can not be configured correctly when configuring multiple channels
* [Fix] - Counter initial and maximal value is not appropriate when configuring output compare or timer mode
* [Fix] - Odd channel io status is always 0 in complementary mode
* [Fix] - ACMP channel output status can not be obtained in continuous mode
* [Fix] - Warning reported when compiling linflexd file
* [Fix] - Continuous enable will not configured correctly in the ACMP_DRV_Init function
* [Fix] - In i2c slave mode, the restart interrupt event and the address match interrupt event may have an execution sequence error.
* [Fix] - When emu supports 64-bit injection, the EMU_SetErrInjectData function fails to inject a single bit and injects multiple bits.
* [Fix] - Wdg clock source error on file YTM32B1HA0_features.h .
* [Fix] - HA0 Secure Boot default configuration error
* [Fix] - FMU enumerated type mixed with another type which lead to iar compile warning.
* [Fix] - Mix type for s_sentFastDMASrc and s_sentSlowDMASrc which will lead to iar compile warning.
* [Fix] - Add fast bus clock for HA0 ipc clock source
* [New] - Add period calculation in capture period event
* [New] - Add new API MPWM_DRV_EnableDma() to enable channel dma feature
* [New] - Use customerized section to place ram function other than __ramfunc
* [New] - Add Emu_DoubleBit_Demo.
* [New] - Remove EMU_DRV_GetErrReportDataBit() API.

---

## YTM32B1ME0

### 1_3_1 2024-07-18
* [Opt] - [SDK_GUI] Freertos and secure boot demo are generated through YCT.
* [Opt] - Add FXOSC gain scan when enabling FXOSC or waking up from low power mode.
* [Opt] - Display WDG, EWDG, EMU, pTMR instance index for ipc clock name and irq name.
* [Opt] - Put DMA_DRV_ClearIntStatus api in front of callback in the DMA_DRV_IRQHandler function.
* [Opt] - Remove Timeout enable and disable operation in linflexd uart isr handler, and set to configurable in the linflexd yct demo by user.
* [Opt] - Dummy wait to ensure transfer complete signal is 0 for linflexd module.
* [Opt] - Add ipc module reset api for module reset function.
* [Fix] - RTOS would cause LPACK reset after wakeup.
* [Fix] - The pcs continuous function of spi is only valid for the first transmission.
* [Fix] - I2C timeout flag is set abnormally.
* [Fix] - I2C bus busy condition process in the I2C_DRV_MasterInit api.
* [Fix] - ECC error when viewing register value of flexcan in the debug mode.
* [Fix] - DEV_ASSERT error in can pal layer when using enhanced FIFO.
* [Fix] - The input size of flash api check issue.
* [Fix] - [SDK_GUI] The sequence of pins which have been configured will change when modifying the feature of pinout.
* [Fix] - [SDK_GUI] There's no value in parameter pullConfig of pinmux.c when just enabling pull enable.
* [Fix] - [SDK_GUI] Interrupt vector table update.
* [Fix] - [SDK_GUI] Data section is not initialzied completely when power on reset.
* [Fix] - [SDK_GUI] Configuration issue of driveSelect in pinout configuration.
* [Fix] - [SDK-GUI] DMA callback function is not declared externally when generating dma configuration.
* [Fix] - [SDK_GUI] WKU channel configuration issue.
* [Fix] - [SDK-GUI] FlexCAN payload size label is incorrect.
* [Fix] - [SDK_GUI] eTMR measurement configuration in input capture mode is missing.
* [Fix] - [SDK_GUI] ADC trigger type does not support hardware trigger.
* [Fix] - [SDK_GUI] The number of ADC channel is incorrect in 64pin configuration.
* [Fix] - [SDK_GUI] INTM can not load IRQn of chip in the GUI.
* [Fix] - [SDK_GUI] INTM module initial api parameter issue.
* [New] - [SDK_GUI] Add api highlight for pTMR_DRV_InitChannel api.
* [New] - [YCT_Demo] Add flash disable debugger and flash ecc demos.
* [New] - Add eTMR_DRV_SetCounterMid and eTMR_DRV_GetMid api.
* [New] - Add EMU_DRV_GetChannelSBInterruptFlag api.
* [New] - Add EWDG_DRV_DisableInterrupt api.

### 1_3_0 2024-04-19
* [Fix] - [FLEXCAN] Add enhanceRxFifoState.state init when init flexcan.
* [Fix] - I2C won't detect line status when init function calls.
* [Fix] - I2C won't process SDA SCL low timeout event, user may need this callback for bus error process.
* [Fix] - LINFlexD deinit function disable IRQ flow error.

### 1_2_1 2024-03-28
* [Opt] - [FEE] Loosen the restriction that pflash cannot use fee
* [Fix] - [FEE] Fix sync mode donot launch command in RAM.
* [Fix] - Fix ME0 tmr driver to solve IAR warning
* [Fix] - [DMA] It issues BTCE error, if call DMA_CTSClearReg() when dma request enable.
* [Fix] - YCT Demos YT_LINK issue and the size of generated binary file is overlarge.

### 1_2_0 2024-03-12
* [Opt] - Enable/Disable EFM ECC Error interrupt should not change NVIC configuration
* [Opt] - Add FEATURE_EFM_OPERATION_CLOCK_SOURCE for YTM32B1Mx.
* [Opt] - Disable or enable each arithmetic by the HCU features of each device
* [Opt] - DMA_REQ_Disable change to DMA_REQ_DISABLED
* [Opt] - Optimize ACMP YCT demos
* [Opt] - Add a step to clear injection error for Emu_Demo.
* [Opt] - Wdg_Demo is always reset, added abort condition.
* [Opt] - MPU configuration would be overlapped, when boot uses SDK and app uses MCAL.
* [Fix] - Add global interrupt disable in the FLEXCAN_SetMsgBuffIntCmd() function
* [Fix] - The event type of error_callback in the FlexCANState should be flexcan_error_event_type_t 
* [Fix] - [TRNG] Cannot generate random data after TRNG_DRV_DeInit() and reinit.
* [Fix] - The return value of CRC_GetProtocolWidth function is not correct
* [Fix] - [Flash] Boot swap alway fail if fail once.
* [Fix] - Lin will stop transfer when bit error occurs.
* [Fix] - PWM channel pair set error when configuring odd channel invert in complementary mode
* [Fix] - CR[DSDIS] and CR[DBGDIS] bit logic error in the WDG driver.
* [Fix] - The value of typeOfUpdate is wrong when using eTMR_DRV_UpdatePwmPeriodAndDuty function
* [Fix] - VAL0 and VAL1 can't sync when using eTMR_DRV_InitPwm after calling eTMR_DRV_DeinitPwm function
* [Fix] - Flash async mode cannot get idle by FLASH_DRV_GetBusyStatus() if occur access error.
* [Fix] - It would hardfault, if callback is not NULL in flash sync mode.
* [Fix] - Data left in RxFIFO cannot be read during i2c transmission timeout
* [Fix] - i2c deinit function does not clear the flag register
* [Fix] - The value of clock out divider can not be the value+1
* [Fix] - loading source and opportunity will be changed when calling eTMR_DRV_SetChnOutMask API
* [Fix] - Remove loading source and opportunity configuration in eTMR_DRV_SetCounterInit API
* [Fix] - It will generate assert error when calling eTMR_DRV_QuadDecodeStop API
* [Fix] - VAL0 and VAL1 can't sync when using eTMR_DRV_InitOutputCompare after calling eTMR_DRV_DeinitOutputCompare API
* [Fix] - Etmr channel n(n>0) interrupt can not be set correctly
* [Fix] - Assert will be triggered when to capture just one channel
* [Fix] - When using the GPIO falling edge interrupt, an interrupt will be mistakenly responded to.
* [Fix] - Dead time can not be configured correctly when configuring multiple channels
* [Fix] - Counter initial and maximal value is not appropriate when configuring output compare or timer mode
* [Fix] - Odd channel io status is always 0 in complementary mode
* [Fix] - ACMP channel output status can not be obtained in continuous mode
* [Fix] - Warning reported when compiling linflexd file
* [Fix] - Continuous enable will not configured correctly in the ACMP_DRV_Init function
* [Fix] - In i2c slave mode, the restart interrupt event and the address match interrupt event may have an execution sequence error.
* [Fix] - [FLASH] Add nop command for FLASH_LaunchCommandSequence.
* [New] - Add EWDG_DeInit API
* [New] - Add period calculation in capture period event
* [New] - Add Emu_DoubleBit_Demo.
* [New] - Use customerized section to place ram function other than __ramfunc
* [New] - Remove EMU_DRV_GetErrReportDataBit() API.

---

## YTM32B1MD1

### 1_3_1 2024-07-18
* [Opt] - [SDK_GUI] Freertos and secure boot demo are generated through YCT.
* [Opt] - Add FXOSC gain scan when enabling FXOSC or waking up from low power mode.
* [Opt] - Display WDG, EWDG, EMU, pTMR instance index for ipc clock name and irq name.
* [Opt] - Put DMA_DRV_ClearIntStatus api in front of callback in the DMA_DRV_IRQHandler function.
* [Opt] - Remove Timeout enable and disable operation in linflexd uart isr handler, and set to configurable in the linflexd yct demo by user.
* [Opt] - Dummy wait to ensure transfer complete signal is 0 for linflexd module.
* [Opt] - Add ipc module reset api for module reset function.
* [Fix] - RTOS would cause LPACK reset after wakeup.
* [Fix] - The pcs continuous function of spi is only valid for the first transmission.
* [Fix] - I2C timeout flag is set abnormally.
* [Fix] - I2C bus busy condition process in the I2C_DRV_MasterInit api.
* [Fix] - ECC error when viewing register value of flexcan in the debug mode.
* [Fix] - DEV_ASSERT error in can pal layer when using enhanced FIFO.
* [Fix] - The input size of flash api check issue.
* [Fix] - [YCT_Demo] EWDG demo can not enter into isr handler.
* [Fix] - [SDK_GUI] The sequence of pins which have been configured will change when modifying the feature of pinout.
* [Fix] - [SDK_GUI] There's no value in parameter pullConfig of pinmux.c when just enabling pull enable.
* [Fix] - [SDK_GUI] Interrupt vector table update.
* [Fix] - [SDK_GUI] Data section is not initialzied completely when power on reset.
* [Fix] - [SDK_GUI] Configuration issue of driveSelect in pinout configuration.
* [Fix] - [SDK-GUI] DMA callback function is not declared externally when generating dma configuration.
* [Fix] - [SDK_GUI] WKU channel configuration issue.
* [Fix] - [SDK-GUI] FlexCAN payload size label is incorrect.
* [Fix] - [SDK_GUI] eTMR measurement configuration in input capture mode is missing.
* [Fix] - [SDK_GUI] ADC trigger type does not support hardware trigger.
* [New] - [SDK_GUI] Add pulse check in sent configuation ui.
* [New] - [SDK_GUI] Add api highlight for pTMR_DRV_InitChannel api.
* [New] - [YCT_Demo] Add flash disable debugger and flash ecc demos.
* [New] - Add eTMR_DRV_SetCounterMid and eTMR_DRV_GetMid api.
* [New] - Add EMU_DRV_GetChannelSBInterruptFlag api.
* [New] - Add EWDG_DRV_DisableInterrupt api.

### 1_3_0 2024-04-19
* [Fix] - [FLEXCAN] Add enhanceRxFifoState.state init when init flexcan.
* [Fix] - I2C won't detect line status when init function calls.
* [Fix] - I2C won't process SDA SCL low timeout event, user may need this callback for bus error process.
* [Fix] - LINFlexD deinit function disable IRQ flow error.

### 1_2_1 2024-03-28
* [Opt] - [FEE] Loosen the restriction that pflash cannot use fee
* [Fix] - [FEE] Fix sync mode donot launch command in RAM.
* [Fix] - [DMA] It issues BTCE error, if call DMA_CTSClearReg() when dma request enable.
* [Fix] - YCT Demos YT_LINK issue and the size of generated binary file is overlarge.

### 1_2_0 2024-03-12
* [Opt] - Enable/Disable EFM ECC Error interrupt should not change NVIC configuration
* [Opt] - Add FEATURE_EFM_OPERATION_CLOCK_SOURCE for YTM32B1Mx.
* [Opt] - Disable or enable each arithmetic by the HCU features of each device
* [Opt] - DMA_REQ_Disable change to DMA_REQ_DISABLED
* [Opt] - Optimize ACMP YCT demos
* [Opt] - Add a step to clear injection error for Emu_Demo.
* [Opt] - Update all spi demos for new YT link.
* [Opt] - MPU configuration would be overlapped, when boot uses SDK and app uses MCAL.
* [Fix] - Add global interrupt disable in the FLEXCAN_SetMsgBuffIntCmd() function
* [Fix] - The event type of error_callback in the FlexCANState should be flexcan_error_event_type_t 
* [Fix] - [TRNG] Cannot generate random data after TRNG_DRV_DeInit() and reinit.
* [Fix] - The return value of CRC_GetProtocolWidth function is not correct
* [Fix] - [Flash] Boot swap alway fail if fail once.
* [Fix] - Lin will stop transfer when bit error occurs.
* [Fix] - PWM channel pair set error when configuring odd channel invert in complementary mode
* [Fix] - CR[DSDIS] and CR[DBGDIS] bit logic error in the WDG driver.
* [Fix] - The value of typeOfUpdate is wrong when using eTMR_DRV_UpdatePwmPeriodAndDuty function
* [Fix] - VAL0 and VAL1 can't sync when using eTMR_DRV_InitPwm after calling eTMR_DRV_DeinitPwm function
* [Fix] - Flash async mode cannot get idle by FLASH_DRV_GetBusyStatus() if occur access error.
* [Fix] - It would hardfault, if callback is not NULL in flash sync mode.
* [Fix] - Data left in RxFIFO cannot be read during i2c transmission timeout
* [Fix] - i2c deinit function does not clear the flag register
* [Fix] - The value of clock out divider can not be the value+1
* [Fix] - loading source and opportunity will be changed when calling eTMR_DRV_SetChnOutMask API
* [Fix] - Remove loading source and opportunity configuration in eTMR_DRV_SetCounterInit API
* [Fix] - It will generate assert error when calling eTMR_DRV_QuadDecodeStop API
* [Fix] - VAL0 and VAL1 can't sync when using eTMR_DRV_InitOutputCompare after calling eTMR_DRV_DeinitOutputCompare API
* [Fix] - Etmr channel n(n>0) interrupt can not be set correctly
* [Fix] - Assert will be triggered when to capture just one channel
* [Fix] - When using the GPIO falling edge interrupt, an interrupt will be mistakenly responded to.
* [Fix] - Dead time can not be configured correctly when configuring multiple channels
* [Fix] - Counter initial and maximal value is not appropriate when configuring output compare or timer mode
* [Fix] - Odd channel io status is always 0 in complementary mode
* [Fix] - ACMP channel output status can not be obtained in continuous mode
* [Fix] - Warning reported when compiling linflexd file
* [Fix] - Modify rtc clock out name from RTC_CLKOUT_SRC_32KHZ to RTC_CLKOUT_SRC_CLKSEL
* [Fix] - Continuous enable will not configured correctly in the ACMP_DRV_Init function
* [Fix] - In i2c slave mode, the restart interrupt event and the address match interrupt event may have an execution sequence error.
* [Fix] - [FLASH] Add nop command for FLASH_LaunchCommandSequence.
* [New] - Remove EMU_DRV_GetErrReportDataBit() API.
* [New] - Add EWDG_DeInit API
* [New] - User can disable ECC or disable debugger by configuring CTRL_OVRD register
* [New] - Add period calculation in capture period event
* [New] - Add Emu_DoubleBit_Demo.
* [New] - Add Wdg_Demo.
* [New] - Use customerized section to place ram function other than __ramfunc

---

## YTM32B1MC0

### 1_3_1 2024-07-18
* [Opt] - [SDK_GUI] Freertos and secure boot demo are generated through YCT.
* [Opt] - Add FXOSC gain scan when enabling FXOSC or waking up from low power mode.
* [Opt] - Display WDG, EWDG, EMU, pTMR instance index for ipc clock name and irq name.
* [Opt] - Put DMA_DRV_ClearIntStatus api in front of callback in the DMA_DRV_IRQHandler function.
* [Opt] - Remove Timeout enable and disable operation in linflexd uart isr handler, and set to configurable in the linflexd yct demo by user.
* [Opt] - Dummy wait to ensure transfer complete signal is 0 for linflexd module.
* [Opt] - Add ipc module reset api for module reset function.
* [Fix] - RTOS would cause LPACK reset after wakeup.
* [Fix] - The pcs continuous function of spi is only valid for the first transmission.
* [Fix] - I2C timeout flag is set abnormally.
* [Fix] - I2C bus busy condition process in the I2C_DRV_MasterInit api.
* [Fix] - UART Tx/Rx DMA enablement can not be clear.
* [Fix] - ECC error when viewing register value of flexcan in the debug mode.
* [Fix] - DEV_ASSERT error in can pal layer when using enhanced FIFO.
* [Fix] - The input size of flash api check issue.
* [Fix] - MPWM disable counter api can not disable the counter of mpwm.
* [Fix] - The variable type of monitorIrqn in the intm_config_t will cause compile warning when using iar project.
* [Fix] - [SDK_GUI] The sequence of pins which have been configured will change when modifying the feature of pinout.
* [Fix] - [SDK_GUI] There's no value in parameter pullConfig of pinmux.c when just enabling pull enable.
* [Fix] - [SDK_GUI] Interrupt vector table update.
* [Fix] - [SDK_GUI] Data section is not initialzied completely when power on reset.
* [Fix] - [SDK_GUI] Configuration issue of driveSelect in pinout configuration.
* [Fix] - [SDK-GUI] DMA callback function is not declared externally when generating dma configuration.
* [Fix] - [SDK-GUI] FlexCAN payload size label is incorrect.
* [Fix] - [SDK_GUI] eTMR measurement configuration in input capture mode is missing.
* [Fix] - [SDK_GUI] ADC trigger type does not support hardware trigger.
* [New] - [SDK_GUI] Add api highlight for pTMR_DRV_InitChannel api.
* [New] - [SDK_GUI] Add mpwm module configuration ui.
* [New] - [YCT_Demo] Add flash disable debugger and flash ecc demos.
* [New] - [YCT_Demo] Add mpwm pwm demo.
* [New] - Open debug disable bit of CTRL_OVRD register.
* [New] - Add EMU_DRV_GetChannelSBInterruptFlag api.
* [New] - Add EWDG_DRV_DisableInterrupt api.

### 1_3_0 2024-04-19
* [Fix] - [FLEXCAN] Add enhanceRxFifoState.state init when init flexcan.
* [Fix] - I2C won't detect line status when init function calls.
* [Fix] - I2C won't process SDA SCL low timeout event, user may need this callback for bus error process.

### 1_2_1 2024-03-28
* [Opt] - [FEE] Loosen the restriction that pflash cannot use fee
* [Fix] - [FEE] Fix sync mode donot launch command in RAM.
* [Fix] - [DMA] It issues BTCE error, if call DMA_CTSClearReg() when dma request enable.
* [Fix] - YCT Demos YT_LINK issue and the size of generated binary file is overlarge.

### 1_2_0 2024-03-12
* [Opt] - DMA_REQ_Disable change to DMA_REQ_DISABLED
* [Opt] - Uart adds fifo function, one wire mode function and rts cts function
* [Opt] - Add API for customers to modify rx fifo watermark
* [Opt] - In Ewdg_Demo, replace the disabled interrupt with the deinit function.
* [Opt] - Optimize lin demo for MC0
* [Opt] - Add uart continuous transmission demo for MC0
* [Opt] - Optimize YTC uart demo for MC0
* [Opt] - Optimize YTC i2c demo for MC0
* [Opt] - Add a button to toggle led for Gpio_Interrupt_Demo.
* [Opt] - Optimize Etmr_Pwm_Demo
* [Opt] - Optimize Etmr_Pwm_Complementary_Demo
* [Opt] - Optimize Etmr_Input_Capture_Demo
* [Opt] - Add a step to clear injection error for Emu_Demo.
* [Opt] - [YCT_Demo] Add recovery method in CMU demo.
* [Opt] - [YCT_Demo] Add async flash demo.
* [Opt] - [YCT_Demo] Simplify Fee demo
* [Opt] - [YCT_Demo] Add lowest power demo for YTM32B1MC0 and YTM32B1LE0.
* [Opt] - Change the rx fifo watermark of uart to 0
* [Opt] - Optimize ACMP YCT demos
* [Opt] - In Lin mode, the user layer's modification of txbuff will cause a readback error event generated by the driver layer exception.
* [Opt] - [FLASH] Add erase quick for YTM32B1MC0.
* [Fix] - The return value of CRC_GetProtocolWidth function is not correct
* [Fix] - UART compilation generates warnings
* [Fix] - MPWM Set LDOK task error
* [Fix] - MPWM will set PWM to error frequency if frequency too low.
* [Fix] - PWM channel pair set error when configuring odd channel invert in complementary mode
* [Fix] - CR[DSDIS] and CR[DBGDIS] bit logic error in the WDG driver.
* [Fix] - Readme file need to update.
* [Fix] - The value of typeOfUpdate is wrong when using eTMR_DRV_UpdatePwmPeriodAndDuty function
* [Fix] - VAL0 and VAL1 can't sync when using eTMR_DRV_InitPwm after calling eTMR_DRV_DeinitPwm function
* [Fix] - Flash async mode cannot get idle by FLASH_DRV_GetBusyStatus() if occur access error.
* [Fix] - It would hardfault, if callback is not NULL in flash sync mode.
* [Fix] - Data left in RxFIFO cannot be read during i2c transmission timeout
* [Fix] - i2c deinit function does not clear the flag register
* [Fix] - The value of clock out divider can not be the value+1
* [Fix] - loading source and opportunity will be changed when calling eTMR_DRV_SetChnOutMask API
* [Fix] - Remove eTMR_DRV_SetCounterInit API
* [Fix] - eTMR_IPC_CLOCK is wrong, it leads the failure of supporting ipc clock for etmr clock source
* [Fix] - Add measurementPeriod state element for YTM32B1MC0 input capture
* [Fix] - It can't get channel flag for etmr1 in the input capture handler
* [Fix] - Lack of callback function handling
* [Fix] - It will generate assert error when calling eTMR_DRV_QuadDecodeStop API
* [Fix] - VAL0 and VAL1 can't sync when using eTMR_DRV_InitOutputCompare after calling eTMR_DRV_DeinitOutputCompare API
* [Fix] - heap and stack size is a bit large in the YTM32B1MC0 gcc link file
* [Fix] - lpTMR init parameter error in Wdg_Demo，so it starts timing when it initializes.
* [Fix] - When using the GPIO falling edge interrupt, an interrupt will be mistakenly responded to.
* [Fix] - Counter initial and maximal value is not appropriate when configuring output compare or timer mode
* [Fix] - Odd channel io status is always 0 in complementary mode
* [Fix] - ACMP channel output status can not be obtained in continuous mode
* [Fix] - ACMP output flag is not correct in non-continuous mode
* [Fix] - In i2c slave mode, the restart interrupt event and the address match interrupt event may have an execution sequence error.
* [New] - Remove EMU_DRV_GetErrReportDataBit() API.
* [New] - Add EWDG_DeInit API
* [New] - Add Emu_DoubleBit_Demo
* [New] - The hardfault interrupt function declaration (naked) of Emu_DoubleBit_Demo reports an error in keil.
* [New] - Add new API MPWM_DRV_EnableDma() to enable channel dma feature
* [New] - Use customerized section to place ram function other than __ramfunc

---

## YTM32B1LE0

### 1_3_1 2024-07-18
* [Opt] - [SDK_GUI] Freertos and secure boot demo are generated through YCT.
* [Opt] - Add FXOSC gain scan when enabling FXOSC or waking up from low power mode.
* [Opt] - Display WDG, EMU, pTMR instance index for ipc clock name and irq name.
* [Opt] - Put DMA_DRV_ClearIntStatus() api in front of callback in the DMA_DRV_IRQHandler() function.
* [Opt] - Dummy wait to ensure transfer complete signal is 0 for uart and lin.
* [Opt] - Add ipc module reset api, update clock initialize flow.
* [Opt] - Clear the count of capture when measurement complete.
* [Opt] - [YCT_Demo] Accomplish multiple continuous transfer in the Etmr_Input_Capture_Dma_Demo.
* [Fix] - RTOS would cause LPACK reset after wakeup.
* [Fix] - The pcs continuous function of spi is only valid for the first transmission.
* [Fix] - I2C timeout flag is set abnormally.
* [Fix] - I2C bus busy condition process in the I2C_DRV_MasterInit api.
* [Fix] - UART Tx/Rx DMA enablement can not be clear.
* [Fix] - ECC error when viewing register value of flexcan in the debug mode.
* [Fix] - DEV_ASSERT error in can pal layer when using enhanced FIFO.
* [Fix] - [SDK_GUI] The sequence of pins which have been configured will change when modifying the feature of pinout.
* [Fix] - [SDK_GUI] There's no value in parameter pullConfig of pinmux.c when just enabling pull enable.
* [Fix] - [SDK_GUI] Interrupt vector table update.
* [Fix] - [SDK_GUI] Data section is not initialzied completely when power on reset.
* [Fix] - [SDK_GUI] Configuration issue of driveSelect in pinout configuration.
* [Fix] - [SDK-GUI] DMA callback function is not declared externally when generating dma configuration.
* [Fix] - [SDK-GUI] FlexCAN payload size label is incorrect.
* [Fix] - [SDK_GUI] eTMR measurement configuration in input capture mode is missing.
* [Fix] - [SDK_GUI] eTMR overflow and fault handler name both mismatch.
* [Fix] - [SDK_GUI] ADC trigger type does not support hardware trigger.
* [New] - [SDK_GUI] Add FEE module.
* [New] - [SDK_GUI] Add api highlight for pTMR_DRV_InitChannel api.
* [New] - [YCT_Demo] Add flash disable debugger and flash ecc demos.
* [New] - [YCT_Demo] Add etmr input capture demo without dma.
* [New] - Open debug disable bit of CTRL_OVRD register.
* [New] - Add EMU_DRV_GetChannelSBInterruptFlag api.

### 1_3_0 2024-04-19
* [Fix] - [FLEXCAN] Add enhanceRxFifoState.state init when init flexcan.
* [Fix] - I2C won't detect line status when init function calls.
* [Fix] - I2C won't process SDA SCL low timeout event, user may need this callback for bus error process.

### 1_2_1 2024-03-28
* [Opt] - [FEE] Loosen the restriction that pflash cannot use fee
* [Fix] - [DMA] It issues BTCE error, if call DMA_CTSClearReg() when dma request enable.
* [Fix] - YCT Demos YT_LINK issue and the size of generated binary file is overlarge.

### 1_2_0 2024-03-12
* [Opt] - Optimize lin demo for LE0
* [Opt] - Optimize uart demo for LE0
* [Opt] - Optimize i2c demo for LE0
* [Opt] - [YCT_Demo] Add recovery method in CMU demo.
* [Opt] - [YCT_Demo] Add lowest power demo for YTM32B1MC0 and YTM32B1LE0.
* [Opt] - Change the rx fifo watermark of uart to 0
* [Opt] - Added steps to clear injection errors for Emu_Demo.
* [Opt] - Delete unnecessary comments for Wdg_Demo.
* [Opt] - Add a button to toggle led for Gpio_Interrupt_Demo.
* [Opt] - Optimize ACMP YCT demos
* [Opt] - In Lin mode, the user layer's modification of txbuff will cause a readback error event generated by the driver layer exception.
* [Fix] - UART compilation generates warnings
* [Fix] - PWM channel pair set error when configuring odd channel invert in complementary mode
* [Fix] - CR[DSDIS] and CR[DBGDIS] bit logic error in the WDG driver.
* [Fix] - Readme file need to update.
* [Fix] - The value of typeOfUpdate is wrong when using eTMR_DRV_UpdatePwmPeriodAndDuty function
* [Fix] - VAL0 and VAL1 can't sync when using eTMR_DRV_InitPwm after calling eTMR_DRV_DeinitPwm function
* [Fix] - Flash async mode cannot get idle by FLASH_DRV_GetBusyStatus() if occur access error.
* [Fix] - It would hardfault, if callback is not NULL in flash sync mode.
* [Fix] - Data left in RxFIFO cannot be read during i2c transmission timeout
* [Fix] - i2c deinit function does not clear the flag register
* [Fix] - The value of clock out divider can not be the value+1
* [Fix] - when channel number of different instance is not same, writing register will generate error
* [Fix] - loading source and opportunity will be changed when calling eTMR_DRV_SetChnOutMask API
* [Fix] - Remove eTMR_DRV_SetCounterInit API
* [Fix] - It will generate assert error when calling eTMR_DRV_QuadDecodeStop API
* [Fix] - VAL0 and VAL1 can't sync when using eTMR_DRV_InitOutputCompare after calling eTMR_DRV_DeinitOutputCompare API
* [Fix] - Etmr channel n(n>0) interrupt can not be set correctly
* [Fix] - When using the GPIO falling edge interrupt, an interrupt will be mistakenly responded to.
* [Fix] - Counter initial and maximal value is not appropriate when configuring output compare or timer mode
* [Fix] - Odd channel io status is always 0 in complementary mode
* [Fix] - eTMR instance 2 will be traversed from 0 to 7 for YTM32B1LE0
* [Fix] - ACMP channel output status can not be obtained in continuous mode
* [Fix] - In i2c slave mode, the restart interrupt event and the address match interrupt event may have an execution sequence error.
* [New] - Add API for customers to modify rx fifo watermark
* [New] - Remove EMU_DRV_GetErrReportDataBit() API.
* [New] - Add uart continuous transmission demo for LE0
* [New] - Improve input capture feature for YTM32B1LE0
* [New] - Add Emu_DoubleBit_Demo.
* [New] - FlexCAN: add safety library
* [New] - Modify link and startup file to support SafLib.
* [New] - Use customerized section to place ram function other than __ramfunc
