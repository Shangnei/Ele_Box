#ifndef MAIN_H
#define MAIN_H

#include "sdk_project_config.h"
#include "u8g2.h"
#include "YTM32B1LE0.h"
#include "pins_gpio_hw_access.h"
#include "u8g2_d.h"
#include "Joystick.h"
// #include "Ina226.h"
#include "Autofox_INA226_c.h"

#define SPI_INST         (2)
#define SPI_TRANS_LENGTH (8)
#define PTMR_INST 0

extern double Current_vlue;

#endif