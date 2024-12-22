/**
 * lab4_template.c
 *
 * Template file for CprE 288 lab 4
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 */

#include "button.h"
#include "Timer.h"
#include "lcd.h"
#include "cyBot_uart.h"  // Functions for communicating between CyBot and Putty (via UART)
// PuTTy: Baud=115200, 8 data bits, No Flow Control, No Parity, COM1

#define REPLACEME 0

void cyBot_sendString(char data[]);

int main(void)
{
    timer_init(); // Must be called before lcd_init(), which uses timer functions
    cyBot_uart_init();
    lcd_init();
    button_init();

    // YOUR CODE HERE
    uint8_t x = 0;
    char output[20];
//    cyBot_sendString("test");
//    cyBot_sendByte('a');
    while (1)
    {
        // YOUR CODE HERE
        x = button_getButton();
        lcd_printf("%d", x);
        if (x != 0)
        {
            sprintf(output, "You pressed button %d", x);
            cyBot_sendString(output);
        }
    }
}

void cyBot_sendString(char data[])
{
    int dataLen = strlen(data);
    int i;
    for (i = 0; i < dataLen; i++)
    {
        cyBot_sendByte(data[i]);
    }
    cyBot_sendByte('\n');
}

