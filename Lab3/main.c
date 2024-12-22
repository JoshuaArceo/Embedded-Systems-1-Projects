#include "lcd.h"
#include "open_interface.h"
#include "Timer.h"
#include "movement.h"
#include "cyBot_uart.h"
#include "cyBot_Scan.h"
#include <string.h>
#include "lab3.h"


//calibration code
//int main(void){
//
//    timer_init();
//    lcd_init();
//    cyBOT_init_Scan(0b0111);
//    cyBOT_SERVO_cal();
//
//    return 0;
//}

/**
 * main.c
 */
int main(void){
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data); // should turn the iRobot Create's "DirtDetect" LED to blue
    lcd_init();
    cyBot_uart_init();

    int current = 0;
    cyBOT_Scan_t scan;
    cyBOT_init_Scan(0b0111);


    int userInput = cyBot_getByte();
    char data[50];
    int ran = 0;
    while(userInput != 'm' && !ran) {
        sprintf(data, "Pressed %c", userInput);
        cyBot_sendString(data);
        lcd_printf("Got a %c",userInput);
        userInput = cyBot_getByte();
    }
    ran=1;
    objectData objects[20] = {0};
    int found = 0;
    found = cyBot_scanRange(0, 180, &scan, &*objects,&sensor_data);

//    double smallestAngle;
//	double smallestWidth = 180;
//	double objW, objD;
//	int i;
//    for(i = 0; i < found; i++){
//		objW = objects[i].width;
//		if(smallestWidth < objW){
//		    smallestWidth = objW;
//			smallestAngle = objects[i].angle;
//			objD = objects[i].distance;
//		}
//
//	   sprintf(data, "\n\r%d\t\t%f\t\t%.2f\t\t%f",
//       objects[i].object + 1,
//       objects[i].angle,
//       objects[i].distance,
//       objects[i].width);
//	   cyBot_sendString(data);
//   }
	//To move to smallest object, find delta of 90 and angle. 
	//Then rotate bot by that delta to face the object and move forward
	//By that distance - 5 for padding
//    objD -= 10;
//     double delta;
//     delta = abs(smallestAngle-90.0);
//     if(smallestAngle<90){
//         turn_right(&sensor_data,delta);
//     }else{
//         turn_left(&sensor_data,delta);
//     }
//
//     move_forward(&sensor_data, objD);

    oi_free(sensor_data);
    return 0;
}
