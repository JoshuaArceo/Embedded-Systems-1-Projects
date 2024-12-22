/// Simple 'Hello, world' program
/**
 * This program prints "Hello, world" to the LCD screen
 * @author Chad Nelson
 * @date 06/26/2012
 *
 * updated: phjones 9/3/2019
 * Description: Added timer_init call, and including Timer.h
 */

#include "Timer.h"
#include "lcd.h"
#include <stdio.h>
#include <stdlib.h>


int main (void) {

	timer_init(); // Initialize Timer, needed before any LCD screen functions can be called 
	              // and enables time functions (e.g. timer_waitMillis)

	lcd_init();   // Initialize the LCD screen.  This also clears the screen. 

	lcd_rotatingBanner("hello world");
	// Print "Hello, world" on the LCD
//	lcd_printf("Hello, world");

	// lcd_puts("Hello, world"); // Replace lcd_printf with lcd_puts
        // step through in debug mode and explain to TA how it works
    
	// NOTE: It is recommended that you use only lcd_init(), lcd_printf(), lcd_putc, and lcd_puts from lcd.h.
       // NOTE: For time functions, see Timer.h

	return 0;
}

void lcd_rotatingBanner(char data[]){
    strcat(data, " ");
    while(1){
        timer_waitMillis(300);
        int i;
        int textSize = strlen(data);
        char toPrint[20];
        for(i = 0; i<textSize +20; i++){
            timer_waitMillis(300);
            lcd_clear();
            int j;
            if(i<=20){
                for(j = 0; j<20-i; j++){
                    lcd_putc(32);
                }
                for(j = 0; j < i && j < textSize; j++){
                                lcd_putc(data[j]);
                            }
            }else{
                for(j = abs(i-20); j < textSize; j++){
                    lcd_putc(data[j]);
                }
            }


        }
    }

}
