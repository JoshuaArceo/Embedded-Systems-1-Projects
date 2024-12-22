/**
 * @file lab9_template.c
 * @author
 * Template file for CprE 288 Lab 9
 */

#include "Timer.h"
#include "lcd.h"
#include "ping.h"
#include "uart-interrupt.h"

// Uncomment or add any include directives that are needed


int main(void) {
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
	uart_interrupt_init();
	ping_init();





	// YOUR CODE HERE
	ping_trigger();
	float dist;
	while(1)
	{
	    timer_waitMillis(1000);
        dist = ping_getDistance();
        lcd_printf("%.2f",dist);
//
//
//      // YOUR CODE HERE
	}

}
