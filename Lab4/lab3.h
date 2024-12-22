/*
 * uartHelp.h
 *
 *  Created on: Feb 7, 2024
 *      Author: bellas23
 */

#ifndef LAB3_H_
#define LAB3_H_

#include "cyBot_Scan.h"


typedef struct {
    int object;
    int angle;
    float distance;
    int width;
} objectData;


void cyBot_sendString(char data[]);
int cyBot_scanRange(int lower, int upper, cyBOT_Scan_t *getScan, objectData *objects, oi_t *sensor_data);


#endif /* LAB3_H_ */
