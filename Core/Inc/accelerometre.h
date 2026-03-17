/*
 * accelerometre.h
 *
 *  Created on: 25 mars 2024
 *      Author: jginer
 */


#include "stm32f4xx_hal.h"
#include "spi.h"
#include "usart2.h"

//defines the adress of the register
#define CTRL_REG5 	 0x24
#define CTRL_REG5 	 0x24
#define CTRL_REG1 	 0x21
#define CTRL_REG3	 0x23
#define CTRL_REG4	 0x20
#define THRS2_1		 0x56
#define THRS1_1		 0x57
#define ST1_1		 0x40
#define ST1_2		 0x41
#define MASK1_B		 0x59
#define MASK1_A		 0x5A
#define SETT1		 0x5B
#define OUT_X_H		 0x29
#define OUT_Y_H		 0x2B
#define OUT_Z_H		 0x2D


extern float X ;
extern float Y  ;
extern float Z ;

//prototypes
int detecVol (int sensi);
void init_accel(void);
void init_accel_ISR(void);
void accelRead(float *value_x, float *value_y, float *value_z);
void accelReadnPrint(float *value_x, float *value_y, float *value_z);
void debug_accel(void);



