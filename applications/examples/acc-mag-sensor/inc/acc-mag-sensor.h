#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "platform.h"

osThreadId defaultTaskHandle;
osTimerId iwdgTimId;
osMutexId i2cMutexId;

struct RGBvalues
{
  /* data */
  int red, green, blue;
};

void StartDefaultTask(void const *argument);
void lightsensor_measurement(void);
void RGBsensor_measurement(void);
void print_light_intensity(int data);
void print_color(struct RGBvalues values);

#ifdef __cplusplus
}
#endif
#endif /* __MAIN_H */

#include "platform.h"


osTimerId accelerometer_timer_id;
osTimerId lightsensor_timer_id;

uint8_t count=0;


void StartDefaultTask(void const *argument);

void accelerometer_measurement(void);
void print_accelerometer(uint16_t data[]);

