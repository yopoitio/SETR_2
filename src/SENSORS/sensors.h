#ifndef SENSORS_H_
#define SENSORS_H_

#include <stdio.h>

#define SAMPLES 50
#define HISTORY 20

int simulate_temp_sensor(char action, int history[]);

int simulate_hum_sensor(char action, int history[]);

int simulate_co2_sensor(char action, int history[]);
#endif