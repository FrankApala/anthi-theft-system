/*
 * rtc.h
 *
 *  Created on: Mar 21, 2024
 *      Author: tonts
 */
#include<stdint.h>
#include "usart2.h"




#define RTC_BASE_ADDRESS 0x400028000
#define RTC_TR  (*(volatile uint32_t *)(RTC_BASE_ADDRESS + 0x00))


void RTC_Init(void);
char * RTC_get_time(void);
char * RTC_get_date(void);
void RTC_set_date(uint8_t jour, uint8_t mois, uint16_t annee);
void RTC_set_time(uint8_t heures, uint8_t minutes, uint8_t secondes);
void process_received_data(void) ;
