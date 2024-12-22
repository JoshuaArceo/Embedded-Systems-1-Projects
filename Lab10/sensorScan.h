/*
 * sensorScan.h
 *
 *  Created on: Mar 4, 2024
 *      Author: jarceo
 */

#ifndef SENSORSCAN_H_
#define SENSORSCAN_H_

#include "cyBot_Scan.h"


typedef struct {
    int angle;
    float distance;
    int rad_width;
    double lin_width;
} objectData;


void cyBot_sendString(char data[]);
int cyBot_scanRange(int lower, int upper, cyBOT_Scan_t *getScan, objectData *objects, oi_t *sensor_data);



#endif /* SENSORSCAN_H_ */
