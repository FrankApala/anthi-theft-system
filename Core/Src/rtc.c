/*
 * rtc.c
 *
 *  Created on: Mar 21, 2024
 *      Author: tonts
 */

#include "rtc.h"
#include "stm32f407xx.h"
#include<stdio.h>
#include "usart2.h"

extern char received_buffer[24];
extern int received_index;



typedef struct{
	uint8_t Heures;
	uint8_t Minutes;
	uint8_t Secondes;
}RTC_TimeTypeDef;

typedef struct{
	uint8_t Month;
	uint8_t Day;
	uint8_t Year;
}RTC_DateTypeDef;

RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;



void RTC_Init(void){
    RCC->APB1ENR |= RCC_APB1ENR_PWREN; // Activer l'horloge de l'interface d'alimentation

    PWR->CR |= PWR_CR_DBP;             // Permettre l'accès en écriture aux registres de la RTC

    RCC->CSR |= RCC_CSR_LSION;         // Activer l'oscillateur interne à basse vitesse

    while(!(RCC->CSR & RCC_CSR_LSIRDY)); // Attendre que l'oscillateur soit prêt
    RCC->BDCR |= 0x8200;               // Sélectionner la source de l'horloge et activer l'accès à la RTC

    // Désactiver la protection d'écriture
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;

    RTC->ISR |= RTC_ISR_INIT;          // Entrer en mode d'initialisation
    while(!(RTC->ISR & RTC_ISR_INITF)); // Attendre que la RTC soit prête pour être configurée

    // Configurer les prédiviseurs
    RTC->PRER = 0xF9;                  // Prédiviseur synchrone
    RTC->PRER |= 0x7F << 16;           // Prédiviseur asynchrone

    // Définir l'heure et la date
    // Les valeurs doivent être encodées en BCD. Pour les secondes à 55, le BCD est 0x55.
    // Assurez-vous que l'heure (13h) et les minutes (57) sont également en BCD.
    // Ici, l'exemple définit l'heure à 13:57:55.
    RTC->TR = (0x13 << 16) | (0x57 << 8) | 0x55; // Format BCD pour HH:MM:SS

    RTC->DR = 0x215124; // Définir la date - cette valeur doit également être vérifiée pour le format BCD correct

    RTC->CR |= RTC_CR_BYPSHAD;         // Activer le mode de contournement de l'ombre
    RTC->ISR &= ~RTC_ISR_INIT;         // Sortir du mode d'initialisation
    PWR->CR &= ~PWR_CR_DBP;            // Bloquer l'accès en écriture
}


/*char * RTC_get_time(void){
	static char time[10];


	sTime.Secondes=(((RTC->TR & 0x7f) >> 4)*10)+(RTC->TR &0xf);
	sTime.Minutes=((RTC->TR & 0x7f00) >> 8);
	sTime.Minutes=(((sTime.Minutes & 0x7f)>>4)*10)+(sTime.Minutes & 0xf);
	sTime.Heures=((RTC->TR & 0x7f0000) >> 16);
	sTime.Heures=(((sTime.Heures & 0x7f) >>4) *10)+(sTime.Heures & 0xf);

    sprintf(time,"%d:%d:%d\n", sTime.Heures,sTime.Minutes,sTime.Secondes );

	return(time);
}*/

char * RTC_get_time(void){
	static char time[10];

	sTime.Secondes = (((RTC->TR >> 4) & 0x7) * 10) + (RTC->TR & 0xF);
	sTime.Minutes = (((RTC->TR >> 12) & 0x7) * 10) + ((RTC->TR >> 8) & 0xF);
	sTime.Heures = (((RTC->TR >> 20) & 0x3) * 10) + ((RTC->TR >> 16) & 0xF);

    sprintf(time,"%02d:%02d:%02d", sTime.Heures,sTime.Minutes,sTime.Secondes );
 return time;
}


char * RTC_get_date(void){
	static char time[10];

	sDate.Year=((RTC->DR >> 20)*10)+((RTC->DR >>16) &0xf);
	sDate.Month=((RTC->DR >> 12) & 1)* 10 +((RTC->DR >>8) &0xf);
	sDate.Day=((RTC->DR >> 4) & 3)* 10 +(RTC->DR &0xf);

    sprintf(time,"%02d/%02d/%02d", sDate.Day,sDate.Month,sDate.Year );
    return time;

}

void RTC_set_time(uint8_t heures, uint8_t minutes, uint8_t secondes) {

    PWR->CR |= PWR_CR_DBP;// Permettre l'accès en écriture aux registres de la RTC

    RTC->WPR = 0xCA; // Clé magique 1 pour désactiver la protection
    RTC->WPR = 0x53; // Clé magique 2 pour désactiver la protection

    // Entrer en mode d'initialisation
    RTC->ISR |= RTC_ISR_INIT; // Mettre le bit d'initialisation à 1 pour commencer la configuration
    while (!(RTC->ISR & RTC_ISR_INITF)); // Attendre que le mode d'initialisation soit prêt (INITF à 1)

    // Convertir les heures, minutes et secondes en BCD et configurer l'heure
    // Cette opération convertit chaque composante (heures, minutes, secondes) en format BCD
    // et les place dans le registre TR à la position correcte.
    uint32_t time = (uint32_t)(((heures / 10) << 20) | ((heures % 10) << 16) |
                               ((minutes / 10) << 12) | ((minutes % 10) << 8) |
                               ((secondes / 10) << 4) | (secondes % 10));
    RTC->TR = time; // Écrire le temps configuré dans le registre TR

    // Sortir du mode d'initialisation
    RTC->ISR &= ~RTC_ISR_INIT; // Remettre le bit d'initialisation à 0 pour quitter le mode d'initialisation

    // Réactiver la protection en écriture
    RTC->WPR = 0xFF; // Réactiver la protection contre les écritures accidentelles
    PWR->CR &= ~PWR_CR_DBP;            // Bloquer l'accès en écriture

}


void RTC_set_date(uint8_t jour, uint8_t mois, uint16_t annee) {
    PWR->CR |= PWR_CR_DBP;             // Permettre l'accès en écriture aux registres de la RTC

	RTC->WPR = 0xCA; // Clé magique 1 pour désactiver la protection
    RTC->WPR = 0x53; // Clé magique 2 pour désactiver la protection

    // Entrer en mode d'initialisation
    RTC->ISR |= RTC_ISR_INIT; // Mettre le bit d'initialisation à 1 pour commencer la configuration
    __NOP();
    __NOP();
    while (!(RTC->ISR & RTC_ISR_INITF)); // Attendre que le mode d'initialisation soit prêt (INITF à 1)

    // Convertir le jour, le mois et l'année en BCD et configurer la date
    // Cette opération convertit chaque composante (jour, mois, année) en format BCD
    // et les place dans le registre DR à la position correcte.
    uint32_t date = (uint32_t)(((annee / 10) << 20) | ((annee % 10) << 16) |
                               ((mois / 10) << 12) | ((mois % 10) << 8) |
                               ((jour / 10) << 4) | (jour % 10));
    RTC->DR = date; // Écrire la date configurée dans le registre DR

    // Sortir du mode d'initialisation
    RTC->ISR &= ~RTC_ISR_INIT; // Remettre le bit d'initialisation à 0 pour quitter le mode d'initialisation

    // Réactiver la protection en écriture
    RTC->WPR = 0xFF; // Réactiver la protection contre les écritures accidentelles
    PWR->CR &= ~PWR_CR_DBP;            // Bloquer l'accès en écriture

}

void process_received_data(void) {
	char date_str[10]; // YYYY/MM/DD
	char time_str[8];  // HH:MM:SS
	int day, month, year, hour, minute, second;

    // Vérifier si une trame complète est reçue
	newLine();
	serial_puts("synchro") ; //Envoie synchro sur le port série
	newLine();
	while(1)
	{
    if (received_index >= 20 && received_buffer[19] == '<') { // Vérifier si le buffer contient au moins 20 caractères et se termine par '<'
        // Extraction de la date
        sscanf(received_buffer, "%10s", date_str);
        sscanf(date_str + 2, "%2d/%d/%d", &year, &month, &day); //YY/MM/DD format
        RTC_set_date(day, month, year);

        // Extraction de l'heure
        sscanf(received_buffer + 11, "%8s", time_str);
        sscanf(time_str, "%d:%d:%d", &hour, &minute, &second);
        RTC_set_time(hour, minute, second);

        // Réinitialiser le buffer et l'index
        received_index = 0;
        for (int j = 0; j < 24; j++) {
            received_buffer[j] = '\0';
        }
		break;
    }
	}
}




