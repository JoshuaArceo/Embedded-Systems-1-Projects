/*
 * servo.h
 *
 *  Created on: Apr 3, 2024
 *      Author: jarceo
 */

#include <stdint.h>
#include <stdbool.h>
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

#ifndef SERVO_H_
#define SERVO_H_


void servo_init(void);
void servo_move(uint16_t degrees);


#endif /* SERVO_H_ */
