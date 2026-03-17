#include "dac.h"

uint8_t I2C_buffer[2];					//I2C data buffer array
uint16_t I2S_buffer[1];					//I2S data buffer array

volatile uint8_t tonality = 0;
volatile uint8_t timer_interrupt_occurred = 0;
int counter = 0; //only for testing
uint8_t beep_num = MEDIUM_SOUND; //base frequency

//GPIO init
void DAC_GPIO_Init(void)
{
  /* GPIO Ports Clock Enable */
  RCC->AHB1ENR |= 0x8F;

  /*Configure GPIO pin : RESET_DAC_Pin */
  GPIOD->MODER |= 0x100;
  GPIOD->OTYPER &= ~(1<<4);
  GPIOD->PUPDR &= ~(3<<8);
  GPIOD->OSPEEDR &= ~(3<<8);
}

//DAC start
void DAC_Power_Up(){

	GPIOD->BSRR = 0x00000010;				//Pin PD4 high (reset pin of the DAC)

	I2C_buffer[0] = POWER_CTRL_2;
	I2C_buffer[1] = 0xA0;					//Turning headphone channel ON
	I2C_Master_Transmit(DAC_ADDR_W, I2C_buffer, 2);

	I2C_buffer[0] = ITRF_CTRL_1;
	I2C_buffer[1] = 0x87;					//Enable I2S clocking control, Master only
	I2C_Master_Transmit(DAC_ADDR_W, I2C_buffer, 2);

	//dac init with the datasheet
	I2C_buffer[0] = 0x00;
	I2C_buffer[1] = 0x99;
	I2C_Master_Transmit(DAC_ADDR_W, I2C_buffer, 2);

	I2C_buffer[0] = 0x47;
	I2C_buffer[1] = 0x80;
	I2C_Master_Transmit(DAC_ADDR_W, I2C_buffer, 2);

	I2C_buffer[0] = 0x32;
	I2C_buffer[1] = 0x80;
	I2C_Master_Transmit(DAC_ADDR_W, I2C_buffer, 2);

	I2C_buffer[0] = 0x32;
	I2C_buffer[1] = 0x00;
	I2C_Master_Transmit(DAC_ADDR_W, I2C_buffer, 2);

	I2C_buffer[0] = 0x00;
	I2C_buffer[1] = 0x00;
	I2C_Master_Transmit(DAC_ADDR_W, I2C_buffer, 2);

	I2C_buffer[0] = 0x02;						//Power Control 1 register
	I2C_buffer[1] = 0x9E;						//Power-Up the DAC
	I2C_Master_Transmit(DAC_ADDR_W, I2C_buffer, 2);

}

//DAC stop
void DAC_Power_Down(){


	I2C_buffer[0] = BEEP_CONFIG_ADDR;		//DAC register of address 0x1E
	I2C_buffer[1] = 0x00;					//Disable beep
	I2C_Master_Transmit(DAC_ADDR_W, I2C_buffer, 2);


	I2C_buffer[0] = POWER_CTRL_2;
	I2C_buffer[1] = 0xF0;					//Turning headphone channel and speaker channel OFF
	I2C_Master_Transmit(DAC_ADDR_W, I2C_buffer, 2);

	I2C_buffer[0] = POWER_CTRL_1;
	I2C_buffer[1] = 0x9F;					//Powering Down the DAC
	I2C_Master_Transmit(DAC_ADDR_W, I2C_buffer, 2);

	HAL_Delay(1);							//Wait 1ms

	GPIOD->BSRR = 0x00100000;				//Pin PD4 low (reset pin of the DAC)
}

//configuration of the song
void Beep_Config(uint8_t beepDuration, uint8_t silenceDuration, uint8_t frequency){
/*
  //Configure the type of beep signal (multiple beep)
	  I2C_buffer[0] = BEEP_CONFIG_ADDR;						//DAC register of address 0x1E
	  I2C_buffer[1] = 0x80;									//Select the multiple beep mode
	  I2C_Master_Transmit(DAC_ADDR_W, I2C_buffer, 2);
*/
  //Configure the type of beep signal (continuous beep)
	  I2C_buffer[0] = BEEP_CONFIG_ADDR;						//DAC register of address 0x1E
	  I2C_buffer[1] = 0xC0;									//Select the continuous beep mode
	  I2C_Master_Transmit(DAC_ADDR_W, I2C_buffer, 2);

  //Configure the beep frequency and the beep ON time
	  I2C_buffer[0] = BEEP_FREQ_ADDR;						//DAC register of address 0x1C
	  I2C_buffer[1] = (frequency << 4) | beepDuration;		//Set sound frequency and sound duration
	  I2C_Master_Transmit(DAC_ADDR_W, I2C_buffer, 2);

  //Configure the beep OFF time and the beep volume
	  I2C_buffer[0] = BEEP_VOLUME_ADDR;						//DAC register of address 0x1D
	  I2C_buffer[1] = (silenceDuration << 5) | 0x1A;		//Set silence duration
	  I2C_Master_Transmit(DAC_ADDR_W, I2C_buffer, 2);

}

//alarm
void Run_Alarm(void)
{
    I2S_buffer[0] = 0xAAAA; // Frame to send on the I2S bus
    HAL_I2S_Transmit(&hi2s3, I2S_buffer, 1, 500); // Transmit the frame
}

//configuration cubemx --> tim3 : Internal Clock + prescaler and period
// NVIC tim3 global interrupt :
/*
static void MX_TIM3_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 8400;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 5000;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}
*/

//configuration cubemx I2S3 :
/*
static void MX_I2S3_Init(void)
{
  hi2s3.Instance = SPI3;
  hi2s3.Init.Mode = I2S_MODE_MASTER_TX;
  hi2s3.Init.Standard = I2S_STANDARD_PHILIPS;
  hi2s3.Init.DataFormat = I2S_DATAFORMAT_16B;
  hi2s3.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
  hi2s3.Init.AudioFreq = I2S_AUDIOFREQ_8K;
  hi2s3.Init.CPOL = I2S_CPOL_LOW;
  hi2s3.Init.ClockSource = I2S_CLOCK_PLL;
  hi2s3.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_ENABLE;
  if (HAL_I2S_Init(&hi2s3) != HAL_OK)
  {
    Error_Handler();
  }
}
*/

//à mettre dans le main.c
/*
// interruption callback function
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM3) {
        tonality = 1;
		timer_interrupt_occurred = 1;
		counter++;
	}
}
*/

//à mettre dans le int main()
/*
MX_GPIO_Init();
MX_I2S3_Init();
MX_TIM3_Init();

//timer start
HAL_TIM_Base_Start_IT(&htim3);

// I2C init
init_I2C1();
// GPIO init
DAC_GPIO_Init();

//DAC start
DAC_Power_Up();
*/

//à mettre dans le while(1) du int main()
/*
//modification of the song frequency
if(tonality == 1)
{
  tonality = 0;
  if(beep_num == MEDIUM_SOUND)
  {
	  Beep_Config(SHORT_BEEP, SHORT_SILENCE, HIGH_PITCHED_SOUND);
	  beep_num = HIGH_PITCHED_SOUND;
  }
  else if(beep_num == HIGH_PITCHED_SOUND)
  {
	  Beep_Config(SHORT_BEEP, SHORT_SILENCE, MEDIUM_SOUND);
	  beep_num = MEDIUM_SOUND;
  }
}

//playing the song while there is interruption for the example
if (timer_interrupt_occurred) {
	Run_Alarm();
	if(counter == 50)
	{
		HAL_TIM_Base_Stop_IT(&htim3);
		timer_interrupt_occurred = 0;
	}
}
else
{
	DAC_Power_Down();
}
*/
