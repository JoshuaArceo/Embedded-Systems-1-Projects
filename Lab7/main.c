#include "lcd.h"
#include "open_interface.h"
#include "Timer.h"
#include "movement.h"
#include "uart-interrupt.h"
#include "cyBot_Scan.h"
#include <string.h>
#include "lab3.h"

/**
 * main.c
 */
int main(void)
{
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data); // should turn the iRobot Create's "DirtDetect" LED to blue
    lcd_init();
    uart_interrupt_init();
    int current = 0;
    cyBOT_Scan_t scan;
    cyBOT_init_Scan(0b0111);
    char data[50];
    int ran = 0;
    int found = 0;
    right_calibration_value = 301000;
    left_calibration_value = 1298500;
    lcd_printf("INITIALIZED1");
    while (1)
    {
        if (command_flag)
        {


            if (command_byte == 'g') //begin scan
            {
                command_flag = 0;
                sprintf(data, "\nPressed %c", command_byte);
                cyBot_sendString(data);
                objectData objects[20];
                found = cyBot_scanRange(0, 180, &scan, &*objects, &sensor_data);
                ran = 1;
            }


           if(command_byte == 'm') //enter manual mode
           {
               lcd_printf("M PRESSED MAIN");
           }

//           if(command_byte == 'w'){
//               move_forward_manual();
//               timer_waitMillis(250);
//           }
//           if(command_byte == 'a'){
//               turn_left_manual();
//               timer_waitMillis(250);
//              }
//           if(command_byte == 's'){
//               move_backward_manual();
//               timer_waitMillis(250);
//                      }
//           if(command_byte == 'd'){
//               turn_right_manual();
//               timer_waitMillis(250);
//              }

            command_flag = 0;
        }else{
//            stop_manual();
        }
    }

    oi_free(sensor_data);
    return 0;
}
