/*
 * movement.h
 *
 *  Created on: Jan 31, 2024
 *      Author: bellas23
 */
#include "open_interface.h"

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

int move_forward_obj(oi_t *sensor_data, double distance_mm);
double move_forward(oi_t *sensor_data, double distance_mm);
double move_backward(oi_t *sensor_data, double distance_mm);
double turn_right(oi_t *sensor, double degrees);
double turn_left(oi_t *sensor, double degrees);

void bump_left(oi_t *sensor_data);
void bump_right(oi_t *sensor_data);


void move_forward_manual();
void move_backward_manual();
void turn_right_manual();
void turn_left_manual();
void stop_manual();

#endif /* MOVEMENT_H_ */
