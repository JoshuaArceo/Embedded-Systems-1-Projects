#include "cyBot_uart.h"
#include "cyBot_Scan.h"
#include "Timer.h"
#include "movement.h"

typedef struct {
    int object;
    int angle;
    float distance;
    int width;
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




int cyBot_scanRange(int lower, int upper, cyBOT_Scan_t *getScan, objectData *objects, oi_t *sensor_data)
{
    char data[50];
    sprintf(data, "%s, %20s", "Degrees", "PING Distance (cm)");
    cyBot_sendString(data);
    int i;
    int current = 0;
    float lastDist = 0;
    float currDist = 0;
    int first_angle = -1;
    int last_angle = -1;
    int lookingAt = 0;
    cyBOT_Scan(180, *&getScan);
    timer_waitMillis(500);
    for(i = 0; i<2; i++){
        cyBOT_Scan(0, *&getScan);
        timer_waitMillis(500);
    }



    int k;
    for(i = lower; i <= upper; i +=2){
        currDist = 0;
              for(k = 0; k<2; k++){
                  cyBOT_Scan(i, *&getScan);
                  currDist += getScan -> sound_dist;
                  sprintf(data, "%d %30f", i, currDist);
                  cyBot_sendString(data);
              }
              currDist/=2;
              sprintf(data, "Avg: %d %30f", i, currDist);
              cyBot_sendString(data);

//              irData = getScan-> IR_raw_val;
//              irDatainCent = 6000000 * pow(irData, -1.681);


              if(currDist <= 90 ){
                  lookingAt = 1;
              }else{
                  lookingAt = 0;
              }

              if(abs(lastDist-currDist) >= 80 && lookingAt){ //looking at an object
                  if(first_angle == -1){ //will only set first angle if it is the first time detected
                      first_angle = i;
                      cyBot_sendString("Found?");
                  }
              }

              else if (abs(currDist - lastDist) > 50 && last_angle == -1 && first_angle != -1) { //moving off object, incrementing current
                  last_angle = i - 2; //set last angle it was seen
                  //set all values for last object
                  objects[current].object = current;
                  objects[current].angle = (first_angle+last_angle)/2; //center of the object
                  objects[current].distance = lastDist;
                  objects[current].width = last_angle - first_angle;
                  current++; //increment so it will count the next object
                  first_angle = -1; //set angles to 0 to get ready for the next one
                  last_angle = -1;
                  sprintf(data, "FOUND");
                  cyBot_sendString(data);
              }
              lastDist = currDist; //set last dist to the current distance to update it
          }

    sprintf(data, "Objects found: %d", current);
    cyBot_sendString(data);
    int a, w, smallestWidth, smallestAngle, smallestObj;
    double d, objD;
    smallestWidth = 200;
    smallestAngle = 200;
    for (i = 0; i < current; i++)
    {
        a = objects[i].angle;
        w = objects[i].width;
        d = objects[i].distance;

        if(smallestWidth > w){
            smallestWidth = w;
            smallestAngle = a;
            objD = d;
            smallestObj = i;
        }


        sprintf(data, "%s: %5f %10s %5d %10s %5d", "Dist", d,
                "Angle", a, "Radial Width",
                w);
        cyBot_sendString(data);
    }

    sprintf(data, "Smallest object is: %d\n%s: %5f %10s %5d %10s %5d", smallestObj, "Dist", objD, "Angle", smallestAngle, "Radial Width", smallestWidth);
    cyBot_sendString(data);


//    sprintf(data, "Smallest angle is: %d", smallestAngle);
//    cyBot_sendString(data);

         sprintf(data, "Gonna turn!");
         cyBot_sendString(data);

         int delta;
         objD*=10;
         objD-=40;
         delta = abs(smallestAngle - 90);

//         sprintf(data, "Delta of: %d",delta);
//          cyBot_sendString(data);

          delta*=3;
          delta/=4;

         if(smallestAngle<90){
             sprintf(data, "Turning %d %s", delta, "to the right");
             cyBot_sendString(data);
             turn_right(&sensor_data,delta);
         }else{
             sprintf(data, "Turning %d %s", delta, "to the left");
             cyBot_sendString(data);
             turn_left(&sensor_data,delta);
         }


//         cyBot_sendString(data);

         move_forward(&sensor_data, objD);

    return current;
}

