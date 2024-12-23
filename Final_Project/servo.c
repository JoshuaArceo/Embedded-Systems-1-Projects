/*
 * servo.c
 *
 *  Created on: Apr 3, 2024
 *      Author: jarceo
 */

#include <servo.h>
#include "Timer.h"
#include <stdint.h>
#include "driverlib/interrupt.h"

int rightAngle = 8000;
int leftAngle = 36000;


void servo_init(void){

        SYSCTL_RCGCGPIO_R |= 0x2;
        GPIO_PORTB_DIR_R |= 0x20;
        GPIO_PORTB_DEN_R |= 0x20;
        GPIO_PORTB_AFSEL_R |= 0x20;
        GPIO_PORTB_PCTL_R |= 0x700000;
        SYSCTL_RCGCTIMER_R |= 0x2;

        TIMER1_CTL_R &= 0x7EFF;
        TIMER1_CFG_R |= 0x4;
        TIMER1_TBMR_R |= 0xA;
        TIMER1_CTL_R |= ~0x3FFF;
        TIMER1_TBILR_R = 0xE200;
        TIMER1_TBPR_R = 0x4;
        TIMER1_TBMATCHR_R = 22000;
        TIMER1_TBPMR_R &= 0x0;
        TIMER1_CTL_R |= 0x100;

}



void servo_move(uint16_t degrees){
    //TODO add limits
//    timer_waitMillis(250);
    TIMER1_TBMATCHR_R = degrees*155+rightAngle;
}
