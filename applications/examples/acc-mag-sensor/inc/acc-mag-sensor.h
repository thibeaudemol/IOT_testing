#include "platform.h"

osTimerId accelerometer_timer_id;
osTimerId lightsensor_timer_id;
osMutexId i2cMutexId;
osThreadId defaultTaskHandle;
osTimerId iwdgTimId;

void StartDefaultTask(void const *argument);
void accelerometer_measurement(void);
void print_accelerometer(uint16_t data[]);
void lightsensor_measurement(void);
void print_light_intensity(int data);
