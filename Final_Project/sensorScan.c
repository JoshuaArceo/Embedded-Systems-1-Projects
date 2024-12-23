/*
 * sensorScan.h
 *
 *  Created on: Mar 4, 2024
 *      Author: jarceo
 */
#include "uart-interrupt.h"
#include "Timer.h"
#include "movement.h"
#include "servo.h"
#include "adc.h"
#include "ping.h"
#include <math.h>

typedef struct
{
    int angle;
    float distance;
    int rad_width;
    double lin_width;
} objectData;

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

const int IR_DELTA = 100;

int cyBot_scanRange(int lower, int upper, objectData *objects, oi_t *sensor_data)
{

    char data[50];
    sprintf(data, "%s, %20s, %20s", "Degrees", "PING Distance (cm)",
            "IR_Data (Raw)");
    cyBot_sendString(data);

    int i, k;
    int current = 0;

    int first_angle = -1;
    int last_angle = -1;

    int IR_Data = 0;
    int IR_Data_Prev = -1;

    float PING_Dist = 0;

    int scanAngle, a, w;
    double d, lw;

    int IR_Array[90];
    float PING_Array[90];
    float IR_Dis_Array[90];

    int delta;

    double IR_Dist;
   double cm;
   double sum;

   float objDEnd;

   int smallestW, smallestA, smallestObj;
   double objD = 99999;

//    while(objD > 10.0){

    current = 0;

    memset(objects, 0, sizeof(objects));

//    servo_move(180);
//    timer_waitMillis(500);

    servo_move(0);
    timer_waitMillis(500);


    for (i = lower; i <= upper; i += 2)
    {
        IR_Data = 0;
        PING_Dist = 0;
        servo_move(i);
        timer_waitMillis(15);
        for (k = 0; k < 2; k++)
        {
            IR_Data += adc_read();
            PING_Dist +=  ping_getDistance();
        }

        // Average Data
        IR_Data /= 2;
        PING_Dist /= 2;

        IR_Array[i/2] = IR_Data;
        PING_Array[i/2] = PING_Dist;


        cm = 0;
               sum = 0;
               for(k = 0; k <16; k++){
                   cm = 5000000 * pow(IR_Data, -1.66);
                   sum += cm;
               }
               IR_Dist = sum / 16;
       IR_Dis_Array[i/2] = IR_Dist;
        sprintf(data, "%d %18f%s %20d, %18f%s", i, PING_Dist, IR_Data, IR_Dist, "cm");
        cyBot_sendString(data);

//        IR_Data = 0;
//       PING_Dist = 0;

    }

//    for(i = 0; i<90; i++){
//
//        if(first_angle == -1 && i > 0  && abs(IR_Array[i] - IR_Array[i-1]) > 200){
//            if(abs(IR_Dis_Array[i] < 90)){
//                if(abs(IR_Dis_Array[i] - PING_Array[i]) < 15){
//                    first_angle = 2*i;
//                }
//            }
//            else{
//                first_angle = 2*i;
//                }
//            }
//
//        if(first_angle != -1 && abs(IR_Dis_Array[i] - PING_Array[i]) > 15 && abs(IR_Array[i] - IR_Array[i-2]) > 200){
//            last_angle = (2*i) - 2;
//        }
//
//        if(last_angle != -1){
//            objects[current].angle = (first_angle + last_angle) / 2; //center of the object
//            objects[current].rad_width = last_angle - first_angle;
//            current++;
//            last_angle = -1;
//            first_angle = -1;
//        }
//    }
    uart_sendStr("END");
    //Second Scan to find distance

//    for(i = 0; i < current; i++){
//        servo_move(objects[i].angle);
//        PING_Dist = 0;
//        for (k = 0; k < 4; k++)
//               {
//                   PING_Dist += ping_getDistance();
//               }
//
//            PING_Dist /= 4;
//            objects[i].distance = PING_Dist;
//            objects[i].lin_width = 2.0 * M_PI * PING_Dist * (objects[i].rad_width / 360.0);
//            a = objects[i].angle;
//            w = objects[i].rad_width;
//            d = PING_Dist;
//            lw = objects[i].lin_width = 2.0 * M_PI * PING_Dist * (w / 360.0);
//               lcd_printf("Object: %d", i);
//               sprintf(data, "%s: %5f %10s %5d %10s %5d %10s %5f", "Dist", d, "Angle", a,
//                       "Radial Width", w, "Linear Width", lw);
//               cyBot_sendString(data);
//    }
//
//
//    smallestW = 180;
//    for (i = 0; i < current; i++)
//       {
//           a = objects[i].angle;
//           w = objects[i].lin_width;
//           d = objects[i].distance;
//
//           if(smallestW > w){
//               smallestW = w;
//               smallestA = a;
//               objD = d;
//               smallestObj = i;
//           }
//
//       }
//    if(objD <= 10){
//        break;
//    }
//
//
//             delta = abs(smallestA - 90);
//
//             //TODO change to trig conversions to calc how much bot should turn
//              delta*=3;
//              delta/=4;
//
//             if(smallestA<90){
//                 sprintf(data, "Turning %d %s", delta, "to the right");
//                 cyBot_sendString(data);
//                 turn_right(&sensor_data,delta);
//             }else{
//                 sprintf(data, "Turning %d %s", delta, "to the left");
//                 cyBot_sendString(data);
//                 turn_left(&sensor_data,delta);
//             }
//
//             sprintf(data, "Driving %f",objD);
//             cyBot_sendString(data);
//
//             objDEnd = objD - (move_forward_obj(&sensor_data, (objD*9.90))/10);
//             sprintf(data, "I should be %fcm from object",objDEnd);
//             cyBot_sendString(data);
//
//
//    }


    return current;
}


