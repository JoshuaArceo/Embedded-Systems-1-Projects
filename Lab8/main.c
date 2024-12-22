#include "lcd.h"
#include "open_interface.h"
#include "Timer.h"
#include "movement.h"
#include "uart-interrupt.h"
#include "cyBot_Scan.h"
#include <string.h>
#include "lab3.h"
#include "adc.h"

/**
 * main.c
 */
int main(void)
{
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data); // should turn the iRobot Create's "DirtDetect" LED to blue
    timer_init();
    lcd_init();
    uart_interrupt_init();
    int current = 0;
    cyBOT_Scan_t scan;
    cyBOT_init_Scan(0b0111);
    adc_init();
    char data[50];
//    int ran = 0;
//    int found = 0;
//    right_calibration_value = 295750;
//    left_calibration_value = 1261750;
    lcd_printf("INITIALIZED");

    cyBOT_Scan(90, &scan);
    int IRData, sum, sum0, sum1;
    int i;
    int IRScanData;
    float IR_Dist, PING_Dist;;
    int cm, k;



    while (1)
    {
        if (command_flag)
        {
            if(command_byte == 'g'){
                sum0 = 0;
                    sum1 = 0;
                    PING_Dist = 0;
                    for(i = 0; i<5; i++){
                        cyBOT_Scan(90, &scan);
                        IRData = adc_read();
                        PING_Dist += scan.sound_dist;
                        IRScanData = scan.IR_raw_val;
                        sprintf(data, "IRData: %d", IRData);
                        uart_sendStr(data);
//                        sprintf(data, "IRScanData: %d", IRScanData);
//                                uart_sendStr(data);
                        sum0+= IRData;
                        sum1+= IRScanData;
                    }
                    sum0 /= i;
                    sum1/= i;
                    PING_Dist /=i;
                    IRData = sum0;
                    IRScanData = sum1;

                  IR_Dist = 102.57 - 0.0884 * IRData + (2.23 * pow(10,-5)) * pow(IRData, 2);
                                  cm = 0;
                                 sum = 0;
                                 for(k = 0; k <16; k++){
                                     cm = 5000000 * pow(IRData, -1.66);
                                     sum += cm;
                                 }
                                 IR_Dist = sum / 16;

                    sprintf(data, "Final IRData: %d", IRData);
                    uart_sendStr(data);
                    sprintf(data, "Final IR Dist: %fcm", IR_Dist);
                    uart_sendStr(data);

                    sprintf(data, "Final Sound Dist: %fcm", PING_Dist);
                    uart_sendStr(data);
                    sprintf(data, "Dist: %13f\nData: %14d", IR_Dist, IRData);
                    lcd_printf(data);

//                    sprintf(data, "Final IRScanData: %d", IRScanData);
//                    uart_sendStr(data);
            }

            command_flag = 0;
        }
    }

    oi_free(sensor_data);
    return 0;
}
