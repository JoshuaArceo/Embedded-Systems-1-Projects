/**
 * @file main.c
 * @author JArceo
 */

#include "Timer.h"
#include "lcd.h"
#include "ping.h"
#include "servo.h"
#include "adc.h"
#include "movement.h"
#include "uart-interrupt.h"
#include "button.h"
#include "open_interface.h"
#include "sensorScan.h"



void song_init(void){

    unsigned char notes_num1 = 2;
    unsigned char notes1[2] = {72, 64};
    unsigned char duration1[2] = {25, 50};
    oi_loadSong(0, notes_num1, notes1, duration1);

    unsigned char notes_num2 = 1;
    unsigned char notes2[1] = {72};
    unsigned char duration2[1] = {25};
    oi_loadSong(1, notes_num2, notes2, duration2);

}

extern int short_count;

int main(void)
{
    timer_init();
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    lcd_init();
    uart_interrupt_init();
    button_init();
    adc_init();
    ping_init();
    servo_init();

   song_init();

    int i, k;
    uint32_t IR_Data = 0;

    float PING_Dist = 0;

    double IR_Dist;
    double cm;
    double sum;

    char data[80];

    lcd_printf("Initialized");

//    oi_play_song(0);

    while (1)
    {
        if (command_flag)
        {
            //TODO UART send back data for GUI handling

            if (command_byte == 'g') //Scan
            {

                servo_move(0);
                timer_waitMillis(250);
                for (i = 0; i <= 180; i += 2)
                {
                    servo_move(i);
                    IR_Data = 0;
                    PING_Dist = 0;

//                    timer_waitMillis(20);
                    for (k = 0; k < 2; k++)
                    {

                        IR_Data += adc_read();
                        PING_Dist += ping_getDistance();
                    }

                    // Average Data
                    IR_Data /= 2;
                    PING_Dist /= 2;

                    cm = 0;
                    sum = 0;
                    for (k = 0; k < 16; k++)
                    {
                        cm = 5000000 * pow(IR_Data, -1.66);
                        sum += cm;
                    }
                    IR_Dist = sum / 16;
                    sprintf(data, "%d %18f%s %20d, %18f%s", i, PING_Dist, "cm",
                            IR_Data, IR_Dist, "cm");
                    uart_sendStr(data);
                }
                uart_sendStr("END");

            }

            if (command_byte == 'w') //Move forward
            {
                move_forward(&sensor_data, 60);
            }
            else if (command_byte == 'a') //Turn left
            {
                turn_left(&sensor_data, 5);
            }
            else if (command_byte == 's') //Move back
            {
                move_backward(&sensor_data, 100);
            }
            else if (command_byte == 'd') //Turn right
            {
                turn_right(&sensor_data, 5);
            }
            if(command_byte == 'm'){
                for(i = 0; i < short_count; i++){
                    oi_play_song(1);
                    timer_waitMillis(500);
                }
            }
            command_flag = 0;
        }
        else
        {
            stop_wheels(&sensor_data);
        }
    }
}
