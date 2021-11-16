#include "acc-mag-sensor.h"
#include "LSM303AGRSensor.h"
#include "TCS3472.h"

#define accelerometer_timer    3
#define lightsensor_timer    3

int main(void)
{
  Initialize_Platform();
  GPIO_InitTypeDef GPIO_InitStruct;

  LSM303AGR_setI2CInterface(&common_I2C);
  LSM303AGR_ACC_reset();
	LSM303AGR_MAG_reset();
  HAL_Delay(100);
	LSM303AGR_init();
  TC3472_init(&common_I2C); //light sensor

  /* GPIO Ports Clock Enable */
   HAL_Init();
 __HAL_RCC_GPIOA_CLK_ENABLE();

 /*Configure GPIO pin : PB13*/
 GPIO_InitStruct.Pin = GPIO_PIN_13;
 GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
 GPIO_InitStruct.Pull = GPIO_PULLDOWN;
 HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

 HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  
  osThreadDef(defaultTask, StartDefaultTask, osPriorityLow, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  //feed IWDG every 5 seconds
  IWDG_feed(NULL);
  osTimerDef(iwdgTim, IWDG_feed);
  iwdgTimId = osTimerCreate(osTimer(iwdgTim), osTimerPeriodic, NULL);
  osTimerStart(iwdgTimId, 5 * 1000);

  osTimerDef(accsensor_Tim, accelerometer_measurement);
  accelerometer_timer_id = osTimerCreate(osTimer(accsensor_Tim), osTimerPeriodic, NULL);
  osTimerStart(accelerometer_timer_id, accelerometer_timer * 1000);

  osTimerDef(lightsensor_Tim, lightsensor_measurement);
  lightsensor_timer_id = osTimerCreate(osTimer(lightsensor_Tim), osTimerPeriodic, NULL);
  osTimerStart(lightsensor_timer_id, lightsensor_timer * 1000);

  // MUTEX
  osMutexDef(txMutex);
  i2cMutexId = osMutexCreate(osMutex(txMutex));

  osKernelStart();

  while (1)
  {
  }

	
}

//interrupt voor led

void EXTI15_10_IRQHandler(void) {
  
  __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13); // removes interrupt from pin
  LSM303AGR_writeRegister(0x30, 0x0A, 0); //step 8
  LSM303AGR_readRegister(0x31,0x00000000,0); //step 10
  printINF("it works");
 }

 void lightsensor_measurement(void){
  osMutexWait(i2cMutexId, osWaitForever);
  int clear_light = getClearData();
  osMutexRelease(i2cMutexId);
  print_light_intensity(clear_light);
}

void print_light_intensity(int data){
  printINF("Light intensity: %d  lux \r\n", data);
}

void accelerometer_measurement(void){
	uint16_t accDataRaw[3];
  osMutexWait(i2cMutexId, osWaitForever);
  LSM303AGR_ACC_readAccelerationRawData(accDataRaw);
  osMutexRelease(i2cMutexId);
  print_accelerometer(accDataRaw);
}

void print_accelerometer(uint16_t data[]){
  printINF("Accelerometer data: X: %d Y: %d Z: %d \r\n", data[0], data[1], data[2]);
}

void StartDefaultTask(void const *argument)
{
  for (;;)
  {
    osDelay(1);
  }
}