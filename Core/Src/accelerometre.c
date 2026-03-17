/*
 * accelerometre.c
 *
 *  Created on: 25 mars 2024
 *      Author: jginer
 */
 float X =0;
 float Y =0 ;
 float Z =0;

#include "accelerometre.h"

/**
  * @brief  Initializes the LIS3DSH for polling mode and the GPIO for the interrupts
  * @param  None
  * @retval None
  */
void init_accel(void){

	//init_usart();
	init_SPI();


    //On initialise l'accéleromètre
    			//Ecrire valeurs dans CTRL_REG5
     			//default BW, 2g scale, self stale disabled, SPI 4Wires)
     			SPIwrite(CTRL_REG5 & 0x7f, 0x00);

     			//Ecrire valeurs dans CTRL_REG4
     			//400Hz data rate, continuous update, axis enabled
     			SPIwrite(CTRL_REG4 & 0x7f, 0x77);

}



/**
  * @brief  Initialize the State Machine 1 of the LIS3DSH and use the interrupt on Pin PE0
  * @param  None
  * @retval None
  */
void init_accel_ISR(void){

	//INT2
	SPIwrite(CTRL_REG1,0x09);
	//INT2 Enabled
	SPIwrite(CTRL_REG3,0x70);
	SPIwrite(CTRL_REG4,0x67);
	SPIwrite(CTRL_REG5,0x00);
	SPIwrite(THRS1_1,0x55);
	SPIwrite(ST1_1,0x05);
	SPIwrite(ST1_2,0x11);
	SPIwrite(MASK1_A,0xFC);
	SPIwrite(MASK1_B,0xFC);
	SPIwrite(MASK1_A,0xFC);
	SPIwrite(SETT1,0x01);
}
/**
  * @brief  Acquisition XYZ and return on the usart
  * @param  None
  * @retval None
  */


/**
  * @brief  Read the XYZ axis value
  * @param  Pointer on x,y,z value
  * @retval None
  */
void accelRead(float *value_x, float *value_y, float *value_z){
					float sensibilite=0.00001525;
					int Xh,Xl,Yh,Yl,Zh,Zl=0;


				  SPIread(CTRL_REG5 | 0x80); //read CTRL_REG5 2 time before to read the value.
			  	  SPIread(CTRL_REG5 | 0x80);



			  	Xl=SPIread(0xA8);
			  	Xh=SPIread(0xA9);
			  	Yl=SPIread(0xAA);
			  	Yh=SPIread(0xAB);
			  	Zl=SPIread(0xAC);
			  	Zh=SPIread(0xAD);

			  	*value_x=(Xh*256+Xl)*sensibilite;
			  	*value_y=(Yh*256+Yl)*sensibilite;
			  	*value_z=(Zh*256+Zl)*sensibilite;
}

/**
  * @brief  Acquisition XYZ and return on the usart
  * @param  None
  * @retval None
  */
void accelReadnPrint(float *value_x, float *value_y, float *value_z){
					float sensibilite=0.00001525;
					int Xh,Xl,Yh,Yl,Zh,Zl=0;
					char resultatX[20],resultatY[20],resultatZ[20];

				  SPIread(CTRL_REG5 | 0x80); //read CTRL_REG5 2 time before to read the value.
			  	  SPIread(CTRL_REG5 | 0x80);



			  	Xl=SPIread(0xA8);
			  	Xh=SPIread(0xA9);
			  	Yl=SPIread(0xAA);
			  	Yh=SPIread(0xAB);
			  	Zl=SPIread(0xAC);
			  	Zh=SPIread(0xAD);

			  	*value_x=(Xh*256+Xl)*sensibilite;
			  	*value_y=(Yh*256+Yl)*sensibilite;
			  	*value_z=(Zh*256+Zl)*sensibilite;


			  	newLine();newLine();
			  					serial_puts("X: ");
			  					serial_puts(float2string(*value_x,resultatX)); //Transformation flottant vers entier pour usart
			  					serial_puts("  ");
			  					serial_puts("Y: ");
			  					serial_puts(float2string(*value_y,resultatY));
			  					serial_puts("  ");
			  					serial_puts("Z: ");
			  					serial_puts(float2string(*value_z,resultatZ));
}






/**
  * @brief  Movement detection, if thief steal the card
  * @param  Sensibility LIS3DHS
  * @retval Return 1 if  the card is stolen & 0 if everything is OK
  */

int detecVol (int sensi){
	  float X=0,X1=0;
	  float Y=0,Y1=0;
	  float Z=0,Z1=0;

	  	  	  	  SPIread(CTRL_REG5 | 0x80); //read CTRL_REG5 2 time before to read the value.
	 		  	  SPIread(CTRL_REG5 | 0x80);

	 		  	    	//Acquisition
	 		  	  X=SPIread(OUT_X_H | 0x80);
	 		  	  Y=SPIread(OUT_Y_H | 0x80);
	 		  	  Z=SPIread(OUT_Z_H | 0x80);



	 		  	 SPIread(CTRL_REG5 | 0x80); //read CTRL_REG5 2 time before to read the value.
	 		  	 SPIread(CTRL_REG5 | 0x80);

	 		  			  	    	//Acquisition
	 		  	 X1=SPIread(OUT_X_H | 0x80);
	 		  	 Y1=SPIread(OUT_Y_H | 0x80);
	 		  	 Z1=SPIread(OUT_Z_H | 0x80);

	if (X+sensi< X1 || X-sensi > X1 || Y+sensi< Y1 || Z-sensi > Z1 ){	//Comparaison à la valeur des 3 axes précédentes
		return 1; //carte volée							Avec une marge de +-2; changer la valeur de 2 permet de changer la sensibilité
		}
	else {
		return 0;

	}



}

void debug_accel(void){
	int registre_lis[200]={0};
	for (int i=0x0C;i<0x7F;i++){
		registre_lis[i]=SPIread(i|0x80);

	}

}



/* Fonction à mettre dans le main
 * void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_0) {
    flag=1;

  } else {
      __NOP();
  }
}
 *
 */

/* A mettre dans le while
 * 	  if(flag==1){	//La carte est volée
		  SPIread(0x5F| 0x80); //Réarmement ISR sur LIS3DSH
		  flag=0;					  //Remise à 0 du flag pour la prochaine interruption
		  init_accel_ISR();		      //Réinitialisation de la state machine
	  }
 */
