/**
 * @file lab9_template.c
 * @author
 * Template file for CprE 288 Lab 9
 */

#include "Timer.h"
#include "lcd.h"
#include "ping.h"
#include "servo.h"
#include "uart-interrupt.h"
#include "button.h"

// Uncomment or add any include directives that are needed

#warning "Possible unimplemented functions"
#define REPLACEME 0

int main(void) {
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
	uart_interrupt_init();
	button_init();
	ping_init();
	servo_init();


//int i;
//	for(i = 0; i <180; i+=2){
//	    servo_move(i);
//	}

//	while(1){

//        timer_waitMillis(1000);
//	    servo_move(20);
//	    timer_waitMillis(1000);
//	    servo_move(120);
//	}


	// YOUR CODE HERE
//	ping_trigger();
//	float dist;
//	timer_waitMillis(500);
//	servo_move(90);
//	timer_waitMillis(500);
//	servo_move(30);
//    timer_waitMillis(500);
//    servo_move(150);
//    timer_waitMillis(500);
//    servo_move(90);

    int button, servoDir = -1; //-1 clockwise
    int degree = 90;
    while(1)
	{
        timer_waitMillis(100);
		   button = button_getButton();
            switch(button){
            case 1:
                degree += 1*servoDir;
                if(degree < 180 && degree > 0){
                    servo_move(degree);
                }else{
                    degree -= 5*servoDir;
                }
                break;
            case 2:
                degree += 5*servoDir;
                if(degree < 180 && degree > 0){
                    servo_move(degree);
                }else{
                    degree -= 5*servoDir;
                }
                break;
            case 3:
                servoDir *= -1;
                break;
            case 4:
                if(servoDir < 0){
                    degree = 5;
                }else{
                    degree = 175;
                }
                servo_move(degree);
                break;
            }
            if(servoDir < 0){
                lcd_printf("%20s%d","Clockwise", (degree*155+8000));
            }else{
                lcd_printf("%20s%d","Counter",(degree*155+8000));
        }
     }

}
